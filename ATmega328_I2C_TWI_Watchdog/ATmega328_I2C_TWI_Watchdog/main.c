#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL // Frequência do clock do microcontrolador em Hz
#define BAUD_RATE 9600   // Taxa de transmissão da USART em bps

void USART_Init() {
    // Configuração da USART com baud rate de 9600 bps
    UBRR0 = F_CPU / (16 * BAUD_RATE) - 1;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Habilita transmissão e recepção USART
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Configuração de 8 bits de dados
}

char USART_Receive() {
    // Aguarda a recepção de dados pela USART
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0; // Retorna o byte recebido
}

void I2C_Init() {
    // Configuração do barramento I2C/TWI
    TWBR = 32; // Configuração da frequência de operação (para 100 kHz)
}

int I2C_Start() {
    // Inicia a comunicação I2C/TWI
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
    // Verifique o status aqui e retorne um código de erro, se necessário
    return 0;
}

void I2C_Stop() {
    // Encerra a comunicação I2C/TWI
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
    while (TWCR & (1 << TWSTO));
}

int I2C_Write(char data) {
    // Envia um byte pela comunicação I2C/TWI
    TWDR = data;
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
    // Verifique o status aqui e retorne um código de erro, se necessário
    return 0;
}

void LCD_SendData(char data) {
    // Envia dados para o display Nokia 3530 LCD
    // Implementação específica para o display, consulte o datasheet
    // ...
}

void Watchdog_Init() {
    // Configuração do temporizador watchdog com timeout de 2 segundos
    WDTCSR = (1 << WDCE) | (1 << WDE);
    WDTCSR = (1 << WDE) | (1 << WDP2) | (1 << WDP1);
}

void Init() {
    USART_Init();
    I2C_Init();
    Watchdog_Init();
}

int main() {
    Init();

    sei(); // Habilita interrupções globais

    while (1) {
        // Aguarda dados pela USART
        char receivedData = USART_Receive();

        // Envia dados para o display Nokia 3530 LCD usando I2C/TWI
        if (I2C_Start() == 0) {
            if (I2C_Write(receivedData) != 0) {
                // Trate o erro aqui
            }
            I2C_Stop();
        } else {
            // Trate o erro aqui
        }

        // Alimenta o watchdog para evitar reinicialização
        asm volatile("wdr");
    }

    return 0;
}