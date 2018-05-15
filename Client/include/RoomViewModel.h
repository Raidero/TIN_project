#ifndef ROOMVIEWMODEL_H
#define ROOMVIEWMODEL_H

#include <ViewModel.h>


class RoomViewModel : public ViewModel
{
public:
    RoomViewModel(ViewModel* mvm);
    virtual ~RoomViewModel();
    void buttonPressed(int i) {}
    void addLetter(char c) {}
    void refresh(int i) {}

private:
    ViewModel* mvm;
    sf::Text logins;
    sf::Text exit;
    sf::CircleShape ready[MAX_PLAYER_COUNT];

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if(isvisible)
        {
            for(int i = 0; i < numberofbuttons; ++i)
            {
                target.draw(buttons[i]);
            }
            target.draw(logins);
            target.draw(exit);
            for(int i = 0; i < MAX_PLAYER_COUNT; ++i)
            {
                target.draw(ready[i]);
            }

        }
    }
};

#endif // ROOMVIEWMODEL_H
