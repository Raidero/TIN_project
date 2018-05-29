extern "C"
{
#include "../Defines.h"

#include "../RoomService.h"

}


#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>


BOOST_AUTO_TEST_SUITE(RoomServiceTests)


char** testInitLL ()
{
	char** buf;
	buf = new (std::nothrow) char*[MAX_PLAYER_COUNT];
	if (buf == NULL)
		return NULL;

	for (int i = 0; i < MAX_PLAYER_COUNT; ++i)
	{
		buf[i] = new (std::nothrow) char[MAX_LOGIN_LENGTH];
		if (buf[i] == NULL)
		{
			for (;i >-1; --i)
				delete[] buf[i];
			delete[] buf;
			return NULL;
		}
	}
	return buf;
}

void testDeleteLL (char** buf)
{
	for (int i = 0; i < MAX_PLAYER_COUNT; ++i)
		delete[] buf[i];
	delete[] buf;

}

char* testInitLL2 ()
{
	char* buf;
	buf = new (std::nothrow) char[MAX_PLAYER_COUNT*MAX_LOGIN_LENGTH*sizeof(char)];
	return buf;
}

void testDeleteLL2 (char* buf)
{
	delete [] buf;
}


// findFreeRoomForAccount
BOOST_AUTO_TEST_CASE( GivenEmptyRoom_WhenTryingToConnect_UserIsConnected )
{
	initRoomService();
	int roomid = 0;
	int accountid = 0;

	rooms[roomid] = initRoom();
	BOOST_REQUIRE(rooms[roomid] != NULL);

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);
	loggedaccounts[accountid] = account;

	BOOST_CHECK(findFreeRoomForAccount(accountid) == roomid);

	char* ll = testInitLL2();
	BOOST_REQUIRE(ll != NULL);

	BOOST_REQUIRE(refreshRoomService(accountid, roomid, ll) == 0);

	BOOST_CHECK(strcmp(ll,"login1") == 0);

	testDeleteLL2(ll);

	disposeAllRooms();

}

BOOST_AUTO_TEST_CASE( GivenFullRoom_WhenTryingToConnect_ReturnErrorNoRoomFound )
{
	initRoomService();
	int roomid = 0;
	int accountid = 0;

	rooms[roomid] = initRoom();
	BOOST_REQUIRE(rooms[roomid] != NULL);

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);
	loggedaccounts[accountid] = account;

	for (int i = 0; i < MAX_PLAYER_COUNT; ++i)
		BOOST_CHECK(findFreeRoomForAccount(accountid) == roomid);

	char* ll = testInitLL2();
	BOOST_REQUIRE(ll != NULL);

	BOOST_REQUIRE(refreshRoomService(accountid, roomid, ll) == 0);

	for (int i = 0; i < MAX_PLAYER_COUNT; ++i)
		BOOST_REQUIRE(strcmp(&ll[i*MAX_LOGIN_LENGTH],"login1") == 0);

	BOOST_CHECK(findFreeRoomForAccount(accountid) == FREE_ROOM_NOT_FOUND);

	testDeleteLL2(ll);

	disposeAllRooms();

}

BOOST_AUTO_TEST_CASE( GivenRoomInGame_WhenTryingToConnect_ReturnErrorNoRoomFound )
{

	initRoomService();
	int roomid = 0;
	int accountid = 0;

	rooms[roomid] = initRoom();
	BOOST_REQUIRE(rooms[roomid] != NULL);

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);
	loggedaccounts[accountid] = account;

	BOOST_CHECK(findFreeRoomForAccount(accountid) == roomid);

	char* ll = testInitLL2();
	BOOST_REQUIRE(ll != NULL);

	BOOST_REQUIRE(refreshRoomService(accountid, roomid, ll) == 0);

	BOOST_REQUIRE(strcmp(ll,"login1") == 0);

	rooms[roomid]->isingame = true;

	BOOST_CHECK(findFreeRoomForAccount(accountid) == FREE_ROOM_NOT_FOUND);

	testDeleteLL2(ll);

	disposeAllRooms();

}

BOOST_AUTO_TEST_CASE( GivenNoRooms_WhenTryingToConnect_ReturnErrorNoRoomFound )
{
	initRoomService();
	int roomid = 0;
	int accountid = 0;

	BOOST_REQUIRE(rooms[roomid] == NULL);

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);
	loggedaccounts[accountid] = account;

	BOOST_CHECK(findFreeRoomForAccount(accountid) == FREE_ROOM_NOT_FOUND);

	disposeAllRooms();

}


// createRoomForAccount
BOOST_AUTO_TEST_CASE( GivenNoRooms_WhenTryingToConnect_UserIsConnected )
{
	int roomid;
	int accountid = 0;

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);
	loggedaccounts[accountid] = account;

	roomid = createRoomForAccount(accountid);
	BOOST_CHECK(roomid > -1);

	char* ll = testInitLL2();
	BOOST_REQUIRE(ll != NULL);

	BOOST_REQUIRE(refreshRoomService(accountid, roomid, ll) == 0);

	BOOST_CHECK(strcmp(ll,"login1") == 0);

	testDeleteLL2(ll);

	disposeAllRooms();

}

BOOST_AUTO_TEST_CASE( GivenMaxRooms_WhenTryingToConnect_ReturnErrorRoomLimit )
{
	int roomid;
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	char* ll = testInitLL2();
	BOOST_REQUIRE(ll != NULL);

	for (int i = 0; i < MAX_ROOM_COUNT; ++i)
	{
		roomid = createRoomForAccount(account);
		BOOST_CHECK(roomid > -1);

		BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == 0);

		BOOST_CHECK(strcmp(ll[0],"login1") == 0);
	}

	BOOST_CHECK(createRoomForAccount(account) == MAX_ROOM_LIMIT_ERROR);

	testDeleteLL2(ll);

	disposeAllRooms();

}


// refreshRoomService
BOOST_AUTO_TEST_CASE( GivenNonEmptyRoom_WhenRefreshing_PlayerIsInRoom )
{
	initRoomService();
	int roomid = 0;
	rooms[roomid] = initRoom();
	BOOST_REQUIRE(rooms[roomid] != NULL);

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_CHECK(findFreeRoomForAccount(account) == roomid);

	char* ll = testInitLL2();
	BOOST_REQUIRE(ll != NULL);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == 0);

	BOOST_CHECK(strcmp(ll[0],"login1") == 0);

	testDeleteLL2(ll);

	disposeAllRooms();

}

BOOST_AUTO_TEST_CASE( GivenNoRooms_WhenRefreshing_ReturnErrorNotAValidRoom )
{
	initRoomService();
	int roomid = 0;
	BOOST_REQUIRE(rooms[roomid] == NULL);

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	char* ll = testInitLL2();
	BOOST_REQUIRE(ll != NULL);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == OUT_OF_RANGE);

	BOOST_CHECK(strcmp(ll[0],"login1") != 0);

	testDeleteLL2(ll);

	disposeAllRooms();

}

BOOST_AUTO_TEST_CASE( GivenNonEmptyRoom_WhenRefreshing_ReturnErrorPlayerNotInRoom )
{
	initRoomService();
	int roomid = 0;
	rooms[roomid] = initRoom();
	BOOST_REQUIRE(rooms[roomid] != NULL);

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_CHECK(findFreeRoomForAccount(account) == roomid);

	char* ll = testInitLL2();
	BOOST_REQUIRE(ll != NULL);

	account = initAccoundData((char*)"login1",(unsigned char*)"pass1",2,0);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == PLAYER_NOT_FOUND);

	testDeleteLL2(ll);

	disposeAllRooms();

}

BOOST_AUTO_TEST_CASE( GivenEmptyRoom_WhenRefreshing_ReturnErrorPlayerNotInRoom )
{
	initRoomService();
	int roomid = 0;
	rooms[roomid] = initRoom();
	BOOST_REQUIRE(rooms[roomid] != NULL);

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	char* ll = testInitLL2();
	BOOST_REQUIRE(ll != NULL);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == PLAYER_NOT_FOUND);

	testDeleteLL2(ll);

	disposeAllRooms();

}


// exitRoomService
BOOST_AUTO_TEST_CASE( GivenNonEmptyRoom_WhenTryingToDisconnect_PlayerIsDisconnected )
{
	initRoomService();
	int roomid = 0;
	rooms[roomid] = initRoom();
	BOOST_REQUIRE(rooms[roomid] != NULL);

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_REQUIRE(findFreeRoomForAccount(account) == roomid);

	char* ll = testInitLL2();
	BOOST_REQUIRE(ll != NULL);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == 0);

	BOOST_REQUIRE(strcmp(ll[0],"login1") == 0);

	BOOST_CHECK(sweepPlayer(account, roomid) == 0);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == PLAYER_NOT_FOUND);

	BOOST_CHECK(strcmp(ll[0],"login1") != 0);

	testDeleteLL2(ll);

	disposeAllRooms();

}

BOOST_AUTO_TEST_CASE( GivenNonExistentRoom_WhenTryingToDisconnect_ReturnErrorNotAVaildRoom )
{
	initRoomService();
	int roomid = 0;
	BOOST_REQUIRE(rooms[roomid] == NULL);

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_CHECK(sweepPlayer(account, roomid) == OUT_OF_RANGE);

	disposeAllRooms();

}

BOOST_AUTO_TEST_CASE( GivenNonEmptyRoom_WhenTryingToDisconnect_ReturnErrorPlayerNotFound )
{
	initRoomService();
	int roomid = 0;
	rooms[roomid] = initRoom();
	BOOST_REQUIRE(rooms[roomid] != NULL);

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_REQUIRE(findFreeRoomForAccount(account) == roomid);

	char* ll = testInitLL2();
	BOOST_REQUIRE(ll != NULL);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == 0);

	BOOST_REQUIRE(strcmp(ll[0],"login1") == 0);

	account = initAccoundData((char*)"login2",(unsigned char*)"pass1",2,0);
	BOOST_CHECK(sweepPlayer(account, roomid) == PLAYER_NOT_FOUND);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == PLAYER_NOT_FOUND);

	account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);
	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == 0);

	BOOST_CHECK(strcmp(ll[0],"login1") == 0);

	testDeleteLL2(ll);

	disposeAllRooms();

}


BOOST_AUTO_TEST_SUITE_END()
