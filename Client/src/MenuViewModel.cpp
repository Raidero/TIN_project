#include "MenuViewModel.h"

MenuViewModel::MenuViewModel(int soc)
{
    mainsocket = soc;
    initButtons(6);
    exit = false;
    isloggedin = false;
    isvisible = true;
    isactive = true;

    title.setString("Maze Shooter");
    title.setCharacterSize(FONT_BIG);
    title.setFont(font);
    title.setPosition(10, 10);

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
    disposeButtons();
}

void MenuViewModel::buttonPressed(int i)
{
    switch(i)
    {
        case START_GAME:
            break;
        case EXIT_GAME:
            exit = true;
            break;
        case LOG:
            if(!isloggedin)
            {
                loginviewmodel->setActivity(true);
                loginviewmodel->setVisibility(true);
                this->setActivity(false);
            }
            else
            {
                int n = sizeof(uint32_t);
                buffer[0] = REQUEST_LOGOUT;
                while(!send(mainsocket, buffer, 1, 0)) {}
                //buffer =
                while(n > 0)
                {
                    n -= send(mainsocket, buffer, sizeof(uint32_t), 0);
                }

                refresh(PLAYER_LOGGED_OUT);
            }
            break;


    }
}

void MenuViewModel::refresh(int message)
{
    switch(message)
    {
        case PLAYER_LOGGED_IN:
            buttonChangedName(log, "Log out", buttons[2]);
            enableButton(changepassword);
            enableButton(startgame);
            isloggedin = true;
            break;
        case PLAYER_LOGGED_OUT:
            buttonChangedName(log, "Log in", buttons[2]);
            disableButton(changepassword);
            disableButton(startgame);
            isloggedin = false;
            break;
    }
}


