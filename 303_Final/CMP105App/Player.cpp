#include "Player.h"

//main constructor, applies a random colour for the player, as well as a position.
//player colours are local - they are not matched between machines
Player::Player(sf::RenderWindow* window_,sf::Font * font_) {
	srand(time(NULL));
	window = window_;
	playerColour = sf::Color(rand() % 256, rand() % 256, rand() % 256);

	//set up body
	body = constructSphere(sf::Vector2f(0, 0), 30, playerColour, 5, sf::Color::White);
	cannon = constructRectangle(sf::Vector2f(0, 0), sf::Vector2f(80, 20), sf::Color::Black, 3, sf::Color::White);
	healthNow = constructRectangle(sf::Vector2f(0, 0), sf::Vector2f(50, 10), sf::Color::Green);
	healthMax = constructRectangle(sf::Vector2f(0, 0), sf::Vector2f(50, 10), sf::Color::Red);
	
	//set up name
	name_t = sf::Text("", *font_,20);
	name_t.setFillColor(sf::Color::Black);
	name_t.setOutlineThickness(3);
	name_t.setOutlineColor(sf::Color::White);

	currentPos.x = 5000;
	currentPos.y = 5000;
	
	receivedPos = currentPos;
}

//in a separate function from constructior so we don't link enemies to my input - unsafe, unnecessary
void Player::setupInput(Input* input_) {
	input = input_;
	//random starting position
	currentPos.x = -999.5f + (float)(rand() % 2000);
	currentPos.y = -999.5f + (float)(rand() % 2000);
}

void Player::updatePositions() {

	//fix for out of game movements (real or predicted).
	// if the enemy goes over, we set his predicted direction to stay there - fix for always going out and back due to loss of position packets
	if (currentPos.y < -999) {
		currentPos.y = -999;
		moveDirection = sf::Vector2f(0, 0);
	}
	if (currentPos.y > 999) {
		currentPos.y = 999;
		moveDirection = sf::Vector2f(0, 0);

	}
	if (currentPos.x < -999) {
		currentPos.x = -999;
		moveDirection = sf::Vector2f(0, 0);

	}
	if (currentPos.x > 999) {
		currentPos.x = 999;
		moveDirection = sf::Vector2f(0, 0);

	}

	//update body positions
	body.setPosition(currentPos.x - body.getRadius(), currentPos.y - body.getRadius());
	cannon.setPosition(currentPos.x, currentPos.y - cannon.getSize().y / 2);
	healthNow.setPosition(currentPos.x - 25, currentPos.y + 30);
	healthMax.setPosition(currentPos.x - 25, currentPos.y + 30);
	name_t.setPosition(currentPos.x - name_t.getGlobalBounds().width/2, currentPos.y- body.getRadius() - 20);
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

	//update cannon rotation angle
	float rotateAngle = acos(currentLookVector.x) * 57.32;

	if (currentLookVector.y < 0) {
		rotateAngle *= -1;
	}
	//rotate the cannon
	cannon.setRotation(rotateAngle);

}

//render Player object and its parts
void Player::render() {
	{
		window->draw(body);
		window->draw(cannon);
		window->draw(name_t);
		if (health != 5) {
			window->draw(healthMax);
			window->draw(healthNow);
		}
	}
}

void Player::handleInput(float dt) {
	
	//WASD MOVEMENT
	if (!isCapturing) {
		if (input->isKeyDown(sf::Keyboard::D)) { //move left
			currentPos.x += dt * playerSpeed;
		}
		if (input->isKeyDown(sf::Keyboard::A)) {//move right
			currentPos.x -= dt * playerSpeed;
		}
		if (input->isKeyDown(sf::Keyboard::W)) {//move up
			currentPos.y -= dt * playerSpeed;
		}
		if (input->isKeyDown(sf::Keyboard::S)) {//move down
			currentPos.y += dt * playerSpeed;
		}
	}
	//create normalised look vector and save it
	sf::Vector2f playerToCursor = sf::Vector2f(input->getMouseX() - (int)window->getSize().x / 2, input->getMouseY() - (int)window->getSize().y / 2);
	float magnitude = sqrt(playerToCursor.x * playerToCursor.x + playerToCursor.y * playerToCursor.y);
	currentLookVector = playerToCursor / magnitude;

	updatePositions();

	//Begin / End capturing (works for both)
	if (input->isKeyDown(sf::Keyboard::Space)) { //capture
		input->setKeyUp(sf::Keyboard::Space);

		captureTime = 3.0f;
		isCapturing = !isCapturing;
	}

}

void Player::interpolate(float dt) {

	
	//dir by itself is difference between simulated Pos and receivedPos
	sf::Vector2f dir = receivedPos - currentPos;
	float dir_magnitude = sqrt(dir.x * dir.x + dir.y * dir.y);

	//if something is weirdly far (some bug or first position information), just set it there
	if (receivedPos == receivedPos2) {
		if (dir_magnitude > 1000) {
			currentPos = receivedPos;
		}
	}
	

	if (dir_magnitude < 5.0f) { //if we've arrived at the received position
		
		if (predicting) { //if we've yet to predict
			predictNextReceive(); //do prediction
		}
		else { //player is standing / arrived to a predicted position after nw issues
			currentPos = receivedPos;
		}
	}
	else { //if we're not there yet, keep moving
		currentPos += moveDirection * (playerSpeed + 50.0f) * dt; //interpolation
	}

	updatePositions();

}

void Player::updateReceivedValues(sf::Vector2f position, sf::Vector2f lookDirection) {
	//push down the chain of received positions
	receivedPos2 = receivedPos;
	receivedPos = position;
	currentLookVector = lookDirection;

	//no need to predict when we are standing in place / just left
	sf::Vector2f RP1toRP2 = receivedPos - receivedPos2;
	float recPos1toRecPos2distance = sqrt(RP1toRP2.x * RP1toRP2.x + RP1toRP2.y * RP1toRP2.y);
	if (recPos1toRecPos2distance > 200) { 
		predicting = true;
	}

	//set the movement direction based on the receive
	moveDirection = receivedPos - currentPos;
	float dir_magnitude = sqrt(moveDirection.x * moveDirection.x + moveDirection.y * moveDirection.y);
	moveDirection = moveDirection / dir_magnitude;

}

//predict that the next received position is going to be a bit further in the same direction
void Player::predictNextReceive() {
	receivedPos = currentPos += moveDirection * 100.0f;
	receivedPos2 = sf::Vector2f(0, 0);
	predicting = false;
}