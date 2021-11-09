#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string.h>
#include <vector>

struct playerInformation {

	std::string name;
	float position[2];
	float lookAngle;
};
enum Data_Header { PLAYER_POSITION, PLAYER_ANIMATION_STAGE, PARTICLE_POSITION, PARTICLE_ROTATION, MESSAGE_END, ntn };

class CustomPackets
{

	//CUSTOM STRUCT STREAM OVERLOAD
	friend  sf::Packet& operator <<(sf::Packet& packet, const playerInformation& player)
	{
		return packet << player.name << player.position[0] << player.position[1] << player.lookAngle;
	}

	friend sf::Packet& operator >>(sf::Packet& packet, playerInformation& player)
	{
		return packet >> player.name >> player.position[0] >> player.position[1] >> player.lookAngle;
	}
};

