#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"



void MFRC522_Write(uint8_t reg, uint8_t value) {
    PORTB &= ~(1 << PB2); // Pull SS LOW to select the reader
    spi_transfer((reg << 1) & 0x7E); // Address format for writing
    spi_transfer(value);
    PORTB |= (1 << PB2); // Pull SS HIGH to deselect
}

uint8_t MFRC522_Read(uint8_t reg) {
    uint8_t temp;
    PORTB &= ~(1 << PB2); // Pull SS LOW
    spi_transfer(((reg << 1) & 0x7E) | 0x80); // Address format for reading
    temp = spi_transfer(0x00); // Send dummy byte to read data
    PORTB |= (1 << PB2); // Pull SS HIGH
    return temp;
}

void MFRC522_Init(void) {
    // Hard Reset the MFRC522
    PORTB &= ~(1 << PB0); // RST LOW
    _delay_ms(10);
    PORTB |= (1 << PB0); // RST HIGH
    _delay_ms(50);
    
    // Soft Reset Command
    MFRC522_Write(0x01, 0x0F); // CommandReg: SoftReset
    _delay_ms(50);
    
    // Configure Timer
    MFRC522_Write(0x2A, 0x80); // TModeReg: Auto-reload
    MFRC522_Write(0x2B, 0x00); // TPrescalerReg
    MFRC522_Write(0x2C, 0xA9); // TReloadRegL
    MFRC522_Write(0x2D, 0x03); // TReloadRegH
    MFRC522_Write(0x2E, 0x80); // TxModeReg: 106 kbps
    MFRC522_Write(0x2F, 0x80); // RxModeReg
    
    // Turn on antenna
    uint8_t val = MFRC522_Read(0x14); // TxAutoReg
    if ((val & 0x03) != 0x03) {
        MFRC522_Write(0x14, val | 0x03);
    }
}

uint8_t MFRC522_Request(uint8_t *atqa)
{
    // Idle+
    MFRC522_Write(0x01, 0x00);

    // Clear interrupts
    MFRC522_Write(0x04, 0x7F);

    // Flush FIFO
    MFRC522_Write(0x0A, 0x80);

    // Write REQA command
    MFRC522_Write(0x09, 0x26);

    // 7 bits for REQA
    MFRC522_Write(0x0D, 0x07);

    // Transceive
    MFRC522_Write(0x01, 0x0C);

    // StartSend
    MFRC522_Write(0x0D, 0x87);

    // Wait
    _delay_ms(5);

    uint8_t irq = MFRC522_Read(0x04);

    if (!(irq & 0x30))
        return 1;

    uint8_t error = MFRC522_Read(0x06);

    if (error & 0x1B)
        return 2;

    // Read first ATQA byte
    *atqa = MFRC522_Read(0x09);

    return 0;
}

uint8_t MFRC522_ReadUID(uint8_t *uid)
{
    // Stop current command
    MFRC522_Write(0x01, 0x00);

    // Clear interrupts
    MFRC522_Write(0x04, 0x7F);

    // Flush FIFO
    MFRC522_Write(0x0A, 0x80);

    // Anti-collision command
    MFRC522_Write(0x09, 0x93);
    MFRC522_Write(0x09, 0x20);

    // No special bit framing
    MFRC522_Write(0x0D, 0x00);

    // Transceive
    MFRC522_Write(0x01, 0x0C);

    // Start transmission
    MFRC522_Write(0x0D, 0x80);

    // Wait for response
    _delay_ms(5);

    // Check IRQ
    uint8_t irq = MFRC522_Read(0x04);

    if (!(irq & 0x30))
        return 1;

    // Check errors
    uint8_t error = MFRC522_Read(0x06);

    if (error & 0x1B)
        return 2;

    // Read 5 bytes from FIFO
    for (uint8_t i = 0; i < 5; i++)
    {
        uid[i] = MFRC522_Read(0x09);
    }

    return 0;
}




// uint8_t MFRC522_Request(uint8_t reqCode, uint8_t *bufferATQA) {
//     uint8_t status;
//     uint8_t backLen;
    
//     MFRC522_Write(0x01, 0x00); // Clear the CommandReg
//     MFRC522_Write(0x0D, 0x07); // BitFramingReg: Start Send
//     MFRC522_Write(0x02, reqCode); // FIFO data to transmit (ReqA)
//     MFRC522_Write(0x01, 0x0C); // PCD_Transceive command

//     // Wait until the transceiver finishes (poll interrupt register)
//     _delay_ms(50); 
    
//     uint8_t irq = MFRC522_Read(0x04);
//     if (irq & 0x20) { // Error or Collision detected
//         status = 1;
//     } else {
//         status = 0; // Success
//         *bufferATQA = MFRC522_Read(0x3A); // FIFO level, read out ATQA 
//     }
//     return status;
// }
