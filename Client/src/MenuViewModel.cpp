#include "MenuViewModel.h"

MenuViewModel::MenuViewModel(int soc, uint32_t ip)
{
    mainsocket = soc;
    communicationsocket = -1;
    accountdata.currentip = ip;
    initButtons(6);
    exit = false;
    isloggedin = false;
    isvisible = true;
    isactive = true;

    title.setString("Maze Shooter");
    title.setCharacterSize(FONT_BIG);
    title.setFont(font);
    title.setPosition(10, 10);
    loggedinas.setCharacterSize(FONT_SMALL);
    loggedinas.setFont(font);

    createButton(startgame, buttons[0], "Start game", HEIGHT - 3*TEXT_STEP, 10);
    disableButton(startgame);
    createButton(exitgame, buttons[1], "Exit game", HEIGHT - 2*TEXT_STEP, 10);

    createButton(log, buttons[2], "Log in", HEIGHT - 5*TEXT_STEP , WIDTH - 10*TEXT_STEP);
    createButton(createaccount, buttons[3], "Create new account", HEIGHT - 4*TEXT_STEP, WIDTH - 10*TEXT_STEP);
    createButton(deleteaccount, buttons[4], "Delete account", HEIGHT - 3*TEXT_STEP, WIDTH - 10*TEXT_STEP);
    createButton(changepassword, buttons[5], "Change your password", HEIGHT - 2*TEXT_STEP, WIDTH - 10*TEXT_STEP);
    disableButton(changepassword);
}

MenuViewModel::~MenuViewModel()
{
}

void MenuViewModel::buttonPressed(int i)
{
    switch(i)
    {
        case 0: ///start game
        {
            if (isloggedin)
            {
                recv(mainsocket, buffer, BUFFER_SIZE, MSG_DONTWAIT); ///clear the buffer
                errno = ENOENT;
                buffer[0] = REQUEST_START_GAME;
                while(!send(mainsocket, buffer, 1, 0)) {}

                if (initSocket(&communicationsocket, &serveraddress, &sockettimeout) < 0)
                {
                    std::cerr << "Couldn't init communication socket\n";
                }
                serveraddress.sin_port = htons(DEFAULT_PORT + 10);
                if (connect(communicationsocket, (struct sockaddr *) &serveraddress, sizeof(serveraddress)) < 0)
                {
                    std::cerr << "Couldn't connect communication socket\n";
                }
                while(recv(mainsocket, buffer, 1, 0) >= 0)
                {
                    if(buffer[0] == FAILED_TO_START_GAME)
                    {
                        std::cerr << "Failed to start game\n";
                        break;
                    }
                    else if(buffer[0] == START_GAME_SUCCESSFUL)
                    {
                        roomviewmodel->refresh(REFRESH_LOGINS);
                        refresh(PLAYER_STARTED_GAME);
                        break;
                    }
                    else if(buffer[0] == START_GAME_SUCCESSFUL_AND_HOST)
                    {
                        roomviewmodel->refresh(REFRESH_LOGINS);
                        roomviewmodel->refresh(SET_HOST);
                        refresh(PLAYER_STARTED_GAME);
                        break;
                    }
                }
                if(errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    std::cerr << "Can't reach server\n";
                }
            }
            break;
        }
        case 1: ///exit game
        {
            send(mainsocket, NULL, 0, 0);
            exit = true;
            break;
        }
        case 2: ///login or logout
        {
            if(!isloggedin)
            {
                loginviewmodel->setActivity(true);
                loginviewmodel->setVisibility(true);
                this->setActivity(false);
            }
            else
            {
                recv(mainsocket, buffer, BUFFER_SIZE, MSG_DONTWAIT); ///clear the buffer
                errno = ENOENT;
                buffer[0] = REQUEST_LOGOUT;
                while(!send(mainsocket, buffer, 1, 0)) {}
                unsigned char* bufferptr = buffer;
                bufferptr = serializeUint_32_t(bufferptr, accountdata.currentip);
                int alldata = bufferptr - buffer;
                int sendbytes = 0;
                while(sendbytes < alldata)
                {
                    sendbytes = send(mainsocket, buffer + sendbytes, alldata - sendbytes, 0);
                }
                while(recv(mainsocket, buffer, 1, 0) >= 0)
                {
                    if(buffer[0] == FAILED_TO_LOGOUT)
                    {
                        std::cerr << "Failed to log out\n";
                        break;
                    }
                    else if(buffer[0] == LOGOUT_SUCCESSFUL)
                    {
                        refresh(PLAYER_LOGGED_OUT);
                        break;
                    }
                }
                if(errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    std::cerr << "Can't reach server\n";
                }
            }
            break;
        }
        case 3: ///create account
        {
            createaccountviewmodel->setActivity(true);
            createaccountviewmodel->setVisibility(true);
            this->setActivity(false);
            break;
        }
        case 4:
        {
            deleteaccountviewmodel->setActivity(true);
            deleteaccountviewmodel->setVisibility(true);
            this->setActivity(false);
            break;
        }
        case 5:
        {
            if(isloggedin)
            {
                changepasswordviewmodel->setActivity(true);
                changepasswordviewmodel->setVisibility(true);
                this->setActivity(false);
            }
        }
    }
}

void MenuViewModel::refresh(int message)
{
    loggedinas.setString(std::string(""));
    switch(message)
    {
        case PLAYER_LOGGED_IN:
        {
            buttonChangedName(log, "Log out", buttons[2]);
            enableButton(changepassword);
            enableButton(startgame);
            std::string s1("Logged in as:\n");
            std::string s2(accountdata.login);
            loggedinas.setString(std::string(s1 + s2));
            loggedinas.setPosition(WIDTH - loggedinas.getLocalBounds().width - 10, 10);
            isloggedin = true;
            break;
        }
        case PLAYER_LOGGED_OUT:
        {
            loggedinas.setString(std::string(""));
            buttonChangedName(log, "Log in", buttons[2]);
            for(int i = 0; i < MAX_LOGIN_LENGTH; ++i)
            {
                playeraccountdata.login[i] = 0;
            }
            playeraccountdata.currentip = 0;
            disableButton(changepassword);
            disableButton(startgame);
            isloggedin = false;
            break;
        }
        case ACCOUNT_CREATED:
        {
            loggedinas.setString(std::string("Account successfully created"));
            loggedinas.setPosition(WIDTH - loggedinas.getLocalBounds().width - 10, 10);
            break;
        }
        case ACCOUNT_DELETED:
        {
            loggedinas.setString(std::string("Account successfully deleted"));
            loggedinas.setPosition(WIDTH - loggedinas.getLocalBounds().width - 10, 10);
            break;
        }
        case PLAYER_STARTED_GAME:
        {
            roomviewmodel->setActivity(true);
            roomviewmodel->setVisibility(true);
            this->setActivity(false);
            this->setVisibility(false);
            break;
        }

    }
}


