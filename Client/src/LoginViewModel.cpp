#include "LoginViewModel.h"

LoginViewModel::LoginViewModel()
{
    background.setFillColor(sf::Color(50,50,50));
    background.setOutlineColor(sf::Color(50,0,0));
    background.setOutlineThickness(3);
    background.setSize(sf::Vector2f(300, 150));
    background.setOrigin(sf::Vector2f(150, 75));
    background.setPosition(sf::Vector2f(WIDTH/2, HEIGHT/2));
    title.setString("Sign in:");
    title.setCharacterSize(FONT_MEDIUM);
    title.setFont(font);
    title.setPosition(background.getPosition().x - 150, background.getPosition().y - 75);


}

LoginViewModel::~LoginViewModel()
{
    //dtor
}

