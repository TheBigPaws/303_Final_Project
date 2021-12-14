#pragma once
#include "PacketBase.h"

enum connectResult{SUCCESS,NAME_ALREADY_USED,WRONG_INPUT};

class P2P_Network
{
private:

	Peer myInfo;
	sf::TcpListener mylistener;

	// Peers
	std::vector<Peer*> peers = {new Peer};

public:
	//accessible vars from other classes indicating who (if someone did) disconnected
	bool someoneDisconnected = false;
	std::string disconnectedName = "";

	//setup function
	void setup();

	//both return true if succesfully connected
	bool accept_TCP_new();
	connectResult connect_TCP_to(sf::IpAddress hostIP, unsigned short port, bool sharePeers_);

	//packet accessing functions
	bool anyPacketsToRead();
	sf::Packet getPacketToRead();

	void receiveAll_TCP();//receive all packets and sort them depending on where they're from
	void sendAll_TCP(); //send all packets that were built up in the last ~ms

	void pushOutPacket(std::string name,sf::Packet packet); //only push the send packet to specified player - private information send
	void pushOutPacket_all(sf::Packet packet); //push a packet to everyone's send queue

	void sharePeers(); //function that shares my peers with the person connecting to me

	//getters, setters
	int getPeerCount() { return peers.size() - 1; }
	Peer* getPeer(int at) { return peers.at(at); }
	Peer* getLastAddedPeer() { return peers.at(peers.size()-2); }
	Peer* getMyInfo() { return &myInfo; }
	void setMyName(std::string name) { myInfo.name = name; }
	
};

