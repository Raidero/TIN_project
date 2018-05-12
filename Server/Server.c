#include "Server.h"
#include "Defines.h"

pthread_t newClientThread;
int serversocketfd = -1;

int initServer(struct sockaddr_in* serveraddress)
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
    }
    int option = 1; //option is to use in setsockopt, by a pointer to this value

    //creating socket
    serversocketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serversocketfd < 0)
    {
        fprintf(stderr, "Couldn't open a socket\n");
        return ERROR_OPENING_SOCKET;
    }
    //setting socket options
    //level is set to SOL_SOCKET to manipulate options at the sockets API level
    //SO_REUSEADDR to allow reuse of local addresses in bind()
    if(setsockopt(serversocketfd, SOL_SOCKET, SO_REUSEADDR, (char*)&option, sizeof(option)) < 0)
    {
        fprintf(stderr, "setsockopt failed\n");
        return ERROR_SETTING_SOCKET_OPTIONS;
    }
    bzero((char *) serveraddress, sizeof(*serveraddress));

    //preparing sockaddr_in structure
    serveraddress->sin_family = AF_INET; //set address family to Internet Protocol v4 addresses
    serveraddress->sin_addr.s_addr = INADDR_ANY; //set destination ip number
    serveraddress->sin_port = htons(DEFAULT_PORT); //set destination port number

    //binding server to address
    if(bind(serversocketfd, (struct sockaddr *) serveraddress, sizeof(*serveraddress)) < 0)
    {
        fprintf(stderr, "Couldn't bind server to address\n");
        return ERROR_BINDING_SOCKET;
    }
    //start listening for connections on socket
    listen(serversocketfd, MAX_CONNECTION_LIMIT);
    return 0;
}

int startServer(struct sockaddr_in server_address)
{
    struct sockaddr_in client_address;
    unsigned int client_length = sizeof(client_address);
    int socketindex;
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
        //call to function which creates new thread and adds it to treads array
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
        close(socket);
        free(i);
        pthread_exit(NULL);
    }
    printf("New client connected: %d\n", socket);
    char buffer[BUFFER_SIZE];
    int readbytes;
    while(1)
    {
        bzero(buffer, BUFFER_SIZE);
        readbytes = recv(socket, buffer, 1, MSG_WAITALL);
        if(readbytes <= 0)
        {
            if(accountid >= 0)
            {
                disposeAccountData(accountid);
            }
            if(roomid >= 0)
            {
                sweepPlayer(loggedaccounts[accountid], roomid);
                checkIfRoomIsEmptyAndDispose(roomid);
            }
            close(socket);
            free(i);
            pthread_exit(NULL);
            //break;//bye
        }
        else if(readbytes >= 1) //here we come
        {
            switch(buffer[0])
            {
                case REQUEST_LOGIN:
                {
                    send(socket, "Login?", 14, 0);
                    int i, bytestoread, size;
                    size = sizeof(AccountData) + sizeof(int);
                    unsigned char* args = (unsigned char*)malloc(size);
                    readbytes = 0;
                    bytestoread = size;
                    do{
                        readbytes += recv(socket, buffer+readbytes, bytestoread, MSG_WAITALL);
                        bytestoread -= readbytes;
                    }while(readbytes < size);
                    printf("XD");
                    fflush(stdout);
                    for(i = 0; i < size; ++i)
                    {
                        args[i] = buffer[i];
                    }
                    Event* event = createEvent((void (*)(void))logInService, args, socket, REQUEST_LOGIN);
                    addNewElement(event);
                    printf("XD");
                    fflush(stdout);
                    break;
                }
                ///TODO....
            }
        }

        printf("%s", buffer);
        bzero(buffer, BUFFER_SIZE);
        write(socket, "got your message", 17);
    }
    return NULL;
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
