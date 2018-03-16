#include "keypad.h"

uint8_t getKey(void) {
	// Keypad connections
	// Top view, bottom 1-8 from left to right:
	//  1   2   3   4   5   6   7   8
	// C0  C1  C2  C3  R0  R1  R2  R3
	// D8  D9  D10 D11 D0  D1  D2  D3
	// IN  IN  IN  IN  OUT OUT OUT OUT
	// Keys:
	// C0 C1 C2 C3
	//  1  2  3  A  R0
	//  4  5  6  B  R1
	//  7  8  9  C  R2
	//  *  0  #  D  R3
	// GPIOD 0000 CCCC 0000 RRRR
	//            3210      3210
	const uint8_t keymap[4][4]={
		{'1','2','3','A'},
		{'4','5','6','B'},
		{'7','8','9','C'},
		{'*','0','#','D'}
	};
	uint8_t k_row, k_col;
	// Clear bits for R0-R3=0
	GPIOD->BSRR = ((K_R0 | K_R1 | K_R2 | K_R3) << 16);
	k_col = (GPIOD->IDR & 0x0f00) >> 8;	// 0000 CCCC 0000 RRRR
	if (k_col == 0x0f) {
		return 0;	// No key pressed
	}
	while (1) {
		k_row = 0;
		GPIOD->BSRR = (K_R0 | K_R1 | K_R2 | K_R3); // R0-R3=1
		GPIOD->BSRR = (K_R0 << 16);	// R0=0
		delay_ms(10);
		k_col = (GPIOD->IDR & 0x0f00) >> 8;	// 0000 CCCC 0000 RRRR
		if (k_col != 0x0f) {
			break;
		}
		k_row = 1;
		GPIOD->BSRR = (K_R0 | K_R1 | K_R2 | K_R3); // R0-R3=1
		GPIOD->BSRR = (K_R1 << 16);	// R1=0
		delay_ms(10);
		k_col = (GPIOD->IDR & 0x0f00) >> 8;	// 0000 CCCC 0000 RRRR
		if (k_col != 0x0f) {
			break;
		}
		k_row = 2;
		GPIOD->BSRR = (K_R0 | K_R1 | K_R2 | K_R3); // R0-R3=1
		GPIOD->BSRR = (K_R2 << 16);	// R1=0
		delay_ms(10);
		k_col = (GPIOD->IDR & 0x0f00) >> 8;	// 0000 CCCC 0000 RRRR
		if (k_col != 0x0f) {
			break;
		}
		k_row = 3;
		GPIOD->BSRR = (K_R0 | K_R1 | K_R2 | K_R3); // R0-R3=1
		GPIOD->BSRR = (K_R3 << 16);	// R1=0
		delay_ms(10);
		k_col = (GPIOD->IDR & 0x0f00) >> 8;	// 0000 CCCC 0000 RRRR
		if (k_col != 0x0f) {
			break;
		}
		return 0;	// No key pressed (not expected)
	}
	if (k_col == 0x0E) {	// 1110
		return keymap[k_row][0];
	}
	if (k_col == 0x0D) {	// 1101
		return keymap[k_row][1];
	}
	if (k_col == 0x0B) {	// 1011
		return keymap[k_row][2];
	}
	if (k_col == 0x07) {	// 0111
		return keymap[k_row][3];
	}
	return 0;
}
