/*
File contains the code for "Tiny Encryption Algorithm (TEA)"

*/


#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "TEA.h"
#include "NxUDP-client.h"
#include "NxUDP_pkt_parser.h"

 uint32_t * Format_key(uint8_t * key);
 void encrypt(uint32_t* v, uint32_t* k);
 void decrypt(uint32_t* v, uint32_t* k);

 
 
uint8_t key_buff[16];
uint32_t Encryptd_data[ENCRYPTION_BUFF_SIZE];
uint8_t Decryptd_data[ENCRYPTION_BUFF_SIZE];
uint32_t Data_len = 0;


/*
*Function Encrypts data using Encryption key passed as argument
*and returns Encrypted buffer.
*/
uint8_t * Do_TEA_Encryption(uint8_t * data, uint32_t size, uint8_t * key)
{
	uint32_t  len;
	uint32_t * key32;
	uint32_t blocks, i;
	uint32_t * data32;
	len = size;
	memset(Encryptd_data,0,10000); 
	// treat the data as 32 bit unsigned integers
	data32 = (uint32_t *)data;
	
	// treat the data as 32 bit unsigned integers
	key32 = Format_key(key);


	Data_len = 0;
	for (i = 0; i< len; i+=2)
	{
		uint32_t temp_buff[2];
		temp_buff[0] = data[i];
		temp_buff[1] = data[i+1];
		encrypt(&temp_buff, key32);
		Encryptd_data[Data_len] = temp_buff[0];
		Encryptd_data[Data_len + 1] = temp_buff[1];
		Data_len += 2;
	}
	
	return (uint8_t *)Encryptd_data;
}
/*
*Function Decrypts data using Decryption key passed as argument
*and returns Decrypted buffer.
*/
uint8_t * Do_TEA_Decryption(uint8_t * data, uint32_t size, uint8_t * key)
{
	uint32_t  len;
	uint32_t blocks, i;
	uint32_t * data32;
	uint32_t * key32;
	len = size;
	memset(Decryptd_data,0,ENCRYPTION_BUFF_SIZE);
	//len = strlen(data);
	// treat the data as 32 bit unsigned integers
	data32 = (uint32_t *)data;
	
	// treat the data as 32 bit unsigned integers
	key32 = Format_key(key);

	// Find the number of 8 byte blocks
	blocks = ((len) / 4);

	Data_len = 0;
	for (i = 0; i<=blocks; i += 2)
	{
		uint32_t temp_buff[2];
		temp_buff[0] = data32[i];
		temp_buff[1] = data32[i + 1];
		decrypt(&temp_buff, key32);
		Decryptd_data[Data_len] = (uint8_t)temp_buff[0];
		Decryptd_data[Data_len + 1] = (uint8_t)temp_buff[1];
		Data_len += 2;
	}
	Decryptd_data[blocks]=NULL;

	return Decryptd_data;
}

/// Format key as 32bit value
uint32_t * Format_key(uint8_t * key)
{
	int len = sizeof(nxudp_pkt.cy_key);

	memset(key_buff, ' ', 16);
	for (int i = 0; i < len; i++)
	{
		key_buff[i] = *key++;
	}
	return (uint32_t *)key_buff;
}

#pragma region TEA BLOCK
///TEA encryption Block

void encrypt(uint32_t* v, uint32_t* k) {
	uint32_t v0 = (uint32_t)v[0], v1 = (uint32_t)v[1], sum = 0, i;           /* set up */
	uint32_t delta = 0x9e3779b9;                     /* a key schedule constant */
	uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];   /* cache key */
	for (i = 0; i < 32; i++) {                       /* basic cycle start */
		sum += delta;
		v0 += ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
		
		v1 += ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
	}                                              /* end cycle */
	v[0] = v0; v[1] = v1;
}

///TEA Decryption Block

void decrypt(uint32_t* v, uint32_t* k) {
	uint32_t v0 = v[0], v1 = v[1], sum = 0xC6EF3720, i;  /* set up */
	uint32_t delta = 0x9e3779b9;                     /* a key schedule constant */
	uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];   /* cache key */
	for (i = 0; i<32; i++) {                         /* basic cycle start */
		v1 -= ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
		
		v0 -= ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
		sum -= delta;
	}                                              /* end cycle */
	v[0] = v0; v[1] = v1;
}

#pragma endregion TEA BLOCK
