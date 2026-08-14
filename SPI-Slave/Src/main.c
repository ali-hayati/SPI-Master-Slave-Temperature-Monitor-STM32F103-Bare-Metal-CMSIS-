#include "stdio.h"
#include "spi.h"
#include "adc.h"
#include "stm32f103xb.h"
 uint8_t recieved_data=0;
volatile uint8_t dama;
volatile uint16_t adc=0;

int main()
{
	float Temp;
	GPIOA_SPI1_INIT();
	SPI1_INIT();
    adc_PB0_init();
    start_conversion();
    while(1)
    {

      adc=adc_read();
      Temp =adc/8.22f;
      dama=(int)Temp;
      //sprintf(str,"T:%d",dama);
      //usart_write_string(str);
      if (recieved_data==0x01)
      {

    	  SPI_Transmitt(dama);
    	  recieved_data=0;
      }
    }
}
