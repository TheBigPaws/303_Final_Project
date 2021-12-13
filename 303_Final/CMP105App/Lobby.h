#pragma once
#include "Screens_Base.h"
#include "Chat.h"
#include "UI_tools.h"
#include <iostream>

class Lobby : public Screens_Base
{
private:
	std::vector<graphicPeer> peersInLob;
	std::vector<graphicPeerConnectLine> peersConnectLines;
	std::vector<sf::RectangleShape> rectangles;
	std::vector<sf::Text> texts;
	sf::Text gameTimer;
	float countDownTimer = 10.0f;
	bool allReady = false;
public:
	Button readyButton;
	Chat chat;
	bool startGame = false;

	void setup(sf::RenderWindow* window_, Input* input_);
	void render();
	void update(float dt);

	void addPeer(std::string name, std::string IP_, std::string listPort);
	int displayedPeerNr() { return peersInLob.size() - 1; }

	void disconnectPlayer(std::string name);
	void setReady(std::string name,bool ready);

	void checkReadiness();

};