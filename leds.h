#ifndef __LEDS_H__
#define __LEDS_H__

#include <stdint.h>
#include "stm32f4xx.h" 

#define LED_LD8_R (1U<<5)
#define LED_LD4_G (1U<<12)
#define LED_LD3_O (1U<<13)
#define LED_LD5_R (1U<<14)
#define LED_LD6_B (1U<<15)
#define USER_BUTTON (1U<<0)

void LED_On(uint16_t led);
void LED_Off(uint16_t led);
void LED_Toggle(uint16_t led);
void LED_Set(uint16_t led, uint8_t status);
uint8_t ButtonIsPressed(uint16_t button);

#endif
