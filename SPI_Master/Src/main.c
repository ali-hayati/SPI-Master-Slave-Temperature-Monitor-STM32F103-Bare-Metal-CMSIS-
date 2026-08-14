#include "Uart.h"
#include "stdio.h"
#include "spi.h"

void Clear_Buffer(char *str);
#define RX_BUFFER_SIZE  10
char recieved_data[RX_BUFFER_SIZE];
uint8_t Row=0;
uint8_t Temp;
int main()
{
	char str1[RX_BUFFER_SIZE];
	USART2_INIT();
	GPIOA_SPI1_INIT();
	SPI1_INIT();
	while (1)
	{
	char received_char;

	received_char = Read_Uasrt();

	/* Echo received character */
	usart_Write_char(received_char);

	/* Check for Enter */
	if (received_char == '\r' || received_char == '\n')
	{
		/* Terminate string */
		recieved_data[Row] = '\0';

		/* Check command */
		if (strcmp(recieved_data, "start") == 0)
		{
			Temp = SPI_Read_Teamprature();

			sprintf(str1, "T:%d\r\n", Temp);

			usart_write_string(str1);
		}

		/* Reset buffer */
		Row = 0;
		Clear_Buffer(recieved_data);
	}
	else
	{
		/* Store character */
		if (Row < RX_BUFFER_SIZE - 1)
		{
			recieved_data[Row] = received_char;
			Row++;
		}
		else
		{
			/* Buffer overflow */
			Row = 0;
			Clear_Buffer(recieved_data);

			usart_write_string("\r\nBuffer Overflow\r\n");
		}
	}
	}
}
void Clear_Buffer(char *str)
{
	for (int i=0; i<RX_BUFFER_SIZE;i++)
	{
		str[i]='\0';
	}
}
