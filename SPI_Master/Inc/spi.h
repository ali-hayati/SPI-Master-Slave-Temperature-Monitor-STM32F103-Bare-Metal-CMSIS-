#ifndef SPI_HAL
#define SPI_HAL
#include "stm32f103xb.h"
#include "Uart.h"
//extern char recieved_data[10];
void SPI1_INIT(void);
void GPIOA_SPI1_INIT(void);
uint8_t SPI_Read_Teamprature(void);
uint8_t SPI_TransmitRecieved(uint8_t data);
void error_SPI(char *p);
#endif
