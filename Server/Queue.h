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
typedef struct
{
    void (*functionpointer)(void);
    char* argumentsbuffer;
    int socket;
    char message;
}Event;

Event* queue[MAX_QUEUE_LENGTH];

void initQueue();
void disposeQueue();
void addElement();
void popElement();
void* startEventHandler();

#endif // QUEUE_H
