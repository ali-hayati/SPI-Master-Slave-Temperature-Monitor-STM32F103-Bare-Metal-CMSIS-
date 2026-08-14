#include "spi.h"

#define GPIOAEN             (1U << 2)
#define SPI1EN              (1U << 12)

/* GPIO configuration */
#define GPIOA_PIN4_NSS      (0x2U << 16)   /* PA4: General Purpose Output Push-Pull, 2 MHz */
#define GPIOA_PIN5_SCK      (0xBU << 20)   /* PA5: Alternate Function Push-Pull, 50 MHz */
#define GPIOA_PIN6_MISO     (0x4U << 24)   /* PA6: Input Floating */
#define GPIOA_PIN7_MOSI     (0xBU << 28)   /* PA7: Alternate Function Push-Pull, 50 MHz */

/* SPI CR1 */
#define CPOL                (1U << 1)
#define CPHA                (1U << 0)
#define MASTER_EN           (1U << 2)

#define BR0                 (1U << 3)
#define BR1                 (1U << 4)
#define BR2                 (1U << 5)
#define BR_MASK             (BR0 | BR1 | BR2)

#define SPI_BAUD_DIV2       (0U << 3)
#define SPI_BAUD_DIV4       (1U << 3)
#define SPI_BAUD_DIV8       (2U << 3)
#define SPI_BAUD_DIV16      (3U << 3)
#define SPI_BAUD_DIV32      (4U << 3)
#define SPI_BAUD_DIV64      (5U << 3)
#define SPI_BAUD_DIV128     (6U << 3)
#define SPI_BAUD_DIV256     (7U << 3)

#define LSBFIRST            (1U << 7)
#define MSBFIRST            (0U << 7)

#define SSI                 (1U << 8)
#define SSM                 (1U << 9)

#define DFF_MASK            (1U << 11)
#define DFF_8BIT            (0U << 11)
#define DFF_16BIT           (1U << 11)

#define SPI_EN              (1U << 6)

/* SPI CR2 */
#define RXNEIE              (1U << 6)

/* SPI SR */
#define RXNE_FLAG           (1U << 0)
#define TXE_FLAG            (1U << 1)
#define OVR_FLAG            (1U << 6)
#define BSY_FLAG            (1U << 7)

#define CMD_READ_TEAMP 0x01

void GPIOA_SPI1_INIT(void)
{
    /* Enable clock access to GPIOA */
    RCC->APB2ENR |= GPIOAEN;

    /*
     * Configure:
     * PA4 -> GPIO Output Push-Pull (NSS / CS)
     * PA5 -> Alternate Function Push-Pull (SPI1_SCK)
     * PA6 -> Input Floating (SPI1_MISO)
     * PA7 -> Alternate Function Push-Pull (SPI1_MOSI)
     */

    GPIOA->CRL &= ~(
            (0xFU << 16) |
            (0xFU << 20) |
            (0xFU << 24) |
            (0xFU << 28)
            );

    GPIOA->CRL |= (
            GPIOA_PIN4_NSS |
            GPIOA_PIN5_SCK |
            GPIOA_PIN6_MISO |
            GPIOA_PIN7_MOSI
            );

    /* CS inactive */
    GPIOA->ODR |= (1U << 4);
}


void SPI1_INIT(void)
{
    /* Enable clock access to SPI1 */
    RCC->APB2ENR |= SPI1EN;

    /*
     * SPI Mode 0:
     * CPOL = 0
     * CPHA = 0
     */
    SPI1->CR1 &= ~(CPOL | CPHA);

    /* Configure SPI as Master */
    SPI1->CR1 |= MASTER_EN;

    /* Set SPI clock to PCLK / 4 */
    SPI1->CR1 &= ~BR_MASK;
    SPI1->CR1 |= SPI_BAUD_DIV4;

    /* Set MSB first */
    SPI1->CR1 &= ~LSBFIRST;

    /* Set 8-bit data frame */
    SPI1->CR1 &= ~DFF_MASK;
    SPI1->CR1 |= DFF_8BIT;

    /*
     * Software Slave Management
     * SSM = 1
     * SSI = 1
     */
    SPI1->CR1 |= (SSM | SSI);


    /* Enable SPI1 */
    SPI1->CR1 |= SPI_EN;
}


uint8_t SPI_TransmitRecieved(uint8_t data)
{
	uint8_t recieved_data;

	while(!(SPI1->SR & (TXE_FLAG)));
	SPI1->DR = data;

	while (!(SPI1->SR & (RXNE_FLAG)));
	recieved_data=SPI1->DR;
	return recieved_data;
}

uint8_t SPI_Read_Teamprature(void)
{
	usart_write_string("SPI OK\r");
	uint8_t Temp;
	GPIOA->ODR &=~(1<<4);
	SPI_TransmitRecieved(CMD_READ_TEAMP);
	Temp =SPI_TransmitRecieved(0x00);

	Temp =SPI_TransmitRecieved(0x00);
	while(SPI1->SR & (BSY_FLAG));
	GPIOA->ODR |=(1<<4);
    return Temp;
}

void error_SPI(char *p)
{
    usart_write_string(p);
}
