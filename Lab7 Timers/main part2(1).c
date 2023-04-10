#include "stm32l476xx.h"

// use registers TIM2_PSC, TIM2_ARR, and TIM2_CCR1 to create
	// a timing waveform with 1sec period (1Hz) from default sysclk of 4MHz

void config_LED(void) {
	
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  GPIOA -> MODER &= ~(3UL<<10);
	GPIOA -> MODER |= 2UL<<10; // Set the GPIO Port A mode to AF (10)
	GPIOA -> AFR[0] |= 1<<20; // AFSEL5[3:0] is pin 5, bits 0-3, AF1 is 0001
	//pg 309
	GPIOA -> OSPEEDR &= (0UL<<10); // low speed
  GPIOA -> PUPDR &= ~(3UL<<10); // no pull-up pull-down
	
	RCC -> APB1ENR1 |= RCC_APB1ENR1_TIM2EN; // enable timer 2 clock
				// pg256/1903
	TIM2 -> PSC = 3999; // pg1076/1903, pre-scaling clock by an amount (4Mhz/(PSC + 1)), 16-bit number, 65536 is max number
				// pg for x = 2 to 5
	TIM2 -> ARR = 999; // pg1018/1903, up-counting goes from 0 to auto-reload value
				// pg1077/1903
	TIM2 -> CCR1 = 199; // the value that the timer switches on, can be any value btw 0 and ARR
				// pg1078/1903
	TIM2 -> CCMR1 |= (6<<4);
				// pg1071 OC1M 0110, PWM mode 1, bit 4+5+6+16
	TIM2 -> CCER = 1<<0; // bit 0, OC1 is output to output pin
				// pg1074
	TIM2 -> CR1 = 0<<4; // upcounter
	TIM2 -> CR1 = 1<<0; // enable counter / timer 2
				// pg1059
}

int main(void) {
	config_LED();
	while(1);
	
}