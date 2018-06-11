#include "RoomViewModel.h"
#define RADIUS 3

RoomViewModel::RoomViewModel(ViewModel* mvm)
{
    for(int i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        isplayerready[i] = 2;
    }
    msgreadbytes = 0;
    ishost = false;
    playerready = false;
    lastloginrefresh = 0;
    writeaccess = -1;
    messageinboxstring = "";
    allmessagesstring = "";
    this->mvm = mvm;
    initButtons(4);
    logins.setCharacterSize(FONT_SMALL);
    logins.setPosition(10+MARGIN+RADIUS, 10);
    logins.setFont(font);
    createButton(startgame, buttons[0], "Start Game", 10, WIDTH - 150, FONT_SMALL);
    disableButton(startgame);
    createButton(setready, buttons[1], "Set ready", 10 + TEXT_STEP, startgame.getPosition().x, FONT_SMALL);
    createButton(exit, buttons[2], "Exit room", 10 + 2*TEXT_STEP, startgame.getPosition().x, FONT_SMALL);
    buttons[3].setSize(sf::Vector2f(WIDTH - 2*MARGIN, FONT_SMALL + MARGIN));
    buttons[3].setPosition(sf::Vector2f(MARGIN, HEIGHT - 10 - FONT_SMALL - MARGIN));
    messageinbox.setPosition(sf::Vector2f(buttons[3].getPosition().x + MARGIN/2, buttons[3].getPosition().y + MARGIN/2));
    messageinbox.setFont(font);
    messageinbox.setCharacterSize(FONT_SMALL);
    messageinbox.setColor(sf::Color(0,0,0));
    allmessages.setPosition(10, HEIGHT/2.0);
    allmessages.setFont(font);
    allmessages.setCharacterSize(FONT_SMALL);
    allmessages.setColor(sf::Color(255,255,255));
    for(int i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        ready[i].setOrigin(RADIUS,RADIUS);
        ready[i].setRadius(RADIUS);
        ready[i].setPosition(10, 22.5 + FONT_SMALL*i);
        ready[i].setFillColor(sf::Color(0,255,0));
        ready[i].setOutlineColor(sf::Color(0,255,0));
        notready[i].setOrigin(RADIUS,RADIUS);
        notready[i].setRadius(RADIUS);
        notready[i].setPosition(10, 22.5 + FONT_SMALL*i);
        notready[i].setFillColor(sf::Color(255,0,0));
        notready[i].setOutlineColor(sf::Color(255,0,0));
    }
}

RoomViewModel::~RoomViewModel()
{

}

void RoomViewModel::refresh(int message)
{
    switch(message)
    {
        case REFRESH_LOGINS:
        {
            if(clock() - lastloginrefresh > (CLOCKS_PER_SEC/10))
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
                this->refresh(REFRESH_PLAYER_READYNESS);
                lastloginrefresh = clock();
            }
            break;
        }
        case REFRESH_PLAYER_READYNESS:
        {
            recv(mainsocket, buffer, BUFFER_SIZE, MSG_DONTWAIT); ///clear the buffer
            errno = ENOENT;
            buffer[0] = REQUEST_REFRESH_READYNESS;
            while(!send(mainsocket, buffer, 1, 0)) {}

            int returnmessage = recv(mainsocket, buffer, 1, 0);
            if(returnmessage > 0 && buffer[0] == REFRESH_READYNESS_SUCCESSFUL)
            {
                int alldata, readbytes = 0;
                alldata = MAX_PLAYER_COUNT*sizeof(char);
                do{
                    int check = recv(mainsocket, buffer+readbytes, alldata-readbytes, 0);
                    if(check < 0)
                        break;
                    readbytes += check;
                }while(readbytes < alldata);
                deserializeCharArray(buffer, isplayerready, MAX_PLAYER_COUNT);
            }
            else if(returnmessage > 0 && buffer[0] == FAILED_TO_REFRESH_LOGINS)
            {
                ///TODO
            }
            else if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                std::cerr << "Can't reach server\n";
            }
            break;
        }
        case SET_HOST:
        {
            ishost = true;
            enableButton(startgame);
            break;
        }
        case EXITED_ROOM:
        {
            for(int i = 0; i < MAX_PLAYER_COUNT; ++i)
            {
                isplayerready[i] = 2;
            }
            ishost = false;
            playerready = false;
            lastloginrefresh = 0;
            logins.setString("");
            buttonChangedName(setready, "Set ready", buttons[1]);
            disableButton(startgame);
            break;
        }
        case SEND_MESSAGE:
        {
            int i;
            recv(mainsocket, buffer, BUFFER_SIZE, MSG_DONTWAIT); ///clear the buffer
            errno = ENOENT;
            buffer[0] = REQUEST_SEND_MESSAGE;
            while(!send(mainsocket, buffer, 1, 0)) {}
            char* cmessage = new char[FINAL_MESSAGE_LENGTH];
            for(i = 0; i < MAX_LOGIN_LENGTH; ++i)
            {
                if(playeraccountdata.login[i] == '\0')
                {
                    cmessage[i++] = ':';
                    cmessage[i++] = '\n';
                    break;
                }
                cmessage[i] = playeraccountdata.login[i];
            }
            for(unsigned int j = 0; j < messageinboxstring.size(); ++j)
            {
                cmessage[i++] = messageinboxstring[j];
            }
            cmessage[i++] = '\n';
            cmessage[i] = '\0';
            unsigned char* bufferptr = buffer;
            bufferptr = serializeCharArray(bufferptr, cmessage, FINAL_MESSAGE_LENGTH);
            int sendbytes = 0;
            int alldata = bufferptr - buffer;
            while(sendbytes < alldata)
            {
                sendbytes += send(mainsocket, buffer + sendbytes, alldata - sendbytes, 0);
            }
            while(recv(mainsocket, buffer, 1, 0) >= 0)
            {
                if(buffer[0] == FAILED_TO_SEND_MESSAGE)
                {
                    std::cerr << "Failed to send message\n";
                    break;
                }
                else if(buffer[0] == SEND_MESSAGE_SUCCESSFUL)
                {
                    messageinboxstring = "";
                    messageinbox.setString(messageinboxstring);
                    break;
                }
            }
            if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                std::cerr << "Can't reach server\n";
            }
        }
        case RECV_MESSAGE:
        {
            msgreadbytes += recv(communicationsocket, msgbuffer + msgreadbytes, FINAL_MESSAGE_LENGTH - msgreadbytes, MSG_DONTWAIT);
            if(msgreadbytes == FINAL_MESSAGE_LENGTH)
            {
                if(*msgbuffer == SERVER_REQUESTS_TO_START_GAME)
                {
                    if(joinMulticastGroup(&multicastsocket, &multicastGroup,
                     DEFAULT_MULTICAST_ADDRESS, DEFAULT_MULTICAST_PORT + msgbuffer[1],
                      DEFAULT_INTERFACE_ADDRESS, 0))
                    {
                        std::cerr << "Couldn't join multicast group\n";
                    }
                    numberofplayersingame = msgbuffer[2];
                    gvm->refresh(GET_READY);
                    break;
                }
                int entercount = 0;
                int erase = 0;
                for(unsigned int i = 0; i < allmessagesstring.size(); ++i)
                    if(allmessagesstring[i] == '\n')
                    {
                        entercount++;
                        if(entercount == 2) erase = i;
                    }

                if(entercount >= 12)
                {
                    allmessagesstring.erase(0, erase + 1);
                }
                std::cerr << "XXXXXXXXXXXXXXXXX\n";
                msgreadbytes = 0;
                allmessagesstring.append(std::string((char*)msgbuffer));

                allmessages.setString(allmessagesstring);
            }
            else if(msgreadbytes < 0)
            {
                //std::cerr << "XD\n";
                msgreadbytes = 0;
            }
        }
    }
}

void RoomViewModel::buttonPressed(int i)
{
    buttons[3].setFillColor(sf::Color(255,255,255));
    writeaccess = -1;
    switch(i)
    {
        case 0: ///start game button
        {
            if(ishost)
            {
                recv(mainsocket, buffer, BUFFER_SIZE, MSG_DONTWAIT); ///clear the buffer
                errno = ENOENT;
                buffer[0] = REQUEST_START_MATCH;
                while(!send(mainsocket, buffer, 1, 0)) {}

                while(recv(mainsocket, buffer, 1, 0) >= 0)
                {
                    if(buffer[0] == FAILED_TO_START_MATCH)
                    {
                        std::cerr << "Failed to set or unset ready\n";
                        break;
                    }
                    else if(buffer[0] == START_MATCH_SUCCESSFUL)
                    {
                        break;
                    }
                }
                if(errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    std::cerr << "Can't reach server\n";
                }
            }
            break;
        }
        case 1: ///set ready button
        {
            recv(mainsocket, buffer, BUFFER_SIZE, MSG_DONTWAIT); ///clear the buffer
            errno = ENOENT;
            buffer[0] = REQUEST_TOGGLE_READY;
            while(!send(mainsocket, buffer, 1, 0)) {}

            while(recv(mainsocket, buffer, 1, 0) >= 0)
            {
                if(buffer[0] == FAILED_TO_TOGGLE_READYNESS)
                {
                    std::cerr << "Failed to set or unset ready\n";
                    break;
                }
                else if(buffer[0] == TOGGLE_READYNESS_SUCCESSFUL)
                {
                    if(!playerready)
                    {
                        buttonChangedName(setready, "Set not ready", buttons[1]);
                        playerready = true;
                    }
                    else
                    {
                        buttonChangedName(setready, "Set ready", buttons[1]);
                        playerready = false;
                    }
                    lastloginrefresh = 0;
                    break;
                }
            }
            if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                std::cerr << "Can't reach server\n";
            }
            break;
        }
        case 2: ///exit room button
        {
            recv(mainsocket, buffer, BUFFER_SIZE, MSG_DONTWAIT); ///clear the buffer
            errno = ENOENT;
            buffer[0] = REQUEST_EXIT_ROOM;
            while(!send(mainsocket, buffer, 1, 0)) {}

            while(recv(mainsocket, buffer, 1, 0) >= 0)
            {
                if(buffer[0] == FAILED_TO_EXIT_ROOM)
                {
                    std::cerr << "Failed to set or unset ready\n";
                    break;
                }
                else if(buffer[0] == EXIT_ROOM_SUCCESSFUL)
                {
                    this->refresh(EXITED_ROOM);
                    this->setActivity(false);
                    this->setVisibility(false);
                    mvm->setActivity(true);
                    mvm->setVisibility(true);
                    break;
                }
            }
            if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                std::cerr << "Can't reach server\n";
            }
            break;
        }
        case 3: ///message box
        {
            buttons[3].setFillColor(sf::Color(255,200,200));
            writeaccess = 3;
            break;
        }
    }
}

void RoomViewModel::addLetter(char c)
{
    if(c == '\t')
        return;
    if(c == 13 && messageinboxstring.size() > 0)
    {
        refresh(SEND_MESSAGE);
        return;
    }
    switch(writeaccess)
    {
        case 3:
        {
            if(c == 8 && messageinboxstring.size() > 0)
            {
                messageinboxstring.pop_back();
            }
            else if(c != 8 && messageinboxstring.size() < MAX_MESSAGEINBOX_LENGTH - 1)
            {
                messageinboxstring.push_back(c);
            }
            messageinbox.setString(messageinboxstring);
            break;
        }
    }
}

void RoomViewModel::setLogins(char* lgs)
{
    std::string alllogins;
    for(int i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        if(lgs[i*MAX_LOGIN_LENGTH] != '\0')
        {
            std::string login((lgs+i*MAX_LOGIN_LENGTH));
            login.append("\n");
            alllogins += login;
        }
    }
    alllogins.append("\0");
    logins.setString(alllogins);
}
