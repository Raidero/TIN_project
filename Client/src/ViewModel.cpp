#include "ViewModel.h"

int mainsocket;
int communicationsocket;
AccountData accountdata;
AccountData playeraccountdata;

unsigned char buffer[BUFFER_SIZE];

ViewModel::ViewModel()
{
    font.loadFromFile("font");
    isvisible = false;
    isactive = false;
}

ViewModel::~ViewModel()
{
    disposeButtons();
}

void ViewModel::createButton(sf::Text& name, sf::RectangleShape& buttonrect, std::string text, float height, float width, int fontsize)
{
    name.setString(text);
    name.setCharacterSize(fontsize);
    name.setFont(font);
    name.setPosition(width, height);

    buttonrect.setSize(sf::Vector2f(name.getGlobalBounds().width + MARGIN, name.getGlobalBounds().height + MARGIN));
    buttonrect.setPosition(name.getGlobalBounds().left - MARGIN/2, name.getGlobalBounds().top - MARGIN/2);
    buttonrect.setOutlineColor(sf::Color(255,255,255));
    buttonrect.setFillColor(sf::Color(255,255,255, 0));
    buttonrect.setOutlineThickness(1);
}

void ViewModel::disableButton(sf::Text& name)
{
    name.setColor(sf::Color(100,100,100));
}

void ViewModel::enableButton(sf::Text& name)
{
    name.setColor(sf::Color(255,255,255));
}

void ViewModel::buttonChangedName(sf::Text& name, std::string newtext, sf::RectangleShape& buttonrect)
{
    name.setString(newtext);
    buttonrect.setSize(sf::Vector2f(name.getGlobalBounds().width + MARGIN, name.getGlobalBounds().height + MARGIN));
}

void ViewModel::setVisibility(bool isvis)
{
    isvisible = isvis;
}

void ViewModel::setActivity(bool isact)
{
    isactive = isact;
}

void ViewModel::initButtons(int n)
{
    numberofbuttons = n;
    buttons = new sf::RectangleShape[numberofbuttons];
}

void ViewModel::disposeButtons()
{
    delete[] buttons;
}

int ViewModel::checkButtonsPressed(float x, float y)
{
    for(int i = 0; i < numberofbuttons; ++i)
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
