#include "Screens_Base.h"

void Screens_Base::setup(sf::RenderWindow* window_, Input* input_) {
	window = window_;
	input = input_;
	arialF.loadFromFile("font/arial.ttf");

}