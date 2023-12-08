/*
 * contadorwatdog.c
 *
 * Created: 07/07/2022 20:14:05
 * Author : jefferson elias
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "i2c.h"
#include "LCD_I2c.h"

int main(void)
{
	i2c_init();
	i2c_start();
	i2c_write(0x70);
	lcd_init();

	cli();
	MCUSR &= ~(1 << WDRF);
	WDTCSR = 0x00;
	WDTCSR = (1<<WDCE)|(1<<WDE);
	WDTCSR = (1<<WDIE)|(1<<WDE)|(1<<WDP3);
	sei();

	DDRB &=~ 0b11111110;
	int contdr = 0;

	while(1)
	{
		PORTB = 0x01;
		PORTB = (1<<PORTC0);

		char buffer[3];
		itoa(contdr, buffer, 10); // Convertendo o contador para string

		lcd_cmd(0x80);
		lcd_msg(buffer);

		_delay_ms(100);
		if ((PINB & (1<<PORTB0))==1){
			contdr++;
			WDTCSR |= (1<<WDIE);
			_delay_ms(100);
		}
	}
}