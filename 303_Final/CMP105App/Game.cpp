#include "Game.h"

//updates the given enemy's received values
void Game::updateEnemyVals(std::string name, playerPosLookDir playerVals) {
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies.at(i).name == name) {
			//update values
			enemies.at(i).updateReceivedValues(playerVals.position, playerVals.lookDir);
			break;
		}
	}
}

//add a nw enemy to the game
void Game::addEnemy(std::string name) {
	Player protot = Player(window,&arialF);
	protot.name = name;
	protot.name_t.setString(name);
	enemies.push_back(protot);
}

void Game::setup(sf::RenderWindow* window_, Input* input_) {
	
	// sets up window, input and font
	Screens_Base::setup(window_, input_);

	//set up player
	player = Player(window,&arialF);
	player.setupInput(input);
	view.reset(sf::FloatRect(0, 0, window->getSize().x, window->getSize().y));

	//create game world
	int tileSize = 100;
	for (int x = -10; x < 10; x++) { //create tiles
		for (int y = -10; y < 10; y++) {
			sf::RectangleShape protot = sf::RectangleShape(sf::Vector2f(tileSize, tileSize));
			protot.setFillColor(sf::Color(200, 200, 200, 255));
			protot.setOutlineColor(sf::Color::Black);
			protot.setOutlineThickness(2.0f);
			protot.setPosition(x * tileSize, y * tileSize);
			gameField[y + 10][x + 10] = protot;
		}
	}


	//set up other texts + game over screen tint 
	gameTexts.push_back(sf::Text("", arialF)); //player capture time
	gameTexts.push_back(sf::Text("You Died!", arialF,50));//death text
	gameTexts.back().setStyle(sf::Text::Bold);
	gameTexts.back().setFillColor(sf::Color::Red);
	gameOverTint = sf::RectangleShape(sf::Vector2f(window->getSize().x, window->getSize().y));
	gameOverTint.setFillColor(sf::Color(255,0,0,100));
}

void Game::render() {

	for (int x = 0; x < 20; x++) {
		for (int y = 0; y < 20; y++) {
			window->draw(gameField[x][y]);
		}
	}

	for (int i = 0; i < myBullets.size(); i++) {
		window->draw(myBullets.at(i).bullet);
	}

	for (int i = 0; i < enemyBullets.size(); i++) {
		window->draw(enemyBullets.at(i).bullet);
	}

	for (int i = 0; i < enemies.size(); i++) {
		enemies.at(i).render();
	}


	if (player.isCapturing) { //if player is capturing also render the capture time
		gameTexts.at(0).setString(std::to_string(player.captureTime));
		gameTexts.at(0).setPosition(player.currentPos.x, player.currentPos.y);
		window->draw(gameTexts.at(0));
	}

	if (playerAlive) { //player lives, show him
		player.render();
	}
	else { //player died, show game over screen
		window->draw(gameOverTint);
		window->draw(gameTexts.at(1));
	}
}


void Game::update(float dt) {
	
	//only update player if he is alive
	if (playerAlive) {
		handleGameInput(dt);
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

	player.handleInput(dt); //handle player's movement and rotation


	if (input->isMouseLDown()) { //shoot (currently as fast as you can click)
		input->setMouseLDown(false);//fix for holding
		
		//shoot my bullet
		myBullets.push_back(Bullet(player.currentPos, player.currentLookVector));
		
		//create an event reference about  it
		eventInfo shotInfo;
		shotInfo.type = BULLET_SHOT_;
		shotInfo.v1 = player.currentPos;
		shotInfo.v2 = player.currentLookVector;
		shotInfo.a = (int)myBullets.back().id;
		importantEvents.push(shotInfo);
	}
	if (player.isCapturing) { //only move when not capturing

		if (player.captureTime <= 0) { //if we finished capturing
			int idxOfTileOn[2];
			idxOfTileOn[0] = player.currentPos.x / 100 + 10;
			idxOfTileOn[1] = player.currentPos.y / 100 + 10;

			gameField[idxOfTileOn[1]][idxOfTileOn[0]].setFillColor(player.playerColour); //paint the tile we are standing on
			player.isCapturing = false;
			std::cout << "captured";


			//create an event reference about  it
			eventInfo captureInfo;
			captureInfo.type = AREA_CAPTURED_;
			captureInfo.v1 = sf::Vector2f(idxOfTileOn[0],idxOfTileOn[1]);
			importantEvents.push(captureInfo);
		}
	}
	

}

//updates all bullets and handles collision - every player ONLY checks collision of all ENEMY bullets with THEMSELVES - most fair, distributed work
void Game::updateBullets(float dt) {

	for (int i = 0; i < enemyBullets.size(); i++) {
		enemyBullets.at(i).update(dt);

		sf::Vector2f playerToBullet = enemyBullets.at(i).position - player.currentPos;
		if (abs(playerToBullet.x) < player.body.getRadius() && abs(playerToBullet.y) < player.body.getRadius()) { //player was hit
			
			//decrement my health
			player.health -= 1.0f;

			//create a game event information about bullet hitting
			eventInfo playerHitInfo;
			playerHitInfo.type = PLAYER_HIT_;
			playerHitInfo.a = enemyBullets.at(i).id;
			playerHitInfo.str = player.name;

			//send info about the event and delete the bullet
			importantEvents.push(playerHitInfo);
			enemyBullets.erase(enemyBullets.begin() + i);

			if (player.health <= 0.0f) { //death condition
				playerAlive = false;
				gameTexts.at(1).setPosition(player.currentPos.x - gameTexts.at(1).getLocalBounds().width / 2, player.currentPos.y - gameTexts.at(1).getLocalBounds().height / 2);
				gameOverTint.setPosition(player.currentPos.x - window->getSize().x / 2, player.currentPos.y - window->getSize().y / 2);

			}
			break; //can jump out of the loop
		}

		//also can deal with enemy bullets flying out of arena
		if (enemyBullets.at(i).position.x < -1000 || enemyBullets.at(i).position.y < -1000 || enemyBullets.at(i).position.x > 1000 || enemyBullets.at(i).position.y > 1000) {
			enemyBullets.erase(enemyBullets.begin() + i);
			break;
		}
	}

	//deal with my bullets flying out of arena
	for (int i = 0; i < myBullets.size(); i++) {
		myBullets.at(i).update(dt);
		if (myBullets.at(i).position.x < -1000 || myBullets.at(i).position.y < -1000 || myBullets.at(i).position.x > 1000 || myBullets.at(i).position.y > 1000) {
			myBullets.erase(myBullets.begin() + i);
		}
	}
}

//set the tile colour to the enemy's colour
void Game::captureTile(int x, int y, std::string capturer_name) {
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies.at(i).name == capturer_name) {
			gameField[y][x].setFillColor(enemies.at(i).playerColour);
			return;
		}
	}
}

//applies hit event
void Game::enemyGotHit(std::string hit_player, int bullet_id) {
	for (int i = 0; i < enemies.size(); i++) { //first we decrement hit enemy's health
		if (enemies.at(i).name == hit_player) {
			enemies.at(i).health -= 1.0f;
			if (enemies.at(i).health <= 0) {
				enemies.erase(enemies.begin() + i);
			}
			break;
		}
	}

	//then erase the bullet
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
