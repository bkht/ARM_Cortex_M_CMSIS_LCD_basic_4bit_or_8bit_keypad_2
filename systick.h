#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h" 

#define ONESEC 16000000 - 1

void Systick_Init(void);
bool Timeout(void);

#endif
