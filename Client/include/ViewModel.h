#ifndef VIEWMODEL_H
#define VIEWMODEL_H
extern "C"
{
    #include <stdio.h>
    #include <stdlib.h>
    #include "../Client.h"
    #include "Serialization.h"
    #include "Defines.h"
}
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <openssl/sha.h>

extern AccountData accountdata;
extern AccountData playeraccountdata;
extern int mainsocket;
extern int communicationsocket;
extern unsigned char buffer[BUFFER_SIZE];
extern int multicastsocket;
extern int numberofplayersingame;
class ViewModel: public sf::Drawable
{
public:
    ViewModel();
    virtual ~ViewModel();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
    virtual void refresh(int message) = 0;
    int checkButtonsPressed(float x, float y);
    void setVisibility(bool isvis);
    void setActivity(bool isact);
    bool isVisible() { return isvisible; }
    bool isActive() { return isactive; }
    virtual void addLetter(char c) = 0;
protected:
    void createButton(sf::Text& name, sf::RectangleShape& buttonrect, std::string text, float height, float width, int fontsize = FONT_MEDIUM);
    void disableButton(sf::Text& name);
    void enableButton(sf::Text& name);
    void buttonChangedName(sf::Text& name, std::string newtext, sf::RectangleShape& buttonrect);
    virtual void buttonPressed(int i) = 0;
    void initButtons(int n);
    void disposeButtons();
    sf::Font font;
    sf::Text title;
    bool isvisible;
    bool isactive;
    int numberofbuttons;
    sf::RectangleShape* buttons;
};

#endif // VIEWMODEL_H
