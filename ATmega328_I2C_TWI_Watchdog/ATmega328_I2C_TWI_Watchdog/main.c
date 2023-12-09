/**
 * @file main.c
 * @brief Programa principal para controle de um microcontrolador ATmega328 com comunicação I2C e Watchdog Timer.
 * Este programa realiza a inicialização do I2C, inicia a comunicação I2C, escreve em um display LCD, 
 * configura e utiliza o Watchdog Timer para reiniciar o microcontrolador em caso de falhas.
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/**
 * @brief Função principal do programa.
 * @return Retorna 0 ao final da execução.
 */
int main(void)
{
    i2c_init(); // Inicializa a comunicação I2C
    i2c_start(); // Inicia a comunicação I2C
    i2c_write(0x70); // Escreve no endereço 0x70 do barramento I2C
    lcd_init(); // Inicializa o display LCD

    cli(); // Desativa todas as interrupções
    MCUSR &= ~(1<<WDRF); // Reseta o bit de Watchdog Reset
    // Inicia a sequência de atualização do watchdog timer
    WDTCSR = (1<<WDCE) | (1<<WDE);
    // Define o tempo de espera para 2.0s e habilita a interrupção do watchdog timer
    WDTCSR = (1<<WDIE) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);
    sei(); // Reativa todas as interrupções

    DDRB &=~ 0b11111110; // Configura o pino PB0 como entrada

    int contdr = 0; // Variável de contador

    while(1)
    {
        PORTB = 0x01; // Define o pino PB0 como saída e nível lógico alto
        PORTB = (1<<PORTC0); // Define o pino PC0 como saída e nível lógico alto

        char buffer[3];
        itoa(contdr, buffer, 10); // Converte o contador para string

        lcd_cmd(0x80); // Define o cursor na primeira linha do display LCD
        lcd_msg(buffer); // Escreve a string no display LCD

        _delay_ms(100); // Aguarda 100ms

        if ((PINB & (1<<PORTB0))==1){ // Verifica se o pino PB0 está em nível lógico alto
            contdr++; // Incrementa o contador
            WDTCSR |= (1<<WDIE); // Habilita a interrupção do watchdog timer
            _delay_ms(100); // Aguarda 100ms
        }
    }
}

/**
 * @brief Função para alternar o estado do pino Enable da LCD.
 */
void toggle()
{
    TWDR |= 0x02; // Define o pino Enable da LCD como nível lógico alto
    TWCR = (1<<TWINT) | (1<<TWEN); // Habilita a comunicação I2C e limpa a interrupção
    while  (!(TWCR &(1<<TWINT))); // Aguarda até que a comunicação I2C seja concluída
    delay(1); // Aguarda 1ms
    TWDR &= ~0x02; // Define o pino Enable da LCD como nível lógico baixo
    TWCR = (1<<TWINT) | (1<<TWEN); // Habilita a comunicação I2C e limpa a interrupção
    while  (!(TWCR &(1<<TWINT))); // Aguarda até que a comunicação I2C seja concluída
}

/**
 * @brief Função para enviar um comando de 4 bits para a LCD.
 * @param v1 O comando de 4 bits a ser enviado.
 */
void lcd_cmd_hf(char v1)
{
    TWDR &=~0x01; // Define o pino RS da LCD como nível lógico baixo (comando)
    TWCR = (1<<TWINT) | (1<<TWEN); // Habilita a comunicação I2C e limpa a interrupção
    while  (!(TWCR &(1<<TWINT))); // Aguarda até que a comunicação I2C seja concluída
    TWDR &= 0x0F; // Limpa os 4 bits superiores
    TWCR = (1<<TWINT) | (1<<TWEN); // Habilita a comunicação I2C e limpa a interrupção
    while  (!(TWCR &(1<<TWINT))); // Aguarda até que a comunicação I2C seja concluída
    TWDR |= (v1 & 0xF0); // Define os 4 bits superiores com o comando
    TWCR = (1<<TWINT) | (1<<TWEN); // Habilita a comunicação I2C e limpa a interrupção
    while  (!(TWCR &(1<<TWINT))); // Aguarda até que a comunicação I2C seja concluída
    toggle(); // Alterna o estado do pino Enable da LCD
}

/**
 * @brief Função para enviar um comando de 8 bits para a LCD.
 * @param v2 O comando de 8 bits a ser enviado.
 */
void lcd_cmd(char v2)
{
    TWDR &=~0x01; // Define o pino RS da LCD como nível lógico baixo (comando)
    TWCR = (1<<TWINT) | (1<<TWEN); // Habilita a comunicação I2C e limpa a interrupção
    while  (!(TWCR &(1<<TWINT))); // Aguarda até que a comunicação I2C seja concluída
    TWDR &= 0x0F; // Limpa os 4 bits superiores
    TWCR = (1<<TWINT) | (1<<TWEN); // Habilita a comunicação I2C e limpa a interrupção
    while  (!(TWCR &(1<<TWINT))); // Aguarda até que a comunicação I2C seja concluída
    TWDR |= (v2 & 0xF0); // Define os 4 bits superiores com o comando
    TWCR = (1<<TWINT) | (1<<TWEN); // Habilita a comunicação I2C e limpa a interrupção
    while  (!(TWCR &(1<<TWINT))); // Aguarda até que a comunicação I2C seja concluída
    toggle(); // Alterna o estado do pino Enable da LCD

    TWDR &= 0x0F; // Limpa os 4 bits superiores
    TWCR = (1<<TWINT) | (1<<TWEN); // Habilita a comunicação I2C e limpa a interrupção
    while  (!(TWCR &(1<<TWINT))); // Aguarda até que a comunicação I2C seja concluída
    TWDR |= ((v2 & 0x0F)<<4); // Define os 4 bits inferiores com o comando
    TWCR = (1<<TWINT) | (1<<TWEN); // Habilita a comunicação I2C e limpa a interrupção
    while  (!(TWCR &(1<<TWINT))); // Aguarda até que a comunicação I2C seja concluída
    toggle(); // Alterna o estado do pino Enable da LCD
}

/**
 * @brief Função para enviar um dado de 8 bits para a LCD.
 * @param v3 O dado de 8 bits a ser enviado.
 */
void lcd_dwr(char v3)
{
    TWDR|=0x01; // Define o pino RS da LCD como nível lógico alto (dado)
    TWCR = (1<<TWINT) | (1<<TWEN); // Habilita a comunicação I2C e limpa a interrupção
    while  (!(TWCR &(1<<TWINT))); // Aguarda até que a comunicação I2C seja concluída
    TWDR &= 0x0F; // Limpa os 4 bits superiores
    TWCR = (1<<TWINT) | (1<<TWEN); // Habilita a comunicação I2C e limpa a interrupção
    while  (!(TWCR &(1<<TWINT))); // Aguarda até que a comunicação I2C seja concluída
    TWDR |= (v3 & 0xF0); // Define os 4 bits superiores com o dado
    TWCR = (1<<TWINT) | (1<<TWEN); // Habilita a comunicação I2C e limpa a interrupção
    while  (!(TWCR &(1<<TWINT))); // Aguarda até que a comunicação I2C seja concluída
    toggle(); // Alterna o estado do pino Enable da LCD

    TWDR &= 0x0F; // Limpa os 4 bits superiores
    TWCR = (1<<TWINT) | (1<<TWEN); // Habilita a comunicação I2C e limpa a interrupção
    while  (!(TWCR &(1<<TWINT))); // Aguarda até que a comunicação I2C seja concluída
    TWDR |= ((v3 & 0x0F)<<4); // Define os 4 bits inferiores com o dado
    TWCR = (1<<TWINT) | (1<<TWEN); // Habilita a comunicação I2C e limpa a interrupção
    while  (!(TWCR &(1<<TWINT))); // Aguarda até que a comunicação I2C seja concluída
    toggle(); // Alterna o estado do pino Enable da LCD
}

/**
 * @brief Função para inicializar o display LCD.
 */
void lcd_init()
{
    lcd_cmd_hf(0x30); // Sequência para inicializar o display LCD
    lcd_cmd_hf(0x30); // Sequência para inicializar o display LCD
    lcd_cmd_hf(0x20); // Sequência para inicializar o display LCD
    lcd_cmd(0x28); // Seleciona o modo 4 bits e 2 linhas do display LCD
    lcd_cmd(0x0C); // Liga o display e desliga o cursor
    lcd_cmd(0x01); // Limpa o display
    lcd_cmd(0x06); // Configura o cursor para incrementar automaticamente
    lcd_cmd(0x80); // Define o cursor na primeira linha do display
}

/**
 * @brief Função de atraso em milissegundos.
 * @param ms O tempo de atraso em milissegundos.
 */
void delay(int ms)
{
    int i,j;
    for(i=0;i<=ms;i++)
        for(j=0;j<=120;j++);
}

/**
 * @brief Função para escrever uma mensagem no display LCD.
 * @param c A mensagem a ser escrita.
 */
void lcd_msg(char *c)
{
    while(*c != 0) // Aguarda até que todas as strings sejam passadas para o display LCD
        lcd_dwr(*c++); // Envia a string para o display LCD
}

/**
 * @brief Função para realizar um deslocamento para a direita no display LCD.
 */
void lcd_rig_sh()
{
    lcd_cmd(0x1C); // Comando para deslocar o conteúdo do display para a direita
    delay(400); // Aguarda 400ms
}

/**
 * @brief Função para realizar um deslocamento para a esquerda no display LCD.
 */
void lcd_lef_sh()
{
    lcd_cmd(0x18); // Comando para deslocar o conteúdo do display para a esquerda
    delay(200); // Aguarda 200ms
}

/**
 * @brief Função para inicializar a comunicação I2C.
 */
void i2c_init(){
    TWBR = 0x62; // Configura a taxa de transmissão do I2C
    TWCR = (1<<TWEN); // Habilita a comunicação I2C
    TWSR = 0x00; // Configura o prescaler para 1
}

/**
 * @brief Função para iniciar a comunicação I2C.
 */
void i2c_start(){
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA); // Condição de início da comunicação I2C
    while (!(TWCR & (1<<TWINT))); // Verifica se a condição de início foi realizada
}

/**
 * @brief Função para escrever um dado no barramento I2C.
 * @param x O dado a ser escrito.
 */
void i2c_write(char x){
    TWDR = x; // Move o valor para o I2C
    TWCR = (1<<TWINT) | (1<<TWEN); // Habilita a comunicação I2C e limpa a interrupção
    while  (!(TWCR &(1<<TWINT))); // Aguarda até que a comunicação I2C seja concluída
}

/**
 * @brief Função para ler um dado do barramento I2C.
 * @return O dado lido.
 */
char i2c_read(){
    TWCR  = (1<<TWEN) | (1<<TWINT); // Habilita a comunicação I2C e limpa a interrupção
    while (!(TWCR & (1<<TWINT))); // Verifica se a leitura foi concluída com sucesso
    return TWDR; // Retorna o dado lido
}
