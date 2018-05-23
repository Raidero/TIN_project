#include "RoomViewModel.h"
#define RADIUS 10

RoomViewModel::RoomViewModel(ViewModel* mvm)
{
    lastloginrefresh = 0;
    this->mvm = mvm;
    initButtons(3);
    logins.setPosition(10+MARGIN+RADIUS, 10);
    exit.setPosition(WIDTH, 10);
    for(int i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        ready[i].setRadius(RADIUS/2);
        ready[i].setPosition(10, (float)((RADIUS+MARGIN)*i));
        ready[i].setFillColor(sf::Color(255,0,0));
        ready[i].setOutlineColor(sf::Color(255,0,0));
    }
}

RoomViewModel::~RoomViewModel()
{
    //dtor
}

void RoomViewModel::refresh(int message)
{
    switch(message)
    {
        case REFRESH_LOGINS:
        {
            if(clock() - lastloginrefresh > CLOCKS_PER_SEC)
            {
                recv(mainsocket, buffer, BUFFER_SIZE, MSG_DONTWAIT); ///clear the buffer
                errno = ENOENT;
                buffer[0] = REQUEST_REFRESH_LOGINS;
                while(!send(mainsocket, buffer, 1, 0)) {}
                unsigned char* bufferptr = buffer;

                serializeAccountData(bufferptr, &playeraccountdata);
                int alldata = bufferptr - buffer;
                int sendbytes = 0;
                while(sendbytes < alldata)
                {
                    sendbytes = send(mainsocket, buffer + sendbytes, alldata - sendbytes, 0);
                }
                int readbytes = 0;
                alldata = sizeof(AccountData);
                do{
                    readbytes += recv(mainsocket, buffer+readbytes, alldata-readbytes, 0);
                }while(readbytes < alldata);

                if(errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    std::cerr << "Can't reach server\n";
                }
                lastloginrefresh = clock();
            }
        }
    }
}
