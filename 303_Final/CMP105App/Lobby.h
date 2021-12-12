#pragma once
#include "Screens_Base.h"
#include "Chat.h"
#include "UI_tools.h"


class Lobby : public Screens_Base
{
private:
	std::vector<graphicPeer> peersInLob;
	std::vector<graphicPeerConnectLine> peersConnectLines;
	std::vector<sf::RectangleShape> rectangles;
	std::vector<sf::Text> texts;
	sf::Text gameTimer;
	float countDownTimer = 30.0f;
	bool buttonPressed = false;
public:
	Chat chat;
	bool startGame = false;

	void setup(sf::RenderWindow* window_, Input* input_);
	void render();
	void update(float dt);

	void addPeer(std::string name, std::string IP_, std::string listPort);
	int displayedPeerNr() { return peersInLob.size() - 1; }

	void disconnectPlayer(std::string name);

};