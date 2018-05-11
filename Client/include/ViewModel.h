#ifndef VIEWMODEL_H
#define VIEWMODEL_H
extern "C"
{
    #include <stdio.h>
    #include <stdlib.h>
    #include "../Client.h"
}
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "Defines.h"
#include <openssl/sha.h>
#include <inttypes.h>

#define WIDTH 640
#define HEIGHT 480
#define FONT_BIG 25
#define FONT_MEDIUM 20
#define FONT_SMALL 15
#define TEXT_STEP 27
#define MARGIN 4
#define NUMBER_OF_BUTTONS_LOGIN 4
#define NUMBER_OF_BUTTONS_MENU 6
#define NO_BUTTON_IS_PRESSED -1

typedef struct
{
    char login[MAX_LOGIN_LENGTH];
    unsigned char passwordhash[MAX_PASSHASH_LENGTH];
    uint32_t currentip;                     //changed so it now reflects ip from in_addr struct
    int votercounter;
}AccountData;

extern AccountData accountdata;
extern int mainsocket;
extern char buffer[BUFFER_SIZE];

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
