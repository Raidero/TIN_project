#include "AccountService.h"

int logInService(char* login, char* passhash, char* ip)
{
    if(isLoggedIn(ip))
    {
        fprintf(stderr, "Account %s already logged in\n", login);
        return LOGGED_IN_ERROR;
    }
    if(verifyLoginAndPassword(login, passhash) == 1)
    {
        fprintf(stderr, "Account %s already logged in\n", login);
        return WRONG_LOGIN_OR_PASSWORD;
    }
    //TODO
    return 0;
}

int logOutService(char* ip)
{
    if(!isLoggedIn(ip))
    {
        fprintf(stderr, "Player is not logged in\n");
        return LOGGED_OUT_ERROR;
    }
    //TODO
    return 0;
}

int createAccountService(char* login, char* passhash)
{
    if(isLoginUsed(login))
    {
        fprintf(stderr, "Account %s already exists\n", login);
        return CREATE_ACCOUNT_ERROR;
    }
    //TODO
    return 0;
}

int deleteAccountService(char* login, char* passhash)
{
    if(!isLoginUsed(login))
    {
        fprintf(stderr, "Account %s doesn't exists\n", login);
        return DELETE_ACCOUNT_ERROR;
    }
    //TODO
    return 0;
}

int changePasswordService(char* ip, char* oldpasshash, char* newpasshash)
{
    if(!isLoggedIn(ip))
    {
        fprintf(stderr, "Player is not logged in\n");
        return LOGGED_OUT_ERROR;
    }
    //TODO
    return 0;
}

int updateStats(struct AccountStatistics* stats)
{
    //TODO
    return 0;
}

bool isLoginUsed(char* login)
{
    //TODO
    return 0;
}

bool isLoggedIn(char* ip)
{
    //TODO
    return 0;
}

bool verifyLoginAndPassword(char* login, char* passhash)
{
    //TODO
    return 0;
}
