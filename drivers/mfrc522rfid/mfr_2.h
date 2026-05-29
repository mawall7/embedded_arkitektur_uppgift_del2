#ifndef MFR2_H
#define MFR_2
#include <stdio.h>

void MFRC522_Init(void);
void MFRC522_Write(uint8_t reg, uint8_t value);
uint8_t MFRC522_Read(uint8_t reg);
uint8_t MFRC522_Request(uint8_t reqCode);
uint8_t MFRC522_ReadUID(uint8_t *uid);

#endif