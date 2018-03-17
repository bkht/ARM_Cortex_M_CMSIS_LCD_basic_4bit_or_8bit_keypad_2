#include "init_gpio.h"

void GPIO_Init(void) {
	// PORTA User Button
    // Enable clock to GPIO Port
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;            // enable clock for GPIOA (button)

	// These bits are written by software to configure the I/O direction mode.
	// 00: Input (reset state)
	// 01: General purpose output mode
	// All inputs
	// PORTA User Button is input
	// 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
	//                                              UB
	// 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
	//   0     0     0     0     0     0     0     0
//	GPIOA->MODER = 0x00000000;
//	GPIOA->OSPEEDR = 0;
	//	These bits are written by software to configure the output type of the I/O port.
	//	0: Output push-pull (reset state)
	//	1: Output open-drain
//	GPIOA->OTYPER = 0;
//	GPIOA->PUPDR = 0;

	// Now, lock GPIOA configuration
//	GPIOA->LCKR = (USER_BUTTON | LCKK);
//	GPIOA->LCKR = (USER_BUTTON);
//	GPIOA->LCKR = (USER_BUTTON | LCKK);
//	uint32_t lockedA = GPIOA->LCKR;
//	lockedA = GPIOA->LCKR;	// 2nd read needed, as the LCKK bit is updated after the first read

	// PORTD KEYPAD + LEDs
    // Enable clock to GPIO Port
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;            // enable clock for GPIOD (LEDs)
	
	// These bits are written by software to configure the I/O direction mode.
	// 00: Input (reset state)
	// 01: General purpose output mode
	// PORTD KEYPAD + LEDs: LEDs are outputs, Keypad Colums are inputs, Keypad Rows are outputs
	// 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
	// LB LR LO LG C3 C2 C1 C0       LE    R3 R2 R1 R0
	// 01 01 01 01 00 00 00 00 00 00 01 00 01 01 01 01
	//   5     5     0     0     0     4     5     5
	GPIOD->MODER = 0x55000455;
	GPIOD->OSPEEDR = 0;
	//	These bits are written by software to configure the output type of the I/O port.
	//	0: Output push-pull (reset state)
	//	1: Output open-drain
	// 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
	// LB LR LO LG C3 C2 C1 C0       LE    R3 R2 R1 R0
	//  0  0  0  0  0  0  0  0  0  0  0  0  1  1  1  1
	//       0           0           0           f
	GPIOD->OTYPER = 0x000f;
	//	These bits are written by software to configure the I/O pull-up or pull-down
	//	00: No pull-up, pull-down
	//	01: Pull-up
	//	10: Pull-down	
	// 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
	// LB LR LO LG C3 C2 C1 C0       LE    R3 R2 R1 R0
	// 00 00 00 00 01 01 01 01 00 00 00 00 00 00 00 00
	//   0     0     5     5     0     0     0     0
	GPIOD->PUPDR = 0x00550000;

	// Now, lock GPIOD configuration
//	GPIOD->LCKR = (LED_LD5_R | LED_LD4_G | LED_LD6_B | LED_LD3_O | LED_LD8_R | LCKK);
//	GPIOD->LCKR = (LED_LD5_R | LED_LD4_G | LED_LD6_B | LED_LD3_O | LED_LD8_R);
//	GPIOD->LCKR = (LED_LD5_R | LED_LD4_G | LED_LD6_B | LED_LD3_O | LED_LD8_R | LCKK);
//	uint32_t lockedD = GPIOD->LCKR;
//	lockedD = GPIOD->LCKR;	// 2nd read needed, as the LCKK bit is updated after the first read

	// PORTE LCD
    // Enable clock to GPIO Port
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;            // enable clock for GPIOE (HD44780)

#ifdef DISPLAY_4_BIT_MODE
	// Discovery STM32F4
	// Use the display in 4-bit mode
	// These bits are written by software to configure the I/O direction mode.
	// 00: Input (reset state)
	// 01: General purpose output mode
	// PORTE display
	// 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
	// D7 D6 D5 D4 -- -- -- -- RW RS  E
	// 01 01 01 01 00 00 00 00 01 01 01 00 00 00 00 00
	//   5     5     0     0     5     4     0     0
	GPIOE->MODER = 0x55005400;
#else
	// Discovery STM32F4
	// Use the display in 8-bit mode
	// These bits are written by software to configure the I/O direction mode.
	// 00: Input (reset state)
	// 01: General purpose output mode
	// PORTE display
	// 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
	// D7 D6 D5 D4 D5 D6 D7 D8 RW RS  E
	// 01 01 01 01 01 01 01 01 01 01 01 00 00 00 00 00
	//   5     5     5     5     5     4     0     0
	GPIOE->MODER = 0x55555400;
#endif
	GPIOE->OSPEEDR = 0;
	//	These bits are written by software to configure the output type of the I/O port.
	//	0: Output push-pull (reset state)
	//	1: Output open-drain
	GPIOE->OTYPER = 0;
	GPIOE->PUPDR = 0;

	// Now, lock GPIOE configuration
	// 0xffe0 11111111 11100000
//	GPIOE->LCKR = 0xffe0 | LCKK;
//	GPIOE->LCKR = 0xffe0;
//	GPIOE->LCKR = 0xffe0 | LCKK;
//	uint32_t lockedE = GPIOE->LCKR;
//	lockedE = GPIOE->LCKR;	// 2nd read needed, as the LCKK bit is updated after the first read

//	// Indicate GPIOs are locked
//	if ((lockedA & LCKK) && (lockedD & LCKK) && (lockedE & LCKK)) {
//		GPIOD->BSRR = LED_LD3_O;	// Orange LED indicates successful lock
//	}
}
