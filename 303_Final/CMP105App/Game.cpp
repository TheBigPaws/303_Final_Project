#include "Game.h"

void Game::updateEnemyVals(std::string name, playerPosLookDir playerVals) {
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies.at(i).name == name) {
			enemies.at(i).receivedPos2 = enemies.at(i).receivedPos;
			enemies.at(i).receivedPos = playerVals.position;
			enemies.at(i).currentLookVector = playerVals.lookDir;
			enemies.at(i).arrivedAtRecPos = false;
			enemies.at(i).body.setFillColor(enemies.at(i).playerColour);
			
			enemies.at(i).predictedDirection = enemies.at(i).receivedPos - enemies.at(i).currentPos;
			
			float dir_magnitude = sqrt(enemies.at(i).predictedDirection.x * enemies.at(i).predictedDirection.x + enemies.at(i).predictedDirection.y * enemies.at(i).predictedDirection.y);
			
			enemies.at(i).predictedDirection = enemies.at(i).predictedDirection / dir_magnitude;

			//if (dir_magnitude < 0.2f) {
			//	enemies.at(i).arrivedAtRecPos = true;
			//
			//}
			//
			//if (enemies.at(i).receivedPos == enemies.at(i).receivedPos2 && enemies.at(i).arrivedAtRecPos) {
			//	enemies.at(i).predictedDirection = sf::Vector2f(0, 0);
			//}

			break;
		}
	}
}

void Game::addEnemy(std::string name) {
	Player protot = Player(window);
	protot.name = name;
	protot.name_t.setString(name);
	protot.currentPos.x = 0;
	protot.currentPos.y = 0;
	enemies.push_back(protot);
}

void Game::setup(sf::RenderWindow* window_, Input* input_) {
	window = window_; input = input_;
	player = Player(window);
	player.setupInput(input);
	//set up gameWorld

	view.reset(sf::FloatRect(0, 0, window->getSize().x, window->getSize().y));

	int tileSize = 100;

	for (int x = -10; x < 10; x++) {
		for (int y = -10; y < 10; y++) {
			sf::RectangleShape protot = sf::RectangleShape(sf::Vector2f(tileSize, tileSize));
			protot.setFillColor(sf::Color(200, 200, 200, 255));
			protot.setPosition(x * tileSize, y * tileSize);
			gameField[y + 10][x + 10] = protot;
		}
	}


	for (int x = -10; x <= 10; x++) {
		gameShapes.push_back(constructRectangle(x * tileSize, 0, 2, 20 * tileSize, sf::Color::Black));

	}
	for (int y = -10; y <= 10; y++) {
		gameShapes.push_back(constructRectangle(0, y * tileSize, 20 * tileSize, 2, sf::Color::Black));
	}

	arialF.loadFromFile("font/arial.ttf");

	gameTexts.push_back(sf::Text("ajo", arialF));
	gameTexts.push_back(sf::Text("You Died!", arialF,50));
	gameTexts.back().setStyle(sf::Text::Bold);
	gameTexts.back().setFillColor(sf::Color::Red);
	//gameTexts.back().setPosition(0, 0);
	player.currentPos.x = -999.5f + rand() % 2000;
	player.currentPos.y = -999.5f + rand() % 2000;

	enemyBullets.push_back(Bullet(sf::Vector2f(100, 100), sf::Vector2f(0, 0)));

	gameOverTint = sf::RectangleShape(sf::Vector2f(window->getSize().x, window->getSize().y));
	gameOverTint.setFillColor(sf::Color(255,0,0,100));
}

void Game::render() {

	for (int x = 0; x < 20; x++) {
		for (int y = 0; y < 20; y++) {
			window->draw(gameField[x][y]);
		}
	}
	for (int i = 0; i < gameShapes.size(); i++) {
		window->draw(gameShapes.at(i));
	}
	//for (int i = 1; i < gameTexts.size(); i++) {
	//	window->draw(gameTexts.at(i));
	//}


	for (int i = 0; i < myBullets.size(); i++) {
		window->draw(myBullets.at(i).bullet);
	}

	for (int i = 0; i < enemyBullets.size(); i++) {
		window->draw(enemyBullets.at(i).bullet);
	}

	for (int i = 0; i < enemies.size(); i++) {
		enemies.at(i).render();
	}

	if (player.isCapturing) {
		gameTexts.at(0).setString(std::to_string(player.captureTime));
		gameTexts.at(0).setPosition(player.currentPos.x, player.currentPos.y);
		window->draw(gameTexts.at(0));
	}

	if (playerAlive) {
		player.render();
	}
	else {
		window->draw(gameOverTint);
		window->draw(gameTexts.at(1));
	}
}


void Game::update(float dt) {
	
	if (playerAlive) {
		handleGameInput(dt);
		player.handleInput(dt);
		player.update(dt);
		view.setCenter(player.currentPos.x, player.currentPos.y);
		window->setView(view);
	}
	
	
	for (int i = 0; i < enemies.size();i++) {
		enemies.at(i).update(dt);
		enemies.at(i).interpolate(dt);
	}
	updateBullets(dt);
}

void Game::handleGameInput(float dt) {
	if (input->isMouseLDown()) { //shoot anytime
		input->setMouseLDown(false);
		myBullets.push_back(Bullet(player.currentPos, player.currentLookVector));
		std::cout << "In game, i (" << player.name << ") shot a bullet with id " << myBullets.back().id << std::endl;
		eventInfo shotInfo;
		shotInfo.type = BULLET_SHOT_;
		shotInfo.v1 = player.currentPos;
		shotInfo.v2 = player.currentLookVector;
		shotInfo.a = (int)myBullets.back().id;
		importantEvents.push(shotInfo);
	}
	if (!player.isCapturing) { //only move when not capturing
		if (input->isKeyDown(sf::Keyboard::D)) { //move left
			player.currentPos.x += dt * player.playerSpeed;
			if (player.currentPos.x > 999) {
				player.currentPos.x = 999;
			}
		}
		if (input->isKeyDown(sf::Keyboard::A)) {//move right
			player.currentPos.x -= dt * player.playerSpeed;
			if (player.currentPos.x < -999) {
				player.currentPos.x = -999;
			}
		}
		if (input->isKeyDown(sf::Keyboard::W)) {//move up
			player.currentPos.y -= dt * player.playerSpeed;
			if (player.currentPos.y < -999) {
				player.currentPos.y = -999;
			}
		}
		if (input->isKeyDown(sf::Keyboard::S)) {//move down
			player.currentPos.y += dt * player.playerSpeed;
			if (player.currentPos.y > 999) {
				player.currentPos.y = 999;
			}
		}

	}
	else {
		if (player.captureTime <= 0) {
			int idxOfTileOn[2];
			idxOfTileOn[0] = player.currentPos.x / 100 + 10;
			idxOfTileOn[1] = player.currentPos.y / 100 + 10;

			gameField[idxOfTileOn[1]][idxOfTileOn[0]].setFillColor(player.playerColour);
			player.isCapturing = false;

			eventInfo captureInfo;
			captureInfo.type = AREA_CAPTURED_;
			captureInfo.v1 = sf::Vector2f(idxOfTileOn[0],idxOfTileOn[1]);
			importantEvents.push(captureInfo);
		}
	}
	

}



void Game::updateBullets(float dt) {
	for (int i = 0; i < enemyBullets.size(); i++) {
		enemyBullets.at(i).update(dt);

		sf::Vector2f playerToBullet = enemyBullets.at(i).position - player.currentPos;
		if (abs(playerToBullet.x) < player.body.getRadius() && abs(playerToBullet.y) < player.body.getRadius()) { //player was hit
			player.health -= 1.0f;

			//create a game event information about bullet hitting
			eventInfo playerHitInfo;
			playerHitInfo.type = PLAYER_HIT_;
			playerHitInfo.a = enemyBullets.at(i).id;
			playerHitInfo.str = player.name;

			std::cout << playerHitInfo.str << " was hit by bullet with id " << playerHitInfo.a;

			//send info about the event and delete the bullet
			importantEvents.push(playerHitInfo);
			enemyBullets.erase(enemyBullets.begin() + i);

			if (player.health <= 0.0f) {
				playerAlive = false;
				gameTexts.at(1).setPosition(player.currentPos.x - gameTexts.at(1).getLocalBounds().width / 2, player.currentPos.y - gameTexts.at(1).getLocalBounds().height / 2);
				gameOverTint.setPosition(player.currentPos.x - window->getSize().x / 2, player.currentPos.y - window->getSize().y / 2);

			}
			break;
		}

		//also can deal with bullets flying out of arena
		if (enemyBullets.at(i).position.x < -1000 || enemyBullets.at(i).position.y < -1000 || enemyBullets.at(i).position.x > 1000 || enemyBullets.at(i).position.y > 1000) {
			enemyBullets.erase(enemyBullets.begin() + i);
		}
	}
	for (int i = 0; i < myBullets.size(); i++) {
		myBullets.at(i).update(dt);
		//deal with bullets flying out of arena
		if (myBullets.at(i).position.x < -1000 || myBullets.at(i).position.y < -1000 || myBullets.at(i).position.x > 1000 || myBullets.at(i).position.y > 1000) {
			myBullets.erase(myBullets.begin() + i);
		}
	}
}

void Game::captureTile(int x, int y, std::string capturer_name) {
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies.at(i).name == capturer_name) {
			gameField[y][x].setFillColor(enemies.at(i).playerColour);
			return;
		}
	}
}

void Game::enemyGotHit(std::string hit_player, int bullet_id) {
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies.at(i).name == hit_player) {
			enemies.at(i).health -= 1.0f;
			if (enemies.at(i).health <= 0) {
				enemies.erase(enemies.begin() + i);
			}
			break;
		}
	}

	//have to iterate through both my and enemy bullets, as it could be either of those
	for (int i = 0; i < enemyBullets.size(); i++) {
		if (enemyBullets.at(i).id == bullet_id) {
			enemyBullets.erase(enemyBullets.begin() + i);
			return;
		}
	}

	for (int i = 0; i < myBullets.size(); i++) {
		if (myBullets.at(i).id == bullet_id) {
			myBullets.erase(myBullets.begin() + i);
			return;
		}
	}
}

void Game::disconnectPlayer(std::string name) {
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies.at(i).name == name) {
			enemies.erase(enemies.begin() + i);
			break;
		}
	}
}
