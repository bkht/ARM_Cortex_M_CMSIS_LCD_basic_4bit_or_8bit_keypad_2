#include "leds.h"

void LED_On(uint16_t led)
{
	GPIOD->BSRR = led; // Lit LED
}

void LED_Off(uint16_t led)
{
	GPIOD->BSRR = (led<<16); // Dim LED
}

void LED_Toggle(uint16_t led)
{
	GPIOD->ODR ^= led; // Toggle LED
}

void LED_Set(uint16_t led, uint8_t status)
{
	if (status) {
		GPIOD->BSRR = led; // Lit LED
	} else {
		GPIOD->BSRR = (led<<16); // Dim LED
	}
}

uint8_t ButtonIsPressed(uint16_t button)
{
	return (GPIOA->IDR & button) == 0 ? 0 : 1;
}
