#pragma once
#include "SFML/Graphics.hpp"


//more compact function to create and return a rectangle
static sf::RectangleShape constructRectangle(int midPosX, int midPosY, int sizeX, int sizeY, sf::Color fillColour = sf::Color::White, float outlineThickness = 0.0f, sf::Color outlineColour = sf::Color::Black) {
	sf::RectangleShape returnRect = sf::RectangleShape(sf::Vector2f(sizeX, sizeY));
	returnRect.setFillColor(fillColour);
	returnRect.setOutlineColor(outlineColour);
	returnRect.setOutlineThickness(outlineThickness);
	returnRect.setPosition(sf::Vector2f(midPosX - sizeX/2, midPosY - sizeY / 2));

	return returnRect;
}

//more compact function to create and return a Sphere
static sf::CircleShape constructSphere(int midPosX, int midPosY, float radius, sf::Color fillColour = sf::Color::White, float outlineThickness = 0.0f, sf::Color outlineColour = sf::Color::Black) {
	sf::CircleShape returnCirc = sf::CircleShape(radius);
	returnCirc.setFillColor(fillColour);
	returnCirc.setOutlineColor(outlineColour);
	returnCirc.setOutlineThickness(outlineThickness);
	returnCirc.setPosition(sf::Vector2f(midPosX - radius, midPosY - radius));

	return returnCirc;
}

//more compact function to create and return Text
static sf::Text constructText(int midPosX, int midPosY, int characterSize,std::string text, sf::Font font, sf::Color fillColour = sf::Color::Black, float outlineThickness = 0.0f, sf::Color outlineColour = sf::Color::White) {
	
	sf::Text returnText = sf::Text(text,font,characterSize);

	returnText.setFillColor(fillColour);
	returnText.setOutlineColor(outlineColour);
	returnText.setOutlineThickness(outlineThickness);
	returnText.setPosition(sf::Vector2f(midPosX - returnText.getLocalBounds().width/2, midPosY - returnText.getLocalBounds().height / 2));

	return returnText;
}
