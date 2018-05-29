#ifndef GAME_SERVICE_H
#define GAME_SERVICE_H

#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "RoomService.h"
#include "Defines.h"

// server defaults
#define DEFAULT_MULTICAST_ADDRESS "226.1.1.1"
#define DEFAULT_MULTICAST_PORT 4321
#define DEFAULT_INTERFACE_ADDRESS "192.168.1.24"

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
int startMatchService (int roomid, int accountid);
int startMulticasting (int roomid);
int initMulticastGroup(int* sendsocketfd, struct sockaddr_in* groupsock, const char* mcastgroupaddr, uint16_t mcastgroupport, const char* myinterfaceaddr);	// finished, somewhat tested

#endif // GAME_SERVICE_H
