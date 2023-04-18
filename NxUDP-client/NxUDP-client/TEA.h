#ifndef TEA_H
#define TEA_H
#include <stdint.h>

uint8_t * Do_TEA_Encryption(uint8_t * data, uint32_t size, uint8_t * key);
uint8_t * Do_TEA_Decryption(uint8_t * data, uint32_t size, uint8_t * key);

#endif