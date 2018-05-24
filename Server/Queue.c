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
int addNewElement(Event* element)
{
    pthread_mutex_lock(&queuelock);
    if(lastindex == firstindex && isfull)
    {
        fprintf(stderr, "Queue is full, can't add new events\n");
        return ERROR_QUEUE_FULL;
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
    return 0;
}

Event* popElement()
{
    Event* element = NULL;
    pthread_mutex_lock(&queuelock);
    if(lastindex == firstindex && !isfull)
    {
        //printf("Nothing to do...\n");
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
            printf("New Event: ");
            switch(event->message)
            {
                case REQUEST_LOGIN:
                {
                    printf("login\n");
                    unsigned char answer;
                    int (*func)(AccountData*, int) = (int (*)(AccountData*, int))event->functionpointer;
                    AccountData* accdata = (AccountData*)malloc(sizeof(AccountData));
                    unsigned char* bufferptr = event->argumentsbuffer;
                    bufferptr = deserializeAccountData(bufferptr, accdata);
                    int* playerid = (int*)malloc(sizeof(int));
                    deserializeInt(bufferptr, playerid);
                    if(func(accdata, *playerid))
                    {
                        answer = FAILED_TO_LOGIN;
                    }
                    else
                    {
                        answer = LOGIN_SUCCESSFUL;
                    }
                    while(!send(event->socket, &answer, 1, 0)) {}
                    free(playerid);
                    break;
                }
                case REQUEST_LOGOUT:
                {
                    printf("logout\n");
                    unsigned char answer;
                    int (*func)(uint32_t) = (int (*)(uint32_t))event->functionpointer;
                    uint32_t* ip = (uint32_t*)malloc(sizeof(uint32_t));
                    deserializeUint_32_t(event->argumentsbuffer, ip);
                    if(func(*ip))
                    {
                        answer = FAILED_TO_LOGOUT;
                    }
                    else
                    {
                        answer = LOGOUT_SUCCESSFUL;
                    }
                    while(!send(event->socket, &answer, 1, 0)) {}
                    free(ip);
                    break;
                }
                case REQUEST_CREATE_ACCOUNT:
                {
                    printf("create account\n");
                    unsigned char answer;
                    int (*func)(AccountData*) = (int (*)(AccountData*))event->functionpointer;
                    AccountData* accdata = (AccountData*)malloc(sizeof(AccountData));
                    deserializeAccountData(event->argumentsbuffer, accdata);
                    if(func(accdata))
                    {
                        answer = FAILED_TO_CREATE_ACCOUNT;
                    }
                    else
                    {
                        answer = CREATE_ACCOUNT_SUCCESSFUL;
                    }
                    while(!send(event->socket, &answer, 1, 0)) {}
                    free(accdata);
                    break;
                }
                case REQUEST_DELETE_ACCOUNT:
                {
                    printf("delete account\n");
                    unsigned char answer;
                    int (*func)(AccountData*) = (int (*)(AccountData*))event->functionpointer;
                    AccountData* accdata = (AccountData*)malloc(sizeof(AccountData));
                    deserializeAccountData(event->argumentsbuffer, accdata);
                    if(func(accdata))
                    {
                        answer = FAILED_TO_DELETE_ACCOUNT;
                    }
                    else
                    {
                        answer = DELETE_ACCOUNT_SUCCESSFUL;
                    }
                    while(!send(event->socket, &answer, 1, 0)) {}
                    free(accdata);
                    break;
                }
                case REQUEST_CHANGE_PASSWORD:
                {
                    printf("change password\n");
                    unsigned char answer;
                    int (*func)(AccountData*, unsigned char*) = (int (*)(AccountData*, unsigned char*))event->functionpointer;
                    AccountData* accdata = (AccountData*)malloc(sizeof(AccountData));
                    unsigned char* passwordhash = (unsigned char*)malloc(MAX_PASSHASH_LENGTH*sizeof(unsigned char));
                    unsigned char* bufferptr = event->argumentsbuffer;
                    bufferptr = deserializeAccountData(bufferptr, accdata);
                    deserializeUnsignedCharArray(bufferptr, passwordhash, MAX_PASSHASH_LENGTH);
                    if(func(accdata, passwordhash))
                    {
                        answer = FAILED_TO_CHANGE_PASSWORD;
                    }
                    else
                    {
                        answer = CHANGE_PASSWORD_SUCCESSFUL;
                    }
                    while(!send(event->socket, &answer, 1, 0)) {}
                    free(accdata);
                    free(passwordhash);
                    break;
                }

                case REQUEST_START_GAME:
                {
                /// TODO get and send player list in room
                    printf("start game\n");
                    unsigned char answer;
                    int (*func)(int) = (int (*)(int))event->functionpointer;
                    int* roomid = NULL;
                    int* address =(int*)malloc(sizeof(int));
                    int* accountid = (int*)malloc(sizeof(int));
                    unsigned char* bufferptr = event->argumentsbuffer;
                    bufferptr = deserializeInt(bufferptr, accountid);
                    bufferptr = deserializePointer(bufferptr, address);
                    roomid = (int*)*address;
                    *roomid = func(*accountid);
                    if(*roomid < 0)
                    {
                        answer = FAILED_TO_START_GAME;
                    }
                    else
                    {
                        answer = START_GAME_SUCCESSFUL;
                        printf("connected to room number: %d\n", *roomid);
                    }
                    while(!send(event->socket, &answer, 1, 0)) {}
                    free(accountid);
                    free(address);
                    break;
                }

                case REQUEST_REFRESH_LOGINS:
                {
                    printf("refresh logins\n");
                    int size = MAX_PLAYER_COUNT*MAX_LOGIN_LENGTH*sizeof(char);
                    char answer;
                    char* logins = (char*)malloc(size);
                    int (*func)(int, int, char*) = (int (*)(int, int, char*))event->functionpointer;
                    unsigned char* bufferptr = event->argumentsbuffer;

                    int *roomid = (int*)malloc(sizeof(int));
                    int *accountid = (int*)malloc(sizeof(int));
                    bufferptr = deserializeInt(bufferptr, accountid);
                    bufferptr = deserializeInt(bufferptr, roomid);

                    if(func(*accountid, *roomid, logins))
                    {
                        answer = FAILED_TO_REFRESH_LOGINS;
                        while(!send(event->socket, &answer, 1, 0)) {}
                    }
                    else
                    {
                        answer = REFRESH_LOGINS_SUCCESSFUL;
                        while(!send(event->socket, &answer, 1, 0)) {}
                        int sendbytes = 0;
                        unsigned char* buffer = (unsigned char*)malloc(size);
                        serializeCharArray(buffer, logins, size);
                        while(sendbytes < size)
                        {
                            sendbytes = send(event->socket, buffer + sendbytes, size - sendbytes, 0);
                        }
                        free(buffer);
                    }
                    free(accountid);
                    free(roomid);
                    free(logins);
                    break;
                }
                ///TODO, there are many other messages that need being handled
            }
            free(event->argumentsbuffer);
            free(event);
            event = NULL;
        }
        else
        {

        }

    }
}

