#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string.h>
#include <vector>
#include <queue>
#include <chrono>

struct header {
	float game_elapsed_time;
	sf::Uint16 information_type;
	sf::Uint16 information_amount;
};

struct playerInformation {

	std::string name;
	float position[2];
	float lookAngle;
};

struct peerNWinfo {
	sf::Uint32 ipAddress;
	sf::Uint16  listenerPort;
};

struct Peer {
	std::string name;

	//tcp socket belonging to this peer
	sf::TcpSocket socket;

	//variables because SFML needs consts for some functions, 
	sf::IpAddress IpAddress = sf::IpAddress::None;
	unsigned short TCP_localPort = 0;
	unsigned short TCP_remotePort = 0;
	unsigned short TCP_listener_Port = 0;

	//queues of packets (will be treated as tasks)
	std::queue<sf::Packet> receivedPackets;
	std::queue<sf::Packet> outgoingPackets;
};

enum Data_Header { ntn, PLAYER_POSITION, PLAYER_ANIMATION_STAGE, PARTICLE_POSITION, PARTICLE_ROTATION, TEST, IP_ADDRESS, LISTENER_PORT, NW_INFO, PING };

class PacketBase
{
public:
	//CUSTOM STRUCT STREAM OVERLOAD
	//friend sf::Packet& operator <<(sf::Packet& packet, const playerInformation& player) // player info packet ***
	//{
	//	return packet << player.name << player.position[0] << player.position[1] << player.lookAngle;
	//}
	//
	//friend sf::Packet& operator >>(sf::Packet& packet, playerInformation& player)
	//{
	//	return packet >> player.name >> player.position[0] >> player.position[1] >> player.lookAngle;
	//}

	
};

static sf::Packet& operator <<(sf::Packet& packet, const peerNWinfo& peerInfo) // peer info packet ***
{
	return packet << peerInfo.ipAddress << peerInfo.listenerPort;
}

static sf::Packet& operator >>(sf::Packet& packet, peerNWinfo& peerInfo)
{
	return packet >> peerInfo.ipAddress >> peerInfo.listenerPort;
}


static sf::Packet& operator <<(sf::Packet& packet, const header& header_) // peer info packet ***
{
	return packet << header_.game_elapsed_time << header_.information_type << header_.information_amount;
}

static sf::Packet& operator >>(sf::Packet& packet, header& header_)
{
	return packet >> header_.game_elapsed_time >> header_.information_type >> header_.information_amount;
}