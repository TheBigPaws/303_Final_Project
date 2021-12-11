#pragma once
#include "PacketBase.h"
class P2P_Network : public PacketBase
{
private:

	Peer myInfo;

	std::string myName = "";

	//  Sockets
	std::vector<Peer*> peers = {new Peer};

	sf::TcpListener mylistener;

	bool isSetup = false;

	

public:
	////setup functions
	void setup();

	bool accept_TCP_new();
	bool connect_TCP_to(sf::IpAddress hostIP, unsigned short port, bool sharePeers_); //returns true if succesfully connected

	bool anyPacketsToRead();
	sf::Packet getPacketToRead();

	void receiveAll_TCP();//receive all packets and sort them depending on where they're from
	void sendAll_TCP(); //send all packets that were built up in the last ~ms

	void pushOutPacket(std::string name,sf::Packet packet);
	void pushOutPacket_all(sf::Packet packet);

	void sharePeers();

	//void decodePackets();


	int getPeerCount() { return peers.size() - 1; }
	Peer* getPeer(int at) { return peers.at(at); }
	Peer* getMyInfo() { return &myInfo; }
	void setMyName(std::string name) { myInfo.name = name; }
	bool someoneDisconnected = false;
	std::string disconnectedName = "";
};

