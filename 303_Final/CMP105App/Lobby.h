#pragma once
#include "HelperFunctions.h"
#include "Framework/Input.h"

//struct holding display  info of peer
struct graphicPeer {
	sf::Text listPortT, remoteIPT;
	sf::CircleShape circle_, circleOutline_;
	int positionX, positionY;
};

class Lobby :public GameObject
{
private:
	sf::RenderWindow* window;
	Input* input;
	sf::Font arialF;

	std::vector<graphicPeer> peersInLob;
	std::vector<sf::RectangleShape> rectangles;
	std::vector<sf::Text> texts;

public:
	void setup(sf::RenderWindow* window_, Input* input_);
	void addPeer(std::string IP_, std::string listPort);
	void render();
	void update(float dt);
};