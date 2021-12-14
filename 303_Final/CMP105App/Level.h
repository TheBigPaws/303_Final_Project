#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/AudioManager.h"
#include <string>
#include <iostream>
#include "P2P_Network.h"

#include "MainMenu.h"
#include "Lobby.h"
#include "Game.h"

enum GameState{MAIN_MENU,LOBBY,GAME};

class Level{
public:
	Level(sf::RenderWindow* hwnd, Input* in);
	~Level();

	void handleInput(float dt);
	void update(float dt);
	void render();

	void decodePacket(sf::Packet packet);

	void decodeImportantGameEvs();
private:
	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	float nwShareTimer = 0.1f;

	MainMenu mainMenu;
	Lobby lobby;
	Game game;

	P2P_Network networkModule;

	// Default variables for level class.
	sf::RenderWindow* window;
	Input* input;
	AudioManager* audio;
	
	GameState gameState = MAIN_MENU;
};