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


BOOST_AUTO_TEST_CASE( GivenAccount_TryToCreateAccount_Successful )
{
	datafile = fopen(DATA_FILE_NAME, "wr+b");
	fclose(datafile);

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


BOOST_AUTO_TEST_SUITE_END()
