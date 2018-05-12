#ifndef DEFINES_H
#define DEFINES_H

#define FREE_ROOM_NOT_FOUND -1
#define PLAYER_NOT_FOUND -2
#define IP_NOT_FOUND -3
#define PLAYER_ID_NOT_FOUND -4

#define ERROR_OPENING_SOCKET -101
#define ERROR_BINDING_SOCKET -102
#define ERROR_STARTING_SERVER -103
#define ERROR_CREATING_THREAD -104
#define ERROR_SETTING_SOCKET_OPTIONS -105

#define OPEN_FILE_ERROR -201
#define FILE_NOT_OPEN -202
#define FILE_WRITE_ERROR -203
#define FILE_READ_ERROR -204

#define CREATE_ACCOUNT_ERROR -302
#define DELETE_ACCOUNT_ERROR -303
#define WRONG_LOGIN_OR_PASSWORD -304
#define LOGGED_IN_ERROR -305
#define LOGGED_OUT_ERROR -306
#define ACCOUNT_PLACE_TAKEN_ERROR -307

#define OUT_OF_RANGE -401

#define MAX_ROOM_LIMIT_ERROR -501
#define MAX_ACCOUNTS_LOGGED_IN_ERROR -502
#define MAX_THREADS_LIMIT_ERROR -503
#define MAX_SOCKETS_LIMIT_ERROR -504

// in CommunicationService
#define PLAYER_ID_OUT_OF_RANGE -701
#define ERROR_SENDING_MESSAGE -702
#define ERROR_IDENTIFYING_ROOM -703

#define MAX_LOGIN_LENGTH 32
#define MAX_PASSWORD_LENGTH 32
#define MAX_PASSHASH_LENGTH 32
#define MAX_ROOM_COUNT 16
#define MAX_PLAYER_COUNT 16 //in room
#define MAX_CONNECTION_LIMIT 128 //how many clients can wait in queue on accept connection
#define MAX_ACCOUNTS_COUNT 512
#define MAX_THREADS_COUNT 512
#define MAX_SOCKETS_COUNT 512
#define MAX_QUEUE_LENGTH 1024
#define BUFFER_SIZE 1024
#define MAX_MESSAGE_LENGTH 1024 //in CommunicationService

#define DATA_FILE_NAME "accountdata"
#define DATA_FILE_MODE "r+b"

#define REQUEST_LOGIN 1
#define REQUEST_LOGOUT 2
#define REQUEST_CREATE_ACCOUNT 3
#define REQUEST_DELETE_ACCOUNT 4
#define REQUEST_CHANGE_PASSWORD 5

#define FAILED_TO_LOGIN 255
#define LOGIN_SUCCESSFUL 0
#define FAILED_TO_LOGOUT 254
#define LOGOUT_SUCCESSFUL 1
#define FAILED_TO_CREATE_ACCOUNT 253
#define CREATE_ACCOUNT_SUCCESSFUL 2
#define FAILED_TO_DELETE_ACCOUNT 252
#define DELETE_ACCOUNT_SUCCESSFUL 3
#define FAILED_TO_CHANGE_PASSWORD 251
#define CHANGE_PASSWORD_SUCCESSFUL 4
///client
#define ERROR_GETTING_HOST_NAME -601
#define ERROR_CONNECTING_CLIENT -602
#define ERROR_WRITING_TO_SOCKET -603
#define ERROR_READING_FROM_SOCKET -604


///views communication
#define PLAYER_LOGGED_IN 1
#define PLAYER_LOGGED_OUT 2
#define RETURN_TO_MENU_VIEW 3
#define ACCOUNT_CREATED 4
#endif // DEFINES_H

