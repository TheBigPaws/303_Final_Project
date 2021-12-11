#pragma once
#include "HelperFunctions.h"
#include "Framework/Input.h"
class Player
{
public:
	std::string name = "aa";
	sf::Font arial_F;
	sf::Text name_t;
	sf::CircleShape body;
	sf::RectangleShape cannon, healthMax, healthNow;
	sf::Color playerColour;

	sf::RenderWindow* window;
	Input* input;

	bool isCapturing = false;
	bool arrivedAtRecPos = false;
	float captureTime = 3.0f;
	float playerSpeed = 500.0f;
	float health = 5.0f;

	sf::Vector2f currentPos, receivedPos, receivedPos2, currentLookVector,receivedLookVector,predictedDirection;

	Player() {};
	Player(sf::RenderWindow* window_);
	void setupInput(Input* input_);
	void render() {
		window->draw(body);
		window->draw(cannon);
		//window->draw(name_t);
		if (health != 5) {
			window->draw(healthMax);
			window->draw(healthNow);
		}
	}
	void updatePositions();
	void update(float dt);
	void handleInput(float dt);
	void interpolate(float dt);
};

