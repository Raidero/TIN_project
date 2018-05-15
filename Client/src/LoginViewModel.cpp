#include "LoginViewModel.h"

LoginViewModel::LoginViewModel(ViewModel* mvm)
{
    initButtons(4);
    menuviewmodel = mvm;
    loginpos = 0;
    passwordpos = 0;
    for(int i = 0; i < MAX_LOGIN_LENGTH; ++i)
    {
        login[i] = 0;
    }
    for(int i = 0; i < MAX_PASSWORD_LENGTH; ++i)
    {
        password[i] = 0;
    }
    writeaccess = -1;
    background.setFillColor(sf::Color(50,50,50));
    background.setOutlineColor(sf::Color(100,0,0));
    background.setOutlineThickness(3);
    background.setSize(sf::Vector2f(BACKGROUND_WIDTH, BACKGROUND_HEIGHT));
    background.setOrigin(sf::Vector2f(BACKGROUND_WIDTH/2, BACKGROUND_HEIGHT/2));
    background.setPosition(sf::Vector2f(WIDTH/2, HEIGHT/2));
    title.setString("Sign in:");
    title.setCharacterSize(FONT_SMALL);
    title.setFont(font);
    title.setPosition(background.getPosition().x - BACKGROUND_WIDTH/2 + MARGIN, background.getPosition().y - BACKGROUND_HEIGHT/2 + MARGIN);
    createButton(loginbutton, buttons[0], "login", title.getPosition().y + 3*TEXT_STEP, title.getPosition().x);
    createButton(cancelbutton, buttons[1], "cancel", loginbutton.getPosition().y, loginbutton.getPosition().x + loginbutton.getLocalBounds().width + 10);
    buttons[2].setSize(sf::Vector2f(background.getSize().x - 2*MARGIN, FONT_SMALL + MARGIN));
    buttons[2].setPosition(sf::Vector2f(title.getPosition().x, title.getPosition().y + 1*TEXT_STEP));
    loginview.setPosition(sf::Vector2f(buttons[2].getPosition().x + 2, buttons[2].getPosition().y + 2));
    loginview.setFont(font);
    loginview.setCharacterSize(FONT_SMALL);
    loginview.setColor(sf::Color(0,0,0));
    buttons[3].setSize(sf::Vector2f(background.getSize().x - 2*MARGIN, FONT_SMALL + MARGIN));
    buttons[3].setPosition(sf::Vector2f(title.getPosition().x, title.getPosition().y + 2*TEXT_STEP));
    passwordview.setPosition(sf::Vector2f(buttons[3].getPosition().x + 2, buttons[3].getPosition().y + 2));
    passwordview.setFont(font);
    passwordview.setCharacterSize(FONT_SMALL);
    passwordview.setColor(sf::Color(0,0,0));
    message.setCharacterSize(FONT_SMALL);
    message.setFont(font);
    message.setPosition(background.getPosition().x - 20, title.getPosition().y + 5);
}

LoginViewModel::~LoginViewModel()
{
}

void LoginViewModel::buttonPressed(int i)
{
    buttons[2].setFillColor(sf::Color(255,255,255));
    buttons[3].setFillColor(sf::Color(255,255,255));
    message.setString(std::string(""));
    writeaccess = -1;
    switch(i)
    {
        case 0: //login button
            if(loginpos != 0 && passwordpos != 0)
            {
                SHA256((unsigned char*)&password, passwordpos, (unsigned char*)&passwordhash);

                for(int i = 0; i < MAX_PASSHASH_LENGTH; ++i)
                {
                    accountdata.passwordhash[i] = this->passwordhash[i];
                }
                for(int i = 0; i < loginpos; ++i)
                {
                    accountdata.login[i] = login[i];
                }
                accountdata.login[loginpos] = 0;
                buffer[0] = REQUEST_LOGIN;
                while(!send(mainsocket, buffer, 1, 0)) {}
                unsigned char* bufferptr = buffer;
                bufferptr = serializeAccountData(bufferptr, &accountdata);
                int alldata = bufferptr - buffer;
                int sendbytes = 0;
                while(sendbytes < alldata)
                {
                    sendbytes += send(mainsocket, buffer + sendbytes, alldata - sendbytes, 0);
                }
                while(!recv(mainsocket, buffer, 1, MSG_WAITALL)) {}
                if(buffer[0] == FAILED_TO_LOGIN)
                {
                    message.setString(std::string("Wrong login or password"));
                    message.setPosition(
                        background.getGlobalBounds().left + BACKGROUND_WIDTH - message.getLocalBounds().width - MARGIN,
                        background.getGlobalBounds().top + MARGIN);
                }
                else if(buffer[0] == LOGIN_SUCCESSFUL)
                {
                    this->refresh(PLAYER_LOGGED_IN);
                    menuviewmodel->refresh(PLAYER_LOGGED_IN);
                    for(int i = 0; i < MAX_LOGIN_LENGTH; ++i)
                    {
                        playeraccountdata.login[i] = accountdata.login[i];
                    }
                    playeraccountdata.currentip = accountdata.currentip;
                }
            }
            else
            {
                message.setString(std::string("Login or password empty"));
                message.setPosition(
                 background.getGlobalBounds().left + BACKGROUND_WIDTH - message.getLocalBounds().width - MARGIN,
                 background.getGlobalBounds().top + MARGIN);
                std::cerr << "Login or password field is empty\n";
            }
            break;
        case 1: // cancel button
            this->setVisibility(false);
            this->setActivity(false);
            menuviewmodel->setActivity(true);
            break;
        case 2:
            message.setString(std::string("Enter login"));
            message.setPosition(
                 background.getGlobalBounds().left + BACKGROUND_WIDTH - message.getLocalBounds().width - MARGIN,
                 background.getGlobalBounds().top + MARGIN);
            buttons[2].setFillColor(sf::Color(255,200,200));
            writeaccess = 2;
            break;
        case 3:
            message.setString(std::string("Enter password"));
            message.setPosition(
                 background.getGlobalBounds().left + BACKGROUND_WIDTH - message.getLocalBounds().width - MARGIN,
                 background.getGlobalBounds().top + MARGIN);
            buttons[3].setFillColor(sf::Color(255,200,200));
            writeaccess = 3;
            break;
    }
}

void LoginViewModel::addLetter(char c)
{
    if(c == '\t')
    {
        if(writeaccess < 3)
        {
            buttons[writeaccess++].setFillColor(sf::Color(255,255,255));
            buttons[writeaccess].setFillColor(sf::Color(255,200,200));
        }
        return;
    }
    switch(writeaccess)
    {
        case 2: ///login text box
        {
            if(c == 8 && loginpos > 0)
            {
                login[--loginpos] = '\0';
            }
            else if(c != 8 && loginpos < MAX_LOGIN_LENGTH - 1)
            {
                login[loginpos++] = c;
            }
            std::string s(login);
            loginview.setString(s);
            break;
        }
        case 3: ///password text box
        {
            if(c == 8 && passwordpos > 0)
            {
                password[--passwordpos] = '\0';
            }
            else if(c != 8 && passwordpos < MAX_PASSWORD_LENGTH - 1)
            {
                password[passwordpos++] = c;
            }
            std::string s;
            for(int i = 0; i < passwordpos; ++i)
                s += '*';
            passwordview.setString(s);
            break;
        }
    }
}

void LoginViewModel::refresh(int message)
{
    switch(message)
    {
        case PLAYER_LOGGED_IN:
        {
            this->setVisibility(false);
            this->setActivity(false);
            menuviewmodel->setActivity(true);
        }
    }
}
