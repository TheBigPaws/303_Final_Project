#pragma once
#include "HelperFunctions.h"
class Bullet
{
public:
	int id;
	sf::Vector2f position, direction;
	sf::CircleShape bullet;
	Bullet(sf::Vector2f position_, sf::Vector2f direction_);
	void update(float dt);
};