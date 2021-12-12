#pragma once

#include "Framework/GameObject.h"

class Screens_Base
{
protected:
	Input* input;
	sf::Font arialF;
	sf::RenderWindow* window;
public:

	virtual void setup(sf::RenderWindow* window_, Input* input_);
};

