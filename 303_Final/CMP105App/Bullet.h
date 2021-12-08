#pragma once
#include "HelperFunctions.h"
class Bullet
{
public:
	sf::Uint16 id;
	sf::Vector2f position, direction;
	sf::CircleShape bullet;
	Bullet(sf::Vector2f position_, sf::Vector2f direction_);
	Bullet(sf::Vector2f position_, sf::Vector2f direction_,sf::Uint16 id_);
	void update(float dt);
};