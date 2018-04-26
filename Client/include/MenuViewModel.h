#ifndef MENUVIEWMODEL_H
#define MENUVIEWMODEL_H
#include "LoginViewModel.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#define WIDTH 640
#define HEIGHT 480
#define FONT_BIG 25
#define FONT_MEDIUM 20
#define TEXT_STEP 25
#define NUMBER_OF_BUTTONS 6
#define NO_BUTTON_IS_PRESSED -1
class MenuViewModel: public sf::Drawable
{
public:
    MenuViewModel();
    virtual ~MenuViewModel();
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if(isvisible)
        {
            for(int i = 0; i < NUMBER_OF_BUTTONS; ++i)
            {
                target.draw(buttons[i]);
            }
            target.draw(title);
            target.draw(startgame);
            target.draw(exitgame);
            target.draw(createaccount);
            target.draw(log);
            target.draw(deleteaccount);
            target.draw(changepassword);
        }
    }
    void createMenuButton(sf::Text& name, sf::RectangleShape& buttonrect, std::string text, float height, float width);
    void disableButton(sf::Text& name);
    void buttonChangedName(sf::Text& name, std::string newtext);
    int checkButtonsPressed(float x, float y);
    void buttonPressed(int i);
    sf::Font font;
    sf::Text title;

    sf::RectangleShape buttons[NUMBER_OF_BUTTONS];
    sf::Text startgame;
    sf::Text exitgame;
    sf::Text createaccount;
    sf::Text log;
    sf::Text deleteaccount;
    sf::Text changepassword;
    LoginViewModel loginviewmodel;
    bool isloggedin;
    bool isvisible;
};

#endif // MENUVIEWMODEL_H
