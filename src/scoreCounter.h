//Number Display Sequences
unsigned char zeroTR[] = {0x00,0x00,0x1C,0x1C,0x0C,0x0C,0x0C,0x0C};
unsigned char zeroBR[] = {0x0C,0x0C,0x0C,0x0C,0x1C,0x1C,0x00,0x00};
unsigned char zeroTL[] = {0x00,0x00,0x07,0x07,0x06,0x06,0x06,0x06};
unsigned char zeroBL[] = {0x06,0x06,0x06,0x06,0x07,0x07,0x00,0x00};

unsigned char oneTR[] = {0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10};
unsigned char oneBR[] = {0x10,0x10,0x10,0x10,0x1C,0x1C,0x00,0x00};
unsigned char oneTL[] = {0x00,0x00,0x07,0x07,0x01,0x01,0x01,0x01};
unsigned char oneBL[] = {0x01,0x01,0x01,0x01,0x07,0x07,0x00,0x00};

unsigned char twoTR[] = {0x00,0x00,0x1C,0x1C,0x0C,0x0C,0x0C,0x1C};
unsigned char twoBR[] = {0x1C,0x00,0x00,0x00,0x1C,0x1C,0x00,0x00};
unsigned char twoTL[] = {0x00,0x00,0x07,0x07,0x00,0x00,0x00,0x07};
unsigned char twoBL[] = {0x06,0x06,0x06,0x06,0x07,0x07,0x00,0x00};

unsigned char threeTR[] = {0x00,0x00,0x1C,0x1C,0x0C,0x0C,0x0C,0x1C};
unsigned char threeBR[] = {0x1C,0x0C,0x0C,0x0C,0x1C,0x1C,0x00,0x00};
unsigned char threeTL[] = {0x00,0x00,0x07,0x07,0x00,0x00,0x00,0x07};
unsigned char threeBL[] = {0x07,0x00,0x00,0x00,0x07,0x07,0x00,0x00};

unsigned char fourTR[] = {0x00,0x00,0x0C,0x0C,0x0C,0x0C,0x0C,0x1C};
unsigned char fourBR[] = {0x1C,0x0C,0x0C,0x0C,0x0C,0x0C,0x00,0x00};
unsigned char fourTL[] = {0x00,0x00,0x06,0x06,0x06,0x06,0x06,0x07};
unsigned char fourBL[] = {0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

unsigned char fiveTR[] = {0x00,0x00,0x1C,0x1C,0x00,0x00,0x00,0x1C};
unsigned char fiveBR[] = {0x1C,0x0C,0x0C,0x0C,0x1C,0x1C,0x00,0x00};
unsigned char fiveTL[] = {0x00,0x00,0x07,0x07,0x06,0x06,0x06,0x07};
unsigned char fiveBL[] = {0x07,0x00,0x00,0x00,0x07,0x07,0x00,0x00};

unsigned char sixTR[] = {0x00,0x00,0x1C,0x1C,0x00,0x00,0x00,0x1C};
unsigned char sixBR[] = {0x1C,0x0C,0x0C,0x0C,0x1C,0x1C,0x00,0x00};
unsigned char sixTL[] = {0x00,0x00,0x07,0x07,0x06,0x06,0x06,0x07};
unsigned char sixBL[] = {0x07,0x06,0x06,0x06,0x07,0x07,0x00,0x00};

unsigned char sevenTR[] = {0x00,0x00,0x1C,0x1C,0x0C,0x0C,0x0C,0x0C};
unsigned char sevenBR[] = {0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x00,0x00};
unsigned char sevenTL[] = {0x00,0x00,0x07,0x07,0x00,0x00,0x00,0x00};
unsigned char sevenBL[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

unsigned char eightTR[] = {0x00,0x00,0x1C,0x1C,0x0C,0x0C,0x0C,0x1C};
unsigned char eightBR[] = {0x1C,0x0C,0x0C,0x0C,0x1C,0x1C,0x00,0x00};
unsigned char eightTL[] = {0x00,0x00,0x07,0x07,0x06,0x06,0x06,0x07};
unsigned char eightBL[] = {0x07,0x06,0x06,0x06,0x07,0x07,0x00,0x00};

unsigned char nineTR[] = {0x00,0x00,0x1C,0x1C,0x0C,0x0C,0x0C,0x1C};
unsigned char nineBR[] = {0x1C,0x0C,0x0C,0x0C,0x1C,0x1C,0x00,0x00};
unsigned char nineTL[] = {0x00,0x00,0x07,0x07,0x06,0x06,0x06,0x07};
unsigned char nineBL[] = {0x07,0x00,0x00,0x00,0x07,0x07,0x00,0x00};

//Addresses for indices of scoreboard
const unsigned char creditAddress0 = 0x40, creditAddress1 = 0x60;
unsigned char credit0 = 0, credit1 = 0, gameover = 1;
//write number to lcd
void character(unsigned char TL[], unsigned char TR[], unsigned char BL[], unsigned char BR[], unsigned char L1, unsigned char L2, unsigned char SA, unsigned char DN){
	unsigned char index = 0;
	unsigned char displayNumber = DN;
	//TL
	LCD_WriteCommand(SA);  //Address in memory where values of the first custom character is stored
	for(index = 0; index < 8; ++index)
		LCD_WriteData(TL[index]);
	LCD_WriteCommand(L1);  //Address of the location where the character is to be displayed on LCD
	LCD_WriteData(displayNumber);  // Displaying the character created at memory address
	delay_ms(10);
	SA += 0x08;
	++displayNumber;
	++L1;
	//TR
	LCD_WriteCommand(SA);  //Address in memory where values of the first custom character is stored
	for(index = 0; index < 8; ++index)
		LCD_WriteData(TR[index]);
	LCD_WriteCommand(L1);  //Address of the location where the character is to be displayed on LCD
	LCD_WriteData(displayNumber);  // Displaying the character created at memory address
	delay_ms(10);
	SA += 0x08;
	++displayNumber;
	//BL
	LCD_WriteCommand(SA);  //Address in memory where values of the first custom character is stored
	for(index = 0; index < 8; ++index)
		LCD_WriteData(BL[index]);
	LCD_WriteCommand(L2);  //Address of the location where the character is to be displayed on LCD
	LCD_WriteData(displayNumber);  // Displaying the character created at memory address
	delay_ms(10);
	SA += 0x08;
	++displayNumber;
	++L2;
	//BR
	LCD_WriteCommand(SA);  //Address in Memory where values of the first custom character is stored
	for(index = 0; index < 8; ++index)
		LCD_WriteData(BR[index]);
	LCD_WriteCommand(L2);  //Address of the location where the character is to be displayed on LCD
	LCD_WriteData(displayNumber);  // Displaying the character created at memory address
	delay_ms(10);
	LCD_Cursor(0);
}

void sendCredits(unsigned char number, unsigned char L1, unsigned char L2, unsigned char SA, unsigned char DN){
	if(number == 0)
	character(zeroTL, zeroTR, zeroBL, zeroBR, L1, L2, SA, DN);
	else if(number == 1)
	character(oneTL, oneTR, oneBL, oneBR, L1, L2, SA, DN);
	else if(number == 2)
	character(twoTL, twoTR, twoBL, twoBR, L1, L2, SA, DN);
	else if(number == 3)
	character(threeTL, threeTR, threeBL, threeBR, L1, L2, SA, DN);
	else if(number == 4)
	character(fourTL, fourTR, fourBL, fourBR, L1, L2, SA, DN);
	else if(number == 5)
	character(fiveTL, fiveTR, fiveBL, fiveBR, L1, L2, SA, DN);
	else if(number == 6)
	character(sixTL, sixTR, sixBL, sixBR, L1, L2, SA, DN);
	else if(number == 7)
	character(sevenTL, sevenTR, sevenBL, sevenBR, L1, L2, SA, DN);
	else if(number == 8)
	character(eightTL, eightTR, eightBL, eightBR, L1, L2, SA, DN);
	else if(number == 9)
	character(nineTL, nineTR, nineBL, nineBR, L1, L2, SA, DN);
}

void updateCredits(signed char newCredits){
	unsigned char L1 = 0x8E, L2 = 0xCE, DN = 0x00;
	char temp = 0;
	if(newCredits > 0){
		temp = newCredits/10;
		newCredits -= temp * 10;
		if(temp != 0)
			credit1 += temp;
		temp = newCredits;
		if(temp != 0)
			credit0 += temp;
		if(credit0 > 9 && credit1 < 9){
			credit0 = credit0 - 10;
			++credit1;
		}
	}
	else if(newCredits < 0){
		if(credit0 > 0)
			--credit0;
		else if(credit0 == 0 && credit1 > 0){
			--credit1;
			credit0 = 9;
		}
		else{
			gameover = 1;
			PORTB = SetBit(PORTB, 0, 0);
		}
	}
	sendCredits(credit0,L1,L2,creditAddress0,DN);
	L1 -= 2; L2 -= 2; DN += 0x04;
	sendCredits(credit1,L1,L2,creditAddress1,DN);
}

//Score / Highscore Variables
unsigned char scores[] = {0,0,0,0,0};
unsigned char highscores[] = {0,7,0,0,0};
unsigned char HSholder[] = {'T','A','Z'};

void sendScore(unsigned char cursor){
	unsigned char temp = 0;
	for(temp = 0; temp < 5; ++temp){
		LCD_Cursor(cursor + (5 -temp));
		LCD_WriteData('0' + scores[temp]);
	}
	LCD_Cursor(0);
}

void updateScore(unsigned short newscore){
	unsigned char i = 0, temp = 0, scoreIndex = 0, newLocation = 10, fullindex = 4;
	for(scoreIndex = 0; scoreIndex < 4; ++scoreIndex){
		temp = newscore % newLocation;
		newscore /= newLocation;
		scores[scoreIndex] += temp;
		while(scores[scoreIndex] > 9){
			++scores[scoreIndex + 1];
			scores[scoreIndex] = scores[scoreIndex] - 10;
		}
		newLocation *= 10;
	}
	if(scores[4] > 9)	//cap the max score
	for(scoreIndex = 0; scoreIndex < 5; ++scoreIndex)
	scores[scoreIndex] = 9;
	sendScore(22);
}

void screenFormat(){
	unsigned char scoreMessage[] = {'S','c','o','r','e',':'};
	unsigned char sz = 6, temp = 0, cursor = 17;
	LCD_DisplayString(1, "    Credits: ");
	for(temp = 0; temp < sz; ++temp){
		LCD_Cursor(cursor + temp);
		LCD_WriteData(scoreMessage[temp]);
	}
	sendScore(22);
	updateCredits(0);
	LCD_Cursor(0);
}

void HSscreenFormat(){
	unsigned char cursor = 20, temp = 0;
	LCD_DisplayString(1, "   High-Score: ");
	for(temp = 0; temp < 3; ++temp){
		LCD_Cursor(cursor + temp);
		LCD_WriteData(HSholder[temp]);
	}
	LCD_Cursor(24);
	LCD_WriteData('~');
	cursor = 25;
	for(temp = 0; temp < 5; ++temp){
		LCD_Cursor(cursor + (5 - temp));
		LCD_WriteData('0' + highscores[temp]);
	}
	LCD_Cursor(0);
}
void newHSformat(){
	unsigned char cursor = 23, temp = 0;
	LCD_DisplayString(1, "Enter Initials:");
	for(temp = 0; temp < 3; ++temp){
		LCD_Cursor(cursor + temp);
		HSholder[temp] = 'A';
		LCD_WriteData(HSholder[temp]);
	}
	LCD_Cursor(23);
}
int scoreCompare(){
	unsigned char i = 0;
	for(i = 4; i >= 0; --i){
		if(scores[i] > highscores[i])
			return 1;
		else if(highscores[i] > scores[i])
			return -1;
	}
	return 0;
}
void transferHS(){
	int i = 0;
	for(i = 4; i >= 0; --i){
		highscores[i] = scores[i];
	}
}
void reconfigureGame(){
	credit0 = 1;
	credit1 = 0;
	gameover = 0;
	int i = 0;
	for(i = 0; i < 5; ++i){
		scores[i] = 0;
	}
	PORTB = SetBit(PORTB, 0, 1);
}
//4 ->, 5 <-
void waitStartMessage(){
	LCD_DisplayString(1, "<- Play         -> Hi-Score");
	LCD_Cursor(0);
}