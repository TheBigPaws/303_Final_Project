#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/AudioManager.h"
#include <string>
#include <iostream>
#include "P2P_Network.h"

#include "MainMenu.h"
#include "Lobby.h"

enum GameState{MAIN_MENU,LOBBY,GAME};

class Level{
public:
	Level(sf::RenderWindow* hwnd, Input* in);
	~Level();

	void handleInput(float dt);
	void update(float dt);
	void render();

private:
	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	MainMenu mainMenu;
	Lobby lobby;

	P2P_Network networkModule;
	sf::Packet test_pkt_;
	header test_pkt_hdr_;

	// Default variables for level class.
	sf::RenderWindow* window;
	Input* input;
	AudioManager* audio;
	
	GameState gameState = MAIN_MENU;

	sf::View view1;
};