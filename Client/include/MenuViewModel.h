#ifndef MENUVIEWMODEL_H
#define MENUVIEWMODEL_H
#include "ViewModel.h"
#define START_GAME 0
#define EXIT_GAME 1
#define LOG 2

class MenuViewModel: public ViewModel
{
public:
    MenuViewModel(int soc);
    virtual ~MenuViewModel();
    bool exitGame() { return exit; }
    void linkLoginViewModel(ViewModel* lvm) { loginviewmodel = lvm; }
    virtual void addLetter(char c) {}
    virtual void refresh(int message);
    ViewModel* loginviewmodel;
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
