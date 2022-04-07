#include "main.h"
#include "LCD2004.h"
#include "string.h"
#include "stm32f4xx_hal.h"
#include "stdio.h"

GPIO_TypeDef* letterD0 = GPIOA;
GPIO_TypeDef* letterD1 = GPIOA;
GPIO_TypeDef* letterD2 = GPIOA;
GPIO_TypeDef* letterD3 = GPIOA;
GPIO_TypeDef* letterD4 = GPIOA;
GPIO_TypeDef* letterD5 = GPIOA;
GPIO_TypeDef* letterD6 = GPIOA;
GPIO_TypeDef* letterD7 = GPIOA;
GPIO_TypeDef* letterRS = GPIOA;
GPIO_TypeDef* letterE = GPIOA;

uint16_t numberD0 = GPIO_PIN_0;
uint16_t numberD1 = GPIO_PIN_1 ;
uint16_t numberD2 = GPIO_PIN_2;
uint16_t numberD3 = GPIO_PIN_3;
uint16_t numberD4 = GPIO_PIN_4;
uint16_t numberD5 = GPIO_PIN_5;
uint16_t numberD6 = GPIO_PIN_6;
uint16_t numberD7 = GPIO_PIN_7;
uint16_t numberRS = GPIO_PIN_8;
uint16_t numberE = GPIO_PIN_9;

GPIO_TypeDef* get_letter(char letter[]) {
	if (strcmp(letter,"A") == 0) {
		return GPIOA;
	} else if (strcmp(letter,"B") == 0) {
		return GPIOB;
	} else if (strcmp(letter,"C") == 0) {
		return GPIOC;
	} else if (strcmp(letter,"D") == 0) {
		return GPIOD;
	} else if (strcmp(letter,"E") == 0) {
		return GPIOE;
	} else {
		return GPIOA;
	}
}

uint16_t get_number(int number) {
	switch (number) {
		case 0:
			return GPIO_PIN_0;
		case 1:
			return GPIO_PIN_1;
		case 2:
			return GPIO_PIN_2;
		case 3:
			return GPIO_PIN_3;
		case 4:
			return GPIO_PIN_4;
		case 5:
			return GPIO_PIN_5;
		case 6:
			return GPIO_PIN_6;
		case 7:
			return GPIO_PIN_7;
		case 8:
			return GPIO_PIN_8;
		case 9:
			return GPIO_PIN_9;
		case 10:
			return GPIO_PIN_10;
		case 11:
			return GPIO_PIN_11;
		case 12:
			return GPIO_PIN_12;
		case 13:
			return GPIO_PIN_13;
		case 14:
			return GPIO_PIN_14;
		case 15:
			return GPIO_PIN_15;
	}
	return 0;
}


void LCD_init_port(char name[],char letter[], int number) {
	if (strcmp(name,"D0") == 0) {
		letterD0 = get_letter(letter);
		numberD0 = get_number(number);
	} else if (strcmp(name,"D1") == 0) {
		letterD1 = get_letter(letter);
		numberD1 = get_number(number);
	} else if (strcmp(name,"D2") == 0) {
		letterD2 = get_letter(letter);
		numberD2 = get_number(number);
	} else if (strcmp(name,"D3") == 0) {
		letterD3 = get_letter(letter);
		numberD3 = get_number(number);
	} else if (strcmp(name,"D4") == 0) {
		letterD4 = get_letter(letter);
		numberD4 = get_number(number);
	} else if (strcmp(name,"D5") == 0) {
		letterD5 = get_letter(letter);
		numberD5 = get_number(number);
	} else if (strcmp(name,"D6") == 0) {
		letterD6 = get_letter(letter);
		numberD6 = get_number(number);
	} else if (strcmp(name,"D7") == 0) {
		letterD7 = get_letter(letter);
		numberD7 = get_number(number);
	} else if (strcmp(name,"RS") == 0) {
		letterRS = get_letter(letter);
		numberRS = get_number(number);
	} else if (strcmp(name,"E") == 0) {
		letterE = get_letter(letter);
		numberE = get_number(number);
	}
}

void LCD_init(void) {
	LCD(0x38, 0);
	LCD(0x0C, 0);
	LCD(0x06, 0);
	LCD(0x01, 0);
	LCD(0x80, 0);
}

void LCD_print(char msg[]){
	for (uint8_t i = 0; i < strlen(msg); i++) {
		  LCD((msg[i]),1);
		  HAL_Delay(1);
	  }
}

void LCD_ASCII(int code) {
	LCD(code,1);
}

void LCD_set_cursor(int x, int y) {
	int currentY;
	switch (y) {
		case 0:
			LCD(0x80, 0);
			currentY = 0x80;
		break;
		case 1:
			LCD(0xC0, 0);
			currentY = 0xC0;
		break;
		case 2:
			LCD(0x94, 0);
			currentY = 0x94;
		break;
		case 3:
			LCD(0xD4, 0);
			currentY = 0xD4;
		break;
	}
	LCD(currentY+(x%20), 0);
}

void LCD(uint8_t val_l, uint8_t cmd) {
	uint8_t data1;
	HAL_GPIO_WritePin(letterRS, numberRS, cmd);
	data1 = val_l & 0x01;
	HAL_GPIO_WritePin(letterD0, numberD0, data1);
	data1 = (val_l>>1) & 0x01;
	HAL_GPIO_WritePin(letterD1, numberD1, data1);
	data1 = (val_l>>2) & 0x01;
	HAL_GPIO_WritePin(letterD2, numberD2, data1);
	data1 = (val_l>>3) & 0x01;
	HAL_GPIO_WritePin(letterD3, numberD3, data1);
	data1 = (val_l>>4) & 0x01;
	HAL_GPIO_WritePin(letterD4, numberD4, data1);
	data1 = (val_l>>5) & 0x01;
	HAL_GPIO_WritePin(letterD5, numberD5, data1);
	data1 = (val_l>>6) & 0x01;
	HAL_GPIO_WritePin(letterD6, numberD6, data1);
	data1 = (val_l>>7) & 0x01;
	HAL_GPIO_WritePin(letterD7, numberD7, data1);
	HAL_GPIO_WritePin(letterE, numberE, GPIO_PIN_SET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(letterE, numberE, GPIO_PIN_RESET);
}
