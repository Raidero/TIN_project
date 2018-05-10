#ifndef MENUVIEWMODEL_H
#define MENUVIEWMODEL_H
#include "LoginViewModel.h"

#define START_GAME 0
#define EXIT_GAME 1
#define LOG 2

class MenuViewModel: public ViewModel
{
public:
    MenuViewModel();
    virtual ~MenuViewModel();
    bool exitGame() { return exit; }
    virtual int checkButtonsPressed(float x, float y);
    LoginViewModel loginviewmodel;
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if(isvisible)
        {
            for(int i = 0; i < NUMBER_OF_BUTTONS_MENU; ++i)
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
    virtual void buttonPressed(int i);



    sf::RectangleShape buttons[NUMBER_OF_BUTTONS_MENU];
    sf::Text startgame;
    sf::Text exitgame;
    sf::Text createaccount;
    sf::Text log;
    sf::Text deleteaccount;
    sf::Text changepassword;
    bool isloggedin;
    bool exit;
};

#endif // MENUVIEWMODEL_H
