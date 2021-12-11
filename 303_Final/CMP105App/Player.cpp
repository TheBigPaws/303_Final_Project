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
	if (health < 5) {
		health += dt / 2;
		healthNow.setSize(sf::Vector2f(health * 10.0f, 10));
	}
	if (health > 5) {
		health = 5.0f;
	}

	float rotateAngle = acos(currentLookVector.x) * 57.32;

	if (currentLookVector.y < 0) {
		rotateAngle *= -1;
	}
	//rotate the cannon
	cannon.setRotation(rotateAngle);

}

void Player::handleInput(float dt) {
	
	sf::Vector2f playerToCursor = sf::Vector2f(input->getMouseX() - (int)window->getSize().x / 2, input->getMouseY() - (int)window->getSize().y / 2);

	float magnitude = sqrt(playerToCursor.x * playerToCursor.x + playerToCursor.y * playerToCursor.y);

	//normalised look vector
	currentLookVector = playerToCursor / magnitude;

	updatePositions();

	if (input->isKeyDown(sf::Keyboard::Space)) { //capture
		input->setKeyUp(sf::Keyboard::Space);

		captureTime = 3.0f;
		isCapturing = !isCapturing;
	}

}

void Player::interpolate(float dt) {
	//if (abs(currentLookVector.x - receivedLookVector.x) > 0.05) {
	//	if (currentLookVector.x < receivedLookVector.x) {
	//		currentLookVector.x += dt * 2;
	//	}
	//	else {
	//		currentLookVector.x -= dt * 2;
	//	}
	//
	//}
	//
	//if (abs(currentLookVector.y - receivedLookVector.y) > 0.05) {
	//	if (currentLookVector.y < receivedLookVector.y) {
	//		currentLookVector.y += dt * 2;
	//	}
	//	else {
	//		currentLookVector.y -= dt * 2;
	//	}
	//}
	
	//if (!arrivedAtRecPos) {
	//
		
	//	if (dist_magnitude < 0.2f) {
	//		body.setFillColor(sf::Color::Red);		//arrived
	//		arrivedAtRecPos = true;
	//		playerSpeed = 300.0f;
	//	}
	//
	//}
	//else {
	//
	//}

	

	sf::Vector2f dir = receivedPos - currentPos;
	float dir_magnitude = sqrt(dir.x * dir.x + dir.y * dir.y);
	//dir = dir / di

	if (receivedPos == receivedPos2) {//if arrived to standing in place
		predictedDirection = sf::Vector2f(0, 0);
		if (dir_magnitude > 0.4f) {
			currentPos += dir * dt;
		}
		else {
			currentPos = receivedPos;

		}
	}
	else {
		currentPos += predictedDirection * playerSpeed * dt;
	}

	updatePositions();

}
