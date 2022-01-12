/*
 * LCD2004.h
 *
 *  Created on: Jan 6, 2022
 *      Author: Nikita Sukhachev
 *      inspired by: https://www.youtube.com/channel/UC0I4lX6emUM2DwFeSFdoJcQ
 */

#ifndef SRC_LCD2004_H_
#define SRC_LCD2004_H_

void LCD_init(void);
void LCD_print(char[]);
void LCD_ASCII(int);
void LCD_set_cursor(int, int);
void LCD(uint8_t, uint8_t);
void LCD_init_port(char[],char[], int);


#endif /* SRC_LCD2004_H_ */
