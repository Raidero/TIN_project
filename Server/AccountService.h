#ifndef ACCOUNT_SERVICE_H
#define ACCOUNT_SERVICE_H
#include <stdio.h>

#define LOGGED_IN_ERROR 1
#define WRONG_LOGIN_OR_PASSWORD 2
#define LOGGED_OUT_ERROR 1
#define CREATE_ACCOUNT_ERROR 1
#define DELETE_ACCOUNT_ERROR 1

typedef char bool;

typedef struct
{

} AccountStatistics;

typedef struct
{
    char* login;
    char* passwordhash;
    char* currentip;
    bool ishost;
} AccountData;

int logInService(char* login, char* passhash, char* ip);
int logOutService(char* ip);
int createAccountService(char* login, char* passhash);
int deleteAccountService(char* login, char* passhash);
int changePasswordService(char* ip, char* oldpasshash, char* newpasshash);


int updateStats(AccountStatistics* stats);
bool isLoginUsed(char* login);
bool isLoggedIn(char* ip);
bool verifyLoginAndPassword(char* login, char* passhash);
char* loginToIp(AccountData* loggedaccounts, int numberofaccounts, char* login);

#endif // ACCOUNT_SERVICE_H
