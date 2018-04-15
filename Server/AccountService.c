#include "AccountService.h"
#include <stdio.h>
#include <string.h>

AccountData* loggedaccounts[MAX_ACCOUNTS_COUNT];

int logInService(AccountData* account)
{
    if(isLoggedIn(account->currentip))
    {
        fprintf(stderr, "Player %s already logged in\n", account->login);
        return LOGGED_IN_ERROR;
    }
    if(verifyLoginAndPassword(account))
    {
        fprintf(stderr, "Player %s already logged in\n", account->login);
        return WRONG_LOGIN_OR_PASSWORD;
    }
    //TODO find login and hash for password in file, if there is, load it
    return 0;
}

int logOutService(uint32_t ip)
{
    int i;
    for(i = 0; i < MAX_ACCOUNTS_COUNT; ++i)
    {
        if(loggedaccounts[i] != NULL && loggedaccounts[i]->currentip == ip)
        {
            free(loggedaccounts[i]);    //only one person can logout own account, so it is safe not to use any mutex
            loggedaccounts[i] = NULL;
            return 0;
        }
    }
    fprintf(stderr, "Player is not logged in\n");
    return LOGGED_OUT_ERROR;
}

int createAccountService(AccountData* account)
{
    if(isLoginUsed(account->login))
    {
        fprintf(stderr, "Player %s already exists\n", account->login);
        return CREATE_ACCOUNT_ERROR;
    }
    //TODO save in file data for account, login and password hash, make place for statistics
    return 0;
}

int deleteAccountService(AccountData* account)
{
    if(!isLoginUsed(account->login))
    {
        fprintf(stderr, "Player %s doesn't exists\n", account->login);
        return DELETE_ACCOUNT_ERROR;
    }
    //TODO delete account from file, delete stats
    return 0;
}

int changePasswordService(AccountData* account, char* newpasshash)
{
    if(!isLoggedIn(account->currentip))
    {
        fprintf(stderr, "Player is not logged in\n");
        return LOGGED_OUT_ERROR;
    }
    //TODO
    return 0;
}

int updateStats(AccountStatistics* stats)
{
    //TODO
    return 0;
}

bool isLoginUsed(char* login)
{
    int i;
    for(i = 0; i < MAX_ACCOUNTS_COUNT; ++i)
    {
        if(loggedaccounts[i] != NULL && !strcmp(loggedaccounts[i]->login, login))
        {
            return 1;
        }
    }
    return 0;
}

bool isLoggedIn(uint32_t ip)
{
    int i;
    for(i = 0; i < MAX_ACCOUNTS_COUNT; ++i)
    {
        if(loggedaccounts[i] != NULL && loggedaccounts[i]->currentip == ip)
        {
            return 1;
        }
    }
    return 0;
}

bool verifyLoginAndPassword(AccountData* account)
{
    //TODO check in file if the account login and hash is there, compare them, and if they are ok, return 0 on success
    return 0;
}

uint32_t loginToIp(AccountData** accountsinroom, char* login) //we have to think if it can't be made easier
{
    int i;
    for(i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        if(accountsinroom[i] != NULL && !strcmp(accountsinroom[i]->login, login))
        {
            return accountsinroom[i]->currentip;
        }
    }
    fprintf(stderr, "Failed to find ip for login %s", login);
    return 0;
}
