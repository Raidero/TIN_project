extern "C"
{
#include "../Defines.h"

#include "../AccountService.h"
#include "../RoomService.h"

}

#define BOOST_TEST_MODULE AccountServiceTests


#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(AccountServiceTests)


// createAccountService
BOOST_AUTO_TEST_CASE( GivenAccountData_WhenTryingToCreateAccount_UserIsRegistered )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(verifyLoginAndPassword(account) == 1);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenAccountDataWithRegisteredUser_WhenTryingToCreateAccount_ReturnError )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(createAccountService(account) == CREATE_ACCOUNT_ERROR);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenNonExistentDataFile_WhenTryingToCreateAccount_ReturnError )
{
	deleteDataFile();

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_CHECK(createAccountService(account) == FILE_NOT_OPEN);

}


// deleteAccountService
BOOST_AUTO_TEST_CASE( GivenNonEmptyAccountDataFile_WhenTryingToDeleteUser_UserNoLongerExists )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(deleteAccountService(account) == 0);

	account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);
	BOOST_CHECK(verifyLoginAndPassword(account) == 0);

	deleteDataFile();


}

BOOST_AUTO_TEST_CASE( GivenLoggedInUser_WhenTryingToDeleteItsData_ReturnError )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);
	int playerid = 1;

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(logInService(account, playerid) == 0);

	BOOST_REQUIRE(isLoggedIn(account->currentip) == 1);

	BOOST_CHECK(deleteAccountService(account) == LOGGED_IN_ERROR);

	BOOST_CHECK(verifyLoginAndPassword(account) == 1);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenNonEmptyAccountDataFile_WhenTryingToDeleteNonExistentUser_ReturnError )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_REQUIRE(createAccountService(account) == 0);

	account = initAccoundData((char*)"login2",(unsigned char*)"pass1",1,0);
	BOOST_CHECK(deleteAccountService(account) == DELETE_ACCOUNT_ERROR);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenEmptyAccountDataFile_WhenTryingToDeleteUser_ReturnError )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_CHECK(deleteAccountService(account) == DELETE_ACCOUNT_ERROR);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenWrongPassword_WhenTryingToDeleteUser_UserStillExists )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_REQUIRE(createAccountService(account) == 0);

	account = initAccoundData((char*)"login1",(unsigned char*)"pass2",1,0);

	BOOST_CHECK(deleteAccountService(account) == WRONG_LOGIN_OR_PASSWORD);

	account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_CHECK(verifyLoginAndPassword(account) == 1);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenNonExistentAccountDataFile_WhenTryingToDeleteUser_ReturnError )
{
	deleteDataFile();

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_CHECK(deleteAccountService(account) == FILE_NOT_OPEN);
}


// changePasswordService
BOOST_AUTO_TEST_CASE( GivenNotLoggedInUser_WhenTryingToChangePassword_ReturnError )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(changePasswordService(account, (unsigned char*)"pass2") == LOGGED_OUT_ERROR);

	account = initAccoundData((char*)"login1",(unsigned char*)"pass2",1,0);
	BOOST_CHECK(verifyLoginAndPassword(account) == 0);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenAccountDataFile_WhenTryingToChangePassword_PasswordIsChanged )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);
	int playerid = 1;

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(logInService(account, playerid) == 0);

	BOOST_REQUIRE(isLoggedIn(account->currentip) == 1);

	BOOST_CHECK(changePasswordService(account, (unsigned char*)"pass2") == 0);

	account = initAccoundData((char*)"login1",(unsigned char*)"pass2",1,0);
	BOOST_CHECK(verifyLoginAndPassword(account) == 1);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenEmptyAccountDataFile_WhenTryingToChangePassword_ReturnError )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_CHECK(changePasswordService(account, (unsigned char*)"pass2") != 0);

}

BOOST_AUTO_TEST_CASE( GivenAccountDataWithWrongPassword_WhenTryingToChangePassword_ReturnError )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);
	int playerid = 1;

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(logInService(account, playerid) == 0);

	BOOST_REQUIRE(isLoggedIn(account->currentip) == 1);

	account = initAccoundData((char*)"login1",(unsigned char*)"pass2",1,0);
	BOOST_CHECK(changePasswordService(account, (unsigned char*)"pass3") == WRONG_LOGIN_OR_PASSWORD);

	BOOST_CHECK(verifyLoginAndPassword(account) == 0);


	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenNonExistentAccountDataFile_WhenTryingToChangePassword_ReturnError )
{
	deleteDataFile();

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_CHECK(changePasswordService(account, (unsigned char*)"pass2") == FILE_NOT_OPEN);

	deleteDataFile();

}


// logIn
BOOST_AUTO_TEST_CASE( GivenNonEmptyAccountDataFile_WhenTryingToLogIn_UserIsLoggedIn )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);
	int playerid = 1;	// in case of logInService() return value change to fixed 0

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(logInService(account, playerid) == 0);	// imitation of socketToPlayerId(); also this return is nonsense imo

	BOOST_CHECK(isLoggedIn(account->currentip) == 1);			// those returns man..

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenNonEmptyAccountDataFile_WhenTryingToLogIn_UserIsNotRegistered )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);
	int playerid = 1;

	BOOST_REQUIRE(createAccountService(account) == 0);

	account = initAccoundData((char*)"login2",(unsigned char*)"pass2",1,0);
	BOOST_CHECK(logInService(account, playerid) == WRONG_LOGIN_OR_PASSWORD);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenLoggedInUser_WhenTryingToLogIn_UserIsAlreadyLoggedIn )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);
	int playerid = 1;

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(logInService(account, playerid) == 0);

	BOOST_REQUIRE(isLoggedIn(account->currentip) == 1);

	BOOST_CHECK(logInService(account, playerid) == LOGGED_IN_ERROR);

	deleteDataFile();

}


// logOut
BOOST_AUTO_TEST_CASE( GivenNonEmptyServer_WhenTryingToLogOut_UserIsLoggedOut )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);
	int playerid = 1;

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(logInService(account, playerid) == 0);

	BOOST_REQUIRE(isLoggedIn(account->currentip) == 1);

	BOOST_CHECK(logOutService(account->currentip) == 0);

	BOOST_CHECK(isLoggedIn(account->currentip) == 0);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenEmptyServer_WhenTryingToLogOut_ReturnErrorUserIsNotLoggedIn )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(logOutService(account->currentip) == LOGGED_OUT_ERROR);

	BOOST_CHECK(isLoggedIn(account->currentip) == 0);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenNonEmptyServer_WhenTryingToLogOut_UserIsNotRegistered )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);
	int playerid = 1;

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(logInService(account, playerid) == 0);

	BOOST_REQUIRE(isLoggedIn(account->currentip) == 1);


	account = initAccoundData((char*)"login1",(unsigned char*)"pass2",2,0);
	BOOST_CHECK(logOutService(account->currentip) == LOGGED_OUT_ERROR);

	BOOST_CHECK(isLoggedIn(account->currentip) == 0);

	deleteDataFile();

}


BOOST_AUTO_TEST_SUITE_END()




/*=====================================================*/
// RoomServiceTests

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


// findFreeRoomForAccount
BOOST_AUTO_TEST_CASE( GivenEmptyRoom_WhenTryingToConnect_UserIsConnected )
{
	initRoomService();
	int roomid = 0;
	rooms[roomid] = initRoom();
	BOOST_REQUIRE(rooms[roomid] != NULL);

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_CHECK(findFreeRoomForAccount(account) == roomid);

	char** ll = testInitLL();
	BOOST_REQUIRE(ll != NULL);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == 0);

	BOOST_CHECK(strcmp(ll[0],"login1") == 0);

	testDeleteLL(ll);

	disposeAllRooms();

}

BOOST_AUTO_TEST_CASE( GivenFullRoom_WhenTryingToConnect_ReturnErrorNoRoomFound )
{
	initRoomService();
	int roomid = 0;
	rooms[roomid] = initRoom();
	BOOST_REQUIRE(rooms[roomid] != NULL);

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	for (int i = 0; i < MAX_PLAYER_COUNT; ++i)
		BOOST_CHECK(findFreeRoomForAccount(account) == roomid);

	char** ll = testInitLL();
	BOOST_REQUIRE(ll != NULL);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == 0);

	for (int i = 0; i < MAX_PLAYER_COUNT; ++i)
		BOOST_REQUIRE(strcmp(ll[i],"login1") == 0);

	BOOST_CHECK(findFreeRoomForAccount(account) == FREE_ROOM_NOT_FOUND);

	testDeleteLL(ll);

	disposeAllRooms();

}

BOOST_AUTO_TEST_CASE( GivenRoomInGame_WhenTryingToConnect_ReturnErrorNoRoomFound )
{

	initRoomService();
	int roomid = 0;
	rooms[roomid] = initRoom();
	BOOST_REQUIRE(rooms[roomid] != NULL);

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_CHECK(findFreeRoomForAccount(account) == roomid);

	char** ll = testInitLL();
	BOOST_REQUIRE(ll != NULL);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == 0);

	BOOST_REQUIRE(strcmp(ll[0],"login1") == 0);

	rooms[roomid]->isingame = true;

	BOOST_CHECK(findFreeRoomForAccount(account) == FREE_ROOM_NOT_FOUND);

	testDeleteLL(ll);

	disposeAllRooms();

}

BOOST_AUTO_TEST_CASE( GivenNoRooms_WhenTryingToConnect_ReturnErrorNoRoomFound )
{
	initRoomService();
	int roomid = 0;
	BOOST_REQUIRE(rooms[roomid] == NULL);

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	BOOST_CHECK(findFreeRoomForAccount(account) == FREE_ROOM_NOT_FOUND);

	disposeAllRooms();

}


// createRoomForAccount
BOOST_AUTO_TEST_CASE( GivenNoRooms_WhenTryingToConnect_UserIsConnected )
{
	int roomid;
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	roomid = createRoomForAccount(account);
	BOOST_CHECK(roomid > -1);

	char** ll = testInitLL();
	BOOST_REQUIRE(ll != NULL);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == 0);

	BOOST_CHECK(strcmp(ll[0],"login1") == 0);

	testDeleteLL(ll);

	disposeAllRooms();

}

BOOST_AUTO_TEST_CASE( GivenMaxRooms_WhenTryingToConnect_ReturnErrorRoomLimit )
{
	int roomid;
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	char** ll = testInitLL();
	BOOST_REQUIRE(ll != NULL);

	for (int i = 0; i < MAX_ROOM_COUNT; ++i)
	{
		roomid = createRoomForAccount(account);
		BOOST_CHECK(roomid > -1);

		BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == 0);

		BOOST_CHECK(strcmp(ll[0],"login1") == 0);
	}

	BOOST_CHECK(createRoomForAccount(account) == MAX_ROOM_LIMIT_ERROR);

	testDeleteLL(ll);

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

	char** ll = testInitLL();
	BOOST_REQUIRE(ll != NULL);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == 0);

	BOOST_CHECK(strcmp(ll[0],"login1") == 0);

	testDeleteLL(ll);

	disposeAllRooms();

}

BOOST_AUTO_TEST_CASE( GivenNoRooms_WhenRefreshing_ReturnErrorNotAValidRoom )
{
	initRoomService();
	int roomid = 0;
	BOOST_REQUIRE(rooms[roomid] == NULL);

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	char** ll = testInitLL();
	BOOST_REQUIRE(ll != NULL);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == OUT_OF_RANGE);

	BOOST_CHECK(strcmp(ll[0],"login1") != 0);

	testDeleteLL(ll);

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

	char** ll = testInitLL();
	BOOST_REQUIRE(ll != NULL);

	account = initAccoundData((char*)"login1",(unsigned char*)"pass1",2,0);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == PLAYER_NOT_FOUND);

	testDeleteLL(ll);

	disposeAllRooms();

}

BOOST_AUTO_TEST_CASE( GivenEmptyRoom_WhenRefreshing_ReturnErrorPlayerNotInRoom )
{
	initRoomService();
	int roomid = 0;
	rooms[roomid] = initRoom();
	BOOST_REQUIRE(rooms[roomid] != NULL);

	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);

	char** ll = testInitLL();
	BOOST_REQUIRE(ll != NULL);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == PLAYER_NOT_FOUND);

	testDeleteLL(ll);

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

	char** ll = testInitLL();
	BOOST_REQUIRE(ll != NULL);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == 0);

	BOOST_REQUIRE(strcmp(ll[0],"login1") == 0);

	BOOST_CHECK(sweepPlayer(account, roomid) == 0);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == PLAYER_NOT_FOUND);

	BOOST_CHECK(strcmp(ll[0],"login1") != 0);

	testDeleteLL(ll);

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

	char** ll = testInitLL();
	BOOST_REQUIRE(ll != NULL);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == 0);

	BOOST_REQUIRE(strcmp(ll[0],"login1") == 0);

	account = initAccoundData((char*)"login2",(unsigned char*)"pass1",2,0);
	BOOST_CHECK(sweepPlayer(account, roomid) == PLAYER_NOT_FOUND);

	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == PLAYER_NOT_FOUND);

	account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);
	BOOST_REQUIRE(refreshRoomService(account, roomid, ll) == 0);

	BOOST_CHECK(strcmp(ll[0],"login1") == 0);

	testDeleteLL(ll);

	disposeAllRooms();

}


BOOST_AUTO_TEST_SUITE_END()


/*=====================================================*/
// MenuServiceTests

BOOST_AUTO_TEST_SUITE(MenuServiceTests)


BOOST_AUTO_TEST_CASE( GivenRoomInGame_WhenTryingToConnect_PlayerLandsInNewRoom )
{
// TODO


}







BOOST_AUTO_TEST_SUITE_END()




/*=====================================================*/
// PrivateCommunicationServiceTests

BOOST_AUTO_TEST_SUITE(PrivateCommunicationServiceTests)


BOOST_AUTO_TEST_CASE( GivenNonEmptyServer_WhenTryingToLogOut_ReturnErrorUserIsNotLoggedIn )
{
// TODO




}







BOOST_AUTO_TEST_SUITE_END()


/*=====================================================*/
// ServerTests

BOOST_AUTO_TEST_SUITE(ServerTests)


BOOST_AUTO_TEST_CASE( GivenNonEmptyServer_WhenTryingToLogOut_ReturnErrorUserIsNotLoggedIn )
{
// TODO




}







BOOST_AUTO_TEST_SUITE_END()




/*=====================================================*/
// QueueTests

BOOST_AUTO_TEST_SUITE(QueueTests)


BOOST_AUTO_TEST_CASE( GivenNonEmptyServer_WhenTryingToLogOut_ReturnErrorUserIsNotLoggedIn )
{
// TODO




}







BOOST_AUTO_TEST_SUITE_END()
