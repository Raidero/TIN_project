#include "CreateAccountViewModel.h"

CreateAccountViewModel::CreateAccountViewModel(ViewModel* mvm): LoginViewModel(mvm)
{
    title.setString("Sign up");
    createButton(loginbutton, buttons[0], "create", title.getPosition().y + 3*TEXT_STEP, title.getPosition().x);
    createButton(cancelbutton, buttons[1], "cancel", loginbutton.getPosition().y, loginbutton.getPosition().x + loginbutton.getLocalBounds().width + 10);
    message.setPosition(background.getPosition().x - 35, title.getPosition().y + 5);
}

CreateAccountViewModel::~CreateAccountViewModel()
{
    //dtor
}

void CreateAccountViewModel::buttonPressed(int i)
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
                SHA256((unsigned char*)password, passwordpos, (unsigned char*)passwordhash);

                for(int i = 0; i < MAX_PASSHASH_LENGTH; ++i)
                {
                    accountdata.passwordhash[i] = this->passwordhash[i];
                }
                for(int i = 0; i < loginpos; ++i)
                {
                    accountdata.login[i] = login[i];
                }
                accountdata.login[loginpos] = 0;
                recv(mainsocket, buffer, BUFFER_SIZE, MSG_DONTWAIT); ///clear the buffer
                errno = ENOENT;
                buffer[0] = REQUEST_CREATE_ACCOUNT;
                while(!send(mainsocket, buffer, 1, 0)) {}
                unsigned char* bufferptr = buffer;
                bufferptr = serializeAccountData(bufferptr, &accountdata);
                int alldata = bufferptr - buffer;
                int sendbytes = 0;
                while(sendbytes < alldata)
                {
                    sendbytes += send(mainsocket, buffer + sendbytes, alldata - sendbytes, 0);
                }
                while(recv(mainsocket, buffer, 1, 0) >= 0)
                {
                    if(buffer[0] == FAILED_TO_CREATE_ACCOUNT)
                    {
                        message.setString(std::string("Failed to create account"));
                        message.setPosition(
                            background.getGlobalBounds().left + BACKGROUND_WIDTH - message.getLocalBounds().width - MARGIN,
                            background.getGlobalBounds().top + MARGIN);
                        break;
                    }
                    else if(buffer[0] == CREATE_ACCOUNT_SUCCESSFUL)
                    {
                        this->refresh(ACCOUNT_CREATED);
                        menuviewmodel->refresh(ACCOUNT_CREATED);
                        break;
                    }
                }
                if(errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    message.setString(std::string("Can't reach server"));
                    message.setPosition(
                        background.getGlobalBounds().left + BACKGROUND_WIDTH - message.getLocalBounds().width - MARGIN,
                        background.getGlobalBounds().top + MARGIN);
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

void CreateAccountViewModel::refresh(int message)
{
    switch(message)
    {
        case ACCOUNT_CREATED:
        {
            this->setVisibility(false);
            this->setActivity(false);
            menuviewmodel->setActivity(true);
        }
    }
}
