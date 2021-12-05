#pragma once
#include "HelperFunctions.h"
#include "Framework/Input.h"


class Game
{
private:
	sf::RenderWindow* window;
	Input* input;
	sf::Font arialF;
	std::vector<sf::RectangleShape> gameBackGround;
public:
	void setup(sf::RenderWindow* window_, Input* input_);
	void render();
	void update(float dt);
};

