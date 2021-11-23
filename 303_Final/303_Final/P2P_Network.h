#pragma once
#include "PacketBase.h"
class P2P_Network : public PacketBase
{
private:
	//  General information
	//sf::IpAddress myIpAdress;
	//unsigned short myUDPport;
	Peer myData; //only some variables will be used

	//  Sockets
	//sf::UdpSocket udpSocket;
	std::vector<Peer *> peers;

	sf::TcpListener listener;

	//  Packet storage
	sf::Packet outgoingPacket;
	sf::Packet incomingPacket;
public:
	////setup functions
	void setup();
	//
	////send & receive functions. Both send/receive to/fro all the clients
	//void send_UDP();
	//void receive_UDP();

	bool accept_TCP_new();
	bool connect_TCP_to(sf::IpAddress hostIP, unsigned short port);

	void receiveAll_TCP();
	void sendAll_TCP(sf::Packet packet);

	void sharePeers();
	void receivePeers();

	void decodePacket(sf::Packet packet);
};

