#include "Game.h"

void Game::setup(sf::RenderWindow* window_, Input* input_) {
	window = window_; input = input_;

	//set up gameWorld

	int tileSize = 100;

	gameBackGround.push_back(constructRectangle(0, 0, 20 * tileSize, 20 * tileSize, sf::Color(200, 200, 200, 255)));

	for (int x = -10; x <= 10; x++) {
		gameBackGround.push_back(constructRectangle(x * tileSize, 0, 2, 20 * tileSize, sf::Color::Black));

	}
	for (int y = -10; y <= 10; y++) {
		gameBackGround.push_back(constructRectangle(0, y * tileSize, 20 * tileSize, 2, sf::Color::Black));
	}

	arialF.loadFromFile("font/arial.ttf");
}


void Game::render() {
	for (int i = 0; i < gameBackGround.size(); i++) {
		window->draw(gameBackGround.at(i));
	}
}


void Game::update(float dt) {

}