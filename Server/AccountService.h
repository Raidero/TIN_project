#ifndef ACCOUNT_SERVICE_H
#define ACCOUNT_SERVICE_H

#include <stdint.h>
#include <stdlib.h>
#include "Defines.h"
#include <stdio.h>
#include <string.h>

#ifndef __cplusplus
typedef char bool;
#endif // __cplusplus

typedef struct
{

} AccountStatistics;

typedef struct
{
    char login[MAX_LOGIN_LENGTH];
    unsigned char passwordhash[MAX_PASSHASH_LENGTH];
    uint32_t currentip;                     //changed so it now reflects ip from in_addr struct
} AccountData;

extern AccountData* loggedaccounts[MAX_ACCOUNTS_COUNT];
extern FILE* datafile;

void initAccountService();
AccountData* initAccoundData(char* login, unsigned char* passwordhash, uint32_t currentip, int votercounter);
void disposeAccountData(int i);
void disposeAllAccounts();
int logInService(AccountData* account, int playerid);    //there needs to be added max login length and max passhash length
int logOutService(uint32_t ip); //fully done, someone please check
int createAccountService(AccountData* account); //max..
int deleteAccountService(AccountData* account); //maxx...
int changePasswordService(AccountData* account, unsigned char* newpasshash); //max lenth passhashnew !!

int updateStats(AccountStatistics* stats); //TODO
bool isLoginUsed(char* login); // fully done, Krzysiu or Bartek, please check
bool isLoggedIn(uint32_t ip);  // fully done, Krzysiu or Bartek, please check
bool verifyLoginAndPassword(AccountData* account); // done
uint32_t loginToIp(AccountData** accountsinroom, char* login); //fully done, Krzysiu or Bartek, please check
int loginToPlayerId(char* login);
int initDataFile();
int closeDataFile();
int deleteDataFile();
#endif // ACCOUNT_SERVICE_H
