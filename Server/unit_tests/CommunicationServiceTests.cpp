extern "C"
{
#include "../Defines.h"

#include "../CommunicationService.h"

}
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>


BOOST_AUTO_TEST_SUITE(CommunicationServiceTests)
/// ONE BIG TODO

int getWriteFD()
{
	return open("TestFile", O_TRUNC | O_CREAT | O_WRONLY, S_IRWXO | S_IRWXG | S_IRWXU);
}

int getReadFD()
{
	return open("TestFile", O_RDONLY);
}

int getWRFD()
{
	return open("TestFile", O_TRUNC | O_CREAT | O_RDWR, S_IRWXO | S_IRWXG | S_IRWXU);
}

void closeFD(int fd)
{
	close(fd);
}

// sendAll
BOOST_AUTO_TEST_CASE( GivenLongMessage_WhenSending_EverythingGetsSent )
{
// TODO
	int fd = getWRFD();
	char buf[1024], rd[1024];
	for (int i = 0; i < 1024; ++i)
		buf[i] = ('A'+i) % ('Z'+1);	// "ABCD...XYZABCD..."

	/*BOOST_REQUIRE(fd > 0);	// >0?

	BOOST_REQUIRE(send_all(fd, buf, 1024) == 0);

	read(fd, rd, 1024);

	BOOST_REQUIRE(strcmp(rd,buf) == 0);
*/
	closeFD(fd);

}

BOOST_AUTO_TEST_CASE( Given_When_Return )
{
// TODO


}

// sendMessageToPlayerService
BOOST_AUTO_TEST_CASE( GivenLoggedInPlayer_WhenTryingToSendMessage_MessageIsSent )
{
// TODO
	deleteDataFile();

	initAccountService();
	AccountData* account = initAccoundData((char*)"login1",(unsigned char*)"pass1",1,0);
	int playerid = 1;

	BOOST_REQUIRE(createAccountService(account) == 0);

	BOOST_CHECK(logInService(account, playerid) == 0);

	BOOST_REQUIRE(isLoggedIn(account->currentip) == 1);



}

// sendMessageToRoomService
BOOST_AUTO_TEST_CASE( GivenNotLoggedInPlayer_WhenTryingToSendMessage_ReturnErrorPlayerNotFound )
{
// TODO




}


BOOST_AUTO_TEST_SUITE_END()
