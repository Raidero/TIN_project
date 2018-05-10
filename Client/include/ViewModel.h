#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "Defines.h"
#define WIDTH 640
#define HEIGHT 480
#define FONT_BIG 25
#define FONT_MEDIUM 20
#define FONT_SMALL 15
#define TEXT_STEP 25

#define NUMBER_OF_BUTTONS_LOGIN 2
#define NUMBER_OF_BUTTONS_MENU 6
#define NO_BUTTON_IS_PRESSED -1


class ViewModel: public sf::Drawable
{
public:
    ViewModel();
    virtual ~ViewModel();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
    virtual int checkButtonsPressed(float x, float y) = 0;
    void setVisibility(bool isvis);
    void setActivity(bool isact);
    bool isVisible() { return isvisible; }
    bool isActive() { return isactive; }
protected:
    void createButton(sf::Text& name, sf::RectangleShape& buttonrect, std::string text, float height, float width, int fontsize = FONT_MEDIUM);
    void disableButton(sf::Text& name);
    void enableButton(sf::Text& name);
    void buttonChangedName(sf::Text& name, std::string newtext);
    virtual void buttonPressed(int i) = 0;
    sf::Font font;
    sf::Text title;
    bool isvisible;
    bool isactive;
};

#endif // VIEWMODEL_H
