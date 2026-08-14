#ifndef UART_HAL
#define UART_HAL
#include "stm32f103xb.h"
#include "GPIO.h"
#include  <string.h>
void USART2_INIT(void);
void usart_write(int ch);
void usart_write_string (char *str);
char Read_Uasrt(void);
void usart_Write_char (char ch);
#endif
