# 1. Reading ADC inputs through polling

## ADC configuration and initialization

### 1. Starting the ADC clock

First, the ADC clock's source must be selected since it defaults to none. We
will use the system clock as our source; if we were using the PLL, we could use
one of the PLL's "R" clocks instead.

```cpp
RCC->CCIPR &= ~(RCC_CCIPR_ADCSEL_Msk);
RCC->CCIPR |= 3 << RCC_CCIPR_ADCSEL_Pos; // Select system clock for ADC clock
```

Then, we simply enable the ADC clock through the AHB:
```cpp
RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
```

### 2. Power on the ADC's internal voltage regulator

The ADC's internal regulator must be powered on before enabling the ADC. If the
ADC is disabled later, this regulator can be turned off to save power.

```cpp
ADC1->CR = 0; // The CR register must be zero before setting ADVREGEN
ADC1->CR = ADC_CR_ADVREGEN;
```

The internal regulator should be given time to warm up after turning on so that
its output stabilizes. A delay of around 20 microseconds should be enough; a
very crude delay function is implemented in the example.

### 3. Calibrating the ADC

The ADC should be calibrated when first powered on. The calibration process is
something internal (undocumented) to the microcontroller, so we just tell the
calibration to begin and wait for it to finish.

The microcontroller has the option to do a differential calibration, but we
will skip this since we are only using single-ended channels. This is also a
good time to select single-ended channels for our conversions.

```cpp
ADC1->DIFSEL = 0; // Clear DIFSEL for single-ended conversions.

ADC1->CR &= ~(ADC_CR_ADCALDIF); // Select single-ended calibration
ADC1->CR |= ADC_CR_ADCAL;       // Begin calibration
while ((ADC1->CR & ADC_CR_ADCAL) != 0); // Wait for ADCAL to clear
```

If execution gets stuck in the `while` loop, there is a chance that the ADC's
clock was incorrectly configured.

### 4. Enable the ADC

Now, the ADC is ready to be enabled:

```cpp
ADC1->CR |= ADC_CR_ADEN;
while ((ADC1->ISR & ADC_ISR_ADRDY) == 0); // Wait for the ADC to be ready
```

## Prepare GPIO pin for analog reading

We will use pin PC0 since it is wired to the first ADC channel. See the
STM32L476RG datasheet for a table of connections between pins and ADC channels.

Make sure the GPIO port's clock is enabled:

```cpp
RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
```

Then, set the GPIO pin's mode to analog input:

```cpp
GPIOC->MODER |= 3 << GPIO_MODER_MODE0_Pos;
```

Finally, STM32L47x/L48x processors require an additional register setting to
complete the pin's connection to the ADC. We set bit zero for ADC channel one:

```cpp
GPIOC->ASCR |= 1 << 0;
```

## Prepare and execute the analog conversion

### Conversion sequencing

First we need to set up the ADC's conversion sequence. The ADC can take
multiple conversions in a row, with channels arranged in order according to
the `SQR` registers.

We are doing a single conversion of a single channel, channel one. So, we
write a `1` for the first sequence channel. By setting the whole register with
`=`, we clear the L field of the register to zero; this sets the sequence length
to one.

```cpp
ADC1->SQR1 = 1 << ADC_SQR1_SQ1_Pos;
```

### Other configuration

The `CFGR` register can be used to adjust the ADC's resolution (8 to 12 bits).

The `CFGR2` register can be configured if hardware oversampling will be used.

The `SMPR` registers can be configured to select sampling times. We will leave
this unconfigured to use the default (fastest) time.

The `OFR` register can be configured to have an offset subtracted from the
analog reading during conversion.

### Running the conversion

Below are the steps for completing a single channel conversion. In the example,
this code is contained within the `adc_read` function.

Set `CFGR` for single conversion, then initiate the conversion:

```cpp
ADC1->CFGR &= ~(ADC_CFGR_CONT);

ADC1->CR |= ADC_CR_ADSTART;
```

The ADC will set the end-of-conversion (EOC) bit of its status register `ISR`
once the conversion is complete. If we were using interrupts, we could write
and enable an interrupt handler to detect this; instead, we will simply poll
the register.

Once the EOC bit is set, we need to clear it before the next conversion.

```cpp
while ((ADC1->ISR & ADC_ISR_EOC) == 0);
ADC1->ISR &= ~(ADC_ISR_EOC);
```

The conversion result is now available in the data register `DR`.

## Getting feedback with an LED

We will use the Nucleo's on-board LED to produce feedback on our ADC
reading. The below code will turn the LED on or off depending on if the
ADC reading goes above a certain threshold.

Configure pin PA5 for output to control the LED:

```cpp
RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
GPIOA->MODER &= ~(GPIO_MODER_MODE5_Msk);
GPIOA->MODER |= 1 << GPIO_MODER_MODE5_Pos;
```

Now, we run a simple loop. Our threshold will be 2048, half of the ADC's 12-
bit range. With the default voltage reference of 3.3V, the threshold will be
equal to `3.3V * 2048 / (2^12 - 1) = 1.65V`.

```cpp
while (1) {
    unsigned int reading = adc_read();

    if (reading > 2048)
        GPIOA->BSRR |= 1 << 5;
    else
        GPIOA->BRR |= 1 << 5;
}
```

