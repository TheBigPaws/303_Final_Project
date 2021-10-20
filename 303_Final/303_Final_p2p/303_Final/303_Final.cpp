#include <iostream>
#include <string>
#include <SFML/Network.hpp>
#include "Server.h"
#include "Client.h"
#include <SFML/Window/Keyboard.hpp>

std::string outgoing_message;
bool isHost;

Server server;
Client client;

int main()
{
	std::cout << "Do You want to host(1) or connect(0)?";

	std::cin >> isHost;

	if (isHost) {
		server.setupListener_TCP();
		//server.acceptClient_TCP();
		while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			server.acceptClient_TCP();
			server.receive_TCP();
		}
	}
	else {

		client.connectToHost_TCP(sf::IpAddress::getLocalAddress().toString());

		while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			
			std::cout << "enter a message to send thru \n";

			std::cin >> outgoing_message;

			client.sendToHost_TCP(outgoing_message);
		}
		
	}
	

	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {

	}

	//stopper input so the program doesnt close immediately after completion
	
}
