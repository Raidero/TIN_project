#ifndef GAMEVIEWMODEL_H
#define GAMEVIEWMODEL_H

#include <ViewModel.h>


class GameViewModel : public ViewModel
{
    public:
        GameViewModel(ViewModel* rvm);
        virtual ~GameViewModel();
        void refresh(int message);
        void addLetter(char c) {}
        void buttonPressed(int i) {}
    protected:

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            if(isvisible)
            {
                for(int i = 0; i < numberofplayersingame; ++i)
                {
                    target.draw(playercharacter[i]);
                }
            }
        }
        ViewModel* rvm;
        PlayerData playersinfo[MAX_PLAYER_COUNT];
        sf::CircleShape playercharacter[MAX_PLAYER_COUNT];
        int playeringameid;
};

#endif // GAMEVIEWMODEL_H
