#pragma once
#include "UI_tools.h"
#include <chrono>
#include <ctime>

class Chat {
private:
	sf::Font font_;
	TextField textEntryField;
	sf::RectangleShape chatBG;
	std::vector<sf::Text> chatMessages;
	int bottomChatMsgIndex = 0;
public:
	bool sentSomething = false;
	std::string sent_string;

	//basic game loop oriented functions
	void setup(int midPosX_, int midPosY_, int sizeX_, int sizeY_);
	void render(sf::RenderWindow* window_);
	void update(Input* input_);

	//chat specific functions
	void addMessage(std::string message, sf::Color msgColor = sf::Color::White, std::string senderName = "Me");
};