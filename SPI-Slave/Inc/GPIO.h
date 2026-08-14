#ifndef GPIO_H

#define GPIO_H

#include "stm32f103xb.h"
#include "stdbool.h"
#define GPIOAEN (1U<<2)
#define LED_BSR5 (1U<<5)
#define LED_BRR5 (1U<<21)
#define GPIOA_MOD5 (1U<<20)
#define GPIOA_MOD4 (1U<<16)
#define GPIOBEN  (1U<<3)
#define Btn_Pin (1U<<13)
#define GPIOB_PIN13_ConfigurationReset (0xF<<20)
#define GPIOB_PIN13_ConfigurationInput (0x8<<20)
#define LED_PIN (1U<<5)
#define GPIOA_PWMCH1 (0xb<<0)
void GPIO_Init(void);
void LED_ON (void);
void LED_OFF (void);
void Button_Init(void);
bool get_btn_state (void);
void Toggle_Pin(void);
#endif
