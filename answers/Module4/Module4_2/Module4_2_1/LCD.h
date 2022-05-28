/*
 * LCD.h
 *
 *  Created on: May 14, 2022
 *      Author: traed
 */

#ifndef SRC_LCD_H_
#define SRC_LCD_H_


void LCD_init(void);
void LCD_print(char[]);
void LCD_ASCII(int);
void LCD_set_cursor(int, int);
void LCD(uint8_t, uint8_t);
void LCD_init_port(char[],char[], int);


#endif /* SRC_LCD_H_ */
