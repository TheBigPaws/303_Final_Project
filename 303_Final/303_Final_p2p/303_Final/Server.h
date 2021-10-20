#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string.h>
#include <vector>
class Server
{
private:
	sf::TcpListener listener; //listener for clients
	sf::TcpSocket client; //client holder
	//std::vector<sf::TcpSocket> clients;
public:
	//HOST functions. h_ prefix
	void setupListener_TCP();

	void acceptClient_TCP();

	void receive_TCP();

	void serverLoop();//infinite loop, listens for new clients and gets messages
};

