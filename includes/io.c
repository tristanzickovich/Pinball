#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "io.h"

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))
          
/*-------------------------------------------------------------------------*/

#define DATA_BUS PORTC		// port connected to pins 7-14 of LCD display
#define CONTROL_BUS PORTA	// port connected to pins 4 and 6 of LCD disp.
#define RS 6			// pin number of uC connected to pin 4 of LCD disp.
#define E 7			// pin number of uC connected to pin 6 of LCD disp.

/*-------------------------------------------------------------------------*/

void LCD_ClearScreen(void) {
   LCD_WriteCommand(0x01);
}

void LCD_init(void) {

    //wait for 100 ms.
	delay_ms(100);
	LCD_WriteCommand(0x38);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x0f);
	LCD_WriteCommand(0x01);
	delay_ms(10);						 
}

void LCD_WriteCommand (unsigned char Command) {
   CLR_BIT(CONTROL_BUS,RS);
   DATA_BUS = Command;
   SET_BIT(CONTROL_BUS,E);
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
   delay_ms(2); // ClearScreen requires 1.52ms to execute
}

void LCD_WriteData(unsigned char Data) {
   SET_BIT(CONTROL_BUS,RS);
   DATA_BUS = Data;
   SET_BIT(CONTROL_BUS,E);
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
   delay_ms(1);
}

void LCD_DisplayString( unsigned char column, const unsigned char* string) {
   LCD_ClearScreen();
   unsigned char c = column;
   while(*string) {
      LCD_Cursor(c++);
      LCD_WriteData(*string++);
   }
}

void LCD_Cursor(unsigned char column) {
   if ( column < 17 ) { // 16x1 LCD: column < 9
						// 16x2 LCD: column < 17
      LCD_WriteCommand(0x80 + column - 1);
   } else {
      LCD_WriteCommand(0xB8 + column - 9);	// 16x1 LCD: column - 1
											// 16x2 LCD: column - 9
   }
}

void delay_ms(int miliSec) //for 8 Mhz crystal

{
    int i,j;
    for(i=0;i<miliSec;i++)
    for(j=0;j<775;j++)
  {
   asm("nop");
  }
}

#define KEYPADPORT PORTD
#define KEYPADPIN  PIND
unsigned char GetKeypadKey() {
	// Check keys in col 1
	// Enable col 4 with 0, disable others with 1’s
	// The delay allows PORTC to stabilize before checking
	KEYPADPORT = 0xEF;
	asm("nop");
	if (GetBit(KEYPADPIN,0)==0) { return('1'); }
	if (GetBit(KEYPADPIN,1)==0) { return('2'); }
	if (GetBit(KEYPADPIN,2)==0) { return('3'); }
	if (GetBit(KEYPADPIN,3)==0) { return('A'); }
	// Check keys in col 2
	// Enable col 5 with 0, disable others with 1’s
	// The delay allows PORTC to stabilize before checking
	KEYPADPORT = 0xDF;
	asm("nop");
	if (GetBit(KEYPADPIN,0)==0) { return('4'); }
	if (GetBit(KEYPADPIN,1)==0) { return('5'); }
	if (GetBit(KEYPADPIN,2)==0) { return('6'); }
	if (GetBit(KEYPADPIN,3)==0) { return('B'); }
	// ... *****FINISH*****
	// Check keys in col 3
	// Enable col 6 with 0, disable others with 1’s
	// The delay allows PORTC to stabilize before checking
	KEYPADPORT = 0xBF;
	asm("nop");
	if (GetBit(KEYPADPIN,0)==0) { return('7'); }
	if (GetBit(KEYPADPIN,1)==0) { return('8'); }
	if (GetBit(KEYPADPIN,2)==0) { return('9'); }
	if (GetBit(KEYPADPIN,3)==0) { return('C'); }
	// ... *****FINISH*****
	// Check keys in col 4
	// ... *****FINISH*****
	KEYPADPORT = 0x7F;
	asm("nop");
	if (GetBit(KEYPADPIN,0)==0) { return('*'); }
	if (GetBit(KEYPADPIN,1)==0) { return('0'); }
	if (GetBit(KEYPADPIN,2)==0) { return('#'); }
	if (GetBit(KEYPADPIN,3)==0) { return('D'); }
	return('\0'); // default value
}