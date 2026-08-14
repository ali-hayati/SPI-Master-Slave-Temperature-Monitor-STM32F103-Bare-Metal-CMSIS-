#ifndef ADC_HAL
	#define ADC_HAL
	#include "stm32f103xb.h"
	void adc_PB0_init(void);
	void start_conversion (void);
	uint16_t adc_read(void);
#endif
