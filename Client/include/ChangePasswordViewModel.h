#ifndef CHANGEPASSWORDVIEWMODEL_H
#define CHANGEPASSWORDVIEWMODEL_H

#include "LoginViewModel.h"


class ChangePasswordViewModel: public LoginViewModel
{
public:
    ChangePasswordViewModel(ViewModel* mvm);
    virtual ~ChangePasswordViewModel();
    void buttonPressed(int i);
    void refresh(int message);
    void addLetter(char c);
private:
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
            target.draw(newpasswordview);
        }
    }
    sf::Text newpasswordview;
    char newpassword[MAX_PASSWORD_LENGTH];
    int newpasswordpos;
    unsigned char newpasswordhash[MAX_PASSHASH_LENGTH];
};

#endif // CHANGEPASSWORDVIEWMODEL_H
