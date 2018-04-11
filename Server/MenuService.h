#ifndef MENU_SERVICE_GUARD
#define MENU_SERVICE_GUARD

#include "RoomService.h"
#include "AccountService.h"

int connectAccountToRoomService(struct Room* rooms, int numberofrooms, char* ip);
int setReadyToStartService();
void voteForHostService(struct Room* room, struct AccountData* loggedaccounts, int numberofaccouns, char* login);

#endif // MENU_SERVICE_GUARD
