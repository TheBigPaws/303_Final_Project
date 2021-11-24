#pragma once
#include "PacketBase.h"
class P2P_Network : public PacketBase
{
private:
	std::string myName;

	//  Sockets
	std::vector<Peer *> peers;

	sf::TcpListener mylistener;


public:
	////setup functions
	void setup();

	void accept_TCP_new();
	void connect_TCP_to(sf::IpAddress hostIP, unsigned short port, bool sharePeers_);

	void receiveAll_TCP();//receive all packets and sort them depending on where they're from
	void sendAll_TCP(); //send all packets that were built up in the last ~ms

	void pushOutPacket(std::string name,sf::Packet packet);
	void pushOutPacket_all(sf::Packet packet);

	void sharePeers();

	void decodePackets();

	std::vector<Peer * > getPeerReferences() { return peers; }
};

