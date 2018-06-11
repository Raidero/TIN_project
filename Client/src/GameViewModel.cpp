#include "GameViewModel.h"

GameViewModel::GameViewModel(ViewModel* rvm)
{
    this->rvm = rvm;
    initButtons(0);
}

GameViewModel::~GameViewModel()
{
    disposeButtons();
}

void GameViewModel::refresh(int message)
{
    switch(message)
    {
        case GET_READY:
        {
            this->setVisibility(true);
            this->setActivity(true);
            rvm->setActivity(false);
            rvm->setVisibility(false);

            recv(mainsocket, buffer, BUFFER_SIZE, MSG_DONTWAIT); ///clear the buffer
            errno = ENOENT;
            buffer[0] = REQUEST_STARTING_INFO;
            while(!send(mainsocket, buffer, 1, 0)) {}

            int alldata, readbytes = 0;
            alldata = numberofplayersingame*sizeof(PlayerData) + sizeof(int);
            do{
                int check = recv(mainsocket, buffer+readbytes, alldata-readbytes, 0);
                if(check < 0)
                    break;
                readbytes += check;
            }while(readbytes < alldata);
            unsigned char* ptr = buffer;
            for(int i = 0; i < numberofplayersingame; ++i)
            {
                playercharacter[i].setRadius(10);
                playercharacter[i].setFillColor(sf::Color::Green);
                ptr = deserializePlayerData(ptr, &playersinfo[i]);
                playercharacter[i].setPosition(playersinfo[i].x, playersinfo[i].y);
            }
            deserializeInt(ptr, &playeringameid);

            if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                std::cerr << "Can't reach server\n";
            }
        }
    }
}
