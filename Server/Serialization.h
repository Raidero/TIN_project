#ifndef SERIALIZATION_H
#define SERIALIZATION_H
#include "AccountService.h"
#include <inttypes.h>

unsigned char* serializeInt(unsigned char* buffer, int input);
unsigned char* serializeUint_32_t(unsigned char* buffer, uint32_t input);
unsigned char* serializeCharArray(unsigned char* buffer, char* array, int size);
unsigned char* serializeUnsignedCharArray(unsigned char* buffer, unsigned char* array, int size);
unsigned char* serializeAccountData(unsigned char* buffer, AccountData* accountdata);

unsigned char* deserializeInt(unsigned char* buffer, int *output);
unsigned char* deserializeUint_32_t(unsigned char* buffer, uint32_t *output);
unsigned char* deserializeCharArray(unsigned char* buffer, char* array, int size);
unsigned char* deserializeUnsignedCharArray(unsigned char* buffer, unsigned char* array, int size);
unsigned char* deserializeAccountData(unsigned char* buffer, AccountData* accountdata);

#endif // SERIALIZATION_H





