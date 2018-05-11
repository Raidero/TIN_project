#include "CommunicationService.h"


int sendMessageToPlayerService(char* login, char* message)
{
    //getting player id from login to use it as socket
    int playerid = loginToPlayerId(login);
    //check if player id is in range
    if (playerid < 0 || playerid > MAX_ACCOUNTS_COUNT)
    {
        fprintf(stderr, "Found player id is out of range\n");
        return PLAYER_ID_OUT_OF_RANGE;
    }
    send_all(playerid, message, MAX_MESSAGE_LENGTH);

    return 0;
}

//function to be sure that everything got sent
int send_all(int socket, char *buffer, size_t length)
{
    //pointer to index of message dividing sent and not sent part
    char *ptr = (char*) buffer;
    while (length > 0)
    {
        int i = send(socket, ptr, length, 0);
        if (i < 0)
        {
            fprintf(stderr, "Couldn't send message to player\n");
            return ERROR_SENDING_MESSAGE;
        }
        ptr += i;
        length -= i;
    }
    return 0;
}

int sendMessageToRoomService(char* senderlogin, int roomid, char* message)
{
    int i;
    int numberofplayers;
    //check if room with given roomid exists
    if (roomid< 0 || roomid > MAX_ROOM_COUNT)
    {
        fprintf(stderr, "Room with given room id doesn't exist\n");
        return ERROR_IDENTIFYING_ROOM;
    }
    //sending message to all players in room with given id
    numberofplayers = sizeof(rooms[roomid]->players)/sizeof(rooms[roomid]->players[0]);
    //for (i = 0; rooms[roomid]->players[i]; i++)
    for (i = 0; numberofplayers; i++)
    {
        if (senderlogin!= rooms[roomid]->players[i]->login)
        {
        sendMessageToPlayerService(rooms[roomid]->players[i]->login, message);
        }
    }
    return 0;
}
