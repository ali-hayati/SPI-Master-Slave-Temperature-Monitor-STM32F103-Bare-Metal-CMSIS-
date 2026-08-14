#include "spi.h"

//#define GPIOAEN             (1U << 2)
#define SPI1EN              (1U << 12)

/* GPIO configuration */
#define GPIOA_PIN4_NSS      (0x4U << 16)
#define GPIOA_PIN5_SCK      (0x4U << 20)   /* PA5: Alternate Function Push-Pull, 50 MHz */
#define GPIOA_PIN6_MISO     (0xBU << 24)   /* PA6: Input Floating */
#define GPIOA_PIN7_MOSI     (0x4U << 28)   /* PA7: Alternate Function Push-Pull, 50 MHz */

/* SPI CR1 */
#define CPOL                (1U << 1)
#define CPHA                (1U << 0)
#define MASTER_EN           (1U << 2)

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

#define CMD_READ_TEMP 0x01


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
    /* Enable SPI1 clock */
    RCC->APB2ENR |= SPI1EN;

    /*
     * SPI Mode 0
     */
    SPI1->CR1 &= ~(CPOL | CPHA);

    /*
     * Configure SPI as Slave
     */
    SPI1->CR1 &= ~MASTER_EN;

    /*
     * Slave does NOT configure baud rate.
     * Master generates SCK.
     */

    /*
     * MSB first
     */
    SPI1->CR1 &= ~LSBFIRST;

    /*
     * 8-bit data frame
     */
    SPI1->CR1 &= ~DFF_MASK;
    SPI1->CR1 |= DFF_8BIT;

    /*
     * Hardware NSS management
     *
     * SSM = 0
     */
    SPI1->CR1 &= ~SSM;

    /*
     * Enable RXNE interrupt
     */
    SPI1->CR2 |= RXNEIE;

    /*
     * Enable SPI1 interrupt
     */
    NVIC_EnableIRQ(SPI1_IRQn);

    /*
     * Enable SPI
     */
    SPI1->CR1 |= SPI_EN;
}

void SPI_Transmitt(uint8_t temp)
{
	NVIC_DisableIRQ(SPI1_IRQn);
	while (!(SPI1->SR & (TXE_FLAG)));
	SPI1->DR =temp;
	NVIC_EnableIRQ(SPI1_IRQn);
}

void SPI1_IRQHandler(void)
{
    uint8_t received;
    if (SPI1->SR & RXNE_FLAG)
    {

        /*
         * Read received byte
         */
        received = SPI1->DR;

        recieved_data = received;

        /*
         * Check command
         */
        if (received == CMD_READ_TEMP)
        {

            /*
             * Prepare first temperature byte.
             *
             * Example:
             * temperature = 253
             *
             * 253 = 0x00FD
             */

            SPI1->DR = dama;
        }
        else
        {

            /*
             * This is the next dummy byte.
             *
             * Send low byte.
             */
            SPI1->DR = (dama & 0xFF);
        }
    }


    /*
     * Check Overrun
     */
    if (SPI1->SR & OVR_FLAG)
    {
        /*
         * Clear OVR:
         *
         * Read DR
         * then read SR
         */
        (void)SPI1->DR;
        (void)SPI1->SR;
    }
    //GPIOB->ODR |=(1U<<15);
}

