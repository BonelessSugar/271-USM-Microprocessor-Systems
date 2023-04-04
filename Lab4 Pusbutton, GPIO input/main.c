#include "stm32l476xx.h"

// PA.5  < --> Green LED
// PC.13 <--> Blue user button
// PC.1 <--> Function generator

static char a[] = "abcd ABCD 0123 ()?!\n\t";
unsigned int pTime = 0xF;
	
void configure_LED_pin () {
	// Enable the clock to GPIO Port A
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  RCC -> AHB2ENR |= 0x00000001;
	// Set the GPIO Port A mode to output (01) 
  GPIOA -> MODER &= ~(3UL<<10);
	GPIOA -> MODER |= 1UL<<10; 
	// Set the GPIO Port A output type to push - pull 
  GPIOA -> OTYPER &= ~(1UL<<5);
	// Set the GPIO speed to low
	GPIOA -> OSPEEDR &= (1UL<<10); 
	// Set GPIO Port A Push - Pull to no pull - up, no pull - down
  GPIOA -> PUPDR &= ~(3<<10);
}

void configure_Button_pin (){
	// Enable the clock to GPIO Port C
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
  RCC -> AHB2ENR |= 0x00000001;
	// GPIOC Mode: Input(00)
	GPIOC -> MODER &= ~(3UL<<2);
	// GPIOC Push-Pull: No pull-up, no pull-down (00)
	GPIOC -> PUPDR &= ~(3<<2); 
}

void on_LED(){
	GPIOA -> ODR |= 1<<5;
}

void off_LED(){
	GPIOA -> ODR &= ~ (1<<5);
}

void blink_LED() {
	int i;
	for (i = 0; i < 125000 / 2; i++);
	GPIOA -> ODR ^= 1UL<<5;
}

void toggle_LED() {
	GPIOA -> ODR ^= 1UL<<5;
}

int main (void) {
  configure_LED_pin();
	configure_Button_pin();
	pTime = 0;
	
	while (GPIOC -> IDR & (1<<1)){
		
	}
	while ((GPIOC -> IDR & (1<<1)) == 0x00){
		
	}
	while (GPIOC -> IDR & (1<<1)){
		on_LED();
		pTime++;
	}
	while ((GPIOC -> IDR & (1<<1)) == 0x00){
		off_LED();
		pTime++;
	}
}
