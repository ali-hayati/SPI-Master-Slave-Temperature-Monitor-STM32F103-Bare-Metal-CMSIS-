#ifndef SPI_HAL
#define SPI_HAL
#include "stm32f103xb.h"
#include "Uart.h"
extern uint8_t recieved_data;
extern volatile uint8_t dama;

void SPI1_INIT(void);
void GPIOA_SPI1_INIT(void);
void SPI1_IRQHandler(void);
void SPI_Transmitt(uint8_t temp);
#endif
