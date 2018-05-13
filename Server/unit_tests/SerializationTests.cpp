extern "C"
{
#include "../Defines.h"

#include "../Serialization.h"

}
#include <limits.h>
#include <stdint.h>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>


BOOST_AUTO_TEST_SUITE(SerializationTests)

int getMaxInt()
{
	int i = 1;
	while (i + 1 > 0)
	{
		i = i << 1;
		++i;
	}
	return i;
}

BOOST_AUTO_TEST_CASE( GivenInteger_WhenSerializingAndDeserializing_ValuesAreSame )
{
	int test1 = INT_MIN;
	int test2 = -1;
	int test3 = 0;
	int test4 = 1;
	int test5 = INT_MAX;

	int out1 = 10;
	int out2 = 20;
	int out3 = 30;
	int out4 = 40;
	int out5 = 50;

	unsigned char buf1[20];
	unsigned char* buf2 = buf1;

	buf2 = serializeInt(buf2, test1);
	buf2 = serializeInt(buf2, test2);
	buf2 = serializeInt(buf2, test3);
	buf2 = serializeInt(buf2, test4);
	buf2 = serializeInt(buf2, test5);

	BOOST_CHECK(buf2 == buf1+5*sizeof(int));

	buf2 = buf1;

	buf2 = deserializeInt(buf2, &out1);
	buf2 = deserializeInt(buf2, &out2);
	buf2 = deserializeInt(buf2, &out3);
	buf2 = deserializeInt(buf2, &out4);
	buf2 = deserializeInt(buf2, &out5);
	BOOST_CHECK(buf2 == buf1+5*sizeof(int));

	BOOST_CHECK(test1 == out1);
	BOOST_CHECK(test2 == out2);
	BOOST_CHECK(test3 == out3);
	BOOST_CHECK(test4 == out4);
	BOOST_CHECK(test5 == out5);

}

BOOST_AUTO_TEST_CASE( GivenUint32t_WhenSerializingAndDeserializing_ValuesAreSame )
{
	uint32_t test1 = 0;
	uint32_t test2 = 1;
	uint32_t test3 = UINT32_MAX;

	uint32_t out1 = 10;
	uint32_t out2 = 20;
	uint32_t out3 = 30;

	unsigned char buf1[12];
	unsigned char* buf2 = buf1;

	buf2 = serializeUint_32_t(buf2, test1);
	buf2 = serializeUint_32_t(buf2, test2);
	buf2 = serializeUint_32_t(buf2, test3);

	BOOST_CHECK(buf2 == buf1+3*sizeof(uint32_t));

	buf2 = buf1;

	buf2 = deserializeUint_32_t(buf2, &out1);
	buf2 = deserializeUint_32_t(buf2, &out2);
	buf2 = deserializeUint_32_t(buf2, &out3);
	BOOST_CHECK(buf2 == buf1+3*sizeof(uint32_t));

	BOOST_CHECK(test1 == out1);
	BOOST_CHECK(test2 == out2);
	BOOST_CHECK(test3 == out3);

}

BOOST_AUTO_TEST_CASE( GivenCharArray_WhenSerializingAndDeserializing_ValuesAreSame )
{
	char test1[] = {'\0'};
	char test2[] = "This is a test";

	char out1[1];
	char out2[15];

	unsigned char buf1[16];
	unsigned char* buf2 = buf1;

	buf2 = serializeCharArray(buf2, test1, 1);
	buf2 = serializeCharArray(buf2, test2, 15);

	BOOST_CHECK(buf2 == buf1+16*sizeof(char));

	buf2 = buf1;

	buf2 = deserializeCharArray(buf2, out1, 1);
	buf2 = deserializeCharArray(buf2, out2, 15);
	BOOST_CHECK(buf2 == buf1+16*sizeof(char));

	BOOST_CHECK(strcmp(test1, out1) == 0);
	BOOST_CHECK(strcmp(test2, out2) == 0);

}

BOOST_AUTO_TEST_CASE( GivenUnsignedCharArray_WhenSerializingAndDeserializing_ValuesAreSame )
{
	unsigned char test1[] = {'\0'};
	unsigned char test2[] = "This is a test";

	unsigned char out1[1];
	unsigned char out2[15];

	unsigned char buf1[16];
	unsigned char* buf2 = buf1;

	buf2 = serializeUnsignedCharArray(buf2, test1, 1);
	buf2 = serializeUnsignedCharArray(buf2, test2, 15);

	BOOST_CHECK(buf2 == buf1+16*sizeof(unsigned char));

	buf2 = buf1;

	buf2 = deserializeUnsignedCharArray(buf2, out1, 1);
	buf2 = deserializeUnsignedCharArray(buf2, out2, 15);
	BOOST_CHECK(buf2 == buf1+16*sizeof(unsigned char));

	BOOST_CHECK(memcmp(test1, out1, 1) == 0);
	BOOST_CHECK(memcmp(test2, out2, 15) == 0);

}

BOOST_AUTO_TEST_CASE( GivenAccountData_WhenSerializingAndDeserializing_ValuesAreSame )
{
	AccountData *test1 = initAccoundData((char*)"testlogin1", (unsigned char*)"testpass1", 0, 0);
	AccountData *test2 = initAccoundData((char*)"testlogin2", (unsigned char*)"testpass2", 1, 1);

	AccountData *out1 = (AccountData*)malloc(sizeof(AccountData));
	AccountData *out2 = (AccountData*)malloc(sizeof(AccountData));

	unsigned char buf1[2*(4+4+MAX_LOGIN_LENGTH+MAX_PASSHASH_LENGTH)];
	unsigned char* buf2 = buf1;

	buf2 = serializeAccountData(buf2, test1);
	buf2 = serializeAccountData(buf2, test2);

	BOOST_CHECK(buf2 == buf1+2*(4+4+MAX_LOGIN_LENGTH+MAX_PASSHASH_LENGTH)*sizeof(unsigned char));

	buf2 = buf1;

	buf2 = deserializeAccountData(buf2, out1);
	buf2 = deserializeAccountData(buf2, out2);
	BOOST_CHECK(buf2 == buf1+2*(4+4+MAX_LOGIN_LENGTH+MAX_PASSHASH_LENGTH)*sizeof(unsigned char));

	BOOST_CHECK(test1->currentip == out1->currentip);
	BOOST_CHECK(strcmp(test1->login, out1->login) == 0);
	BOOST_CHECK(memcmp(test1->passwordhash, out1->passwordhash, 10) == 0);
	BOOST_CHECK(test1->votercounter == out1->votercounter);

	BOOST_CHECK(test2->currentip == out2->currentip);
	BOOST_CHECK(strcmp(test2->login, out2->login) == 0);
	BOOST_CHECK(memcmp(test2->passwordhash, out2->passwordhash, 10) == 0);
	BOOST_CHECK(test2->votercounter == out2->votercounter);

}


BOOST_AUTO_TEST_SUITE_END()
