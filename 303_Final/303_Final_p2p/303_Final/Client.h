#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string.h>

class Client
{
private:
	int connect_attempt_time = 6; //how many seconds to wait before attempting reconnect
	//CONNECTING VARS
	sf::TcpSocket socket; //socket of this program that will connect to host
	char data[30];
public:
	//converts a string into char[30], filled with '-' for empty slots
	void convertToData(std::string message);

	void sendToHost_TCP(std::string message);
	void connectToHost_TCP(std::string hostIP);
};

