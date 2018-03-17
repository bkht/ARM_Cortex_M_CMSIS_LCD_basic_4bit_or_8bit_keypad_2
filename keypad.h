#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include <stdint.h>
#include "stm32f4xx.h" 
#include "delay.h" 

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

#define KEYPAD GPIOD
#define K_R0  (1U << 0)
#define K_R1  (1U << 1)
#define K_R2  (1U << 2)
#define K_R3  (1U << 3)
#define K_C0  (1U << 8)
#define K_C1  (1U << 9)
#define K_C2  (1U << 10)
#define K_C3  (1U << 11)

uint8_t getKey(void);

#endif
