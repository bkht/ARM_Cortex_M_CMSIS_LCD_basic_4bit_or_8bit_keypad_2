#include "delay.h"

void delay_ms(uint16_t duration) {
	while(duration-- > 0) {
		delay_us(1000);
	}
}

void delay_us(uint32_t duration) {
	duration *= 3;
	while(duration-- > 0) {
	}
}
