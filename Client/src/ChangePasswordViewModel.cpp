#include "ChangePasswordViewModel.h"

ChangePasswordViewModel::ChangePasswordViewModel(ViewModel* mvm): LoginViewModel(mvm)
{
    disposeButtons();
    initButtons(5);
    newpasswordpos = 0;
    for(int i = 0; i < MAX_PASSWORD_LENGTH; ++i)
    {
        newpassword[i] = 0;
    }
    background.setSize(sf::Vector2f(BACKGROUND_WIDTH, BACKGROUND_HEIGHT + 20));
    background.setOrigin(sf::Vector2f(BACKGROUND_WIDTH/2, BACKGROUND_HEIGHT/2));
    background.setPosition(sf::Vector2f(WIDTH/2, HEIGHT/2));
    title.setString("Change password");
    createButton(loginbutton, buttons[0], "change password", title.getPosition().y + 4*TEXT_STEP, title.getPosition().x);
    createButton(cancelbutton, buttons[1], "back", loginbutton.getPosition().y, loginbutton.getPosition().x + loginbutton.getLocalBounds().width + 10);
    buttons[2].setSize(sf::Vector2f(background.getSize().x - 2*MARGIN, FONT_SMALL + MARGIN));
    buttons[2].setPosition(sf::Vector2f(title.getPosition().x, title.getPosition().y + 1*TEXT_STEP));
    buttons[3].setSize(sf::Vector2f(background.getSize().x - 2*MARGIN, FONT_SMALL + MARGIN));
    buttons[3].setPosition(sf::Vector2f(title.getPosition().x, title.getPosition().y + 2*TEXT_STEP));
    buttons[4].setSize(sf::Vector2f(background.getSize().x - 2*MARGIN, FONT_SMALL + MARGIN));
    buttons[4].setPosition(sf::Vector2f(title.getPosition().x, title.getPosition().y + 3*TEXT_STEP));
    newpasswordview.setPosition(sf::Vector2f(buttons[4].getPosition().x + MARGIN/2, buttons[4].getPosition().y + MARGIN/2));
    newpasswordview.setFont(font);
    newpasswordview.setCharacterSize(FONT_SMALL);
    newpasswordview.setColor(sf::Color(0,0,0));
}

ChangePasswordViewModel::~ChangePasswordViewModel()
{
    //dtor
}

void ChangePasswordViewModel::buttonPressed(int i)
{
    buttons[2].setFillColor(sf::Color(255,255,255));
    buttons[3].setFillColor(sf::Color(255,255,255));
    buttons[4].setFillColor(sf::Color(255,255,255));
    message.setString(std::string(""));
    writeaccess = -1;
    switch(i)
    {
        case 0: //change password button
            if(loginpos != 0 && passwordpos != 0 && newpasswordpos != 0)
            {
                SHA256((unsigned char*)password, passwordpos, (unsigned char*)passwordhash);
                SHA256((unsigned char*)newpassword, newpasswordpos, (unsigned char*)newpasswordhash);
                for(int i = 0; i < MAX_PASSHASH_LENGTH; ++i)
                {
                    accountdata.passwordhash[i] = this->passwordhash[i];
                }
                for(int i = 0; i < loginpos; ++i)
                {
                    accountdata.login[i] = login[i];
                }
                accountdata.login[loginpos] = 0;
                buffer[0] = REQUEST_CHANGE_PASSWORD;
                while(!send(mainsocket, buffer, 1, 0)) {}
                unsigned char* bufferptr = buffer;
                bufferptr = serializeAccountData(bufferptr, &accountdata);
                bufferptr = serializeUnsignedCharArray(bufferptr, newpasswordhash, MAX_PASSHASH_LENGTH);
                int alldata = bufferptr - buffer;
                int sendbytes = 0;
                while(sendbytes < alldata)
                {
                    sendbytes += send(mainsocket, buffer + sendbytes, alldata - sendbytes, 0);
                }
                while(!recv(mainsocket, buffer, 1, MSG_WAITALL)) {}
                if(buffer[0] == FAILED_TO_CHANGE_PASSWORD)
                {
                    message.setString(std::string("Failed to change password"));
                    message.setPosition(
                     background.getGlobalBounds().left + BACKGROUND_WIDTH - message.getLocalBounds().width - MARGIN,
                     background.getGlobalBounds().top + MARGIN);
                }
                else if(buffer[0] == CHANGE_PASSWORD_SUCCESSFUL)
                {
                    this->refresh(CHANGED_PASSWORD);
                }
            }
            else
            {
                message.setString(std::string("Login or password fields empty"));
                message.setPosition(
                     background.getGlobalBounds().left + BACKGROUND_WIDTH - message.getLocalBounds().width - MARGIN,
                     background.getGlobalBounds().top + MARGIN);
                std::cerr << "Login or password field is empty\n";
            }
            break;
        case 1: // back button
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
        case 4: //newpasswordbutton
            message.setString(std::string("Enter new password"));
            message.setPosition(
                 background.getGlobalBounds().left + BACKGROUND_WIDTH - message.getLocalBounds().width - MARGIN,
                 background.getGlobalBounds().top + MARGIN);
            buttons[4].setFillColor(sf::Color(255,200,200));
            writeaccess = 4;
            break;
    }
}

void ChangePasswordViewModel::addLetter(char c)
{
    if(c == '\t')
    {
        if(writeaccess < 4)
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
        case 4: ///newpassword text box
        {
            if(c == 8 && newpasswordpos > 0)
            {
                newpassword[--newpasswordpos] = '\0';
            }
            else if(c != 8 && newpasswordpos < MAX_PASSWORD_LENGTH - 1)
            {
                newpassword[newpasswordpos++] = c;
            }
            std::string s;
            for(int i = 0; i < newpasswordpos; ++i)
                s += '*';
            newpasswordview.setString(s);
            break;
        }
    }
}

void ChangePasswordViewModel::refresh(int message)
{
    switch(message)
    {
        case CHANGED_PASSWORD:
        {
            this->message.setString(std::string("Password has changed"));
            this->message.setPosition(
             background.getGlobalBounds().left + BACKGROUND_WIDTH - this->message.getLocalBounds().width - MARGIN,
             background.getGlobalBounds().top + MARGIN);
        }
    }
}
