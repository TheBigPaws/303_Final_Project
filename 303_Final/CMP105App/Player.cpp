#include "Player.h"

void Player::setup(sf::RenderWindow* window_, Input* input_) {
	input = input_;
	window = window_;

	playerColour = sf::Color(rand() % 256, rand() % 256, rand() % 256);

	body = sf::CircleShape(30);
	body.setFillColor(playerColour);
	body.setOutlineThickness(5);
	body.setOutlineColor(sf::Color::White);
	
	cannon = sf::RectangleShape(sf::Vector2f(80, 20));
	cannon.setFillColor(sf::Color::Black);
	cannon.setOutlineThickness(3);
	cannon.setOutlineColor(sf::Color::White);

	healthNow = sf::RectangleShape(sf::Vector2f(50, 10));
	healthNow.setFillColor(sf::Color::Green);

	healthMax = sf::RectangleShape(sf::Vector2f(50, 10));
	healthMax.setFillColor(sf::Color::Red);

}

void Player::updatePositions() {
	body.setPosition(currentPos.x - body.getRadius(), currentPos.y - body.getRadius());
	cannon.setPosition(currentPos.x, currentPos.y - cannon.getSize().y / 2);
	healthNow.setPosition(currentPos.x - 25, currentPos.y + 30);
	healthMax.setPosition(currentPos.x - 25, currentPos.y + 30);
}

void Player::update(float dt) {

	updatePositions();

	sf::Vector2f playerToCursor = sf::Vector2f(input->getMouseX() - (int)window->getSize().x / 2, input->getMouseY() - (int)window->getSize().y / 2);

	float magnitude = sqrt(playerToCursor.x * playerToCursor.x + playerToCursor.y * playerToCursor.y);
	rotateAngle = acos(playerToCursor.x / magnitude) * 57.32;

	if (playerToCursor.y < 0) {
		rotateAngle *= -1;
	}

	//normalised look vector
	lookVector = playerToCursor / magnitude;
	if (health < 5) {
		health += dt / 2;
		healthNow.setSize(sf::Vector2f(health * 10.0f, 10));
	}
	if (health > 5) {
		health = 5.0f;
	}

}