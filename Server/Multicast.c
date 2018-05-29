#include "Multicast.h"

extern Room* rooms[MAX_ROOM_COUNT];

void initMcastData(int roomid)
{
    multicastdata[roomid].sendsocketfd = -1;
    bzero((char *) &multicastdata[roomid].groupsock, sizeof(multicastdata[roomid].groupsock));
    multicastdata[roomid].mcastgroupaddr = DEFAULT_MULTICAST_ADDRESS;	// ?
    multicastdata[roomid].mcastgroupport = DEFAULT_MULTICAST_PORT + roomid;	// ??
    multicastdata[roomid].myinterfaceaddr= DEFAULT_INTERFACE_ADDRESS;	// this is the interface address of server // so we can change it to the one in defines.h, right?

}

int startMatchService (int roomid, int accountid)
{
    int i;
	// do we really need those checks?
    if(roomid < 0 || roomid >= MAX_ROOM_COUNT || rooms[roomid] == NULL)
    {
        fprintf(stderr, "Room ID out of range: %d\n", roomid);
        return OUT_OF_RANGE;
    }

    if(rooms[roomid]->isingame == 1)
    {
        fprintf(stderr, "Room already in game: %d\n", roomid);
        return OUT_OF_RANGE;	// idk
    }

    for(i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        if(rooms[roomid]->isplayerready[i] != 1)
        {
            fprintf(stderr, "Not all players ready: %d\n", roomid);
            return OUT_OF_RANGE;	// idk
        }
    }

	if (strcmp(loggedaccounts[accountid]->login, rooms[roomid]->players[rooms[roomid]->hostindex]->login) != 0)
	{
		fprintf(stderr, "Match start requested by not a host\n");
		return OUT_OF_RANGE;	// change that later
	}

    if (startMulticasting(roomid) < 0)	// this one is important
    {
        // handle this error
        return -1;
    }

    rooms[roomid]->isingame = 1;

    return 0;
}

int startMulticasting(int roomid)	// it basically wraps initMulticastGroup(..)
{

    initMcastData(roomid);

    if (initMulticastGroup(	&multicastdata[roomid].sendsocketfd,
                            &multicastdata[roomid].groupsock,
                            multicastdata[roomid].mcastgroupaddr,
                            multicastdata[roomid].mcastgroupport,
                            multicastdata[roomid].myinterfaceaddr) != 0)
    {
        //do some error handling
        return -1;
    }

    return 0;
}

int initMulticastGroup(int* sendsocketfd, struct sockaddr_in* groupsock, const char* mcastgroupaddr, uint16_t mcastgroupport, const char* myinterfaceaddr)
{
    struct in_addr localinterface;

    *sendsocketfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (*sendsocketfd < 0)
    {
        fprintf(stderr, "ERROR opening socket\n");
        return ERROR_OPENING_SOCKET;
    }

    bzero((char *) groupsock, sizeof(*groupsock));
    groupsock->sin_family = AF_INET;
    groupsock->sin_addr.s_addr = inet_addr(mcastgroupaddr);
    groupsock->sin_port = htons(mcastgroupport);

    /* we want loopback so we don't have to write additional code for "game host" */

    localinterface.s_addr = inet_addr(myinterfaceaddr);
    if(setsockopt(*sendsocketfd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localinterface, sizeof(localinterface)) < 0)
    {
        fprintf(stderr, "ERROR setting local interface\n");
        return ERROR_SETTING_SOCKET_OPTIONS;
    }

    return 0;
}
