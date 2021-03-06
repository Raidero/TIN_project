#include "MenuViewModel.h"
#include "LoginViewModel.h"
#include "CreateAccountViewModel.h"
#include "DeleteAccountViewModel.h"
#include "ChangePasswordViewModel.h"
#include "RoomViewModel.h"

#define NUMBER_OF_VIEW_MODELS 6
using namespace std;

int main()
{
    int clientsocket;
    struct sockaddr_in serveraddress;
    struct timeval sockettimeout;
    initSocket(&clientsocket, &serveraddress, &sockettimeout);

    if(startClient(clientsocket, serveraddress))
    {
        std::cout << "Couldn't connect to server. Try again later\n";
        //return 1;
    }

    ViewModel* viewmodels[NUMBER_OF_VIEW_MODELS];
    viewmodels[0] = new MenuViewModel(clientsocket, serveraddress.sin_addr.s_addr);
    viewmodels[1] = new LoginViewModel(viewmodels[0]);
    viewmodels[2] = new CreateAccountViewModel(viewmodels[0]);
    viewmodels[3] = new DeleteAccountViewModel(viewmodels[0]);
    viewmodels[4] = new ChangePasswordViewModel(viewmodels[0]);
    viewmodels[5] = new RoomViewModel(viewmodels[0]);
    dynamic_cast<MenuViewModel*>(viewmodels[0])->linkLoginViewModel(viewmodels[1]);
    dynamic_cast<MenuViewModel*>(viewmodels[0])->linkCreateAccountViewModel(viewmodels[2]);
    dynamic_cast<MenuViewModel*>(viewmodels[0])->linkDeleteAccountViewModel(viewmodels[3]);
    dynamic_cast<MenuViewModel*>(viewmodels[0])->linkChangePasswordViewModel(viewmodels[4]);
    dynamic_cast<MenuViewModel*>(viewmodels[0])->linkRoomViewModel(viewmodels[5]);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Maze Shooter");
    window.setFramerateLimit(30);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::TextEntered && event.text.unicode < 128)
            {
                for (int i = 0; i < NUMBER_OF_VIEW_MODELS; ++i)
                {
                    if (viewmodels[i]->isActive())
                        viewmodels[i]->addLetter(event.text.unicode);
                }
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2i coords = sf::Mouse::getPosition(window);
                for (int i = 0; i < NUMBER_OF_VIEW_MODELS; ++i)
                {
                    if (viewmodels[i]->isActive())
                        viewmodels[i]->checkButtonsPressed(coords.x, coords.y);
                }
            }
        }

        if(dynamic_cast<MenuViewModel*>(viewmodels[0])->exitGame())
            window.close();

        window.clear();
        for (int i = 0; i < NUMBER_OF_VIEW_MODELS; ++i)
        {
            window.draw(*viewmodels[i]);
        }
        if(viewmodels[5]->isActive())
        {
            viewmodels[5]->refresh(REFRESH_LOGINS);
            viewmodels[5]->refresh(RECV_MESSAGE);
        }

        window.display();
    }
    close(mainsocket);
    for(int i = 0; i < NUMBER_OF_VIEW_MODELS; ++i)
    {
        delete viewmodels[i];
    }
    return 0;
}
