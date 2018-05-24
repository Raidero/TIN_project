#include "RoomViewModel.h"
#define RADIUS 2

RoomViewModel::RoomViewModel(ViewModel* mvm)
{
    numberofplayers = 0;
    lastloginrefresh = 0;
    this->mvm = mvm;
    initButtons(3);
    logins.setCharacterSize(FONT_SMALL);
    logins.setPosition(10+MARGIN+RADIUS, 10);
    logins.setFont(font);

    exit.setPosition(WIDTH, 10);
    for(int i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        ready[i].setRadius(static_cast<float>(RADIUS));
        ready[i].setPosition(10, 10 + (float)((RADIUS+MARGIN)*i));
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
            if(clock() - lastloginrefresh > CLOCKS_PER_SEC/10)
            {
                recv(mainsocket, buffer, BUFFER_SIZE, MSG_DONTWAIT); ///clear the buffer
                errno = ENOENT;
                buffer[0] = REQUEST_REFRESH_LOGINS;
                while(!send(mainsocket, buffer, 1, 0)) {}

                int returnmessage = recv(mainsocket, buffer, 1, 0);
                if(returnmessage > 0 && buffer[0] == REFRESH_LOGINS_SUCCESSFUL)
                {
                    int alldata, readbytes = 0;
                    alldata = MAX_LOGIN_LENGTH*MAX_PLAYER_COUNT*sizeof(char);
                    do{
                        int check = recv(mainsocket, buffer+readbytes, alldata-readbytes, 0);
                        if(check < 0)
                            break;
                        readbytes += check;
                    }while(readbytes < alldata);
                    char* lgs = new char[MAX_LOGIN_LENGTH*MAX_PLAYER_COUNT];
                    deserializeCharArray(buffer, lgs, MAX_LOGIN_LENGTH*MAX_PLAYER_COUNT);
                    setLogins(lgs);
                    delete[] lgs;
                }
                else if(returnmessage > 0 && buffer[0] == FAILED_TO_REFRESH_LOGINS)
                {
                    ///TODO
                }
                else if(errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    std::cerr << "Can't reach server\n";
                }
                lastloginrefresh = clock();
            }
        }
    }
}

void RoomViewModel::setLogins(char* lgs)
{
    std::string alllogins;
    numberofplayers = 0;
    for(int i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        if(lgs[i*MAX_LOGIN_LENGTH] != '\0')
        {
            std::string login((lgs+i*MAX_LOGIN_LENGTH));
            login.append("\n");
            alllogins += login;
            ++numberofplayers;
        }
    }
    alllogins.append("\0");
    logins.setString(alllogins);
}
