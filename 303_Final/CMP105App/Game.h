#pragma once
#include "Player.h"
#include "Bullet.h"
#include <queue>
#include <iostream>
struct playerPosAngle {
	float rotateAngle;
	sf::Vector2f position;
};
enum eventType { AREA_CAPTURED_, BULLET_SHOT_,PLAYER_HIT_ };
struct eventInfo {
	std::string str;
	int type;
	float f1;
	sf::Vector2f v1;
	sf::Vector2f v2;
	int a;
};

class Game
{
private:

	bool playerDead = false;
	float playerDeadTime = 10.0f;

	sf::RenderWindow* window;
	sf::View view;
	Input* input;
	sf::Font arialF;

	std::vector<sf::RectangleShape> gameShapes;
	sf::RectangleShape gameField[20][20];
	std::vector<sf::Text> gameTexts;

	std::vector <Bullet> myBullets;
	std::vector <Bullet> enemyBullets;

	Player player;
	std::vector<Player> enemies;

	std::queue<eventInfo> importantEvents;
public:
	void setMyName(std::string name) {
		player.name = name;
	}
	void setup(sf::RenderWindow* window_, Input* input_);
	void render();
	void update(float dt);

	void handleGameInput(float dt);
	void updateBullets(float dt);

	void updateEnemyVals(std::string name, playerPosAngle playerVals);
	void addEnemy(std::string name);
	void addEnemyBullet(Bullet bullet) {
		enemyBullets.push_back(bullet);
		std::cout << "an enemy bullet with id " << enemyBullets.back().id << " was just shot.\n";
	}
	playerPosAngle getPlayerPosAngle(){
		playerPosAngle ret;
		ret.rotateAngle = player.rotateAngle;
		ret.position = player.currentPos;
		return ret;
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

	void captureTile(int x, int y, std::string capturer_name);

	void enemyGotHit(std::string hit_player, int bullet_id);
};
