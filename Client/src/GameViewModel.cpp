#include "GameViewModel.h"

GameViewModel::GameViewModel(ViewModel* rvm)
{
    this->rvm = rvm;
    rb = 0;
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

        case REFRESH_MAP:
        {
			errno = ENOENT;
			rb += recv(multicastsocket, buf+rb, 16, MSG_DONTWAIT);
			if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                //std::cerr << "It's k\n";
                rb = 0;
            }
			if (rb == 16)
			{
				int temp;
				//std::cout << playercharacter[temp].getPosition().x;
				//std::cout << playercharacter[temp].getPosition().y << '\n';
				rb = 0;
				deserializeInt(buf+12, &temp);
				float _x = playercharacter[temp].getPosition().x;
				float _y = playercharacter[temp].getPosition().y;

                switch(buf[0])
                {

                case 'a':
                {
                    playercharacter[temp].setPosition(_x-3.0f, _y);
                    break;
                }
                case 'd':
                {
                    playercharacter[temp].setPosition(_x+3.0f, _y);
                    break;
                }
                case 'w':
                {
                    playercharacter[temp].setPosition(_x, _y-3.0f);
                    break;
                }
                case 's':
                {
                    playercharacter[temp].setPosition(_x, _y+3.0f);
                    break;
                }
                }
			}

        }
    }
}


void GameViewModel::addLetter(char c)
{
    unsigned char* ptr = buffer;
	errno = ENOENT;
    buffer[0] = REQUEST_SEND_MULTICAST_DATA;
    while(!send(mainsocket, buffer, 1, 0)) {}

    serializeInt(ptr+12, playeringameid);
    *ptr = c;

    int alldata, readbytes = 0;
    alldata = 16;
    do
    {
        int check = send(mainsocket, buffer+readbytes, alldata-readbytes, 0);
        if(check < 0)
            break;
        readbytes += check;
    }
    while(readbytes < alldata);

    if(errno == EAGAIN || errno == EWOULDBLOCK)
    {
        std::cerr << "Can't reach server\n";
    }

}





