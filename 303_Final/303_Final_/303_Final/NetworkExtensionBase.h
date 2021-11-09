#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string.h>
#include <vector>

enum Data_Header {PLAYER_POSITION, PLAYER_ANIMATION_STAGE, PARTICLE_POSITION, PARTICLE_ROTATION, MESSAGE_END, ntn};

class NetworkExtensionBase
{
protected:
	sf::IpAddress ipAdress;

	sf::UdpSocket udpSocket;
	unsigned short UDPport;

	sf::Packet outgoingPacket;
	sf::Packet incomingPacket;

public:
	//converts a string into char[30], filled with '-' for empty slots
	//void convertToData(std::string message);
	
	void bind_UDP(int port);

	void send_UDP_(sf::IpAddress recepient_IP, unsigned short sender_port); //sends the constructed message and clears the packet
	void receive_UDP_(sf::IpAddress sender_IP);


	sf::IpAddress get_ipAdress(){ return ipAdress; }

	//CUSTOM STRUCTS FOR THE PACKET---------------------------------------------------

	struct playerInformation {

		std::string name;
		float position[2];
		float lookAngle;
	};

	//CUSTOM STRUCT STREAM OVERLOAD
	friend  sf::Packet& operator <<(sf::Packet& packet, const playerInformation& player)
	{
		return packet << player.name << player.position[0] << player.position[1] << player.lookAngle;
	}

	friend sf::Packet& operator >>(sf::Packet& packet, playerInformation& player)
	{
		return packet >> player.name >> player.position[0] >> player.position[1] >> player.lookAngle;
	}

	void addToPacket(float float_);		   //overloads for constructing a packet
	void addToPacket(sf::Uint16 integer);  //overloads for constructing a packet
	void addToPacket(std::string string_); //overloads for constructing a packet
	void addToPacket(Data_Header header);  //overloads for constructing a packet
	//void addToPacket( playerInformation PlayerInformation);  //overloads for constructing a packet
	
	void decodePacket();//decodes packet and updates variables	
};

