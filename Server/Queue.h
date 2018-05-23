#ifndef QUEUE_H
#define QUEUE_H
#include "Defines.h"
#include "AccountService.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "Serialization.h"
typedef struct
{
    void (*functionpointer)(void);
    unsigned char* argumentsbuffer;
    int socket;
    char message;
}Event;

Event* queue[MAX_QUEUE_LENGTH];

void initQueue();
void disposeQueue();
int addNewElement(Event* element);
Event* popElement();
Event* createEvent(void (*func)(void), unsigned char* args, int socket, char message);
void* startEventHandler();

#endif // QUEUE_H
