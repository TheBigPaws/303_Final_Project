#pragma once
#include "Player.h"
#include "Bullet.h"
#include "Screens_Base.h"
#include <queue>
#include <iostream>

struct playerPosLookDir {
	sf::Vector2f position;
	sf::Vector2f lookDir;
};

//struct holding information about important game events - they will be sent straight away, rather than waiting 50 ms
enum eventType { AREA_CAPTURED_, BULLET_SHOT_,PLAYER_HIT_ };
struct eventInfo {
	std::string str;
	int type;
	float f1;
	sf::Vector2f v1;
	sf::Vector2f v2;
	int a;
};

class Game : public Screens_Base
{
private:

	bool playerAlive = true;

	sf::Clock gameClock;
	sf::View view;

	//graphics
	sf::RectangleShape gameField[20][20];
	sf::RectangleShape gameOverTint;
	std::vector<sf::Text> gameTexts;

	//bullets
	std::vector <Bullet> myBullets;
	std::vector <Bullet> enemyBullets;

	//players
	Player player;
	std::vector<Player> enemies;

	//event pipeline
	std::queue<eventInfo> importantEvents;
public:

	//basic game loop functions
	void setup(sf::RenderWindow* window_, Input* input_);
	void render();
	void update(float dt);

	//game update functions
	void handleGameInput(float dt);
	void updateBullets(float dt);
	void updateEnemyVals(std::string name, playerPosLookDir playerVals);

	//object creation functions
	void addEnemy(std::string name);
	void addEnemyBullet(Bullet bullet) {
		enemyBullets.push_back(bullet);
	}

	//event functions
	void captureTile(int x, int y, std::string capturer_name);
	void enemyGotHit(std::string hit_player, int bullet_id);
	void disconnectPlayer(std::string name);



	//getters, setters
	playerPosLookDir getplayerPosLookDir(){
		playerPosLookDir ret;
		ret.lookDir = player.currentLookVector;
		ret.position = player.currentPos;
		return ret;
	}

	void setMyName(std::string name) {
		player.name = name;
	}

	eventInfo getImportantEv() {
		eventInfo ev = importantEvents.front();
		importantEvents.pop();
		return ev;
	}

	bool areImportantEv() {
		if (importantEvents.size() > 0) {
			return true;
		}
		return false;
	}

	sf::Clock* getClock() {
		return &gameClock;
	}
};
