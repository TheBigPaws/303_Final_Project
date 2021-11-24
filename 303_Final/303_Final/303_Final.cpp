#include <iostream>
#include "P2P_Network.h"
#include <SFML/Window/Keyboard.hpp>
bool isHost;
P2P_Network networkModule;

sf::Packet test_pkt_;
header test_pkt_hdr_;
std::vector<Peer*> * peers;
int main()
{
	test_pkt_hdr_.game_elapsed_time = 0.0f;
	test_pkt_hdr_.information_amount = 1;
	test_pkt_hdr_.information_type = 5;
	test_pkt_ << test_pkt_hdr_ << "test text";

	networkModule.setup(); 

	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { //main game
		



		//LISTEN TO NEW PEERS, if succesful share peers with all
		networkModule.accept_TCP_new();



		//OPENS THE CONNECT INTERFACE
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) { //   C for connect

			sf::IpAddress connectTo = sf::IpAddress::getLocalAddress(); //change when multi pc
			//std::cout << "what IP do you want to connect to?\n";
			//std::cin >> connectTo;

			unsigned short port_;
			std::cout << "what listener port do you want to connect to?\n";
			std::cin >> port_;


			networkModule.connect_TCP_to(connectTo, port_, 1);
			
		}

		//should run only once ~ms
		networkModule.sendAll_TCP();

		//LISTEN TO MESSAGES FROM ALL
		networkModule.receiveAll_TCP();

		networkModule.decodePackets();

		//SEND A TCP MESSAGE IF THE USER PRESSES S
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { //   S for send
			networkModule.pushOutPacket_all(test_pkt_);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) { //   S for send
			peers = networkModule.getPeerReferences();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) { //   S for send
			networkModule.`getPeerReferences()->empty();
		}
	}

	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		std::cout << '.';
	}

}