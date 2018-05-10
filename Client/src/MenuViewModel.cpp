#include "MenuViewModel.h"

MenuViewModel::MenuViewModel()
{
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
    //dtor
}







int MenuViewModel::checkButtonsPressed(float x, float y)
{
    for(int i = 0; i < NUMBER_OF_BUTTONS_MENU; ++i)
    {

        if(buttons[i].getPosition().x < x &&
            buttons[i].getPosition().y < y &&
            buttons[i].getSize().x + buttons[i].getPosition().x > x &&
            buttons[i].getSize().y + buttons[i].getPosition().y > y)
        {
            buttonPressed(i);
        }

    }
    return NO_BUTTON_IS_PRESSED;
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
            loginviewmodel.setActivity(true);
            loginviewmodel.setVisibility(true);
            this->setActivity(false);
            break;


    }
}


