#ifndef MENUVIEWMODEL_H
#define MENUVIEWMODEL_H
#include "ViewModel.h"

class MenuViewModel: public ViewModel
{
public:
    MenuViewModel(int soc, uint32_t ip);
    virtual ~MenuViewModel();
    bool exitGame() { return exit; }
    void linkLoginViewModel(ViewModel* lvm) { loginviewmodel = lvm; }
    void linkCreateAccountViewModel(ViewModel* cavm) { createaccountviewmodel = cavm; }
    void linkDeleteAccountViewModel(ViewModel* davm) { deleteaccountviewmodel = davm; }
    void linkChangePasswordViewModel(ViewModel* chpvm) { changepasswordviewmodel = chpvm; }
    void linkRoomViewModel(ViewModel* rvm) { roomviewmodel = rvm; }
    virtual void addLetter(char c) {}
    virtual void refresh(int message);
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if(isvisible)
        {
            for(int i = 0; i < numberofbuttons; ++i)
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
            target.draw(loggedinas);
        }
    }
    virtual void buttonPressed(int i);
    sf::Text loggedinas;
    sf::Text startgame;
    sf::Text exitgame;
    sf::Text createaccount;
    sf::Text log;
    sf::Text deleteaccount;
    sf::Text changepassword;
    bool isloggedin;
    bool exit;
    int numberofplayers;
    ViewModel* loginviewmodel;
    ViewModel* createaccountviewmodel;
    ViewModel* deleteaccountviewmodel;
    ViewModel* changepasswordviewmodel;
    ViewModel* roomviewmodel;
};

#endif // MENUVIEWMODEL_H
