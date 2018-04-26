#include "MenuViewModel.h"

MenuViewModel::MenuViewModel()
{
    isloggedin = false;
    isvisible = true;
    font.loadFromFile("font");
    title.setString("Maze Shooter");
    title.setCharacterSize(FONT_BIG);
    title.setFont(font);
    title.setPosition(10, 10);

    createMenuButton(startgame, buttons[0], "Start game", HEIGHT - 3*TEXT_STEP, 10);
    disableButton(startgame);
    createMenuButton(exitgame, buttons[1], "Exit game", HEIGHT - 2*TEXT_STEP, 10);

    createMenuButton(log, buttons[2], "Log in", HEIGHT - 5*TEXT_STEP , WIDTH - 10*TEXT_STEP);
    createMenuButton(createaccount, buttons[3], "Create new account", HEIGHT - 4*TEXT_STEP, WIDTH - 10*TEXT_STEP);
    createMenuButton(deleteaccount, buttons[4], "Delete account", HEIGHT - 3*TEXT_STEP, WIDTH - 10*TEXT_STEP);
    createMenuButton(changepassword, buttons[5], "Change your password", HEIGHT - 2*TEXT_STEP, WIDTH - 10*TEXT_STEP);
    disableButton(changepassword);
}

MenuViewModel::~MenuViewModel()
{
    //dtor
}

void MenuViewModel::createMenuButton(sf::Text& name, sf::RectangleShape& buttonrect, std::string text, float height, float width)
{
    name.setString(text);
    name.setCharacterSize(FONT_MEDIUM);
    name.setFont(font);
    name.setPosition(width, height);

    buttonrect.setSize(sf::Vector2f(name.getGlobalBounds().width, name.getGlobalBounds().height));
    buttonrect.setPosition(name.getGlobalBounds().left, name.getGlobalBounds().top);
    buttonrect.setOutlineColor(sf::Color(255,255,255));
    buttonrect.setFillColor(sf::Color(255,255,255, 0));
    buttonrect.setOutlineThickness(1);
}

void MenuViewModel::disableButton(sf::Text& name)
{
    name.setColor(sf::Color(100,100,100));
}

void MenuViewModel::buttonChangedName(sf::Text& name, std::string newtext)
{
    name.setString(newtext);
}

int MenuViewModel::checkButtonsPressed(float x, float y)
{
    for(int i = 0; i < NUMBER_OF_BUTTONS; ++i)
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
}


