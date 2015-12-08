/*
*   Tristan Zickovich   tzick002@ucr.edu
*   Lab Section: 23
*   Assignment: Lab# 10 Final Project : Code for Stepper Motor and LED Matrix
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
#include "C:\Users\Tristan\Documents\Atmel Studio\6.2\120B\includes\bit.h"
#include "C:\Users\Tristan\Documents\Atmel Studio\6.2\120B\includes\io.c"
//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "sonar.h"
#include "sonar.c"

enum MotorTrigger_States {standbyMotor, updatemotor, resetposition} motortrigger_state;
enum Motor_States {rotateMotor} motor_state;
enum GameStart_States {gamewait, gwstall, gameplay} gamestart_state;
enum Motion_States {wait, flicker} motion_state;
enum DisplayMatrix_States {cue, display} display_state;
enum USD_States {waitmotion, motiondetected}usd_state;
	
//customizable variables matrix
unsigned char flicker_duration = 50; //how long LED matrix flickers when triggered
unsigned char flick_timer = 0; //how long it stalls before switching patterns for flicker
unsigned char patts[] = {0x55, 0xAA}; //patterns for matrix
unsigned char rows[] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF}; //which rows light pattern

//other variables matrix
unsigned char rowIndex = 0, flickerDurationCounter = 0, design = 0; //design: determines current pattern on LCD
unsigned char flickTriggered = 0; //turn on matrix flickering
unsigned char pattern = 0x00; // LED pattern - 0: LED off; 1: LED on
unsigned char row = 0xFF;  // Row(s) displaying pattern.

//motor variables
//full step sequence: A, AB, B, BC, C, CD, D, and DA
unsigned char steps[] = {0x01, 0x02, 0x04, 0x08};
unsigned char displayvalue = 0, update = 0, direction = 1, initGoalie = 0, stepIndex = 0;
unsigned short rotationCounter = 0, rotationTotal = 0, LRZdir = 0;
short start_position = 0;

unsigned short degreeCalc(unsigned short num ){
	return (num/5.625) * 32;
}

void MotorTrigger_Init(){
	motortrigger_state = standbyMotor;
	motor_state = rotateMotor;
	gamestart_state = gamewait;
	display_state = display;
	motion_state = wait;
	usd_state = waitmotion;
}

//0-2 ccw, 3-5 cw
void MotorTrigger_Tick(){ //commands motor (tells when and how to move)
	unsigned char i = 0;
	switch(motortrigger_state){
		case wait:
			displayvalue = 0; //need this?
			if(initGoalie){
				motortrigger_state = updatemotor;
				rotationCounter = 0;
				rotationTotal = degreeCalc(45);
				direction = 1;
				LRZdir = 1; //starts at 1 to account for missed ++counter
				update = 1;
			}
			break;
		case updatemotor:
			if(!initGoalie){
				motortrigger_state = resetposition;
				if(start_position > 0)
					direction = 1;
				else if(start_position < 0)
					direction = 0;
				else{
					update = 0;
					motortrigger_state = standbyMotor;
					break;
				}
				rotationCounter = 0;
				rotationTotal = abs(start_position);
				stepIndex = 0;
			}
			else if(rotationCounter >= rotationTotal){
				if(LRZdir == 0 || LRZdir == 3)
					direction = 1;
				else
					direction = 0;
				//direction = !direction;
				rotationCounter = 0;
				if(LRZdir < 3)
					++LRZdir;
				else
					LRZdir = 0;
			}
			break;
		case resetposition:
			if(rotationCounter >= rotationTotal){
				update = 0;
				motortrigger_state = standbyMotor;
				start_position = 0;
			}
			break;
		default:
			motortrigger_state = standbyMotor;
			break;
	}
	switch(motortrigger_state){
		default:
		break;
	}
}

void Motor_Tick(){ //drives motor
	unsigned char bumcounter = 0;
	if(update){
		displayvalue = steps[stepIndex];
		if(direction){
			if(stepIndex < 3)
				++stepIndex;
			else
				stepIndex = 0;
		}
		else{
			if(stepIndex > 0)
				--stepIndex;
			else
				stepIndex = 3;
		}
		++rotationCounter;
		if(direction)
			--start_position;
		else
			++start_position;
	}
	
	for(bumcounter = 0; bumcounter < 4; ++bumcounter){	//send signal to motor
		PORTB = SetBit(PORTB, bumcounter, GetBit(displayvalue, bumcounter));
	}
	//PORTB = displayvalue;
}

void GameStart_Tick(){ //awaits game start button press
	switch(gamestart_state){
		case gamewait:
			if(!GetBit(PINA, 0))
				gamestart_state = gwstall;
			break;
		case gwstall:
			if(GetBit(PINA, 0)){
				gamestart_state = gameplay;
				initGoalie = 1;
			}
			break;
		case gameplay:
			if(!GetBit(PINA, 1)){
				gamestart_state = gamewait;
				initGoalie = 0;
			}
			break;
		default:
			break;
	}
	switch(gamestart_state){
		default:
			break;
	}
}

void Motion_Tick() { //waits for ball to cross motion line to light matrix
	switch (motion_state) {
		case wait:
		if(flickTriggered){		//if motion was detected, trigger matrix flicker cycle
			flickerDurationCounter = 0;
			motion_state = flicker;
		}
		case flicker:
			if(flickerDurationCounter >= flicker_duration){
				motion_state = wait;
				flickTriggered = 0;
			}
			break;
		default:
			motion_state = flicker;
			break;
	}
	switch (motion_state) {
		case flicker:
			if(flick_timer < 1)
				++flick_timer;
			else{
				flick_timer = 0;
				design = !design;
			}
			++flickerDurationCounter;
			break;
		default:
			break;
	}
}

void DisplayMatrix_Tick(){ //illuminates matrix
	switch(display_state){
		case cue:
			if(flickTriggered){
				display_state = display;
				rowIndex = 0;
			}
			break;
		case display:
			if(!flickTriggered){
				display_state = cue;
				PORTC = 0x00; //clear matrix when no motion detected
				PORTD = 0xFF;
			}
			else if(design)
				pattern = patts[rowIndex%2];
			else
				pattern = patts[!(rowIndex%2)];
			row = rows[rowIndex];
			break;
		default:
			break;
	}
	switch(display_state){
		case display:
			if(rowIndex < 4)
				++rowIndex;
			else
				rowIndex = 0;
			PORTC = pattern; // Pattern to display
			PORTD = row; // Row(s) displaying pattern
			break;
		default:
			break;
	}
}
/*
void sonar_tick() [
	case init:
	break;
	case trigger:
		PORTB &= 0x10;
	break;
	case echo:
		PORTB &= 0x20;
	break;
}
*/
void USD_Tick(){
	int sonarDist = 0;
	unsigned char boardwidth = 200;
	switch(usd_state){
		case waitmotion:
			sonarDist = read_sonar();
			if(sonarDist != TRIG_ERROR && sonarDist != ECHO_ERROR){
				PORTC = 0xAA;
				PORTD = 0x00;
			}
			else if(sonarDist == TRIG_ERROR) {
				PORTC = 0x10;
				PORTD = 0x00;
			}
			else if(sonarDist == ECHO_ERROR) {
				PORTC = 0x0A;
				PORTD = 0x00;
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
}
/*
void USD_Tick(){
	int sonarDist = 0;
	unsigned char boardwidth = 200;
	switch(usd_state){
		case waitmotion:
			PORTD = 0xFE;
			sonarDist = read_sonar();
			sonarDist = sonarDist - (sonarDist % 10);
			if(sonarDist != 0)
				PORTC = sonarDist;
			if(sonarDist != TRIG_ERROR && sonarDist != ECHO_ERROR && sonarDist < 10){
				//usd_state = motiondetected;
				flickTriggered = 1;
				//SetBit(PORTA, 4, 0);
			}
			break;
		case motiondetected:
			PORTC = 0x00;
			//if(!flickTriggered)
				//usd_state = waitmotion;
			if(!GetBit(PINA, 0))
				usd_state = waitmotion;
				
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

void DisplayMatrixTask()
{
	for(;;)  //while(1)
	{
		DisplayMatrix_Tick();
		vTaskDelay(1);
	}
}
void MotionTask()
{
	for(;;)  //while(1)
	{
		Motion_Tick();
		vTaskDelay(100);
	}
}

void MotorTriggerTask(){
	while(1){
		MotorTrigger_Tick();
		vTaskDelay(100);
	}
}
void MotorTask(){
	while(1){
		Motor_Tick();
		vTaskDelay(4);
	}
}
void GameStartTask(){
	MotorTrigger_Init();
	while(1){
		GameStart_Tick();
		vTaskDelay(50);
	}
}

void USDTask()
{
	for(;;)  //while(1)
	{
		USD_Tick();
		_delay_ms(1000);
		//vTaskDelay(1000);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(GameStartTask, (signed portCHAR *)"GameStartTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
	xTaskCreate(MotorTriggerTask, (signed portCHAR *)"MotorTriggerTask", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	xTaskCreate(MotorTask, (signed portCHAR *)"MotorTask", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	xTaskCreate(MotionTask, (signed portCHAR *)"MotionTask", configMINIMAL_STACK_SIZE, NULL, 4, NULL );
	xTaskCreate(DisplayMatrixTask, (signed portCHAR *)"DisplayMatrixTask", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
	//xTaskCreate(USDTask, (signed portCHAR *)"USDTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x1F; PORTB = 0xF0;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	//init_sonar();
	//Start Tasks
	StartSecPulse(1);
	//RunScheduler
	vTaskStartScheduler();
	return 0;
}