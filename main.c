#include "stm32f4xx.h"                  // Device header
#include <stdio.h>
#include "main.h"
#include "init_gpio.h"
#include "delay.h"
#include "lcd.h"
#include "keypad.h"

int main()
{
	uint8_t key = 0;
	uint8_t key_old = 0;
	
	GPIO_Init();
	
	LCD_Init();

//	GPIOD->BSRR = (LED_LD5_R | LED_LD4_G | LED_LD6_B);
//	delay_ms(1000);
//	GPIOD->BSRR = ((LED_LD5_R | LED_LD4_G | LED_LD6_B) << 16);

	LCD_GotoXY(0,0);
	LCD_String("Hello");
	
	LCD_GotoXY(3,1);
	LCD_String("Display");

	LCD_GotoXY(6,2);
	LCD_String("in");
	
	LCD_GotoXY(9,3);
#ifdef DISPLAY_4_BIT_MODE
	LCD_String("4-bit mode");
#else
	LCD_String("8-bit mode");
#endif
	LCD_GotoXY(0,0);
	while (1) {
		key = getKey();
		if ((key != 0) && (key_old == 0)) {
			//LCD_GotoXY(15,0);
			LCD_Char(key);
		}
		key_old = key;
		
	}
}
