#include <avr/io.h>
#include <avr/wdt.h>

// Configuração do USART
void USART_Init(unsigned int ubrr) {
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

// Leitura de dados do USART
unsigned char USART_Receive(void) {
    while (!(UCSR0A & (1<<RXC0)));
    return UDR0;
}

// Configuração do I2C/TWI
void I2C_Init(void) {
    TWSR = 0x00; // sem prescaler
    TWBR = 0x46; // taxa de transmissão de 100 kHz
    TWCR = (1<<TWEN); // ative o TWI
}

// Envio de dados para o LCD via I2C/TWI
void I2C_LCD_Send(unsigned char data) {
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    TWDR = 0x3C;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    TWDR = data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

// Configuração do Watchdog
void Watchdog_Init(void) {
    wdt_enable(WDTO_1S); // ative o watchdog para resetar a cada 1 segundo
}

int main(void) {
    USART_Init(51); // para 9600 bps com oscilador de 8 MHz
    I2C_Init();
    Watchdog_Init();

    while (1) {
        unsigned char data = USART_Receive();
        I2C_LCD_Send(data);
        wdt_reset(); // resete o watchdog
    }

    return 0;
}