#include <avr/io.h>
#include "spi.h"

/*
Arduino Nano / ATmega328P SPI pins:
- D10 = PB2 = SS   (used as MFRC522 SDA/SS)
- D11 = PB3 = MOSI
- D12 = PB4 = MISO
- D13 = PB5 = SCK
- D9  = PB1 = RST  (handled in main/mfrc522 driver)
*/

void spi_init(void)
{
    /* MOSI, SCK, SS as output. MISO as input. */
    DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB5);
    DDRB &= ~(1 << PB4);

    /* Keep SS high when idle. */
    PORTB |= (1 << PB2);

    /* Enable SPI, Master, Fosc/16 = 1 MHz at 16 MHz CPU. */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
    SPSR = 0x00;
}

uint8_t spi_transfer(uint8_t data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF)))
    {
    }
    return SPDR;
}

// //void SPI_Init(void) {
//     // Set MOSI (PB3), SCK (PB5), and SS (PB2) as outputs
//     DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);
//     // Set MISO (PB4) as input
//     DDRB &= ~(1 << DDB4);
    
//     // Enable SPI, Master mode, set clock rate f_OSC / 4
//     SPCR |= (1 << SPE) | (1 << MSTR);
// }

// uint8_t SPI_Transfer(uint8_t data) {
//     SPDR = data;
//     while (!(SPSR & (1 << SPIF))); // Wait for transmission complete
//     return SPDR;
// }
