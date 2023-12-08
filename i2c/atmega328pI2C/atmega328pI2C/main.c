/*
 * atmega328pI2C.c
 *
 * Created: 13/12/2022 17:21:27
 * Author : isaia
 */ 

#include <avr/io.h>
#include "i2c.h"
#include "LCD_I2c.h"
#include <util/delay.h>

int main (void)
{
	i2c_init();
	i2c_start();
	i2c_write(0x70);
	lcd_init();

	while (1)
	{
		lcd_cmd(0x80); //faz referência a linha 01 do lcd e também faz recuo de linha.
		lcd_msg("UFPA:T Mic Mic");//onde será exibida a mensagem de cada linha do lcd
		lcd_cmd(0xc0);// faz referência a linha 02 do lcd
		lcd_msg("Atmega328p:Pi2c");//onde será exibida a mensagem cada linha do lcd

	}
}


