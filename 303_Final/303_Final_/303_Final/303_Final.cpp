#include <iostream>
#include <string>
#include <SFML/Network.hpp>
#include "P2P_Network.h"
#include <SFML/Window/Keyboard.hpp>

std::string outgoing_message = "ajaj";
bool isHost;

P2P_Network networkModule;

int main()
{
	std::cout << "Do You want to receive(1) or send(0)?";

	std::cin >> isHost;
	networkModule.setup();

	if (isHost) {// ------------------- HOST CODE ---------------------------
		
		
		while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			networkModule.update();
		}
	}
	else { // -----------------------------CLIENT CODE -----------------------------
	
		sf::IpAddress ipadr;
		unsigned short port_;

		std::cout << "enter IP to connect to: ";

		std::cin >> ipadr;

		std::cout << "enter port to connect to: ";

		std::cin >> port_;

		networkModule.connect_TCP(ipadr,port_);

		while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {

			std::cout << "enter a message to send thru \n";
	
			std::cin >> outgoing_message;
			
			networkModule.send_UDP();
			networkModule.send_TCP();

			networkModule.update();

		}
		
	}
	
	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {

	}
	
}
