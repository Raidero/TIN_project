#ifndef PRIVATE_COMMUNICATION_SERVICE_H
#define PRIVATE_COMMUNICATION_SERVICE_H

typedef struct
{
    char* receiver;
    short messagelength;
} Header;

typedef struct
{
    Header header;
    char* message;
} Message;

int sendMessageToPlayerService(Message* message);

#endif // PRIVATE_COMMUNICATION_SERVICE_H
