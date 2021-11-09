#include <iostream>
#include <string>
#include <SFML/Network.hpp>
#include "Server.h"
#include "Client.h"
#include <SFML/Window/Keyboard.hpp>

std::string outgoing_message = "ajaj";
bool isHost;

Server server;
Client client;

int main()
{
	std::cout << "Do You want to host(1) or connect(0)?";

	std::cin >> isHost;

	if (isHost) {// ------------------- HOST CODE ---------------------------
		server.setup(52000);
		
		while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			server.update();
			
		}
	}
	else { // -----------------------------CLIENT CODE -----------------------------
	
		client.connectToHost_TCP(sf::IpAddress::getLocalAddress().toString());
		client.bind_UDP(52000);

		while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {

			std::cout << "enter a message to send thru \n";
	
			std::cin >> outgoing_message;
			
			client.send_UDP();
			client.send_TCP();
		}
		
	}
	
	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {

	}
	
}
