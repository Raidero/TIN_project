#ifndef GAME_SERVICE_H
#define GAME_SERVICE_H

#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "RoomService.h"
#include "Defines.h"

typedef struct
{
	int sendsocketfd;
	struct sockaddr_in groupsock;
	const char* mcastgroupaddr;
	uint16_t mcastgroupport;
	const char* myinterfaceaddr;

} mcastdata;	// later delete what's not needed


mcastdata multicastdata[MAX_ROOM_COUNT];	// every room gets one thread with it's mcast data

void initMcastData(int roomid);
int startMulticasting (int roomid);
int initMulticastGroup(int* sendsocketfd, struct sockaddr_in* groupsock, const char* mcastgroupaddr, uint16_t mcastgroupport, const char* myinterfaceaddr);	// finished, somewhat tested
int sendMulticastData(int roomid, char* buf, int size);

#endif // GAME_SERVICE_H
