#include "systick.h"

void Systick_Init(void)
{
	SysTick->LOAD = ONESEC;
	SysTick->CTRL = 5;
}

bool Timeout(void)
{
	return (SysTick->CTRL & 0x10000);
}
