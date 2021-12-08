#include "Player.h"

Player::Player(sf::RenderWindow* window_) {
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

	arial_F.loadFromFile("font/arial.ttf");
	name_t = sf::Text("aaa", arial_F,20);
	name_t.setFillColor(sf::Color::Black);
	name_t.setOutlineThickness(1);
	name_t.setOutlineColor(sf::Color::White);
}

void Player::setupInput(Input* input_) {
	input = input_;
	

}

void Player::updatePositions() {
	body.setPosition(currentPos.x - body.getRadius(), currentPos.y - body.getRadius());
	cannon.setPosition(currentPos.x, currentPos.y - cannon.getSize().y / 2);
	healthNow.setPosition(currentPos.x - 25, currentPos.y + 30);
	healthMax.setPosition(currentPos.x - 25, currentPos.y + 30);
	name_t.setPosition(currentPos.x, currentPos.y);
}

void Player::update(float dt) {
	
	if (isCapturing) {
		captureTime -= dt;
	}
	//right now just health calculations here
	if (health < 5) {
		health += dt / 2;
		healthNow.setSize(sf::Vector2f(health * 10.0f, 10));
	}
	if (health > 5) {
		health = 5.0f;
	}

}

void Player::handleInput(float dt) {
	sf::Vector2f playerToCursor = sf::Vector2f(input->getMouseX() - (int)window->getSize().x / 2, input->getMouseY() - (int)window->getSize().y / 2);

	float magnitude = sqrt(playerToCursor.x * playerToCursor.x + playerToCursor.y * playerToCursor.y);
	rotateAngle = acos(playerToCursor.x / magnitude) * 57.32;

	if (playerToCursor.y < 0) {
		rotateAngle *= -1;
	}

	//normalised look vector
	lookVector = playerToCursor / magnitude;

	updatePositions();

	if (input->isKeyDown(sf::Keyboard::Space)) { //capture
		input->setKeyUp(sf::Keyboard::Space);

		captureTime = 3.0f;
		isCapturing = !isCapturing;
	}

}
