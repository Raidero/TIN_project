#include "Serialization.h"

unsigned char* serializeInt(unsigned char* buffer, int input)
{
    buffer[0] = input >> 24;
    buffer[1] = input >> 16;
    buffer[2] = input >> 8;
    buffer[3] = input;
    return buffer + sizeof(int);
}
unsigned char* serializeUint_32_t(unsigned char* buffer, uint32_t input)
{
    buffer[0] = input >> 24;
    buffer[1] = input >> 16;
    buffer[2] = input >> 8;
    buffer[3] = input;
    return buffer + sizeof(uint32_t);
}
unsigned char* serializeCharArray(unsigned char* buffer, char* array, int size)
{
    int i;
    for(i = 0; i < size; ++i)
    {
        buffer[i] = array[i];
    }
    return buffer + size;
}

unsigned char* serializeUnsignedCharArray(unsigned char* buffer, unsigned char* array, int size)
{
    int i;
    for(i = 0; i < size; ++i)
    {
        buffer[i] = array[i];
    }
    return buffer + size;
}

unsigned char* serializeAccountData(unsigned char* buffer, AccountData* accountdata)
{
    buffer = serializeUint_32_t(buffer, accountdata->currentip);
    buffer = serializeCharArray(buffer, accountdata->login, MAX_LOGIN_LENGTH);
    buffer = serializeUnsignedCharArray(buffer, accountdata->passwordhash, MAX_PASSHASH_LENGTH);
    return buffer;
}

unsigned char* serializePointer(unsigned char* buffer, int* input)
{
    int address = (int)input;
    buffer[0] = address >> 24;
    buffer[1] = address >> 16;
    buffer[2] = address >> 8;
    buffer[3] = address;
    return buffer + sizeof(int*);
}

unsigned char* deserializeInt(unsigned char* buffer, int* output)
{
    *output = 0;
    *output |= buffer[0] << 24;
    *output |= buffer[1] << 16;
    *output |= buffer[2] << 8;
    *output |= buffer[3];
    return buffer + sizeof(int);
}
unsigned char* deserializeUint_32_t(unsigned char* buffer, uint32_t* output)
{
    *output = 0;
    *output |= buffer[0] << 24;
    *output |= buffer[1] << 16;
    *output |= buffer[2] << 8;
    *output |= buffer[3];
    return buffer + sizeof(uint32_t);
}
unsigned char* deserializeCharArray(unsigned char* buffer, char* array, int size)
{
    int i;
    for(i = 0; i < size; ++i)
    {
        array[i] = buffer[i];
    }
    return buffer + size;
}

unsigned char* deserializeUnsignedCharArray(unsigned char* buffer, unsigned char* array, int size)
{
    int i;
    for(i = 0; i < size; ++i)
    {
        array[i] = buffer[i];
    }
    return buffer + size;
}

unsigned char* deserializeAccountData(unsigned char* buffer, AccountData* accountdata)
{
    buffer = deserializeUint_32_t(buffer, &accountdata->currentip);
    buffer = deserializeCharArray(buffer, accountdata->login, MAX_LOGIN_LENGTH);
    buffer = deserializeUnsignedCharArray(buffer, accountdata->passwordhash, MAX_PASSHASH_LENGTH);
    return buffer;
}

unsigned char* deserializePointer(unsigned char* buffer, int* output)
{
    int address = 0;
    address |= buffer[0] << 24;
    address |= buffer[1] << 16;
    address |= buffer[2] << 8;
    address |= buffer[3];
    *output = address;
    return buffer + sizeof(int*);
}

unsigned char* serializeFloat(unsigned char* buffer, float input)
{
    unsigned int integercast = *((unsigned int*)&input);
    buffer[0] = integercast >> 24;
    buffer[1] = integercast >> 16;
    buffer[2] = integercast >> 8;
    buffer[3] = integercast;
    return buffer + sizeof(unsigned int);
}

unsigned char* deserializeFloat(unsigned char* buffer, float* output)
{
    unsigned int integercast = 0;
    integercast |= buffer[0] << 24;
    integercast |= buffer[1] << 16;
    integercast |= buffer[2] << 8;
    integercast |= buffer[3];
    *output = *((float*)&integercast);
    return buffer + sizeof(unsigned int);
}

unsigned char* serializePlayerData(unsigned char* buffer, PlayerData* playerinfo)
{
    buffer = serializeFloat(buffer, playerinfo->x);
    buffer = serializeFloat(buffer, playerinfo->y);
    buffer = serializeInt(buffer, playerinfo->hp);
    return buffer;
}

unsigned char* deserializePlayerData(unsigned char* buffer, PlayerData* playerinfo)
{
    buffer = deserializeFloat(buffer, &playerinfo->x);
    buffer = deserializeFloat(buffer, &playerinfo->y);
    buffer = deserializeInt(buffer, &playerinfo->hp);
    return buffer;
}
