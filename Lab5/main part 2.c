#include "stm32l476xx.h"

// PA.5 <--> Green LED
// PC.13 <--> Blue user button
// PC.1 <--> Input square wave from PB505

void configure_LED_pin(){
	// Enable the clock to GPIO Port A
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  RCC -> AHB2ENR |= 0x00000001;
	// Set the GPIO Port A mode to output (01) 
  GPIOA -> MODER &= ~(3UL<<10);
	//sets bit 10 to 1
	GPIOA -> MODER |= 1UL<<10;             	
	// Set the GPIO Port A output type to push - pull 
  GPIOA -> OTYPER &= ~(1UL<<5);
	// Set the GPIO speed to low
  GPIOA -> OSPEEDR &= (1UL<<10); 
	// Set GPIO Port A Push - Pull to no pull - up, no pull - down
  GPIOA -> PUPDR &= ~(3<<10);
}

void configure_PC1_pin(){
	// Enable the clock to GPIO Port C
  RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
  RCC -> AHB2ENR |= 0x00000001;
	// GPIOC Mode: Input(00)
	GPIOC -> MODER &= ~(3UL<<2);
	// GPIOC Push-Pull: No pull-up, no pull-down (00)
	GPIOC -> PUPDR &= ~(3<<2); 
}
void configure_EXTI(){
	//use RCC->APB2ENR peripheral clock enable register to enable SYSCFG clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	//use SYSCFG->EXTICR[?] external interrupt configuration register to connect PC.13
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PC;
  //configure EXTI registers to enable interrupts
	// rising edge = 0
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT1;
	// falling edge = 1
	EXTI->FTSR1 |= EXTI_FTSR1_FT1;
	// enable interrupt = unmasked (1) (tells program to not ignore this interrupt)
	EXTI->IMR1 |= EXTI_IMR1_IM1;
	//enable interrupt PC.1
	NVIC_EnableIRQ(EXTI1_IRQn);
}	

//calling from the .s file, change this function by:
void EXTI1_IRQHandler(void){
	if ((EXTI->PR1 & EXTI_PR1_PIF1) == EXTI_PR1_PIF1) {
		//toggle LED
		GPIOA -> ODR ^= 1UL<<5;
			//same as EXTI->PR1 |= (1<<1);
			// book says that setting it to 1 clears the flag
		EXTI->PR1 |= EXTI_PR1_PIF1;
	}
}

int main(void){
	configure_LED_pin();
	configure_PC1_pin();
	configure_EXTI();
	while(1);
		//says ISR latency is 12 cycles
}
