#ifndef __LCD_H__
#define __LCD_H__

#include <stdint.h>
#include "stm32f4xx.h" 

// Display used: 
// Newhaven 4x20 character OLED (green)
// NHD-0420DZW-AG5_Character_OLED_Display_Module
// Note, command delays according to datasheet (600uS, 2mS)
// PORTE
// 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
// D7 D6 D5 D4 D3 D2 D1 D0 RW RS  E
// PORTD KEYPAD, LEDs
// 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
// LB LR LO LG C3 C2 C1 C0       LE    R3 R2 R1 R0

// Use the display in 4-bit mode, otherwise use it in 8-bit mode
#define DISPLAY_4_BIT_MODE

#define DISPLAY_SIZE_X 20
#define DISPLAY_SIZE_Y  4

#define HD44780_E  (1U << 5)
#define HD44780_RS (1U << 6)
#define HD44780_RW (1U << 7)
#define HD44780_D4 (1U << 12)
#define HD44780_D5 (1U << 13)
#define HD44780_D6 (1U << 14)
#define HD44780_D7 (1U << 15)

#define DATA_REGISTER 1
#define COMMAND_REGISTER 0

void LCD_Init(void);

void LCD_Cmd(uint8_t command);
void LCD_Data(uint8_t data);
void LCD_Write(uint8_t data, uint8_t control);

void LCD_GotoXY(uint8_t x, uint8_t y);
void LCD_Char(uint8_t character);
void LCD_String(char *str);
void LCD_Increment_Cursor_Position(void);
void LCD_Clear(void);

#endif	// __LCD_H__
