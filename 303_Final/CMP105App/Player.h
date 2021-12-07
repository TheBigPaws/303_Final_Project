#pragma once
#include "HelperFunctions.h"
#include "Framework/Input.h"
class Player
{
public:
	std::string name;
	sf::RenderWindow* window;
	Input* input;
	float playerSpeed = 500.0f;
	float health = 5.0f;
	sf::Vector2f currentPos, receivedPos, lookVector;
	float rotateAngle;
	sf::Color playerColour;
	void setup(sf::RenderWindow* window_, Input* input_);
	void render() {
		cannon.setRotation(rotateAngle);
		window->draw(body);
		window->draw(cannon);
		if (health != 5) {
			window->draw(healthMax);
			window->draw(healthNow);
		}
	}
	void updatePositions();
	void update(float dt);
	void interpolate();
	sf::CircleShape body;
	sf::RectangleShape cannon, healthMax, healthNow;
};

