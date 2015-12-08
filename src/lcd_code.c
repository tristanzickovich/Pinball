/*
*   Tristan Zickovich   tzick002@ucr.edu
*   Lab Section: 23
*   Assignment: Lab# 10 Final Project : Code for LCD, BlueTooth, Joystick, IR Sensors
*   I acknowledge all content contained herein, excluding template or example
*   code, is my own original work
*/
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
#include "C:\Users\Tristan\Documents\Atmel Studio\6.2\120B\includes\usart_ATmega1284.h"
//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "scoreCounter.h"
#include "A2Dfunctions.h"

enum LCD_States {wait, maxHS, newHS, updatename, lcdgameover, playAgain}lcd_state;
enum newHS_States {upwait, updatename2}newHS_state;
enum addLowScore_States {lowWait, lowAdd}lowScore_state;
enum addMidScore_States {midWait, midAdd}midScore_state;
enum addTopScore_States {topWait, topAdd}topScore_state;
enum loseCredit_States {loseWait, loseSubtract}loseCredit_state;
enum blueTooth_States {btwait, btprocess}blueTooth_state;
void LCD_Init(){
	lcd_state = playAgain;
	newHS_state = upwait;
	lowScore_state = lowWait;
	midScore_state = midWait;
	topScore_state = topWait;
	loseCredit_state = loseWait;
	blueTooth_state = btwait;
}
//IR Actuals: LoseCred (896), Low(864), irMid(896), irTop(896)
//IR Thresholds
unsigned short irLose = 880, irLow = 850, irMid = 880, irTop = 880;
//game variables
unsigned short basevalhigh = 560;
unsigned short basevallow = 500;
unsigned char hsHolderIndex = 0, startNameChange = 0, delayCounter = 4, delayCounter2 = 4;
unsigned char lsdelay = 0, msdelay = 0, tsdelay = 0, lcdelay = 0, godelay = 0;
void LCD_Tick() {
	unsigned char i = 0;
	unsigned short jsRead = 0;
	Set_A2D_Pin(0x00);
	_delay_ms(2);
	jsRead = ADC;
	switch (lcd_state) {
		case playAgain:
			if(delayCounter > 3){
				if(jsRead > basevalhigh){
					lcd_state = maxHS;
					HSscreenFormat();
					delayCounter = 0;
				}
				else if(jsRead < basevallow){
					reconfigureGame();
					lcd_state = wait;
					screenFormat();
					delayCounter = 0;
				}
			}
			break;
		case maxHS:
			if(delayCounter > 2){
				if(jsRead < basevallow){
					waitStartMessage();
					lcd_state = playAgain;
					delayCounter = 0;
				}
			}
			break;
		case wait:
			if(gameover){
				lcd_state = lcdgameover;
				godelay = 0;
				LCD_DisplayString(1,"   Game OVER!     Score: ");
				sendScore(25);
			}
			break;
		case lcdgameover:
			if(godelay > 60){
				if(scoreCompare() > 0){
					LCD_DisplayString(1," New Hi-Score!");
					transferHS();
					delay_ms(4000);
					delayCounter = 0;
					delayCounter2 = 2;
					newHSformat();
					hsHolderIndex = 0;
					startNameChange = 1;
					lcd_state = updatename;
				}
				else{
					waitStartMessage();
					lcd_state = playAgain;
					delayCounter = 0;
				}
				godelay = 0;
			}
			break;
		case updatename:
			if(delayCounter > 3){
				if(jsRead > 1000){
					delayCounter = 0;
					if(hsHolderIndex > 1){
						startNameChange = 0;
						waitStartMessage();
						lcd_state = playAgain;
					}
					else
						++hsHolderIndex;
				}
				else if(jsRead < basevallow){
					delayCounter = 0;
					if(hsHolderIndex > 0)
						--hsHolderIndex;
				}
			}
			break;
		default:
			lcd_state = playAgain;
			break;
	}
	switch (lcd_state) {
		case playAgain:
		case maxHS:
		case updatename:
			if(delayCounter < 100)
				++delayCounter;
			break;
		case lcdgameover:
			++godelay;
			break;
		default:
			break;
	}
}

void newHSname_Tick(){
	unsigned short jsRead = 0;
	Set_A2D_Pin(0x01);
	_delay_ms(1);
	jsRead = ADC;
	switch(newHS_state){
		case upwait:
			if(startNameChange == 1)
				newHS_state = updatename2;
			break;
		case updatename2:
			if(startNameChange == 0)
				newHS_state = upwait;
			else if(delayCounter2 > 1){	
				if(jsRead > basevalhigh){
					if((HSholder[hsHolderIndex] + 1) <= 'Z')
						HSholder[hsHolderIndex] += 1;
					else
						HSholder[hsHolderIndex] = 'A';
				}
				else if(jsRead < basevallow){
					if((HSholder[hsHolderIndex] - 1) >= 'A')
						HSholder[hsHolderIndex] -= 1;
					else
						HSholder[hsHolderIndex] = 'Z';
				}
				delayCounter2 = 0;
			}
			break;
		default:
			break;
	}
	switch(newHS_state){
		case updatename2:
			LCD_Cursor(23 + hsHolderIndex);
			LCD_WriteData(HSholder[hsHolderIndex]);
			LCD_Cursor(23 + (hsHolderIndex));
			++delayCounter2;
			break;
		default:
			break;
	}
}

void addLowScore_Tick(){
	unsigned short distVal = 0;
	Set_A2D_Pin(0x02);
	_delay_ms(1);
	switch(lowScore_state){
		case lowWait:
			distVal = ADC;
			if(distVal < irLow && !gameover){
				lowScore_state = lowAdd;
				//unsigned char test[20];
				//itoa(distVal, test, 10);
				//LCD_DisplayString(1,test);
				//if(distVal < 110)
				//	delay_ms(3000);
				updateScore(50);
				lsdelay = 0;
			}
			break;
		case lowAdd:
			if(lsdelay > 100)
				lowScore_state = lowWait;
			break;
		default:
			break;
	}
	switch(lowScore_state){
		case lowAdd:
			++lsdelay;
			break;
		default:
		break;
	}
}

void addMidScore_Tick(){
	unsigned short distVal = 0;
	Set_A2D_Pin(0x03);
	_delay_ms(1);
	switch(midScore_state){
		case midWait:
			distVal = ADC;
			if(distVal < irMid && !gameover){
				midScore_state = midAdd;
				updateScore(100);
				msdelay = 0;
			}
			break;
		case midAdd:
			if(msdelay > 100)
				midScore_state = midWait;
				break;
		default:
			break;
	}
	switch(midScore_state){
		case midAdd:
			++msdelay;
			break;
		default:
			break;
	}
}

void addTopScore_Tick(){
	unsigned short distVal = 0;
	Set_A2D_Pin(0x04);
	_delay_ms(1);
	switch(topScore_state){
		case topWait:
			distVal = ADC;
			if(distVal < irTop && !gameover){
				topScore_state = topAdd;
				updateScore(500);
				tsdelay = 0;
			}
			break;
		case topAdd:
			if(tsdelay > 100)
				topScore_state = topWait;
			break;
		default:
			break;
	}
	switch(topScore_state){
		case topAdd:
			++tsdelay;
			break;
		default:
			break;
	}
}

void loseCredit_Tick(){
	unsigned short distVal = 0;
	Set_A2D_Pin(0x05);
	_delay_ms(1);
	switch(loseCredit_state){
		case loseWait:
			distVal = ADC;
			if(distVal < irLose && !gameover){
				loseCredit_state = loseSubtract;
				updateCredits(-1);
				lcdelay = 0;
			}
			break;
		case loseSubtract:
			if(lcdelay > 100)
				loseCredit_state = loseWait;
			break;
		default:
			break;
	}
	switch(loseCredit_state){
		case loseSubtract:
			++lcdelay;
			break;
		default:
			break;
	}
}

void blueTooth_Tick(){
	static unsigned char btdata = 0;
	switch(blueTooth_state){
		case btwait:
			if(!gameover){
				USART_Flush(0);
				blueTooth_state = btprocess;
			}
			break;
		case btprocess:
			if(gameover)
				blueTooth_state = btwait;
			else if(USART_HasReceived(0)){
				btdata = USART_Receive(0);
				USART_Flush(0);
				//if(btdata == 0xFF)
					//LCD_DisplayString(1, "Left!");
				//else if(btdata == 0xEE)
					//LCD_DisplayString(1, "Right!");
				//else if(btdata == 0xAA)
					//LCD_DisplayString(1, "+1 life!");
				//else if(btdata == 0x00)
					//LCD_DisplayString(1, "lose 1 life!");
				if(btdata == 0xFF){ //left
					PORTB = SetBit(PORTB, 0, 1);
					delay_ms(100);
					PORTB = SetBit(PORTB, 0, 0);
				}
			}
			
			break;
		default:
			break;
	}
	switch(blueTooth_state){
		default:
			break;
	}
}

void LCDTask()
{
	LCD_Init();
	for(;;)
	{
		LCD_Tick();
		vTaskDelay(100);
	}
}

void newHSnameTask()
{
	for(;;)
	{
		newHSname_Tick();
		vTaskDelay(100);
	}
}
void addLowScoreTask()
{
	for(;;)
	{
		addLowScore_Tick();
		vTaskDelay(50);
	}
}
void addMidScoreTask()
{
	for(;;)
	{
		addMidScore_Tick();
		vTaskDelay(50);
	}
}
void addTopScoreTask()
{
	for(;;)
	{
		addTopScore_Tick();
		vTaskDelay(50);
	}
}
void loseCreditTask()
{
	for(;;)
	{
		loseCredit_Tick();
		vTaskDelay(50);
	}
}
void blueToothTask(){
	for(;;)
	{
		blueTooth_Tick();
		vTaskDelay(50);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LCDTask, (signed portCHAR *)"LCDTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(newHSnameTask, (signed portCHAR *)"newHSnameTask", configMINIMAL_STACK_SIZE, NULL, 2 , NULL );
	xTaskCreate(addLowScoreTask, (signed portCHAR *)"addLowScoreTask", configMINIMAL_STACK_SIZE, NULL, 3, NULL );
	xTaskCreate(addMidScoreTask, (signed portCHAR *)"addMidScoreTask", configMINIMAL_STACK_SIZE, NULL, 4, NULL );
	xTaskCreate(addTopScoreTask, (signed portCHAR *)"addTopScoreTask", configMINIMAL_STACK_SIZE, NULL, 5, NULL );
	xTaskCreate(loseCreditTask, (signed portCHAR *)"loseCreditTask", configMINIMAL_STACK_SIZE, NULL, 6, NULL );
	xTaskCreate(blueToothTask, (signed portCHAR *)"blueToothTask", configMINIMAL_STACK_SIZE, NULL, 7, NULL );
}

int main(void)
{
	DDRA = 0xC0; PORTA = 0x3F;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0x02; PORTD = 0xFD;
	initUSART(0);
	A2D_init();
	LCD_init();
	waitStartMessage();
	//Start Tasks
	StartSecPulse(1);
	//RunScheduler
	vTaskStartScheduler();
	return 0;
}