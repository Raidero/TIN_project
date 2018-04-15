#ifndef ACCOUNT_SERVICE_H
#define ACCOUNT_SERVICE_H

#include <stdint.h>
#include <stdlib.h>
#include "Defines.h"

typedef char bool;

typedef struct
{

} AccountStatistics;

typedef struct
{
    uint16_t accountid;                     //place in file
    char login[MAX_LOGIN_LENGTH];
    char passwordhash[MAX_PASSHASH_LENGTH];
    uint32_t currentip;                     //changed so it now reflects ip from in_addr struct
    int votercounter;
} AccountData;

extern AccountData* loggedaccounts[MAX_ACCOUNTS_COUNT];

int logInService(AccountData* account);    //there needs to be added max login length and max passhash length
int logOutService(uint32_t ip); //fully done, someone please check
int createAccountService(AccountData* account); //max..
int deleteAccountService(AccountData* account); //maxx...
int changePasswordService(AccountData* account, char* newpasshash); //max lenth passhashnew !!


int updateStats(AccountStatistics* stats); //TODO
bool isLoginUsed(char* login); // fully done, Krzysiu or Bartek, please check
bool isLoggedIn(uint32_t ip);  // fully done, Krzysiu or Bartek, please check
bool verifyLoginAndPassword(AccountData* account); //TODO
uint32_t loginToIp(AccountData** accountsinroom, char* login); //fully done, Krzysiu or Bartek, please check

#endif // ACCOUNT_SERVICE_H
