#ifndef SERIALIZATION_H
#define SERIALIZATION_H
#include "AccountService.h"
#include "RoomService.h"
#include <inttypes.h>

unsigned char* serializeInt(unsigned char* buffer, int input);
unsigned char* serializeUint_32_t(unsigned char* buffer, uint32_t input);
unsigned char* serializeCharArray(unsigned char* buffer, char* array, int size);
unsigned char* serializeUnsignedCharArray(unsigned char* buffer, unsigned char* array, int size);
unsigned char* serializeAccountData(unsigned char* buffer, AccountData* accountdata);
unsigned char* serializePointer(unsigned char* buffer, int* input);
unsigned char* serializeFloat(unsigned char* buffer, float input);
unsigned char* serializePlayerData(unsigned char* buffer, PlayerData* playerinfo);

unsigned char* deserializeInt(unsigned char* buffer, int *output);
unsigned char* deserializeUint_32_t(unsigned char* buffer, uint32_t *output);
unsigned char* deserializeCharArray(unsigned char* buffer, char* array, int size);
unsigned char* deserializeUnsignedCharArray(unsigned char* buffer, unsigned char* array, int size);
unsigned char* deserializeAccountData(unsigned char* buffer, AccountData* accountdata);
unsigned char* deserializePointer(unsigned char* buffer, int* input);
unsigned char* deserializeFloat(unsigned char* buffer, float* output);
unsigned char* deserializePlayerData(unsigned char* buffer, PlayerData* playerinfo);

#endif // SERIALIZATION_H





