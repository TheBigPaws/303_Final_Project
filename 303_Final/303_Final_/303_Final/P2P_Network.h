#pragma once
#include "CustomPackets.h"

class P2P_Network : public CustomPackets
{
private:
	//General information
	sf::IpAddress myIpAdress;
	unsigned short myUDPport;
	bool hasMasterCall;
	
	//Sockets
	sf::UdpSocket udpSocket;
	std::vector<sf::TcpSocket*> TCP_connections;
	sf::TcpListener listener; //listener for clients

	//Packet storage
	sf::Packet outgoingPacket;
	sf::Packet incomingPacket;
public:
	//setup functions
	void update();
	void setup();

	//send & receive functions. Both send/receive to/fro all the clients
	void send_UDP();
	void send_TCP();
	void receive_UDP(int recepientIndex);
	void receive_TCP(int recepientIndex);

	void acceptClient_TCP();
	void connect_TCP(sf::IpAddress hostIP, unsigned short clientPort);

	//getters
	sf::IpAddress get_myIpAdress() { return myIpAdress;}
	unsigned short get_myUDPport() { return myUDPport;}
};

