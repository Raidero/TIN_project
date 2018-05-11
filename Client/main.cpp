#include "MenuViewModel.h"
#include "LoginViewModel.h"

#define NUMBER_OF_VIEW_MODELS 2
using namespace std;

int main()
{
    int clientsocket;
    struct sockaddr_in serveraddress;

    initClient(&clientsocket, &serveraddress);

    if(startClient(clientsocket, serveraddress))
    {
        std::cout << "Couldn't connect to server. Try again later\n";
        //return 1;
    }

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Maze Shooter");
    ViewModel* viewmodels[NUMBER_OF_VIEW_MODELS];
    viewmodels[0] = new MenuViewModel(clientsocket);
    viewmodels[1] = new LoginViewModel(viewmodels[0], serveraddress.sin_addr.s_addr);
    dynamic_cast<MenuViewModel*>(viewmodels[0])->linkLoginViewModel(viewmodels[1]);

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
        window.display();
    }

    delete viewmodels[0];
    delete viewmodels[1];
    return 0;
}
