#ifndef ROOMVIEWMODEL_H
#define ROOMVIEWMODEL_H

#include <ViewModel.h>
#include <time.h>

class RoomViewModel : public ViewModel
{
public:
    RoomViewModel(ViewModel* mvm);
    virtual ~RoomViewModel();
    void buttonPressed(int i);
    void addLetter(char c);
    void refresh(int message);
    void setLogins(char* lgs);
private:
    ViewModel* mvm;
    sf::Text logins;
    sf::Text exit;
    sf::Text setready;
    sf::Text startgame;
    sf::Text allmessages;
    sf::Text messageinbox;
    std::string messageinboxstring;
    sf::CircleShape ready[MAX_PLAYER_COUNT];
    sf::CircleShape notready[MAX_PLAYER_COUNT];
    clock_t lastloginrefresh;
    int writeaccess;
    bool ishost;
    bool playerready;
    char isplayerready[MAX_PLAYER_COUNT];
    int communicationsocket;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if(isvisible)
        {
            for(int i = 0; i < numberofbuttons; ++i)
            {
                target.draw(buttons[i]);
            }
            target.draw(logins);
            target.draw(startgame);
            target.draw(setready);
            target.draw(exit);
            target.draw(messageinbox);
            int j = 0;
            for(int i = 0; i < MAX_PLAYER_COUNT; ++i)
            {
                if(isplayerready[i] == 0)
                {
                    target.draw(notready[j++]);
                }
                else if(isplayerready[i] == 1)
                {
                    target.draw(ready[j++]);
                }
            }

        }
    }
};

#endif // ROOMVIEWMODEL_H
