#include "stm32l476xx.h"

// PA.5 <--> Green LED
// PC.13 <--> Blue user button
// PC.1 <--> Input square wave from PB505

void configure_LED_pin(){
	// Enable the clock to GPIO Port A
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  RCC -> AHB2ENR |= 0x00000001;
  GPIOA -> MODER &= ~(3UL<<10);
	GPIOA -> MODER |= 1UL<<10;             	
  GPIOA -> OTYPER &= ~(1UL<<5);
  GPIOA -> OSPEEDR &= (1UL<<10); 
  GPIOA -> PUPDR &= ~(3<<10);
}

void configure_BUTTON_pin(){
  RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
  RCC -> AHB2ENR |= 0x00000001;
	GPIOC -> MODER &= ~(3UL<<26);
	GPIOC -> PUPDR &= ~(3<<26); 
}

void configure_EXTI(){
	//use RCC->APB2ENR peripheral clock enable register to enable SYSCFG clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	//use SYSCFG->EXTICR[?] external interrupt configuration register to connect PC.13
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
  //configure EXTI registers to enable interrupts
	// enable interrupt = unmasked (1) (tells program to not ignore this interrupt)
	EXTI->RTSR1 |= EXTI_RTSR1_RT13;
	EXTI->IMR1 |= EXTI_IMR1_IM13;
	//enable interrupt PC.13
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}	

//calling from the .s file, change this function by:
void EXTI15_10_IRQHandler(void){
	if ((EXTI->PR1 & EXTI_PR1_PIF13) == EXTI_PR1_PIF13) {
		//toggle LED
		GPIOA -> ODR ^= 1UL<<5;
		while (GPIOC -> IDR == 0x0000);
			//same as EXTI->PR1 |= (1<<1);
			// book says that setting it to 1 clears the flag
		EXTI->PR1 |= EXTI_PR1_PIF13;
	}
}

int main(void){
	configure_LED_pin();
	configure_BUTTON_pin();
	configure_EXTI();
	while(1);
		//says ISR latency is 12 cycles
}
