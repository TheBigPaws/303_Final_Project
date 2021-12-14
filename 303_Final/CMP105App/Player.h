#pragma once
#include "HelperFunctions.h"
#include "Framework/Input.h"
#include <iostream>
class Player
{
public:
	//game window and input pointers
	sf::RenderWindow* window;
	Input* input;

	//graphic of the player
	sf::Font arial_F;
	sf::Text name_t;
	sf::CircleShape body;
	sf::RectangleShape cannon, healthMax, healthNow;
	sf::Color playerColour;

	//player variables
	std::string name = "";
	bool isCapturing = false;
	bool arrivedAtRecPos = false;
	bool predicting = true;
	float captureTime = 3.0f;
	float playerSpeed = 500.0f;
	float health = 5.0f;
	sf::Vector2f currentPos, receivedPos, receivedPos2, currentLookVector,receivedLookVector,moveDirection;


	//constructors
	Player() {};
	Player(sf::RenderWindow* window_, sf::Font* font_);
	
	//basic game loop functions
	void render();
	void update(float dt);
	void updatePositions();

	//input handle for local machine user
	void setupInput(Input* input_);
	void handleInput(float dt);

	//network update functions
	void predictNextReceive();
	void interpolate(float dt);
	void updateReceivedValues(sf::Vector2f position, sf::Vector2f lookDirection);
};

