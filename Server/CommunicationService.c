#include "CommunicationService.h"

//function to send message from player to player (1 to 1)
int sendMessageToPlayerService(char* login, char* message)
{
    //getting player id from login to use it as socket
    printf("%s", login);
    printf("%s", message);
    int playerid = loginToPlayerId(login);
    //check if player id is in range
    if (playerid < 0 || playerid > MAX_ACCOUNTS_COUNT)
    {
        fprintf(stderr, "Found player id is out of range\n");
        return PLAYER_ID_OUT_OF_RANGE;
    }
    send_all(communicationsockets[playerid], message, FINAL_MESSAGE_LENGTH);

    return 0;
}

//function to be sure that every character in message got sent
int send_all(int socket, char *buffer, size_t length)
{
    //pointer to index of message dividing sent and not sent part
    char *ptr = (char*) buffer;
    while (length > 0)
    {
        int i = send(socket, ptr, length, 0);
        //error if send returns i less than 0
        if (i < 0)//0 because we let it send messages with 0 characters
        {
            fprintf(stderr, "Couldn't send message to player\n");
            return ERROR_SENDING_MESSAGE;
        }
        ptr += i;
        length -= i;
    }
    return 0;
}

//function to send message to all players in room (1 to multiple)
int sendMessageToRoomService(int roomid, char* message)
{
    int i;
    //check if room with given roomid exists
    if (roomid< 0 || roomid > MAX_ROOM_COUNT)
    {
        fprintf(stderr, "Room with given room id doesn't exist\n");
        return ERROR_IDENTIFYING_ROOM;
    }
    //sending message to all players in room with given id
    for (i = 0; i < MAX_PLAYER_COUNT; i++)
    {
        //checking if this player exists
        if(rooms[roomid]->players[i])
        {
            if(sendMessageToPlayerService(rooms[roomid]->players[i]->login, message))
            {
                return ERROR_SENDING_MESSAGE;
            }
        }
    }
    return 0;
}
