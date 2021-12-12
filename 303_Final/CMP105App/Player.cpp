#include "Player.h"

Player::Player(sf::RenderWindow* window_) {
	window = window_;
	playerColour = sf::Color(rand() % 256, rand() % 256, rand() % 256);


	body = constructSphere(sf::Vector2f(0, 0), 30, playerColour, 5, sf::Color::White);

	cannon = constructRectangle(sf::Vector2f(0, 0), sf::Vector2f(80, 20), sf::Color::Black, 3, sf::Color::White);


	healthNow = constructRectangle(sf::Vector2f(0, 0), sf::Vector2f(50, 10), sf::Color::Green);

	healthMax = constructRectangle(sf::Vector2f(0, 0), sf::Vector2f(50, 10), sf::Color::Red);

	//not working rn
	arial_F.loadFromFile("font/arial.ttf");
	name_t = sf::Text("aaa", arial_F,20);
	name_t.setFillColor(sf::Color::Black);
	name_t.setOutlineThickness(1);
	name_t.setOutlineColor(sf::Color::White);
	srand(time(NULL));

	currentPos.x = -999.5f + (float)(rand() % 2000);
	currentPos.y = -999.5f + (float)(rand() % 2000);
	receivedPos = currentPos;
}

//in a separate function from constructior so we don't link enemies to my input - unsafe, unnecessary
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
	
	//update player variables
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
	
	//create normalised look vector and save it
	sf::Vector2f playerToCursor = sf::Vector2f(input->getMouseX() - (int)window->getSize().x / 2, input->getMouseY() - (int)window->getSize().y / 2);
	float magnitude = sqrt(playerToCursor.x * playerToCursor.x + playerToCursor.y * playerToCursor.y);
	currentLookVector = playerToCursor / magnitude;

	updatePositions();

	if (input->isKeyDown(sf::Keyboard::Space)) { //capture
		input->setKeyUp(sf::Keyboard::Space);

		captureTime = 3.0f;
		isCapturing = !isCapturing;
	}

}

void Player::interpolate(float dt) {

	

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
