#include "AccountService.h"
#include <stdio.h>
#include <string.h>

AccountData* loggedaccounts[MAX_ACCOUNTS_COUNT];
FILE* datafile;

void initAccountService()
{
    int i;
    for(i = 0; i < MAX_ACCOUNTS_COUNT; ++i)
    {
        loggedaccounts[i] = NULL;
    }
    if(initDataFile() < 0)
    {
        fprintf(stderr, "Account Service initialization failed\n");
    }
}

AccountData* initAccoundData(char* login, char* passwordhash, uint32_t currentip, int votercounter)
{
    int i = 0;
    AccountData* acc = (AccountData*)malloc(sizeof(AccountData));
    while(login[i] != '\0' && i < MAX_LOGIN_LENGTH-1)
    {
        acc->login[i] = login[i];
        ++i;
    }
	acc->login[i] = '\0';
    i = 0;
    while(passwordhash[i] != '\0' && i < MAX_PASSHASH_LENGTH-1)
    {
        acc->passwordhash[i] = passwordhash[i];
        ++i;
    }
    acc->passwordhash[i] = '\0';
    acc->currentip = currentip;
    acc->votercounter = votercounter;
    return acc;
}

void disposeAccountData(int i)
{
    if(i >= 0 && i < MAX_ACCOUNTS_COUNT && loggedaccounts[i] != NULL)
    {
        free(loggedaccounts[i]);
        loggedaccounts[i] = NULL;
    }

}

void disposeAllAccounts()
{
	int i;
	for(i = 0; i < MAX_ACCOUNTS_COUNT; ++i)
    {
        if(loggedaccounts[i] != NULL)
        {
            free(loggedaccounts[i]);
        }
    }
}

int logInService(AccountData* account, int playerid)
{
    if(isLoggedIn(account->currentip))
    {
        fprintf(stderr, "Player %s already logged in\n", account->login);
        return LOGGED_IN_ERROR;
    }
    if(!verifyLoginAndPassword(account))
    {
        fprintf(stderr, "Wrong login or password for login: %s\n", account->login);
        return WRONG_LOGIN_OR_PASSWORD;
    }
    if(loggedaccounts[playerid] == NULL)
    {
        loggedaccounts[playerid] = account;
        printf("Player %s logged in", account->login);
        return playerid;
    }
    fprintf(stderr, "Place is already taken\n");
    return ACCOUNT_PLACE_TAKEN_ERROR;
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
    if(datafile == NULL)
    {
        fprintf(stderr, "File is not open\n");
        return FILE_NOT_OPEN;
    }
    if(isLoggedIn(account->currentip))
    {
        fprintf(stderr, "Player %s logged in, cannot delete logged in account\n", account->login);
        return LOGGED_IN_ERROR;
    }
    if(!isLoginUsed(account->login))
    {
        fprintf(stderr, "Player %s doesn't exist\n", account->login);
        return DELETE_ACCOUNT_ERROR;
    }
    if(!verifyLoginAndPassword(account))
    {
        fprintf(stderr, "Wrong login or password for login: %s\n", account->login);
        return WRONG_LOGIN_OR_PASSWORD;
    } //look at the comment in function below (changePAsswordService)

    int where = ftell(datafile);
    fseek(datafile, -sizeof(*account), SEEK_CUR);
    where = ftell(datafile);
    bzero(account, sizeof(*account));
    if(fwrite(account, sizeof(*account), 1, datafile) != 1)
    {
		fprintf(stderr, "fwrite fail\n");
		return 0;
    }
    if(ferror(datafile))
    {
        fprintf(stderr, "Cannot write to file\n");
        return FILE_WRITE_ERROR;
    }
    printf("Deleted account");
    return 0;
}

int changePasswordService(AccountData* account, char* newpasshash)
{
    int i = 0;
	if(datafile == NULL)
    {
        fprintf(stderr, "File is not open\n");
        return FILE_NOT_OPEN;
    }
    if(!isLoggedIn(account->currentip))
    {
        fprintf(stderr, "Player is not logged in\n");
        return LOGGED_OUT_ERROR;
    }
    if(!verifyLoginAndPassword(account))
    {
        fprintf(stderr, "Wrong login or password for login: %s\n", account->login);
        return WRONG_LOGIN_OR_PASSWORD;
    } //now file cursor is set right after the struct Account data for which we look for
    //because verifyLoginAndPassword set it that way, so now we have to back -sizeof(AccountData)
    while(newpasshash[i] != '\0')
    {
        account->passwordhash[i] = newpasshash[i];
        ++i;
        if(i >= MAX_PASSHASH_LENGTH)
        {
            account->login[MAX_PASSHASH_LENGTH - 1] = '\0';
        }
    }
    fseek(datafile, -sizeof(*account), SEEK_CUR);
    fwrite(account, sizeof(*account), 1, datafile);
    if(ferror(datafile))
    {
        fprintf(stderr, "Cannot write to file\n");
        return FILE_WRITE_ERROR;
    }
    return 0;
}

int updateStats(AccountStatistics* stats)
{
    //TODO
    return 0;
}

bool isLoginUsed(char* login)
{
    if(datafile == NULL)
    {
        fprintf(stderr, "File is not open\n");
        return 0;
    }
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
    if(datafile == NULL)
    {
        fprintf(stderr, "File is not open\n");
        return 0;
    }
    AccountData tocompare;
    fseek(datafile, 0L, SEEK_SET);
    while(!feof(datafile))
    {
        if(fread(&tocompare, sizeof(tocompare), 1, datafile) < 1)
        {
			fprintf(stderr, "fread fail\n");
			return 0;
        }
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
    return IP_NOT_FOUND;
}

int loginToPlayerId(char* login)
{
    int i;
    for(i = 0; i < MAX_ACCOUNTS_COUNT; ++i)
    {
        if(loggedaccounts[i] != NULL && !strcmp(loggedaccounts[i]->login, login))
        {
            return i;
        }
    }
    fprintf(stderr, "Failed to find ID for login %s", login);
    return PLAYER_ID_NOT_FOUND;
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

int deleteDataFile()
{
    if(datafile == NULL)
    {
        datafile = fopen(DATA_FILE_NAME, "wr+b");
        fclose(datafile);
        datafile = NULL;
    }
    else
    {
        fclose(datafile);
        datafile = fopen(DATA_FILE_NAME, "wr+b");
        fclose(datafile);
        datafile = NULL;
    }
    return 0;
}
