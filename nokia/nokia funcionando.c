//=================================================================
/* Project useing Nokia 3530 LCD            */
/* 2nd Dec 2005                             */
/* Copyright 2005 Circuits4You.com          */ 
/* WWW - http://www.circuits4you.com        */
/* Email - info@circuits4you.com            */
//=================================================================

#include <avr/io.h>
#include <stdlib.h>
#include "Graphics.h"
/*
LCD pinout:

1. LED
2. SCLK
3. SDATA(MOSI)
4. NC (stands for Not Connected?)
5. REST
6. CS
7. GND
8. VCC

Connections:

RB3 --- 10k resistor   --- SCLK
RB4 --- 10k resistor   --- SDATA
RB6 --- 10k resistor   --- REST
RB7 --- 10k resistor   --- CS
5V  --- 3.3V Regulator --- LED, VCC
VCC --- 1nF capacitor  --- GND (prevent flickering)

Nokia 3510i LCD is desinged to work with 3.3V, but due to an internal voltage clamp 5V can be used for SCLK, SDATA, REST and CS
as long as 3.3V is applied to VCC and LED properly. A voltage divider can't be used to convert between 5V and 3.3V due to the LCD
internal resistance and current consumption.

With the above connections we can only write to the LCD but can't read back the LCD response (3.3v is not high enough to register 
as logic '1' in the PIC). Reading is required when we need to get the LCD version number, etc. In particular, function LCD_Initialize()
calls function readdata() to get LCD type (defined as 'ver', can be 'A', 'B', 'C' 'D'). Without reading of data, LCD type has to be hard-coded.
In this case, LCD type is 'B'. If the type is incorrect, color may not be displayed properly.
*/
void delay(int k);

#define SW1 PD2
#define SW2 PD3
#define SW3 PD6
#define SW4 PD7

unsigned char x1=0,y1=0,px=0,py=0;
unsigned char wave1[96],wave2[96],wave;
char Triggered=0,vmul=1,Tmul=1;
unsigned char mode=0,px,pj,j,px2,pj2;
unsigned char mystr[5];

void DSO(char ds);
void Animation();
//=================================================================
//        Main Function
//=================================================================
int main(void)
{
  unsigned long i;
  DDRD=0x00;
  //DDRB = 0x3E;
  DDRB |= 0b00101110;
  PORTD=0b11001100;  //Pull up switches


  SPSR = (1<<SPI2X);//(1<<CPOL)|
  SPCR = (1<<MSTR)|(1<<CPOL)|(1<<CPHA);
  SPSR = (1<<SPI2X);


  send(0x0A, 0); 
  delay(1);

  LCD_Initialize();    //LCD Initialization  
  LCD_ColorSet(0);   //0: 256 color mode,1: 4096 color moede
  LCD_Clear(255,0);
  
  TCCR1B=0x07;
  mode=1;
  while(1)
  {
  if((PIND & 0x84)==0)
  {
   if(mode<4)
   {mode++;}else{mode=0;} 

   LCD_Clear(255,0);
   sprintf(mystr,"Mode:%01d",mode);
   DisplayText(5,50,mystr,0x30,0);  
   while((PIND & 0x84)==0);
   
   delay(1000);
  }

  if(mode==0)  //Mode=0 Fequency Meter
  {  
    TCNT1=0;
   delay(1000);  //Working Frequency meter
    i=TCNT1;
   i=i*2;
   sprintf(mystr,"%05ldHz",i);
   DisplayText(5,10,"Frequency:",0x0C,0);
   DisplayText(5,12,mystr,0x0F,1);
  }

  if(mode==1)  //Mode=1 Voltmeter
  {
   ADMUX=0xC0;
   ADCSRA=0x87;
   // Starts ADC conversion 
      ADCSRA |= (1 << ADSC); 
   while (!(ADCSRA & (1<<ADIF)));
   //i=ADCH;
   //i=i<<8;
   //i=i | ADCL;
   i=((ADCL) | ((ADCH)<<8));
   ADCSRA |=(1<<4); //ADCSRA = ADCSRA OR (0b00010000)

   i=i*25;
   DisplayText(5,1,"Voltage:",0x0C,0);
   sprintf(mystr,"%05ldV",i);
   //DisplayText(5,15,mystr,0x0F,1);  
   DisplayCharLarge(5,6,mystr[0],0x1E);  
   DisplayCharLarge(11,6,mystr[1],0x1E);
   DisplayCharLarge(17,6,'.',0x1E);  
   DisplayCharLarge(22,6,mystr[2],0x1E);
   DisplayCharLarge(28,6,mystr[3],0x1E);
   DisplayCharLarge(34,6,mystr[4],0x1E);
   DisplayCharLarge(40,6,mystr[5],0x1F);

   //=============Channel 2 voltage
   ADMUX=0xC1;
   ADCSRA=0x87;
   // Starts ADC conversion 
      ADCSRA |= (1 << ADSC); 
   while (!(ADCSRA & (1<<ADIF)));
   //i=ADCH;
   //i=i<<8;
   //i=i | ADCL;
   i=((ADCL) | ((ADCH)<<8));
   ADCSRA |=(1<<4); //ADCSRA = ADCSRA OR (0b00010000)

   i=i*25;
   sprintf(mystr,"%05ldV",i);
   DisplayCharLarge(5,15,mystr[0],0x1E);  
   DisplayCharLarge(11,15,mystr[1],0x1E);
   DisplayCharLarge(17,15,'.',0x1E);  
   DisplayCharLarge(22,15,mystr[2],0x1E);
   DisplayCharLarge(28,15,mystr[3],0x1E);
   DisplayCharLarge(34,15,mystr[4],0x1E);
   DisplayCharLarge(40,15,mystr[5],0x1F);
  }
  
  if(mode==2)  //Mode=2 Dual Channel Oscilloscope
  {   
   DSO(2); 
  }
  if(mode==3)  //Mode=3 Single Channel Oscilloscope
  {
   DSO(1); 
  }
  if(mode==4)
  {
   Animation();
  }
  }
}
//=================================================================
//        DSO Function
//=================================================================
void DSO(char ds)
{
   ADMUX=0xE0;
   ADCSRA=0xC7;
   while (!(ADCSRA & (1<<ADIF)));
   wave=ADCH;
   ADCSRA |= 1<<4; //ADCSRA = ADCSRA OR (0b00010000)
   //Voltage Scaling======================================
   if((PIND & 0x04)==0x00) //Vup
   {
    while((PIND & 0x04)==0x00);
    if(vmul<5)
    vmul++;
   }
   if((PIND & 0x08)==0x00) //Vdown
   {
    while((PIND & 0x08)==0x00);
    if(vmul>1)
    vmul--;
   }
   //Time Scaling======================================
   if((PIND & 0x80)==0x00) //Tup
   {
    while((PIND & 0x80)==0x00);
    if(Tmul<7)
    Tmul++;
   }
   if((PIND & 0x40)==0x00) //Tdown
   {
    while((PIND & 0x40)==0x00);
    if(Tmul>0)
    Tmul--;
   }

   if(wave>0x0F)
   {
    //Start Recording on trigger of 0x0A
    Triggered=1;
   }
   if(Triggered==1)
   {
    for(j=0;j<96;j++)//Record the data
    { 
     ADMUX=0xE0;
     ADCSRA=0xC0 | Tmul;
     while (!(ADCSRA & (1<<ADIF)));
     wave=ADCH>>2;
     ADCSRA |= 1<<4; //ADCSRA = ADCSRA OR (0b00010000)

     wave1[j]=wave*vmul;
     if(ds==2)
     {
      ADMUX=0xE1;
      ADCSRA=0xC0 | Tmul;
      while (!(ADCSRA & (1<<ADIF)));
      wave=ADCH>>2;
      ADCSRA |= 1<<4; //ADCSRA = ADCSRA OR (0b00010000)

      wave2[j]=wave*vmul;
     }
    }

    //Plot the data
    LCD_Clear(255,0);
    
    pj=0;
    px=0;
    pj2=0;
    px2=0;
    for(j=0;j<96;j++)
    {     
     line(j,wave1[j],px,pj,0xE3);
     //PutPixel(j,wave1[j],0x0F);
     pj=wave1[j];
     px=j;

     if(ds==2)
     {
      line(j,wave2[j],px2,pj2,0x3F);
      //PutPixel(j,wave1[j],0x0F);
      pj2=wave2[j];
      px2=j;
     }
    }
    
//    sprintf(mystr,"VDIV:%01d",vmul);
//    DisplayText(5,55,mystr,0x30,0); 
    Triggered=0;
   }
}
//=================================================================
//        Matrix movie like screen saver Function
//=================================================================
void Animation()
{
  ///(RAND_MAX/30);
  
  for(y1=0;y1<66;y1=y1+6)
  {   
   x1=rand()/6;
   DisplayChar(x1,y1,x1+48,x1);
  }
  
  //sprintf(mystr,"%06d",x);
  //DisplayText(1,1,mystr,0xF0);
  //delay(1000);
}
//=================================================================
//        Delay Function
//=================================================================
void delay(int k)
{
 int i,j;
 for(i=0;i<k;i++)
 {
  for(j=0;j<500;j++)
  {
    asm("nop");
  }
 }

}