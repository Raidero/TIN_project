#include "Server.h"
#include "Defines.h"

pthread_t eventhandler;
pthread_t newClientThread;
int serversocketfd = -1;
int servercommunicationsocket = -1;

pthread_t threads[MAX_THREADS_COUNT];
int sockets[MAX_SOCKETS_COUNT];
int initServer(struct sockaddr_in* serveraddress, struct sockaddr_in* servercommunicationaddress)
{
    int i;
    //preparing threads array
    for(i = 0; i < MAX_THREADS_COUNT; ++i)
    {
        threads[i] = -1;
    }
    //preparing sockets array
    for(i = 0; i < MAX_SOCKETS_COUNT; ++i)
    {
        sockets[i] = -1;
        communicationsockets[i] = -1;
    }
    int option = 1; //option is to use in setsockopt, by a pointer to this value

    //creating socket
    serversocketfd = socket(AF_INET, SOCK_STREAM, 0);
    servercommunicationsocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serversocketfd < 0 || servercommunicationsocket < 0)
    {
        fprintf(stderr, "Couldn't open one of sockets\n");
        return ERROR_OPENING_SOCKET;
    }
    //setting socket options
    //level is set to SOL_SOCKET to manipulate options at the sockets API level
    //SO_REUSEADDR to allow reusing local addresses in bind()
    if(setsockopt(serversocketfd, SOL_SOCKET, SO_REUSEADDR, (char*)&option, sizeof(option)) < 0)
    {
        fprintf(stderr, "setsockopt failed\n");
        return ERROR_SETTING_SOCKET_OPTIONS;
    }
    if(setsockopt(servercommunicationsocket, SOL_SOCKET, SO_REUSEADDR, (char*)&option, sizeof(option)) < 0)
    {
        fprintf(stderr, "setsockopt failed\n");
        return ERROR_SETTING_SOCKET_OPTIONS;
    }
    bzero((char *) serveraddress, sizeof(*serveraddress));
    bzero((char *) servercommunicationaddress, sizeof(*serveraddress));
    //preparing sockaddr_in structure
    serveraddress->sin_family = AF_INET; //set address family to Internet Protocol v4 addresses
    serveraddress->sin_addr.s_addr = INADDR_ANY; //set destination ip number
    serveraddress->sin_port = htons(DEFAULT_PORT); //set destination port number

    servercommunicationaddress->sin_family = AF_INET; //set address family to Internet Protocol v4 addresses
    servercommunicationaddress->sin_addr.s_addr = INADDR_ANY; //set destination ip number
    servercommunicationaddress->sin_port = htons(DEFAULT_PORT + 10); //set destination port number
    //binding server to address
    if(bind(serversocketfd, (struct sockaddr *) serveraddress, sizeof(*serveraddress)) < 0)
    {
        fprintf(stderr, "Couldn't bind server to address\n");
        return ERROR_BINDING_SOCKET;
    }

    if(bind(servercommunicationsocket, (struct sockaddr *) servercommunicationaddress, sizeof(*serveraddress)) < 0)
    {
        fprintf(stderr, "Couldn't bind communication socket to address\n");
        return ERROR_BINDING_SOCKET;
    }
    //start listening for connections on socket
    listen(serversocketfd, MAX_CONNECTION_LIMIT);
    listen(servercommunicationsocket, MAX_CONNECTION_LIMIT);
    return 0;
}

int startServer()
{
    struct sockaddr_in client_address;
    unsigned int client_length = sizeof(client_address);
    int socketindex;
    pthread_create(&eventhandler, NULL, startEventHandler, NULL);
    signal(SIGINT, intHandler); //starting function to handle Ctrl-C signal

    while(1)
    {
        socketindex = createNewSocket();
        if(socketindex < 0)
        {
            fprintf(stderr, "Couldn't find free socket for new client\n");
            sleep(10);
            continue;
        }
        //creating a new connected socket for connection request
        sockets[socketindex] = accept(serversocketfd, (struct sockaddr *) &client_address, &client_length);
        if(sockets[socketindex] < 0)
        {
            fprintf(stderr, "Couldn't accept connection\n");
            return ERROR_STARTING_SERVER;
        }
        //call to function which creates new thread and adds it to threads array
        if(createNewThread(&sockets[socketindex]) < 0)
        {
            fprintf(stderr, "Couldn't create new thread");
            return ERROR_CREATING_THREAD;
        }
    }
    return 0;
}

void intHandler(int sig_num)
{
    int i;
    /*for(i = 0; i < MAX_THREADS_COUNT; ++i)
    {
        if(threads[i] != 0)
        {
            pthread_join(threads[i], NULL);
        }
    }*/
    pthread_kill(eventhandler, 0);
    for(i = 0; i < MAX_THREADS_COUNT; ++i)
    {
        if(threads[i] != -1)
        {
            pthread_kill(threads[i], 0);
        }
    } ///in case we want to kill all the threads
    for(i = 0; i < MAX_THREADS_COUNT; ++i)
    {
        if(sockets[i] != -1)
        {
            close(sockets[i]);
        }
    }
    disposeAllAccounts();
    disposeAllRooms();
    if(serversocketfd != -1)
    {
        close(serversocketfd);
    }
    exit(0);
}

void* services(void *i)
{
    int accountid = -1, roomid = -1;
    int socket = *((int *)i);
    accountid = socketToPlayerId(socket);
    if(accountid < 0)
    {
        fprintf(stderr, "Player id not found\n");
        disposeSocket(socket);
        close(socket);
        pthread_exit(NULL);
    }
    printf("New client connected: %d socket, %d id\n", socket, accountid);
    unsigned char buffer[BUFFER_SIZE];
    int readbytes;
    while(1)
    {
        bzero(buffer, BUFFER_SIZE);
        readbytes = recv(socket, buffer, 1, MSG_WAITALL);
        if(readbytes <= 0)
        {
            printf("Player with %d id quit\n", accountid);
            if(roomid >= 0)
            {
                sweepPlayer(accountid, roomid);
                checkIfRoomIsEmptyAndDispose(roomid);
                close(communicationsockets[accountid]);
                communicationsockets[accountid] = -1;
            }
            if(accountid >= 0)
            {
                disposeAccountData(accountid);
            }
            disposeSocket(socket);
            close(socket);
            pthread_exit(NULL);
        }
        else if(readbytes >= 1)
        {
            Event* event = NULL;
            unsigned char answer =  ERROR_QUEUE_FULL;
            unsigned char* args = NULL;
            unsigned char* bufferptr = NULL;
            int size;
            readbytes = 0;
            switch(buffer[0])
            {
                case REQUEST_LOGIN:
                {
                    int alldata;
                    size = sizeof(AccountData) + sizeof(int);
                    args = (unsigned char*)malloc(size);
                    alldata = sizeof(AccountData);
                    do{
                        readbytes += recv(socket, buffer+readbytes, alldata-readbytes, MSG_WAITALL);
                    }while(readbytes < alldata);
                    serializeInt(buffer+readbytes, accountid);
                    copyBuffer(buffer, args, size);
                    event = createEvent((void (*)(void))logInService, args, socket, REQUEST_LOGIN);
                    break;
                }

                case REQUEST_LOGOUT:
                {
                    size = sizeof(uint32_t);
                    args = (unsigned char*)malloc(size);
                    do{
                        readbytes += recv(socket, buffer+readbytes, size-readbytes, MSG_WAITALL);
                    }while(readbytes < size);
                    copyBuffer(buffer, args, size);
                    event = createEvent((void (*)(void))logOutService, args, socket, REQUEST_LOGOUT);
                    break;
                }

                case REQUEST_CREATE_ACCOUNT:
                {
                    size = sizeof(AccountData);
                    args = (unsigned char*)malloc(size);
                    do{
                        readbytes += recv(socket, buffer+readbytes, size-readbytes, MSG_WAITALL);
                    }while(readbytes < size);
                    copyBuffer(buffer, args, size);
                    event = createEvent((void (*)(void))createAccountService, args, socket, REQUEST_CREATE_ACCOUNT);
                    break;
                }

                case REQUEST_DELETE_ACCOUNT:
                {
                    size = sizeof(AccountData);
                    args = (unsigned char*)malloc(size);
                    do{
                        readbytes += recv(socket, buffer+readbytes, size-readbytes, MSG_WAITALL);
                    }while(readbytes < size);
                    copyBuffer(buffer, args, size);
                    event = createEvent((void (*)(void))deleteAccountService, args, socket, REQUEST_DELETE_ACCOUNT);
                    break;
                }

                case REQUEST_CHANGE_PASSWORD:
                {
                    size = sizeof(AccountData) + MAX_PASSHASH_LENGTH*sizeof(unsigned char);
                    args = (unsigned char*)malloc(size);
                    do{
                        readbytes += recv(socket, buffer+readbytes, size-readbytes, MSG_WAITALL);
                    }while(readbytes < size);
                    copyBuffer(buffer, args, size);
                    event = createEvent((void (*)(void))changePasswordService, args, socket, REQUEST_CHANGE_PASSWORD);
                    break;
                }

                case REQUEST_START_GAME:
                {
                    struct sockaddr_in client_address;
                    unsigned int client_length = sizeof(client_address);
                    communicationsockets[accountid] = accept(servercommunicationsocket, (struct sockaddr *) &client_address, &client_length);
                    size = sizeof(int) + sizeof(int);
                    args = (unsigned char*)malloc(size);
                    bufferptr = buffer;
                    bufferptr = serializeInt(bufferptr, accountid);
                    bufferptr = serializePointer(bufferptr, &roomid);
                    copyBuffer(buffer, args, size);
                    event = createEvent((void (*)(void))connectAccountToRoomService, args, socket, REQUEST_START_GAME);
                    break;
                }

                case REQUEST_REFRESH_LOGINS:
                {
                    size = sizeof(int) + sizeof(int);
                    args = (unsigned char*)malloc(size);
                    bufferptr =  buffer;
                    bufferptr = serializeInt(bufferptr, accountid);
                    bufferptr = serializeInt(bufferptr, roomid);
                    copyBuffer(buffer, args, size);
                    event = createEvent((void (*)(void))refreshRoomService, args, socket, REQUEST_REFRESH_LOGINS);
                    break;
                }

                case REQUEST_TOGGLE_READY:
                {
                    size = sizeof(int) + sizeof(int);
                    args = (unsigned char*)malloc(size);
                    bufferptr =  buffer;
                    bufferptr = serializeInt(bufferptr, accountid);
                    bufferptr = serializeInt(bufferptr, roomid);
                    copyBuffer(buffer, args, size);
                    event = createEvent((void (*)(void))toggleReadyService, args, socket, REQUEST_TOGGLE_READY);
                    break;
                }

                case REQUEST_REFRESH_READYNESS:
                {
                    size = sizeof(int) + sizeof(int);
                    args = (unsigned char*)malloc(size);
                    bufferptr =  buffer;
                    bufferptr = serializeInt(bufferptr, accountid);
                    bufferptr = serializeInt(bufferptr, roomid);
                    copyBuffer(buffer, args, size);
                    event = createEvent((void (*)(void))refreshReadyUpService, args, socket, REQUEST_REFRESH_READYNESS);
                    break;
                }

                case REQUEST_EXIT_ROOM:
                {
                    size = sizeof(int) + sizeof(int);
                    args = (unsigned char*)malloc(size);
                    bufferptr =  buffer;
                    bufferptr = serializeInt(bufferptr, accountid);
                    bufferptr = serializePointer(bufferptr, &roomid);
                    copyBuffer(buffer, args, size);
                    event = createEvent((void (*)(void))exitRoomService, args, socket, REQUEST_EXIT_ROOM);
                    break;
                }

                case REQUEST_SEND_MESSAGE:
                {
                    int alldata = FINAL_MESSAGE_LENGTH;
                    size = FINAL_MESSAGE_LENGTH*sizeof(char) + sizeof(int);
                    args = (unsigned char*)malloc(size);

                    do{
                        readbytes += recv(socket, buffer+readbytes, alldata-readbytes, MSG_WAITALL);
                    }while(readbytes < alldata);
                    printf("%s", buffer);
                    bufferptr =  buffer + readbytes;
                    bufferptr = serializeInt(bufferptr, roomid);
                    copyBuffer(buffer, args, size);
                    event = createEvent((void (*)(void))sendMessageToRoomService, args, socket, REQUEST_SEND_MESSAGE);
                    break;
                }

                case REQUEST_START_MATCH:	// after this block struct with essential multicast data [roomid] is all set and ready to be used to send packets
                {
					// do some fun stuff m9
					// args - roomid
					size = sizeof(int) + sizeof(int);
					args = (unsigned char*)malloc(size);
					bufferptr =  buffer;
					bufferptr = serializeInt(bufferptr, accountid);
                    bufferptr = serializeInt(bufferptr, roomid);
                    copyBuffer(buffer, args, size);
					event = createEvent((void (*)(void))startMatchService, args, socket, REQUEST_START_MATCH);
					break;
                }

                case REQUEST_STARTING_INFO:
                {
                    size = sizeof(int) + sizeof(int);
                    args = (unsigned char*)malloc(size);
                    bufferptr =  buffer;
                    bufferptr = serializeInt(bufferptr, roomid);
                    bufferptr = serializeInt(bufferptr, accountid);
                    copyBuffer(buffer, args, size);
                    event = createEvent((void (*)(void))getPlayersInformation, args, socket, REQUEST_STARTING_INFO);
                    break;
                }

                case REQUEST_SEND_MULTICAST_DATA:
                {
					int alldata = 16;
                    do{
                        readbytes += recv(socket, buffer+readbytes, alldata-readbytes, MSG_WAITALL);
                    }while(readbytes < alldata);
					//printf("%s\n", buffer);
					//puts("WHATAERVER"); // without puts prinft doesnt work
					sendMulticastData(roomid, buffer, alldata);
                }

            }
            if (addNewElement(event))
            {
                while(!send(socket, &answer, 1, 0)) {}
            }
            else
            {
                pthread_kill(eventhandler, SIGUSR1);
            }
            readbytes = 0;

        }
        bzero(buffer, BUFFER_SIZE);
    }
    return NULL;
}

void copyBuffer(unsigned char* source, unsigned char* destination, int size)
{
    int i;
    for(i = 0; i < size; ++i)
    {
        destination[i] = source[i];
    }
}
int createNewThread(int* socket)
{
    int i;
    for(i = 0; i < MAX_THREADS_COUNT; ++i)
    {
        if(threads[i] == -1)
        {
            if(pthread_create(&threads[i], NULL, services, socket))
            {
                fprintf(stderr, "Couldn't create new thread");
                return ERROR_CREATING_THREAD;
            }
            printf("New thread with id %d has been created\n", i);
            return i;
        }
    }
    fprintf(stderr, "Couldn't find free thread\n");
    return MAX_THREADS_LIMIT_ERROR;
}

int createNewSocket()
{
    int i;
    for(i = 0; i < MAX_SOCKETS_COUNT; ++i)
    {
        if(sockets[i] == -1)
        {
            return i;
        }
    }
    fprintf(stderr, "All sockets are taken\n");
    return MAX_SOCKETS_LIMIT_ERROR;
}

int socketToPlayerId(int socket)
{
    int i;
    for(i = 0; i < MAX_SOCKETS_COUNT; ++i)
    {
        if(sockets[i] == socket)
        {
            return i;
        }
    }
    fprintf(stderr,"Couldn't find playerid for given socket\n");
    return PLAYER_ID_NOT_FOUND;
}

int disposeSocket(int socket)
{
    int i;
    for(i = 0; i < MAX_SOCKETS_COUNT; ++i)
    {
        if(sockets[i] == socket)
        {
            sockets[i] = -1;
            return 0;
        }
    }
    fprintf(stderr,"Couldn't find open socket with given value\n");
    return SOCKET_NOT_FOUND;
}
