#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string.h>
#include <vector>
#include <queue>

struct playerInformation {

	std::string name;
	float position[2];
	float lookAngle;
};

struct peerNWinfo {
	sf::Uint32 ipAddress;
	sf::Uint16 listenerPort;
};

struct Peer {
	//tcp socket belinging to this peer
	sf::TcpSocket socket;

	//variables because SFML needs consts for some functions, 
	sf::IpAddress IpAddress = sf::IpAddress::None;
	unsigned short localPort = 0;
	unsigned short remotePort = 0;
	unsigned short TCP_listener_Port = 0;

	//queues of packets (will be treated as tasks)
	//std::queue<sf::Packet> receivedPackets;
	//std::queue<sf::Packet> outgoingPackets;
};

enum Data_Header { ntn, PLAYER_POSITION, PLAYER_ANIMATION_STAGE, PARTICLE_POSITION, PARTICLE_ROTATION, MESSAGE_END, NW_INFO,  };

class PacketBase
{
public:
	//CUSTOM STRUCT STREAM OVERLOAD
	
	
};

static sf::Packet& operator <<(sf::Packet& packet, const playerInformation& player) // player info packet ***
{
	return packet << player.name << player.position[0] << player.position[1] << player.lookAngle;
}
static sf::Packet& operator >>(sf::Packet& packet, playerInformation& player)
{
	return packet >> player.name >> player.position[0] >> player.position[1] >> player.lookAngle;
}



//packet structure
//// [header] [type] [packet] [type] [packet] [type]
static sf::Packet& operator >>(sf::Packet& packet, peerNWinfo& peerInfo)
{
	return packet >> peerInfo.ipAddress >> peerInfo.listenerPort;
}
static sf::Packet& operator <<(sf::Packet& packet, const peerNWinfo& peerInfo) // peer info packet ***
{
	return packet << peerInfo.ipAddress << peerInfo.listenerPort;
}