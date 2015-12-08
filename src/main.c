//#include "matrix_code.c"
#include "lcd_code.c"
//#include "stepperMotor_code.c"
/*
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "C:\Users\Tristan\Documents\Atmel Studio\6.2\120B\includes\bit.h"
#include "C:\Users\Tristan\Documents\Atmel Studio\6.2\120B\includes\io.c"
//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "scoreCounter.h"
#include "A2Dfunctions.h"
#include "sonar.h"
#include "sonar.c"

int main(){
	DDRA = 0xF0; PORTA = 0x0F;
	DDRB = 0xFF; PORTB = 0x00;
	
	DDRC = 0xFF; PORTC = 0x00;
	//DDRD = 0x01; PORTD = 0xFE;
	
    unsigned short distance_in_cm=0;
    // initialize display, cursor off
    LCD_init();
    LCD_ClearScreen();
	PORTB = 0x00;
    while(1)
    {

		//PORTB = 0x00;
        distance_in_cm=read_sonar();
		
        if (distance_in_cm == TRIG_ERROR)
        {
            LCD_DisplayString(1,"ERROR!!!");
            _delay_ms(5000);
            LCD_ClearScreen();
            _delay_ms(DELAY_BETWEEN_TESTS/2);
			//PORTB = 0xFF;
        }
        else if (distance_in_cm == ECHO_ERROR)
        {
            LCD_DisplayString(1,"CLEAR!!");
            _delay_ms(5000);
            LCD_ClearScreen();
			//PORTB = 0x00;
        }
        else
        {
			unsigned char theread[20];
			itoa(distance_in_cm, theread, 10);
			LCD_DisplayString(1,theread);
            _delay_ms(DELAY_BETWEEN_TESTS);
            LCD_ClearScreen();
        }
    }
    return 0;
}

void USD_Tick(){
	int sonarDist = 0;
	unsigned char boardwidth = 200;
	switch(usd_state){
		case waitmotion:
			sonarDist = read_sonar();
			//sonarDist = sonarDist - (sonarDist % 10);
			unsigned char test[10];
			itoa(sonarDist, test, 10);
			if(sonarDist != TRIG_ERROR && sonarDist != ECHO_ERROR ){
				LCD_DisplayString(1,test);
			}
			break;
		case motiondetected:
			break;
		default:
			usd_state = waitmotion;
			break;
	}
	switch(usd_state){
		default:
		break;
	}
}*/
