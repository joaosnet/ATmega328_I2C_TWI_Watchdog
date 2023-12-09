//=================================================================
/* Nokia 3530 GLCD Graphics Library         */
/* 2nd Dec 2005                             */
/* Copyright 2005 Circuits4You.com          */ 
/* WWW - http://www.circuits4you.com        */
/* Email - info@circuits4you.com            */
//=================================================================

#include <avr/io.h>
#include <string.h>
#include <avr/pgmspace.h> 

// LCD Pin connections
#define sclk PB5
#define sdata PB3
#define rest PB1
#define cs PB2

void delayms(unsigned  int ii);
void send(unsigned char value, unsigned char cd);
void LCD_Initialize();
void LCD_ColorSet(unsigned char Color);
void addset(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);
void LCD_Clear(unsigned int value,unsigned char Color);
void LCD_Test(void);
unsigned char Ver;  



void Delay1();
void PutPixel(unsigned char x,unsigned char y,char color);
void line(char x0, char y0, char x1, char y1,char color);
void rect(char x0, char y0, char x1, char y1,char color);
//void circle(int xCenter, int yCenter, int radius, char c);
void DisplayChar(unsigned char x,unsigned char y, char d,unsigned char color);
void DisplayCharLarge(unsigned char x,unsigned char y, char d,unsigned char color);
void DisplayText(unsigned char x, unsigned char y, char d[21],unsigned char color,unsigned char size);
void DisplayCharSmall(unsigned char x,unsigned char y,unsigned char d);

/*--------------------------------------------------------------------------------------------------
                                     Character generator

             This table defines the standard ASCII characters in a 5x7 dot format.
--------------------------------------------------------------------------------------------------*/
PROGMEM static const char FontLookup [][5] =
{
 {0x00, 0x00, 0x00, 0x00, 0x00},// (space) 
 {0x00, 0x00, 0x5F, 0x00, 0x00},// ! 
 {0x00, 0x07, 0x00, 0x07, 0x00},// " 
 {0x14, 0x7F, 0x14, 0x7F, 0x14},// # 
 {0x24, 0x2A, 0x7F, 0x2A, 0x12},// $ 
 {0x23, 0x13, 0x08, 0x64, 0x62},// % 
 {0x36, 0x49, 0x55, 0x22, 0x50},// & 
 {0x00, 0x05, 0x03, 0x00, 0x00},// ' 
 {0x00, 0x1C, 0x22, 0x41, 0x00},// ( 
 {0x00, 0x41, 0x22, 0x1C, 0x00},// ) 
 {0x08, 0x2A, 0x1C, 0x2A, 0x08},// * 
 {0x08, 0x08, 0x3E, 0x08, 0x08},// + 
 {0x00, 0x50, 0x30, 0x00, 0x00},// , 
 {0x08, 0x08, 0x08, 0x08, 0x08},// - 
 {0x00, 0x30, 0x30, 0x00, 0x00},// . 
 {0x20, 0x10, 0x08, 0x04, 0x02},// / 
 {0x3E, 0x51, 0x49, 0x45, 0x3E},// 0 
 {0x00, 0x42, 0x7F, 0x40, 0x00},// 1 
 {0x42, 0x61, 0x51, 0x49, 0x46},// 2 
 {0x21, 0x41, 0x45, 0x4B, 0x31},// 3 
 {0x18, 0x14, 0x12, 0x7F, 0x10},// 4 
 {0x27, 0x45, 0x45, 0x45, 0x39},// 5 
 {0x3C, 0x4A, 0x49, 0x49, 0x30},// 6 
 {0x01, 0x71, 0x09, 0x05, 0x03},// 7 
 {0x36, 0x49, 0x49, 0x49, 0x36},// 8 
 {0x06, 0x49, 0x49, 0x29, 0x1E},// 9 
 {0x00, 0x36, 0x36, 0x00, 0x00},// : 
 {0x00, 0x56, 0x36, 0x00, 0x00},// ; 
 {0x00, 0x08, 0x14, 0x22, 0x41},// < 
 {0x14, 0x14, 0x14, 0x14, 0x14},// = 
 {0x41, 0x22, 0x14, 0x08, 0x00},// > 
 {0x02, 0x01, 0x51, 0x09, 0x06},// ? 
 {0x32, 0x49, 0x79, 0x41, 0x3E},// @ 
 {0x7E, 0x11, 0x11, 0x11, 0x7E},// A 
 {0x7F, 0x49, 0x49, 0x49, 0x36},// B 
 {0x3E, 0x41, 0x41, 0x41, 0x22},// C 
 {0x7F, 0x41, 0x41, 0x22, 0x1C},// D 
 {0x7F, 0x49, 0x49, 0x49, 0x41},// E 
 {0x7F, 0x09, 0x09, 0x01, 0x01},// F 
 {0x3E, 0x41, 0x41, 0x51, 0x32},// G 
 {0x7F, 0x08, 0x08, 0x08, 0x7F},// H 
 {0x00, 0x41, 0x7F, 0x41, 0x00},// I 
 {0x20, 0x40, 0x41, 0x3F, 0x01},// J 
 {0x7F, 0x08, 0x14, 0x22, 0x41},// K 
 {0x7F, 0x40, 0x40, 0x40, 0x40},// L 
 {0x7F, 0x02, 0x04, 0x02, 0x7F},// M 
 {0x7F, 0x04, 0x08, 0x10, 0x7F},// N 
 {0x3E, 0x41, 0x41, 0x41, 0x3E},// O 
 {0x7F, 0x09, 0x09, 0x09, 0x06},// P 
 {0x3E, 0x41, 0x51, 0x21, 0x5E},// Q 
 {0x7F, 0x09, 0x19, 0x29, 0x46},// R 
 {0x46, 0x49, 0x49, 0x49, 0x31},// S 
 {0x01, 0x01, 0x7F, 0x01, 0x01},// T 
 {0x3F, 0x40, 0x40, 0x40, 0x3F},// U 
 {0x1F, 0x20, 0x40, 0x20, 0x1F},// V 
 {0x7F, 0x20, 0x18, 0x20, 0x7F},// W 
 {0x63, 0x14, 0x08, 0x14, 0x63},// X 
 {0x03, 0x04, 0x78, 0x04, 0x03},// Y 
 {0x61, 0x51, 0x49, 0x45, 0x43},// Z 
 {0x00, 0x00, 0x7F, 0x41, 0x41},// [ 
 {0x02, 0x04, 0x08, 0x10, 0x20},// "\" 
 {0x41, 0x41, 0x7F, 0x00, 0x00},// ] 
 {0x04, 0x02, 0x01, 0x02, 0x04},// ^ 
 {0x40, 0x40, 0x40, 0x40, 0x40},// _ 
 {0x00, 0x01, 0x02, 0x04, 0x00},// ` 
 {0x20, 0x54, 0x54, 0x54, 0x78},// a 
 {0x7F, 0x48, 0x44, 0x44, 0x38},// b 
 {0x38, 0x44, 0x44, 0x44, 0x20},// c 
 {0x38, 0x44, 0x44, 0x48, 0x7F},// d 
 {0x38, 0x54, 0x54, 0x54, 0x18},// e 
 {0x08, 0x7E, 0x09, 0x01, 0x02},// f 
 {0x08, 0x14, 0x54, 0x54, 0x3C},// g 
 {0x7F, 0x08, 0x04, 0x04, 0x78},// h 
 {0x00, 0x44, 0x7D, 0x40, 0x00},// i 
 {0x20, 0x40, 0x44, 0x3D, 0x00},// j 
 {0x00, 0x7F, 0x10, 0x28, 0x44},// k 
 {0x00, 0x41, 0x7F, 0x40, 0x00},// l 
 {0x7C, 0x04, 0x18, 0x04, 0x78},// m 
 {0x7C, 0x08, 0x04, 0x04, 0x78},// n 
 {0x38, 0x44, 0x44, 0x44, 0x38},// o 
 {0x7C, 0x14, 0x14, 0x14, 0x08},// p 
 {0x08, 0x14, 0x14, 0x18, 0x7C},// q 
 {0x7C, 0x08, 0x04, 0x04, 0x08},// r 
 {0x48, 0x54, 0x54, 0x54, 0x20},// s 
 {0x04, 0x3F, 0x44, 0x40, 0x20},// t 
 {0x3C, 0x40, 0x40, 0x20, 0x7C},// u 
 {0x1C, 0x20, 0x40, 0x20, 0x1C},// v 
 {0x3C, 0x40, 0x30, 0x40, 0x3C},// w 
 {0x44, 0x28, 0x10, 0x28, 0x44},// x 
 {0x0C, 0x50, 0x50, 0x50, 0x3C},// y 
 {0x44, 0x64, 0x54, 0x4C, 0x44},// z 
 {0x00, 0x08, 0x36, 0x41, 0x00},// { 
 {0x00, 0x00, 0x7F, 0x00, 0x00},// | 
 {0x00, 0x41, 0x36, 0x08, 0x00},// } 
 {0x08, 0x08, 0x2A, 0x1C, 0x08},// -> 
 {0x08, 0x1C, 0x2A, 0x08, 0x08} // <- 
};

//Hex And Decimal Numbers in 3 X 5 Format 
PROGMEM static const char FontLookup1 [][3] =
{
    { 0xF8, 0x88, 0xF8 },   // 0
    { 0x48, 0xF8, 0x08 },   // 1
    { 0xB8, 0xA8, 0xE8 },   // 2
    { 0xA8, 0xA8, 0xF8 },   // 3
    { 0xE0, 0x20, 0xF8 },   // 4
    { 0xE8, 0xA8, 0xB8 },   // 5
    { 0xF8, 0xA8, 0xB8 },   // 6
    { 0x80, 0x80, 0xF8 },   // 7
    { 0xF8, 0xA8, 0xF8 },   // 8
    { 0xE8, 0xA8, 0xF8 },   // 9
 { 0xF8, 0xA0, 0xF8 },   // A
    { 0xF8, 0xA8, 0x50 },   // B
    { 0xF8, 0x88, 0x88 },   // C
    { 0x88, 0xF8, 0xF8 },   // D
    { 0xF8, 0xA8, 0xA8 },   // E
    { 0xF8, 0xA0, 0xA0 },   // F
 { 0x00, 0x00, 0x00 },   // Space
};

/*********************************************************************************/
/*         MAIN PROGRAM                                 */
/*********************************************************************************/

/*********************************************************************************/
/*         PUT PIXEL(X,Y,color)                         */
/*********************************************************************************/
void PutPixel(unsigned char x,unsigned char y, char color)
{
 addset(x,y,x,y);
 send(color, 1);
}

/*********************************************************************************/
/*         LINE(X0,Y0,X1,Y1,color)                      */
/*********************************************************************************/

void line(char x0, char y0, char x1, char y1,char color) 
{
 int dy = y1 - y0;
 int dx = x1 - x0;
 int stepx, stepy;
 if (dy < 0) { dy = -dy; stepy = -1; 
 } else { stepy = 1; 
 }
  if (dx < 0) { dx = -dx; stepx = -1; 
 } else { stepx = 1; 
 }
 dy <<= 1;        // dy is now 2*dy
 dx <<= 1;        // dx is now 2*dx
 
 PutPixel(x0, y0,color);

 if (dx > dy) {
  int fraction = dy - (dx >> 1); // same as 2*dy - dx
  while (x0 != x1) {
   if (fraction >= 0) {
    y0 += stepy;
    fraction -= dx;   // same as fraction -= 2*dx
   }
     x0 += stepx;
     fraction += dy;     // same as fraction -= 2*dy
     PutPixel(x0, y0,color);
  }
 } else {
  int fraction = dx - (dy >> 1);
  while (y0 != y1) {
   if (fraction >= 0) {
    x0 += stepx;
    fraction -= dy;
   }
  y0 += stepy;
  fraction += dx;
  PutPixel(x0, y0,color);
  }
 }
} 
/*********************************************************************************/
/*         RECT(X0,Y0,X1,Y1,color)                      */
/*********************************************************************************/
void rect(char x0, char y0, char x1, char y1,char color)
{
 line(x0,y0,x1,y0,color);
 line(x0,y0,x0,y1,color);
 line(x0,y1,x1,y1,color);
 line(x1,y0,x1,y1,color); 
}

/*********************************************************************************/
/*         Circle(X0,Y0,radius,color)                   */
/*********************************************************************************/
/*
void circle(int xCenter, int yCenter, int radius, char c)
    {
        int x, y, r2;

        r2 = radius * radius;
        PutPixel(xCenter, yCenter + radius,c);
        PutPixel(xCenter, yCenter - radius,c);
        PutPixel(xCenter + radius, yCenter,c);
        PutPixel(xCenter - radius, yCenter,c);
        x = 1;
        y = (int) (sqrt(r2 - 1) + 0.5);
        while (x < y) {
            PutPixel(xCenter + x, yCenter + y,c);
            PutPixel(xCenter + x, yCenter - y,c);
            PutPixel(xCenter - x, yCenter + y,c);
            PutPixel(xCenter - x, yCenter - y,c);
            PutPixel(xCenter + y, yCenter + x,c);
            PutPixel(xCenter + y, yCenter - x,c);
            PutPixel(xCenter - y, yCenter + x,c);
            PutPixel(xCenter - y, yCenter - x,c);
            x += 1;
            y = (int) (sqrt(r2 - x*x) + 0.5);
        }
        if (x == y) {
            PutPixel(xCenter + x, yCenter + y,c);
            PutPixel(xCenter + x, yCenter - y,c);
            PutPixel(xCenter - x, yCenter + y,c);
            PutPixel(xCenter - x, yCenter - y,c);
        }
    }

 */
/*********************************************************************************/
/*         Display Text (5x7)                       */
/*********************************************************************************/
void DisplayChar(unsigned char x,unsigned char y, char d,unsigned char color)
{
 unsigned char r,c,c1,k;
 d=d-32;
 //addset(x,y,x+5,y+7);
 for(r=0;r<6;r++)
 {
  c1=0;   //4
  for(c=0;c<8;c++)
  {
   k=((0x01) & (pgm_read_byte(&FontLookup[d][r]) >> c ));
   //k &= 0x01;
   //PutPixel(x+r,y+c1,k*color);//>>(c+3)
   c1=c1+1;
  
   if(r==5)
   {k=0;}
   if(k==0)
   PutPixel(x+r,y+c1,0xFF);//send(0xFF,1);
   else
   PutPixel(x+r,y+c1,color);//send(color,1);
  }
 }
}
/*********************************************************************************/
/*         Display Text (5x7)                       */
/*********************************************************************************/
void DisplayCharLarge(unsigned char x,unsigned char y, char d,unsigned char color)
{
 unsigned char r,c,c1,k;
 d=d-32;
 //addset(x,y,x+5,y+7);
 for(r=0;r<5;r++)
 {
  c1=0;   //4
  for(c=0;c<8;c++)
  {
   k=((0x01) & (pgm_read_byte(&FontLookup[d][r]) >> c ));
   //k &= 0x01;
   //PutPixel(x+r,y+c1,k*color);//>>(c+3)
   c1=c1+1;
   if(k==0)
   {
    PutPixel((x+r)*2,(y+c1)*2,0xFF);//send(0xFF,1);
    PutPixel(((x+r)*2)+1,(y+c1)*2,0xFF);
    PutPixel((x+r)*2,((y+c1)*2)+1,0xFF);
    PutPixel(((x+r)*2)+1,((y+c1)*2)+1,0xFF);
   }
   else
   {
    PutPixel((x+r)*2,(y+c1)*2,color);//send(0xFF,1);
    PutPixel(((x+r)*2)+1,(y+c1)*2,color);
    PutPixel((x+r)*2,((y+c1)*2)+1,color);
    PutPixel(((x+r)*2)+1,((y+c1)*2)+1,color);
   }
  }
 }
}
/*********************************************************************************/
/*         Display Text (3x5)                       */
/*********************************************************************************/
void DisplayCharSmall(unsigned char x,unsigned char y,unsigned char d)
{
unsigned char r,c,c1;
 for(r=0;r<3;r++)
 {
  c1=4;  
  for(c=0;c<5;c++)
  {
   PutPixel(x+r,y+c1,((0x01) & (pgm_read_byte(&FontLookup1[d][r]) >> (c+3))));
   c1=c1-1;
  }
 }
}

/*********************************************************************************/
/*         Display Text (5x7)                       */
/*********************************************************************************/
void DisplayText(unsigned char x, unsigned char y, char d[21],unsigned char color,unsigned char size)
{
int i;
int j;
j=strlen(d);
 for (i=0;i<j;i++)
 {
    if(size==0)
  DisplayChar((i*6)+x,y,d[i],color);
    else
  DisplayCharLarge((i*6)+x,y,d[i],color);
 } 
}

void delayms(unsigned  int ii)
{
 unsigned int i,x;
 for (x=0;x<ii;x++)
 {
  for (i=0;i<230;i++);
 }
}

/*********************************************************************************/
/*         Send Display                              */
/*********************************************************************************/

// Send a command/data code to the LCD
// cd = 0: command
// cd = 1: data
void send(unsigned char value, unsigned char cd) 
 { 
  unsigned char i,dt; 
 PORTB &=~(1<<cs);
 PORTB &=~(1<<sclk);
   if(cd == 0) 
      PORTB &=~(1<<sdata);
   else 
     PORTB |=(1<<sdata);
     PORTB |=(1<<sclk);    

 SPCR |= (1<<SPE);
 dt=value;
 SPDR = dt;
 /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));
 SPCR &=~(1<<SPE);
/*
 for(i=0;i<8;i++)
 { 
  PORTB &=~(1<<sclk);  
  if(dt & 0x80)
   PORTB |=(1<<sdata);
  else
   PORTB &=~(1<<sdata);
  dt=dt<<1;  
  PORTB |=(1<<sclk); 
 } 
*/
    PORTB |=(1<<cs);
}

/*********************************************************************************/
/*       Initialize the LCD                              */
/*********************************************************************************/
void LCD_Initialize()  
{
 unsigned char i;
 Ver='A';

 DDRB=0xFF;
  PORTB |=(1<<rest);
 PORTB |=(1<<cs);
 PORTB &=~(1<<sdata);
 PORTB |=(1<<sclk);
 PORTB &=~(1<<rest);
 delayms(50);
 PORTB |=(1<<rest);
 delayms(50);
 
   send(0x00, 0);      // NOP
   delayms(5); 

   send(0x01, 0);      // LCD Software Reset 
   delayms(5); 
  
   send(0xC6, 0);      // Initial Escape 
      
   send(0xB9, 0);      // Refresh set 
   send(0x00, 1); 

 //TODO: Unable to read LCD response due to voltage difference. Must hard code LCD type
   //readdata();           
   Ver='B';
       
   send(0xB5, 0);      // Gamma Curve Set 
   send(0x01, 1); 

   send(0xbd, 0);      //common driver output select
   if(Ver=='D')
   {
     send(0x04, 1); 
   }
   else
   {
     send(0x00, 1); 
   }

   send(0xBE, 0);      // Power Control 
   send(0x03, 1); 

    
   send(0x11, 0);      // Sleep out 

   send(0xBA, 0);      // Power Control 
   send(0x7F, 1); 
   send(0x03, 1);
   
   send(0x36, 0);      
   if (Ver=='A')
   {
      send(0x00|0x08,1);     //RGB
    }
  else
 {
     send(0x00,1);     //RGB
 }
   
   send(0xB7, 0);      // Temperature gradient set 
   for(i=0; i<14; i++) 
   { 
      send(0, 1); 
   } 


   send(0x29, 0);      //display ON  
       
   send(0x03, 0);      // Booster Voltage ON 
      
   delayms(20);         // Booster Voltage stabilisieren lassen      
    
   send(0x20, 0);     //display inversion OFF

   send(0x25, 0);      // Write contrast   
   switch(Ver)          
   {
     case 'A':  
   send(70, 1);  
   break;
     case 'B':   
   send(67, 1);  
   break;
  case 'C':
   //send(74, 1); 
      send(66, 1); 
  // send(64, 1);   
   break;
  case 'D':    
   send(49, 1); 
   break;
   
   } 
       
}

/*********************************************************************************/
//Set the color mode to work with the LCD
//color=1: 4096 colors
//color=0: 256 colors
/*********************************************************************************/
void LCD_ColorSet(unsigned char Color)
{

 if (Color==1) {
 send(0x3a, 0);  //interface pixel format
 send(0x03, 1);       //0x03 4096 colors
  
 }
 else
 {
 send(0x3a, 0);  //interface pixel format
 send(0x02, 1);       //0x02 256 colors
  
 send(0x2d, 0);  
 if (Ver=='B'||Ver=='C')
  {
    //red
  send(~0x00, 1);
  send(~0x02, 1);
  send(~0x03, 1);
  send(~0x04, 1);
  send(~0x05, 1);
  send(~0x06, 1);
  send(~0x08, 1);
  send(~0x0f, 1);
  
  //green
  send(~0x00, 1);
  send(~0x02, 1);
  send(~0x03, 1);
  send(~0x04, 1);
  send(~0x05, 1);
  send(~0x06, 1);
  send(~0x08, 1);
  send(~0x0f, 1);
  //blue
  send(~0x00, 1);
  send(~0x03, 1);
  send(~0x06, 1);
  send(~0x0f, 1);
  }else
 {
     //red
  send(0x00, 1);
  send(0x02, 1);
  send(0x03, 1);
  send(0x04, 1);
  send(0x05, 1);
  send(0x06, 1);
  send(0x08, 1);
  send(0x0f, 1);
  
  //green
  send(0x00, 1);
  send(0x02, 1);
  send(0x03, 1);
  send(0x04, 1);
  send(0x05, 1);
  send(0x06, 1);
  send(0x08, 1);
  send(0x0f, 1);
  //blue
  send(0x00, 1);
  send(0x03, 1);
  send(0x06, 1);
  send(0x0f, 1);
 }

    
   }
}
/*********************************************************************************/
// Call this to get access to a particular region on the LCD before writing the pixel value
// top left (x1, y1)
// bottom right (x2, y2)
/*********************************************************************************/
void addset(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)  
{
    send(0x2a,0 );//column address set
    send(x1,1 );
 send(x2,1 );
  send(0x2B,0 );//page address set
    send(y1,1 );
 send(y2,1 );
  send(0x2C,0 );//memory write
}

/*********************************************************************************/
//Fill the entire LCD screen with the given color value
//color=1: 4096 colors
//color=0: 256 colors
/*********************************************************************************/
void LCD_Clear(unsigned int value,unsigned char Color)
{
 unsigned char x, y;
 addset(0,0,97,66);
 if (Color==1) 
 {
  for(y = 0; y < 67; y ++)
  {
   for(x = 0; x < 49;x ++)
   { send(value>>4,1);
    send(((value&0x0f)<<4)|(value>>8),1);
    send(value,1);
   } 
   }
 }
 else
 {
  for(y = 0; y < 67; y ++)
  {
   for(x = 0; x < 98; x ++)
    {
     send(value,1);
    }
  }
 }  
}