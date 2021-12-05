#include "Game.h"

void Game::updateEnemyVals(std::string name, float posX, float posY, float rotateAngle) {
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies.at(i).name == name) {
			enemies.at(i).receivedPos[0] = posX;
			enemies.at(i).receivedPos[1] = posY;
			enemies.at(i).rotateAngle = rotateAngle;
			break;
		}
	}
}
void Game::addEnemy(std::string name, float posX, float posY) {
	Player protot;
	protot.name = name;
	protot.currentPos[0] = posX;
	protot.currentPos[1] = posY;
	enemies.push_back(protot);
}

void Game::setup(sf::RenderWindow* window_, Input* input_) {
	window = window_; input = input_;

	player.setup(window, input);

	//set up gameWorld

	view.reset(sf::FloatRect(0, 0, window->getSize().x, window->getSize().y));

	int tileSize = 100;

	gameBackGround.push_back(constructRectangle(0, 0, 20 * tileSize, 20 * tileSize, sf::Color(200, 200, 200, 255)));

	for (int x = -10; x <= 10; x++) {
		gameBackGround.push_back(constructRectangle(x * tileSize, 0, 2, 20 * tileSize, sf::Color::Black));

	}
	for (int y = -10; y <= 10; y++) {
		gameBackGround.push_back(constructRectangle(0, y * tileSize, 20 * tileSize, 2, sf::Color::Black));
	}

	arialF.loadFromFile("font/arial.ttf");

	gameTexts.push_back(sf::Text("ajo", arialF));
	gameTexts.back().setPosition(0, 0);
	player.currentPos[0] = 0.0f;
	player.currentPos[1] = 0.0f;

}

void Game::render() {
	for (int i = 0; i < gameBackGround.size(); i++) {
		window->draw(gameBackGround.at(i));
	}
	for (int i = 0; i < gameTexts.size(); i++) {
		window->draw(gameTexts.at(i));
	}
	for (int i = 0; i < enemies.size(); i++) {
		enemies.at(i).render();
	}
	player.render();
}


void Game::update(float dt) {
	
	handleGameInput(dt);
	player.update(dt);
	gameTexts.at(0).setString(std::to_string(player.rotateAngle));

	view.setCenter(player.currentPos[0],player.currentPos[1]);
	window->setView(view);
}

void Game::handleGameInput(float dt) {
	if (input->isKeyDown(sf::Keyboard::D)) {
		player.currentPos[0] += dt * player.playerSpeed;
		if (player.currentPos[0] > 1000) {
			player.currentPos[0] = 1000;
		}
	}
	if (input->isKeyDown(sf::Keyboard::A)) {
		player.currentPos[0] -= dt * player.playerSpeed;
		if (player.currentPos[0] < -1000) {
			player.currentPos[0] = -1000;
		}
	}
	if (input->isKeyDown(sf::Keyboard::W)) {
		player.currentPos[1] -= dt * player.playerSpeed;
		if (player.currentPos[1] < -1000) {
			player.currentPos[1] = -1000;
		}
	}
	if (input->isKeyDown(sf::Keyboard::S)) {
		player.currentPos[1] += dt * player.playerSpeed;
		if (player.currentPos[1] > 1000) {
			player.currentPos[1] = 1000;
		}
	}

}

void Player::setup(sf::RenderWindow* window_, Input* input_) {
	input = input_;
	window = window_;

	body = sf::CircleShape(30);
	body.setFillColor(sf::Color::Blue);
	body.setOutlineThickness(5);
	body.setOutlineColor(sf::Color::White);

	cannon = sf::RectangleShape(sf::Vector2f(80, 20));
	cannon.setFillColor(sf::Color::Black);
	cannon.setOutlineThickness(3);
	cannon.setOutlineColor(sf::Color::White);
}

void Player::update(float dt) {

	body.setPosition(currentPos[0] - body.getRadius(), currentPos[1] - body.getRadius());
	cannon.setPosition(currentPos[0], currentPos[1] - cannon.getSize().y / 2);

	sf::Vector2f playerToCursor = sf::Vector2f(input->getMouseX() - (int)window->getSize().x/2, input->getMouseY() - (int)window->getSize().y / 2);
	
	float magnitude = sqrt(playerToCursor.x * playerToCursor.x + playerToCursor.y * playerToCursor.y);
	rotateAngle = acos(playerToCursor.x / magnitude) * 57.32;

	if (playerToCursor.y < 0) {
		rotateAngle *= -1;
	}


}
