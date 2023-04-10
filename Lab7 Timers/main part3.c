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
	TIM2 -> PSC = 0; // pg1076/1903, pre-scaling clock by an amount (4Mhz/(PSC + 1)), 16-bit number, 65536 is max number
				// pg for x = 2 to 5
	TIM2 -> ARR = 4; // pg1018/1903, up-counting goes from 0 to auto-reload value
				// pg1077/1903
	TIM2 -> CCR1 = 0; // the value that the timer switches on, can be any value btw 0 and ARR
				// pg1078/1903
	TIM2 -> CCMR1 |= (3<<4);
				// pg1071 OC1M 0110, PWM mode 2, bit 4+5+6+16
	TIM2 -> CCER = 1<<0; // bit 0, OC1 is output to output pin
				// pg1074
	TIM2 -> CR1 = 0<<4; // upcounter
	TIM2 -> CR1 = 1<<0; // enable counter / timer 2
				// pg1059
}

void config_clock_pin(void) {
	// Enable the clock to GPIO Port A
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  RCC -> AHB2ENR |= 0x00000001;
	//output 
  GPIOA -> MODER &= ~(3UL<<16);    
	GPIOA -> MODER |= 2UL<<16;           // 01 general output , 10 alternate function   	
  GPIOA -> OTYPER &= ~(1UL<<8);
  GPIOA -> OSPEEDR &= ~(3UL<<16); 
  GPIOA -> PUPDR &= ~(3<<16);
	GPIOA -> AFR[1] &= ~(7<<0); // fill value with 0000 (for AF0) (pg311/1903)
	RCC -> CFGR &= ~(7<<24);
	RCC -> CFGR |= (2<<24); // MCOSEL[2:0] = 0010 // MSI clock selected (pg 227/1903)
}
	
int main(void) {
	config_LED();
	config_clock_pin();
	while(1);
	
}