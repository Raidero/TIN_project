extern "C"
{
#include "../AccountService.h"
#include "../Defines.h"

}

#define BOOST_TEST_MODULE AccountServiceTests


#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(AccountServiceTests)

//int add( int i, int j ) { return i+j; }


BOOST_AUTO_TEST_CASE( GivenAccountData_TryingToCreateAccount_ProcessIsSuccessful )
{
	/// zmienic totalnie - AccountData* init()
	deleteDataFile();	// empty or create empty datafile

	BOOST_REQUIRE(initDataFile() == 0);

	AccountData account;
	strcpy(account.login, "login1");
	strcpy(account.passwordhash, "hashed1");

	BOOST_REQUIRE(createAccountService(&account) == 0);




    // seven ways to detect and report the same error:
    /*BOOST_CHECK( add( 2,2 ) == 4 );        // #1 continues on error

    BOOST_REQUIRE( add( 2,2 ) == 4 );      // #2 throws on error

    if( add( 2,2 ) != 4 )
      BOOST_ERROR( "Ouch..." );            // #3 continues on error

    if( add( 2,2 ) != 4 )
      BOOST_FAIL( "Ouch..." );             // #4 throws on error

    if( add( 2,2 ) != 4 ) throw "Ouch..."; // #5 throws on error

    BOOST_CHECK_MESSAGE( add( 2,2 ) == 4,  // #6 continues on error
                         "add(..) result: " << add( 2,2 ) );

    BOOST_CHECK_EQUAL( add( 2,2 ), 4 );	  // #7 continues on error*/
}


BOOST_AUTO_TEST_CASE( GivenSameAccountDatas_WhenTryingToCreate2Accounts_ProcessIsUnsuccessful )
{
// TODO

}


// logIn
BOOST_AUTO_TEST_CASE( GivenNonEmptyAccountDataFile_WhenTryingToLogIn_UserIsLoggedIn )
{
// TODO

}

BOOST_AUTO_TEST_CASE( GivenNonEmptyAccountDataFile_WhenTryingToLogIn_UserIsNotRegistered )
{
// TODO

}

BOOST_AUTO_TEST_CASE( GivenLoggedInUser_WhenTryingToLogIn_UserIsAlreadyLoggedIn )
{
// TODO

}

BOOST_AUTO_TEST_CASE( GivenFullServer_WhenTryingToLogIn_ReturnError )
{
// TODO

}


// logOut
BOOST_AUTO_TEST_CASE( GivenNonEmptyServer_WhenTryingToLogOut_UserIsLoggedOut )
{
// TODO

}

BOOST_AUTO_TEST_CASE( GivenNonEmptyServer_WhenTryingToLogOut_ReturnErrorUserIsNotLoggedIn )
{
// TODO

}

BOOST_AUTO_TEST_CASE( GivenNonEmptyServer_WhenTryingToLogOut_UserIsNotRegistered )
{
// TODO

}

BOOST_AUTO_TEST_CASE( GivenEmptyServer_WhenTryingToLogOut_ReturnError )
{
// TODO

}


// createAccountService
BOOST_AUTO_TEST_CASE( GivenAccountData_WhenTryingToCreateAccount_UserIsRegistered )
{
// TODO

}

BOOST_AUTO_TEST_CASE( GivenAccountDataWithRegisteredUser_WhenTryingToCreateAccount_ReturnError )
{
// TODO

}

BOOST_AUTO_TEST_CASE( GivenNonExistentDataFile_WhenTryingToCreateAccount_ReturnError )
{
// TODO

}


// deleteAccountService
BOOST_AUTO_TEST_CASE( GivenNonEmptyAccountDataFile_WhenTryingToDeleteUser_UserNoLongerExists )
{
// TODO

}

BOOST_AUTO_TEST_CASE( GivenLoggedInUser_WhenTryingToDeleteItsData_ReturnError )
{
// TODO

}

BOOST_AUTO_TEST_CASE( GivenNonEmptyAccountDataFile_WhenTryingToDeleteNonExistentUser_ReturnError )
{
// TODO

}

BOOST_AUTO_TEST_CASE( GivenEmptyAccountDataFile_WhenTryingToDeleteUser_ReturnError )
{
// TODO

}

BOOST_AUTO_TEST_CASE( GivenWrongPassword_WhenTryingToDeleteUser_UserStillExists )
{
// TODO

}

BOOST_AUTO_TEST_CASE( GivenNonExistentAccountDataFile_WhenTryingToDeleteUser_ReturnError )
{
// TODO

}


// changePasswordService
BOOST_AUTO_TEST_CASE( GivenAccountDataFile_WhenTryingToChangePassword_PasswordIsChanged )
{
// TODO

}

BOOST_AUTO_TEST_CASE( GivenEmptyAccountDataFile_WhenTryingToChangePassword_ReturnError )
{
// TODO

}

BOOST_AUTO_TEST_CASE( GivenNotLoggedInUser_WhenTryingToChangePassword_ReturnError )
{
// TODO

}

BOOST_AUTO_TEST_CASE( GivenAccountDataWithWrongPassword_WhenTryingToChangePassword_ReturnError )
{
// TODO

}

BOOST_AUTO_TEST_CASE( GivenNonExistentAccountDataFile_WhenTryingToChangePassword_ReturnError )
{
// TODO

}


BOOST_AUTO_TEST_SUITE_END()
