#pragma once
#include "Screens_Base.h"
#include "Chat.h"
#include "UI_tools.h"
#include <iostream>

class Lobby : public Screens_Base
{
private:
	graphicPeer examplePeer;

	std::vector<graphicPeer> peersInLob;
	std::vector<graphicPeerConnectLine> peersConnectLines;
	
	std::vector<sf::RectangleShape> rectangles;
	
	std::vector<sf::Text> playerReadinessText;
	
	sf::Text gameTimer;
	sf::Text examplePeerText;
	sf::Text GraphicVisualisationText;
	
	bool allReady = false;
public:
	float countDownTimer = 10.0f;
	Button readyButton;
	Chat chat;
	bool startGame = false;

	//basic game loop functions
	void setup(sf::RenderWindow* window_, Input* input_);
	void render();
	void update(float dt);

	//add/remove peers
	void addPeer(std::string name, std::string IP_, std::string listPort);
	void disconnectPlayer(std::string name);

	//set peer's ready status
	void setReady(std::string name,bool ready);

	void checkReadiness();
	int displayedPeerNr() { return peersInLob.size() - 1; }

};