#include "AccountService.h"
#include <stdio.h>
#include <string.h>

AccountData* loggedaccounts[MAX_ACCOUNTS_COUNT];
FILE* datafile = NULL;
int logInService(AccountData* account)
{
    int i;
    if(isLoggedIn(account->currentip))
    {
        fprintf(stderr, "Player %s already logged in\n", account->login);
        return LOGGED_IN_ERROR;
    }
    if(verifyLoginAndPassword(account))
    {
        fprintf(stderr, "Wrong login or password for login: %s\n", account->login);
        return WRONG_LOGIN_OR_PASSWORD;
    }
    for(i = 0; i < MAX_ACCOUNTS_COUNT; ++i)
    {
        if(loggedaccounts[i] == NULL)
        {
            loggedaccounts[i] = account;
            printf("Player %s logged in", account->login);
            return 0;
        }
    }
    fprintf(stderr, "Maximum server capacity reached\n");
    return MAX_ACCOUNTS_LOGGED_IN_ERROR;
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
            printf("Player logged out");
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
    if(datafile == NULL)
    {
        fprintf(stderr, "File is not open\n");
        return FILE_NOT_OPEN;
    }
    fseek(datafile, 0L, SEEK_END);
    fwrite(account, sizeof(*account), 1, datafile);
    if(ferror(datafile))
    {
        fprintf(stderr, "Cannot write to file\n");
        return FILE_WRITE_ERROR;
    }
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
    if(verifyLoginAndPassword(account))
    {
        fprintf(stderr, "Wrong login or password for login: %s\n", account->login);
        return WRONG_LOGIN_OR_PASSWORD;
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
    AccountData tocompare;
    fseek(datafile, 0L, SEEK_SET);
    while(!feof(datafile))
    {
        fread(&tocompare, sizeof(tocompare), 1, datafile);
        if(ferror(datafile))
        {
            fprintf(stderr, "Cannot read from file\n");
            return 0;
        }
        if(!strcmp(tocompare.login, login))
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
    AccountData tocompare;
    fseek(datafile, 0L, SEEK_SET);
    while(!feof(datafile))
    {
        fread(&tocompare, sizeof(tocompare), 1, datafile);
        if(ferror(datafile))
        {
            fprintf(stderr, "Cannot read from file\n");
            return 0;
        }
        if(!strcmp(tocompare.login, account->login)
        && !strcmp(tocompare.passwordhash, account->passwordhash))
        {
            return 1;
        }
    }
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

int initDataFile()
{
    datafile = fopen(DATA_FILE_NAME, DATA_FILE_MODE);
    if(datafile == NULL)
    {
        fprintf(stderr, "Can't open data file\n");
        return OPEN_FILE_ERROR;
    }
    return 0;
}

int closeDataFile()
{
    if(datafile == NULL)
    {
        fprintf(stderr, "File is not open\n");
        return FILE_NOT_OPEN;
    }
    fclose(datafile);
    return 0;
}
