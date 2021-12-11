#pragma once
#include "Framework/GameObject.h"
#include <chrono>
#include <ctime>

//class containing helper functions
class HelperFunctions
{

};

//more compact function to create and return a rectangle
static sf::RectangleShape constructRectangle(int midPosX, int midPosY, int sizeX,int sizeY,sf::Color fillColour = sf::Color::White) {
	sf::RectangleShape returnRect;
	returnRect.setFillColor(fillColour);
	returnRect.setSize(sf::Vector2f(sizeX, sizeY));
	returnRect.setPosition(sf::Vector2f(midPosX - sizeX/2, midPosY - sizeY / 2));

	return returnRect;
}

//static void normalizeSFV2(sf::Vector2f & vector) {
//
//	float magnitude = sqrt(vector.x * vector->x + vector->y * vector->y);
//
//	sf::Vector2f a = vector;
//
//	//normalised look vector
//	vector = a / magnitude;
//}


//BUTTON CLASS
class Button {
private:
	sf::Font font_;
	sf::Text text;
	sf::RectangleShape Button, ButtonOutline;
	bool pressed = false;
	bool hovered = false;
public:
	int midPosX, midPosY, sizeX, sizeY;
	sf::Color fillColour_, outlineColour_;
	//basic setters
	void setText(std::string text_) { text.setString(text_); text.setFillColor(sf::Color(255-fillColour_.r, 255 - fillColour_.g, 255 - fillColour_.b,255)); }
	void setColors(sf::Color fillColour, sf::Color outlineColour,bool reset = false);
	void textResetPos(){text.setPosition(midPosX - text.getLocalBounds().width / 2, midPosY - text.getLocalBounds().height);}
	
	//basic getters
	bool isPressed() { return pressed; }
	bool isHovered() { return hovered; }

	//basic game loop oriented functions
	void setup(int midPosX_, int midPosY_, int sizeX_, int sizeY_, sf::String string, int fontSize, sf::Color fillColour = sf::Color::White, sf::Color outlineColour = sf::Color::Black);
	void render(sf::RenderWindow* window_);
	void update(Input* input_);

};

class TextField {
private:
	bool onlyIntsAllowed = false;
	bool selected = false;
	std::string shown_string = "";
	Button textField;
	bool keyPressed = false;
public:
	//basic get & set funcs
	std::string getString() { return shown_string; }
	bool isSelected() { return selected; }

	//clears text field
	void clearTextField(bool wasWrong);
	
	//basic game loop oriented functions
	void setup(int midPosX_, int midPosY_, int sizeX_, int sizeY_, int fontSize, bool onlyInts = false, sf::Color fillColour = sf::Color::White, sf::Color outlineColor = sf::Color::Black);
	void render(sf::RenderWindow* window_);
	void update(Input* input_);
};

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
	void addMessage(std::string message, sf::Color msgColor = sf::Color::White,std::string senderName = "Me");
};