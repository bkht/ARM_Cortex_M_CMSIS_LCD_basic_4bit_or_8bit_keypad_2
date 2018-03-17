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
	for (k_row = 0; k_row <= 3; k_row++) {
		GPIOD->BSRR = (K_R0 | K_R1 | K_R2 | K_R3); // R0-R3=1
		GPIOD->BSRR = ((K_R0 << k_row) << 16);	// 0001, 0010, 0100, 1000
		delay_ms(10);
		k_col = (GPIOD->IDR & 0x0f00) >> 8;	// 0000 CCCC 0000 RRRR
		if (k_col != 0x0f) {
			break;
		}
	}		
	for (int col = 0; col <= 3; col++) {
		if (k_col == ((~(1 << col)) & 0x0F)) {	// 1110, 1101, 1011, 0111
			return keymap[k_row][col];
		}
	}
	return 0;
}
