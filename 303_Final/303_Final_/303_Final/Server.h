#pragma once
#include "NetworkExtensionBase.h"

class Server : public NetworkExtensionBase
{
private:
	sf::TcpListener listener; //listener for clients
	std::vector<sf::TcpSocket*> clients_;

	//private helper functions
	void acceptClient_TCP();
	void setup_TCP();
	
	//TODO UDP and TCP send and receive based on inherited functions

public:

	void update();
	void setup(int UDPport);

	void send_UDP();
	void receive_UDP();

	void send_TCP();
	void receive_TCP();
};
