#ifndef LOGINVIEWMODEL_H
#define LOGINVIEWMODEL_H
#include "ViewModel.h"

class LoginViewModel: public ViewModel
{
public:
    LoginViewModel(ViewModel* mvm);
    virtual ~LoginViewModel();
    virtual void addLetter(char c);
    virtual void refresh(int message);
    ViewModel* menuviewmodel;
protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if(isvisible)
        {
            target.draw(background);
            for(int i = 0; i < numberofbuttons; ++i)
            {
                target.draw(buttons[i]);
            }
            target.draw(title);
            target.draw(loginbutton);
            target.draw(cancelbutton);
            target.draw(loginview);
            target.draw(passwordview);
            target.draw(message);
        }
    }
    virtual void buttonPressed(int i);

    sf::RectangleShape background;

    sf::Text message;

    sf::Text loginbutton;
    sf::Text cancelbutton;
    int writeaccess;

    sf::Text loginview;
    char login[MAX_LOGIN_LENGTH];
    int loginpos;

    sf::Text passwordview;
    char password[MAX_PASSWORD_LENGTH];
    int passwordpos;
    unsigned char passwordhash[MAX_PASSHASH_LENGTH];
};

#endif // LOGINVIEWMODEL_H
