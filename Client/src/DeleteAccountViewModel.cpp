#include "DeleteAccountViewModel.h"

DeleteAccountViewModel::DeleteAccountViewModel(ViewModel* mvm): LoginViewModel(mvm)
{
    title.setString("Delete account");
    createButton(loginbutton, buttons[0], "delete", title.getPosition().y + 3*TEXT_STEP, title.getPosition().x);
    createButton(cancelbutton, buttons[1], "cancel", loginbutton.getPosition().y, loginbutton.getPosition().x + loginbutton.getLocalBounds().width + 10);
    message.setCharacterSize(FONT_SMALL);
    message.setPosition(background.getPosition().x - 20, title.getPosition().y + 5);
}

DeleteAccountViewModel::~DeleteAccountViewModel()
{
    //dtor
}

void DeleteAccountViewModel::buttonPressed(int i)
{
    buttons[2].setFillColor(sf::Color(255,255,255));
    buttons[3].setFillColor(sf::Color(255,255,255));
    message.setString(std::string(""));
    writeaccess = -1;
    switch(i)
    {
        case 0: //delete button
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
                buffer[0] = REQUEST_DELETE_ACCOUNT;
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
                    if(buffer[0] == FAILED_TO_DELETE_ACCOUNT)
                    {
                        message.setString(std::string("Failed to delete account"));
                        message.setPosition(
                            background.getGlobalBounds().left + BACKGROUND_WIDTH - message.getLocalBounds().width - MARGIN,
                            background.getGlobalBounds().top + MARGIN);
                        break;
                    }
                    else if(buffer[0] == DELETE_ACCOUNT_SUCCESSFUL)
                    {
                        this->refresh(ACCOUNT_DELETED);
                        menuviewmodel->refresh(ACCOUNT_DELETED);
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

void DeleteAccountViewModel::refresh(int message)
{
    switch(message)
    {
        case ACCOUNT_DELETED:
        {
            this->setVisibility(false);
            this->setActivity(false);
            menuviewmodel->setActivity(true);
        }
    }
}
