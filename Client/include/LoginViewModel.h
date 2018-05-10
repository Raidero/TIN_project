#ifndef LOGINVIEWMODEL_H
#define LOGINVIEWMODEL_H
#include <ViewModel.h>

class LoginViewModel: public ViewModel
{
public:
    LoginViewModel();
    virtual ~LoginViewModel();
    virtual int checkButtonsPressed(float x, float y) { return NO_BUTTON_IS_PRESSED; }
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if(isvisible)
        {
            for(int i = 0; i < NUMBER_OF_BUTTONS_LOGIN; ++i)
            {
                target.draw(buttons[i]);
            }
            target.draw(background);
            target.draw(title);
        }
    }
    virtual void buttonPressed(int i) {}

    sf::RectangleShape background;
    sf::RectangleShape buttons[NUMBER_OF_BUTTONS_LOGIN];
    sf::Text loginbutton;
    sf::String loginview;
    char login[];
    sf::String passwordview;
    std::string passwordstring;
    sf::Text cancelbutton;

};

#endif // LOGINVIEWMODEL_H
