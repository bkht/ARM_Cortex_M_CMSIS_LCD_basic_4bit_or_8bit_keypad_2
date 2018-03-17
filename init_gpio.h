#ifndef __INIT_GPIO_H__
#define __INIT_GPIO_H__

#include <stdint.h>
#include "stm32f4xx.h" 
#include "lcd.h" 

#define LCKK (1U<<16)

void GPIO_Init(void);

#endif
