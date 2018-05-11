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

int serializeAccountData(unsigned char* buffer, AccountData* accountdata)
{
    unsigned char* start = buffer;
    buffer = serializeUint_32_t(buffer, accountdata->currentip);
    buffer = serializeCharArray(buffer, accountdata->login, MAX_LOGIN_LENGTH);
    buffer = serializeUnsignedCharArray(buffer, accountdata->passwordhash, MAX_PASSHASH_LENGTH);
    buffer = serializeInt(buffer, accountdata->votercounter);
    return buffer - start;
}

unsigned char* deserializeInt(unsigned char* buffer, int output)
{
    output = 0;
    output |= buffer[0] << 24;
    output |= buffer[0] << 16;
    output |= buffer[0] << 8;
    output |= buffer[0];
    return buffer + sizeof(int);
}
unsigned char* deserializeUint_32_t(unsigned char* buffer, uint32_t output)
{
    output = 0;
    output |= buffer[0] << 24;
    output |= buffer[0] << 16;
    output |= buffer[0] << 8;
    output |= buffer[0];
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

int deserializeAccountData(unsigned char* buffer, AccountData* accountdata)
{
    unsigned char* start = buffer;
    buffer = deserializeUint_32_t(buffer, accountdata->currentip);
    buffer = deserializeCharArray(buffer, accountdata->login, MAX_LOGIN_LENGTH);
    buffer = deserializeUnsignedCharArray(buffer, accountdata->passwordhash, MAX_PASSHASH_LENGTH);
    buffer = deserializeInt(buffer, accountdata->votercounter);
    return buffer - start;
}
