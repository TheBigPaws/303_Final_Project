#pragma once
#include "SFML/Graphics.hpp"


//more compact function to create and return a rectangle
static sf::RectangleShape constructRectangle(sf::Vector2f position, sf::Vector2f size, sf::Color fillColour = sf::Color::White, float outlineThickness = 0.0f, sf::Color outlineColour = sf::Color::Black) {
	sf::RectangleShape returnRect = sf::RectangleShape(size);
	returnRect.setFillColor(fillColour);
	returnRect.setOutlineColor(outlineColour);
	returnRect.setOutlineThickness(outlineThickness);
	returnRect.setPosition(position - size/2.0f);

	return returnRect;
}

//more compact function to create and return a Sphere
static sf::CircleShape constructSphere(sf::Vector2f position, float radius, sf::Color fillColour = sf::Color::White, float outlineThickness = 0.0f, sf::Color outlineColour = sf::Color::Black) {
	sf::CircleShape returnCirc = sf::CircleShape(radius);
	returnCirc.setFillColor(fillColour);
	returnCirc.setOutlineColor(outlineColour);
	returnCirc.setOutlineThickness(outlineThickness);
	returnCirc.setPosition(position - sf::Vector2f(radius,radius));

	return returnCirc;
}

//more compact function to create and return Text
static sf::Text constructText(sf::Vector2f position, int characterSize,std::string text, sf::Font font, sf::Color fillColour = sf::Color::Black, float outlineThickness = 0.0f, sf::Color outlineColour = sf::Color::White) {
	
	sf::Text returnText = sf::Text(text,font,characterSize);

	returnText.setFillColor(fillColour);
	returnText.setOutlineColor(outlineColour);
	returnText.setOutlineThickness(outlineThickness);
	returnText.setPosition(sf::Vector2f(position.x - returnText.getLocalBounds().width/2, position.y - returnText.getLocalBounds().height / 2));

	return returnText;
}
