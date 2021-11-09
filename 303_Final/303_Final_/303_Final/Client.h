#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string.h>
#include "NetworkExtensionBase.h"

class Client : public NetworkExtensionBase
{
private:
	int connect_attempt_time = 6; //how many seconds to wait before attempting reconnect
	//CONNECTING VARS
	sf::TcpSocket serverSocket; //socket of this program that will connect to host

	sf::IpAddress serverIP;

	unsigned short serverPort;

public:
	void connectToHost_TCP(sf::IpAddress hostIP);
	
	void send_UDP();
	void receive_UDP();

	void send_TCP();
	void receive_TCP();
};

