extern "C"
{
    #include <stdio.h>
    #include <stdlib.h>
    #include "Client.h"
}
#include "MenuViewModel.h"
#include <iostream>
int main()
{
    MenuViewModel menu;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Maze Shooter");

    int clientsocket;
    short clientport;
    struct sockaddr_in serveraddress;

    initClient(&clientsocket, &clientport, &serveraddress);

    if(startClient(clientsocket, serveraddress))
    {
        std::cout << "Couldn't connect to server. Try again later\n";
        return 1;
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(menu);
        window.display();
    }
    return 0;
}
