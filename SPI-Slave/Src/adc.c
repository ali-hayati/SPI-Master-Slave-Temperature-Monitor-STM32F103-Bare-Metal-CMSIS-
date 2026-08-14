#include "adc.h"
#define ADC1EN              (1U << 9)
#define GPIOBEN             (1U << 3)
#define GPIOA_PIN4_ANALOG   (0xFU << 16)
#define ADC1_CH8            (8U << 0)
#define ADC_SEQ_LEN_1       0x00U
#define CR2_ADCON           (1U << 0)
#define CR2_CONT            (1U << 1)
#define CR2_CAL             (1U << 2)
#define CR2_RSTCAL          (1U << 3)
#define CR2_SWSTART         (1U << 22)
#define SR_EOC              (1U << 1)
#define SR_STRT             (1U << 4)
#define CR2_EXTTRIG         (1U << 20)
void adc_PB0_init(void)
{
    /* Enable GPIOA clock */
    RCC->APB2ENR |= GPIOBEN;
    /* PA4 -> Analog input */
    GPIOB->CRL &= ~(0xFU << 0);
    /* Enable ADC1 clock */
    RCC->APB2ENR |= ADC1EN;
    /* Enable ADC */
    ADC1->CR2 |= CR2_ADCON;
    /* Select channel 8 */
    ADC1->SQR3 = ADC1_CH8;
    /* One conversion */
    ADC1->SQR1 = ADC_SEQ_LEN_1;
    /*
     * Enable continuous conversion
     */
    ADC1->CR2 |= CR2_CONT;
    /*
     * Enable external/software trigger
     */
    ADC1->CR2 |= CR2_EXTTRIG;
    /*
     * Reset calibration
     */
    ADC1->CR2 |= CR2_RSTCAL;

    while (ADC1->CR2 & CR2_RSTCAL)
    {
    }


    /*
     * Start calibration
     */
    ADC1->CR2 |= CR2_CAL;

    while (ADC1->CR2 & CR2_CAL)
    {
    }
    /* Enable ADC again */
    ADC1->CR2 |= CR2_ADCON;
}

void start_conversion(void)
{

    ADC1->CR2 |= CR2_SWSTART;
}
uint16_t adc_read(void)
{
	/*Wait for conversion to be complete*/
	while(!(ADC1->SR & SR_EOC));

	/*Read converted value*/
	return (ADC1->DR);
}

