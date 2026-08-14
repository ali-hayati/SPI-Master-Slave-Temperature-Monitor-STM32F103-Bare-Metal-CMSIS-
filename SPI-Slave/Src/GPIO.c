#include "GPIO.h"
#define LED1_OUTPUT (0x3<<28)
#define LED1_PIN (1U<<15)
void GPIO_Init(void)
{
	RCC->APB2ENR |= GPIOBEN;
	GPIOB->CRH |=LED1_OUTPUT;
	return ;
}
void LED_ON (void)
{
	GPIOA->BSRR |= LED_BSR5;
	return ;
}
 void LED_OFF (void)
 {
	 GPIOA->BSRR &=~(LED_BSR5);
	 GPIOA->BSRR |=LED_BRR5;
	 return ;
 }
 void Button_Init(void)
 {
	RCC->APB2ENR |= GPIOBEN;
	GPIOB->CRH &= ~(GPIOB_PIN13_ConfigurationReset);
	GPIOB->CRH |=  (GPIOB_PIN13_ConfigurationInput);
	GPIOB->ODR |= Btn_Pin;
 }
bool get_btn_state(void)
{
	if (GPIOB->IDR & Btn_Pin)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Toggle_Pin(void)
{
	GPIOB->ODR ^=LED1_PIN;
}
