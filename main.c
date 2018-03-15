#include "stm32f4xx.h"                  // Device header
#include <stdio.h>

// Display used: 
// Newhaven 4x20 character OLED (green)
// NHD-0420DZW-AG5_Character_OLED_Display_Module
// Note, command delays according to datasheet (600uS, 2mS)
// PORTE
// 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
// D7 D6 D5 D4 D3 D2 D1 D0 RW RS  E

// Use the display in 4-bit mode, otherwise use it in 8-bit mode
//#define DISPLAY_4_BIT_MODE

#define LED_LD8_R (1U<<5)
#define LED_LD4_G (1U<<12)
#define LED_LD3_O (1U<<13)
#define LED_LD5_R (1U<<14)
#define LED_LD6_B (1U<<15)
#define USER_BUTTON (1U<<0)

#define LCKK (1U<<16)

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
void LCD_String(char *str);
void LCD_Clear(void);

void GPIO_Init(void);

void delay_ms(uint16_t n);
void delay_us(uint32_t n);

int main()
{
	uint16_t counter = 0;
	char buffer[10];

	LCD_Init();

	GPIOD->BSRR = (LED_LD5_R | LED_LD4_G | LED_LD6_B);
	delay_ms(1000);
	GPIOD->BSRR = ((LED_LD5_R | LED_LD4_G | LED_LD6_B) << 16);

	LCD_GotoXY(0,0);
	LCD_String("Hello");
	
	LCD_GotoXY(2,1);
	LCD_String("Display");

	LCD_GotoXY(4,2);
	LCD_String("in");
	
	LCD_GotoXY(6,3);
#ifdef DISPLAY_4_BIT_MODE
	LCD_String("4-bit mode");
#else
	LCD_String("8-bit mode");
#endif
	while (1) {
		LCD_GotoXY(10,0);
		sprintf(buffer, "%5d", counter);
		LCD_String(buffer);
		counter++;
		delay_ms(1000);
	}
}

void LCD_GotoXY(uint8_t x, uint8_t y)
{
	uint8_t address = 0;
	
	switch (y) {
		case 1:
			address = 0x40 + x;	// Line 1: 0x40, 0x41, 0x42 .. 0x53
			break;
		case 2:
			address = 0x14 + x;	// Line 2: 0x14, 0x15, 0x16 .. 0x27
			break;
		case 3:
			address = 0x54 + x;	// Line 3: 0x54, 0x55, 0x56 .. 0x67
			break;
		default:
			address = 0x00 + x;	// Line 0: 0x00, 0x01, 0x02 .. 0x13
	}
	LCD_Cmd(address | 0x80);
}

void LCD_String(char *str)
{
	while(*str) {
		LCD_Data(*str++);
	}
}

void LCD_Clear(void)
{
	LCD_Cmd(0x01);	// Display clear
	LCD_Cmd(0x80);	// Set DDRAM address to 0
}

void LCD_Init(void)
{
	// Enable clock to GPIO Port
	// Set GPIO Pins as output pins
	// Enable GPIO pins
	// Function set command 0x28 = 4-bit, 2 display lines, 5x7 font
	// Entry mode set command 0x06 incremenet automatically
	// Display control 0x0F Turn on display, cursor blinking
	// Display control 0x01 Clear display

	// Wait for power stabilization: >= 1ms
	delay_ms(2);

    // Enable clock to GPIO Port
    // Set GPIO pins as output pins
    // Enable GPIO pins
	GPIO_Init();
	
#ifdef DISPLAY_4_BIT_MODE
	// Use the display in 4-bit mode
	// Accoring to the datasheet, write this preceeding nibble to the display
	LCD_Write(0x20, COMMAND_REGISTER);
	
    // Function set command - 0x38 = 4-bit, 2 lines, 5x7 font
	LCD_Cmd(0x28);	// 00111000
#else
	// Use the display in 8-bit mode
    // Function set command - 0x38 = 8-bit, 2 lines, 5x7 font
	LCD_Cmd(0x38);	// 00111000
#endif

    // Display off
	LCD_Cmd(0x08);	// 00001000
	
    // Display clear
	LCD_Cmd(0x01);	// 00000001
	
    // Function mode set command - 0x06 = increment cursor automatically
	LCD_Cmd(0x06);	// 00000110
	
	LCD_Cmd(0x02);	// 00000010 Home Command

	// Display control - 0x0F = turn on display, cursor blinking
	LCD_Cmd(0x0F);	// 00001111
	
	LCD_Cmd(0x80);	// 10000000 Set DDRAM address to 0

}

void LCD_Cmd(uint8_t command) {
#ifdef DISPLAY_4_BIT_MODE
	// Use the display in 4-bit mode
	// Write upper 4-bits of command RS = 0
	// Write lower 4-bits of command
	// Secure command, E=1 for a brief moment
	// Delay, allow the display to catch-up with the MCU
	LCD_Write(command & 0xF0, COMMAND_REGISTER); 	// Command high nibble, RS=0
	LCD_Write(command << 4, COMMAND_REGISTER);	// Command low nibble, RS=0
    // Delay - Allow the display to catch-up with the MCU
#else
	// Use the display in 8-bit mode
    // Select Command Register - RS = 0
	LCD_Write(command, COMMAND_REGISTER); 	// Data high nibble, RS=1
#endif
	if (command <= 1) {
		delay_ms(2);
	} else {
		delay_us(600);
	}
}

void LCD_Data(uint8_t data) {
#ifdef DISPLAY_4_BIT_MODE
	// Use the display in 4-bit mode
	// Write upper 4-bits of command RS = 1
	// Write lower 4-bits of command
	// Secure command, E=1 for a brief moment
	// Delay, allow the display to catch-up with the MCU
	LCD_Write(data & 0xF0, DATA_REGISTER); 	// Data high nibble, RS=1
	LCD_Write(data << 4, DATA_REGISTER);	// Data low nibble, RS=1
    // Delay - Allow the display to catch-up with the MCU
#else
	// Use the display in 8-bit mode
    // Select Data Register - RS = 1
	LCD_Write(data, DATA_REGISTER); 	// Data high nibble, RS=1
#endif
	delay_us(4);
}

void LCD_Write(uint8_t data, uint8_t control) {
	// Select Command Register - RS=0, RW=0, E=0
	GPIOE->BSRR = ((HD44780_RW | HD44780_RS | HD44780_E) << 16); // RS=0, RW=0, E=0
	if (control == DATA_REGISTER) {
		// Select Data Register - RS=1, RW=0, E=0
		GPIOE->BSRR = HD44780_RS; // RS=1
	}

#ifdef DISPLAY_4_BIT_MODE
// Use the display in 4-bit mode
// data                    control
// 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
// D7 D6 D5 D4 -- -- -- -- RW RS  E
    // Write data
	// Set up data bits 4-7 on GPIOE12-15
//	GPIOE->ODR &= 0x0fff;
//	GPIOE->ODR |= ((data & 0xF0) << 8);
	GPIOE->BSRR = 0xf0000000;	// Reset bits 12-15
	GPIOE->BSRR = ((data & 0xF0) << 8);		// Extract upper 4bits from data and set bit 12-15 accordingly
#else
// Use the display in 8-bit mode
// data                    control
// 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
// D7 D6 D5 D4 D3 D2 D1 D0 RW RS  E
    // Write data
	// Set up data bits 0-7 on GPIOE8-15
//	GPIOE->ODR &= 0x00ff;
//	GPIOE->ODR |= (data << 8);
	GPIOE->BSRR = 0xff000000;	// Reset bits 8-15
	GPIOE->BSRR = (data << 8);		// Set bit 12-15 according to data
#endif
    // Secure command - E = 1 for a brief moment (20uS)
	GPIOE->BSRR = HD44780_E; // E=1, to secure command
	delay_us(2);
	GPIOE->BSRR = (HD44780_E << 16); // E=0

    // Set back to command register - RS = 0
//	GPIOE->BSRR = (HD44780_RS << 16); // reset RS=0
}

void GPIO_Init(void) {
    // Enable clock to GPIO Port
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;            // enable clock for GPIOA (button)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;            // enable clock for GPIOD (LEDs)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;            // enable clock for GPIOE (HD44780)

    // Set GPIO pins as output pins
    // Enable GPIO pins GPIOD15,14,13,12: 01 01 01 01 0x55000000
								// 15141312 11100908 07060504 03020100
	GPIOD->MODER = 0x55000000; 	// 01010101 00000000 00000000 00000000
	GPIOD->OSPEEDR = 0;
	GPIOD->OTYPER = 0;
	GPIOD->PUPDR = 0;

	// Now, lock GPIOD configuration
	GPIOD->LCKR = (LED_LD5_R | LED_LD4_G | LED_LD6_B | LED_LD3_O | LED_LD8_R | LCKK);
	GPIOD->LCKR = (LED_LD5_R | LED_LD4_G | LED_LD6_B | LED_LD3_O | LED_LD8_R);
	GPIOD->LCKR = (LED_LD5_R | LED_LD4_G | LED_LD6_B | LED_LD3_O | LED_LD8_R | LCKK);
	uint32_t lockedD = GPIOD->LCKR;
	lockedD = GPIOD->LCKR;	// 2nd read needed, as the LCKK bit is updated after the first read

#ifdef DISPLAY_4_BIT_MODE
// Discovery STM32F4
// Use the display in 4-bit mode
// PORTE
// 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
// D7 D6 D5 D4 -- -- -- -- RW RS  E
// 01 01 01 01 00 00 00 00 01 01 01 00 00 00 00 00
//   5     5     0     0     5     4     0     0
	GPIOE->MODER = 0x55005400;
	GPIOE->OSPEEDR = 0;
	GPIOE->OTYPER = 0;
	GPIOE->PUPDR = 0;
#else
// Discovery STM32F4
// Use the display in 8-bit mode
// PORTE
// 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
// D7 D6 D5 D4 D5 D6 D7 D8 RW RS  E
// 01 01 01 01 01 01 01 01 01 01 01 00 00 00 00 00
//   5     5     5     5     5     4     0     0
	GPIOE->MODER = 0x55555400;
	GPIOE->OSPEEDR = 0;
	GPIOE->OTYPER = 0;
	GPIOE->PUPDR = 0;
#endif
    // Now, lock GPIOE configuration
	// 0xffe0 11111111 11100000
//	GPIOE->LCKR = 0xffe0 | LCKK;
//	GPIOE->LCKR = 0xffe0;
//	GPIOE->LCKR = 0xffe0 | LCKK;
//	uint32_t lockedE = GPIOE->LCKR;
//	lockedE = GPIOE->LCKR;	// 2nd read needed, as the LCKK bit is updated after the first read

	// Indicate GPIOs are locked
//	if ((lockedD & LCKK) && (lockedE & LCKK)) {
//		GPIOD->BSRR = LED_LD3_O;	// Orange LED indicates successful lock
//	}
}

void delay_ms(uint16_t duration) {
	while(duration-- > 0) {
		delay_us(1000);
	}
}

void delay_us(uint32_t duration) {
	duration *= 3;
	while(duration-- > 0) {
	}
}
