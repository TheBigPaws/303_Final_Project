#pragma once

#include "Framework/GameObject.h"

class Screens_Base
{
protected:
	Input* input;
	sf::Font arialF;
public:
	sf::RenderWindow* window;

	void setup(sf::RenderWindow* window_, Input* input_);
};

