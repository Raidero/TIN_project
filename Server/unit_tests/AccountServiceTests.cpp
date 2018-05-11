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
	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(verifyLoginAndPassword(account) == 1);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenAccountDataWithRegisteredUser_WhenTryingToCreateAccount_ReturnError )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(createAccountService(account) == CREATE_ACCOUNT_ERROR);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenNonExistentDataFile_WhenTryingToCreateAccount_ReturnError )
{
	deleteDataFile();

	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);

	BOOST_CHECK(createAccountService(account) == FILE_NOT_OPEN);

}


// deleteAccountService
BOOST_AUTO_TEST_CASE( GivenNonEmptyAccountDataFile_WhenTryingToDeleteUser_UserNoLongerExists )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(deleteAccountService(account) == 0);

	account = initAccoundData((char*)"login1",(char*)"pass1",1,0);
	BOOST_CHECK(verifyLoginAndPassword(account) == 0);

	deleteDataFile();


}

BOOST_AUTO_TEST_CASE( GivenLoggedInUser_WhenTryingToDeleteItsData_ReturnError )
{
// TODO

}

BOOST_AUTO_TEST_CASE( GivenNonEmptyAccountDataFile_WhenTryingToDeleteNonExistentUser_ReturnError )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);

	BOOST_REQUIRE(createAccountService(account) == 0);

	account = initAccoundData((char*)"login2",(char*)"pass1",1,0);
	BOOST_CHECK(deleteAccountService(account) == DELETE_ACCOUNT_ERROR);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenEmptyAccountDataFile_WhenTryingToDeleteUser_ReturnError )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);

	BOOST_CHECK(deleteAccountService(account) == DELETE_ACCOUNT_ERROR);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenWrongPassword_WhenTryingToDeleteUser_UserStillExists )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);

	BOOST_REQUIRE(createAccountService(account) == 0);

	account = initAccoundData((char*)"login1",(char*)"pass2",1,0);

	BOOST_CHECK(deleteAccountService(account) == WRONG_LOGIN_OR_PASSWORD);

	account = initAccoundData((char*)"login1",(char*)"pass1",1,0);

	BOOST_CHECK(verifyLoginAndPassword(account) == 1);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenNonExistentAccountDataFile_WhenTryingToDeleteUser_ReturnError )
{
	deleteDataFile();

	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);

	BOOST_CHECK(deleteAccountService(account) == FILE_NOT_OPEN);
}


// changePasswordService
BOOST_AUTO_TEST_CASE( GivenNotLoggedInUser_WhenTryingToChangePassword_ReturnError )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(changePasswordService(account, (char*)"pass2") == LOGGED_OUT_ERROR);

	account = initAccoundData((char*)"login1",(char*)"pass2",1,0);
	BOOST_CHECK(verifyLoginAndPassword(account) == 0);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenAccountDataFile_WhenTryingToChangePassword_PasswordIsChanged )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);
	int playerid = 1;

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(logInService(account, playerid) == playerid);

	BOOST_REQUIRE(isLoggedIn(account->currentip) == 1);

	BOOST_CHECK(changePasswordService(account, (char*)"pass2") == 0);

	account = initAccoundData((char*)"login1",(char*)"pass2",1,0);
	BOOST_CHECK(verifyLoginAndPassword(account) == 1);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenEmptyAccountDataFile_WhenTryingToChangePassword_ReturnError )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);

	BOOST_CHECK(changePasswordService(account, (char*)"pass2") != 0);

}

BOOST_AUTO_TEST_CASE( GivenAccountDataWithWrongPassword_WhenTryingToChangePassword_ReturnError )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);
	int playerid = 1;

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(logInService(account, playerid) == playerid);

	BOOST_REQUIRE(isLoggedIn(account->currentip) == 1);

	account = initAccoundData((char*)"login1",(char*)"pass2",1,0);
	BOOST_CHECK(changePasswordService(account, (char*)"pass3") == WRONG_LOGIN_OR_PASSWORD);

	BOOST_CHECK(verifyLoginAndPassword(account) == 0);


	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenNonExistentAccountDataFile_WhenTryingToChangePassword_ReturnError )
{
	deleteDataFile();

	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);

	BOOST_CHECK(changePasswordService(account, (char*)"pass2") == FILE_NOT_OPEN);

	deleteDataFile();

}


// logIn
BOOST_AUTO_TEST_CASE( GivenNonEmptyAccountDataFile_WhenTryingToLogIn_UserIsLoggedIn )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);
	int playerid = 1;	// in case of logInService() return value change to fixed 0

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(logInService(account, playerid) == playerid);	// imitation of socketToPlayerId(); also this return is nonsense imo

	BOOST_CHECK(isLoggedIn(account->currentip) == 1);			// those returns man..

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenNonEmptyAccountDataFile_WhenTryingToLogIn_UserIsNotRegistered )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);
	int playerid = 1;

	BOOST_REQUIRE(createAccountService(account) == 0);

	account = initAccoundData((char*)"login2",(char*)"pass2",1,0);
	BOOST_CHECK(logInService(account, playerid) == WRONG_LOGIN_OR_PASSWORD);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenLoggedInUser_WhenTryingToLogIn_UserIsAlreadyLoggedIn )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);
	int playerid = 1;

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(logInService(account, playerid) == playerid);

	BOOST_REQUIRE(isLoggedIn(account->currentip) == 1);

	BOOST_CHECK(logInService(account, playerid) == LOGGED_IN_ERROR);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenFullServer_WhenTryingToLogIn_ReturnError )
{
// TODO

}


// logOut
BOOST_AUTO_TEST_CASE( GivenNonEmptyServer_WhenTryingToLogOut_UserIsLoggedOut )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);
	int playerid = 1;

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(logInService(account, playerid) == playerid);

	BOOST_REQUIRE(isLoggedIn(account->currentip) == 1);

	BOOST_CHECK(logOutService(account->currentip) == 0);

	BOOST_CHECK(isLoggedIn(account->currentip) == 0);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenEmptyServer_WhenTryingToLogOut_ReturnErrorUserIsNotLoggedIn )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(logOutService(account->currentip) == LOGGED_OUT_ERROR);

	BOOST_CHECK(isLoggedIn(account->currentip) == 0);

	deleteDataFile();

}

BOOST_AUTO_TEST_CASE( GivenNonEmptyServer_WhenTryingToLogOut_UserIsNotRegistered )
{
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(char*)"pass1",1,0);
	int playerid = 1;

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(logInService(account, playerid) == playerid);

	BOOST_REQUIRE(isLoggedIn(account->currentip) == 1);


	account = initAccoundData((char*)"login1",(char*)"pass2",2,0);
	BOOST_CHECK(logOutService(account->currentip) == LOGGED_OUT_ERROR);

	BOOST_CHECK(isLoggedIn(account->currentip) == 0);

	deleteDataFile();

}


BOOST_AUTO_TEST_SUITE_END()




/*=====================================================*/
// RoomServiceTests

BOOST_AUTO_TEST_SUITE(RoomServiceTests)


BOOST_AUTO_TEST_CASE( GivenEmptyRoom_WhenTryingToConnect_UserIsConnected)
{
// TODO
	initRoomService();
	rooms[0] = initRoom();
	BOOST_REQUIRE(rooms[0] != NULL);	// initRoom should take into account that malloc() can fail


	disposeAllRooms();

}

BOOST_AUTO_TEST_CASE( GivenRoomInGame_WhenTryingToConnect_PlayerLandsInNewRoom)
{
// TODO


}

BOOST_AUTO_TEST_CASE( GivenFullRoom_WhenTryingToConnect_PlayerLandsInNewRoom)
{
// TODO


}


BOOST_AUTO_TEST_CASE( GivenRoomWithAPlayer_WhenTryingToDisconnect_RoomIsEmpty)
{
// TODO


}



BOOST_AUTO_TEST_SUITE_END()


/*=====================================================*/
// MenuServiceTests

BOOST_AUTO_TEST_SUITE(MenuServiceTests)


BOOST_AUTO_TEST_CASE( GivenNonEmptyServer_WhenTryingToLogOut_ReturnErrorUserIsNotLoggedIn )
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
