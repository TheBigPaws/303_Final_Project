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
	float currentPos[2], receivedPos[2];
	float rotateAngle;
	void setup(sf::RenderWindow* window_, Input* input_);
	void render() {
		cannon.setRotation(rotateAngle);
		window->draw(body);
		window->draw(cannon);
	}
	void update(float dt);
	void handleInput(float dt);
	void interpolate();
	sf::CircleShape body;
	sf::RectangleShape cannon;
};

class Game
{
private:
	sf::RenderWindow* window;
	sf::View view;
	Input* input;
	sf::Font arialF;
	std::vector<sf::RectangleShape> gameBackGround;
	std::vector<sf::Text> gameTexts;

	Player player;
	std::vector<Player> enemies;
public:
	void handleGameInput(float dt);
	void setup(sf::RenderWindow* window_, Input* input_);
	void render();
	void update(float dt);
	void updateEnemyVals(std::string name, float posX, float posY, float rotateAngle);
	void addEnemy(std::string name, float posX, float posY);
};



class Bullet
{

};