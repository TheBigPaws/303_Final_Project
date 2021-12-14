#pragma once

#include "Framework/GameObject.h"

//base class for game screens - handles linking window and input, as well as loading a font
class Screens_Base
{
protected:
	Input* input;
	sf::Font arialF;
	sf::RenderWindow* window;
public:

	virtual void setup(sf::RenderWindow* window_, Input* input_);
};

