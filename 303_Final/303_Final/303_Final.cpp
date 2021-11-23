#include <iostream>
#include "P2P_Network.h"
#include <SFML/Window/Keyboard.hpp>
bool isHost;
P2P_Network networkModule;

sf::Packet pkt_;

int main()
{
	
	pkt_ << 1;
	pkt_ << "banana";

	networkModule.setup(); 

	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { //main game
		



		//LISTEN TO NEW PEERS, if succesful share peers with all
		if (networkModule.accept_TCP_new() == true) { // always listen to new connections


		}

		//std::cout << "rcv all func still worked after acceptnew\n";


		//OPENS THE CONNECT INTERFACE
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) { //   C for connect

			sf::IpAddress connectTo = sf::IpAddress::getLocalAddress();
			//std::cout << "what IP do you want to connect to?\n";
			//std::cin >> connectTo;

			unsigned short port_;
			std::cout << "what listener port do you want to connect to?\n";
			std::cin >> port_;


			if (networkModule.connect_TCP_to(connectTo, port_) == true) {


			}
			
		}

		//std::cout << "rcv all func still worked after send t\n";

		//LISTEN TO MESSAGES FROM ALL
		networkModule.receiveAll_TCP();


		//SEND A MESSAGE IF THE USER PRESSES S
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { //   S for send
			networkModule.sendAll_TCP(pkt_);
		}

		//std::cout << "rcv all func still worked after sendall \n";


	}

	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		std::cout << '.';
	}

}