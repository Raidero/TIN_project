#include "Queue.h"
#include <unistd.h>
Event* queue[MAX_QUEUE_LENGTH];
unsigned int lastindex = 0;
unsigned int firstindex = 0;
int isfull = 0;
pthread_mutex_t queuelock;

void initQueue()
{
    int i;
    pthread_mutex_init(&queuelock, NULL);
    for(i = 0; i < MAX_QUEUE_LENGTH; ++i)
    {
        queue[i] = NULL;
    }
}

void disposeQueue()
{
    pthread_mutex_destroy(&queuelock);
}
void addNewElement(Event* element)
{
    pthread_mutex_lock(&queuelock);
    if(lastindex == firstindex && isfull)
    {
        fprintf(stderr, "Queue is full, can't add new events\n");
    }
    else
    {
        queue[lastindex%MAX_QUEUE_LENGTH] = element;
        ++lastindex;
        if(lastindex == firstindex)
        {
            isfull = 1;
        }
    }
    pthread_mutex_unlock(&queuelock);
}

Event* popElement()
{
    Event* element = NULL;
    pthread_mutex_lock(&queuelock);
    if(lastindex == firstindex && !isfull)
    {
        printf("Nothing to do...\n");
    }
    else
    {
        element = queue[firstindex%MAX_QUEUE_LENGTH];
        ++firstindex;
        if(firstindex == lastindex)
        {
            isfull = 0;
        }
    }
    pthread_mutex_unlock(&queuelock);
    return element;
}

Event* createEvent(void (*func)(void), unsigned char* args, int socket, char message)
{
    Event* event = (Event*)malloc(sizeof(Event));
    event->functionpointer = func;
    event->argumentsbuffer = args;
    event->socket = socket;
    event->message = message;
    return event;
}

void* startEventHandler()
{
    Event* event = NULL;
    for(;;)
    {
        event = popElement();
        if(event != NULL)
        {
            printf("New Event\n");
            switch(event->message)
            {
                case REQUEST_LOGIN:
                {
                    printf("request");
                    fflush(stdout);
                    unsigned char answer;
                    int (*func)(AccountData*, int) = (int (*)(AccountData*, int))event->functionpointer;
                    AccountData* accdata = (AccountData*)malloc(sizeof(AccountData));
                    deserializeAccountData(event->argumentsbuffer, accdata);
                    int* playerid = (int*)malloc(sizeof(int));
                    deserializeInt(event->argumentsbuffer, playerid);
                    if(func(accdata, *playerid))
                    {
                        answer = FAILED_TO_LOGIN;
                    }
                    else
                    {
                        answer = LOGIN_SUCCESSFUL;
                    }
                    while(!send(event->socket, &answer, 1, 0)) {}
                    free(event->argumentsbuffer);
                    free(event);
                    event = NULL;
                    break;
                }
                case REQUEST_LOGOUT:
                {
                    int result;
                    int (*func)(uint32_t) = (int (*)(uint32_t))event->functionpointer;
                    uint32_t* ip = (uint32_t*)event->argumentsbuffer;
                    result = func(*ip);
                    send(event->socket, &result, sizeof(int), 0);
                    break;
                }
                case REQUEST_CREATE_ACCOUNT:
                {
                    int result;
                    int (*func)(AccountData*) = (int (*)(AccountData*))event->functionpointer;
                    AccountData* accdata = (AccountData*)event->argumentsbuffer;
                    result = func(accdata);
                    send(event->socket, &result, sizeof(int), 0);
                    break;
                }
                case REQUEST_DELETE_ACCOUNT:
                {
                    int result;
                    int (*func)(AccountData*) = (int (*)(AccountData*))event->functionpointer;
                    AccountData* accdata = (AccountData*)event->argumentsbuffer;
                    result = func(accdata);
                    send(event->socket, &result, sizeof(int), 0);
                    break;
                }
                case REQUEST_CHANGE_PASSWORD:
                {
                    int result;
                    int (*func)(AccountData*, unsigned char*) = (int (*)(AccountData*, unsigned char*))event->functionpointer;
                    AccountData* accdata = (AccountData*)event->argumentsbuffer;
                    unsigned char* newpasshash = &event->argumentsbuffer[sizeof(AccountData)];
                    result = func(accdata, newpasshash);
                    send(event->socket, &result, sizeof(int), 0);
                    break;
                }
                ///TODO, there are many other messages that need being handled
            }
            free(event);
        }
        else
        {
            sleep(1);
        }

    }
}

