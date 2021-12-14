#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string.h>
#include <vector>
#include <queue>
#include <chrono>

//Peer struct that holds all necessary information about everyone
struct Peer {
	std::string name;

	//tcp socket belonging to this peer
	sf::TcpSocket socket;

	sf::IpAddress IpAddress = sf::IpAddress::None;

	unsigned short TCP_listener_Port = 0;

	//queues of packets (will be treated as tasks)
	std::queue<sf::Packet> receivedPackets;
	std::queue<sf::Packet> outgoingPackets;
};

//enum describing what information is going to be in the packet
enum Data_Header { ntn, CHAT_MESSAGE, NW_INFO, PLAYER_POS_ANGLE, BULLET_SHOT, AREA_CAPTURED, PLAYER_HIT, ADD_ME_TO_GAME, LOBBY_READY_STATUS };

//packet header
struct header {
	std::string senderName;
	float game_elapsed_time;
	sf::Uint16 information_type;
	sf::Uint16 information_amount;
};

//struct holding all relevant (networking) peer information
struct peerNWinfo {
	std::string name;
	sf::Uint32 ipAddress;
	sf::Uint16  listenerPort;
};


//packet stream overload functions

static sf::Packet& operator <<(sf::Packet& packet, const peerNWinfo& peerInfo) // peer info packet ***
{
	return packet << peerInfo.name << peerInfo.ipAddress << peerInfo.listenerPort;
}

static sf::Packet& operator >>(sf::Packet& packet, peerNWinfo& peerInfo)
{
	return packet >> peerInfo.name >> peerInfo.ipAddress >> peerInfo.listenerPort;
}


static sf::Packet& operator <<(sf::Packet& packet, const header& header_) // peer info packet ***
{
	return packet << header_.senderName << header_.game_elapsed_time << header_.information_type << header_.information_amount;
}

static sf::Packet& operator >>(sf::Packet& packet, header& header_)
{
	return packet >> header_.senderName >> header_.game_elapsed_time >> header_.information_type >> header_.information_amount;
}