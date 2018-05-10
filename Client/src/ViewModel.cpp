#include "ViewModel.h"

ViewModel::ViewModel()
{
    font.loadFromFile("font");
    isvisible = false;
    isactive = false;
}

ViewModel::~ViewModel()
{
    //dtor
}

void ViewModel::createButton(sf::Text& name, sf::RectangleShape& buttonrect, std::string text, float height, float width, int fontsize)
{
    name.setString(text);
    name.setCharacterSize(fontsize);
    name.setFont(font);
    name.setPosition(width, height);

    buttonrect.setSize(sf::Vector2f(name.getGlobalBounds().width, name.getGlobalBounds().height));
    buttonrect.setPosition(name.getGlobalBounds().left, name.getGlobalBounds().top);
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

void ViewModel::buttonChangedName(sf::Text& name, std::string newtext)
{
    name.setString(newtext);
}

void ViewModel::setVisibility(bool isvis)
{
    isvisible = isvis;
}

void ViewModel::setActivity(bool isact)
{
    isactive = isact;
}
