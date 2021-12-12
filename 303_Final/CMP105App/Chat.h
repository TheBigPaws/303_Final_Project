#pragma once
#include "UI_tools.h"
#include "Screens_Base.h"
#include <chrono>
#include <ctime>

class Chat : public Screens_Base {
private:
	TextField textEntryField;
	sf::RectangleShape chatBG;
	std::vector<sf::Text> chatMessages;
	int bottomChatMsgIndex = 0;
public:
	bool sentSomething = false;
	std::string sent_string;

	//basic game loop oriented functions
	void setPosSize(sf::Vector2f position, sf::Vector2f size);
	void setup(sf::RenderWindow* window_, Input* input_);
	void render();
	void update();

	//chat specific functions
	void addMessage(std::string message, sf::Color msgColor = sf::Color::White, std::string senderName = "Me");
};