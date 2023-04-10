#include <stm32l476xx.h>

// Reads the configured ADC channel and returns its value.
unsigned int adc_read(void);

// Delays for approximately the given amount of microseconds.
void delay_us(volatile int);

int main(void)
{

    // Use the CCIPR register to select the system clock as a source for the
    // ADC clock, then enable the ADC clock through the AHB.
    RCC->CCIPR &= ~(RCC_CCIPR_ADCSEL_Msk);
    RCC->CCIPR |= 3 << RCC_CCIPR_ADCSEL_Pos;
    RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;

    // Enable the ADC's internal voltage regulator, with a delay for its
    // startup time.
    // ADVREGEN requires some bits in the CR register to be zero before being
    // set, so we clear CR to zero first.
    ADC1->CR = 0;
    ADC1->CR = ADC_CR_ADVREGEN;
    delay_us(20);

    // Clear DIFSEL to do single-ended conversions.
    ADC1->DIFSEL = 0;

    // Begin a single-ended calibration and wait for it to complete.
    // If the clock source for the ADC is not correctly configured, the
    // processor may get stuck in the while loop.
    ADC1->CR &= ~(ADC_CR_ADCALDIF); // Select single-ended calibration
    ADC1->CR |= ADC_CR_ADCAL;
    while ((ADC1->CR & ADC_CR_ADCAL) != 0);

    // Enable the ADC and wait for it to be ready.
    ADC1->CR |= ADC_CR_ADEN;
    while ((ADC1->ISR & ADC_ISR_ADRDY) == 0);

    // GPIO pin PC3 will be used as our ADC input.
    // In order: enable the GPIOC clock, set PC3 to analog input mode,
    // connect ADC channel one to its GPIO pin (PC3).
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    GPIOC->MODER |= 3 << GPIO_MODER_MODE0_Pos;
    GPIOC->ASCR |= 1 << 0;

    // Make channel one the first in the ADC's read sequence.
    // Using '=' here also clears the L field of the register, giving the
    // sequence a length of one.
    ADC1->SQR1 = 1 << ADC_SQR1_SQ1_Pos;

    // Configure the Nucleo's LED (pin PA5) for output.
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    GPIOA->MODER &= ~(GPIO_MODER_MODE5_Msk);
    GPIOA->MODER |= 1 << GPIO_MODER_MODE5_Pos;

    // In a loop, read from the ADC channel and set the LED according to
    // the result.
    while (1) {
        unsigned int reading = adc_read();

        if (reading > 2048)
            GPIOA->BSRR |= 1 << 5;
        else
            GPIOA->BRR |= 1 << 5;
    }
}

unsigned int adc_read(void)
{
    ADC1->CFGR &= ~(ADC_CFGR_CONT);         // Single conversion
    ADC1->CR |= ADC_CR_ADSTART;             // Start converting
    while ((ADC1->ISR & ADC_ISR_EOC) == 0); // Wait for end-of-conversion
    ADC1->ISR &= ~(ADC_ISR_EOC);            // Clear the status flag
    return ADC1->DR;                        // Read the result
}

void delay_us(volatile int amount)
{
    // This loop should take around a microsecond to execute per iteration
    // when running at the default speed of 4MHz.
    while (amount > 0) {
        --amount;
    }
}

