#pragma once
#include "Framework/GameObject.h"

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

//BUTTON CLASS
class Button {
private:
	sf::Font font_;
	sf::Text text;
	sf::RectangleShape Button, ButtonOutline;
	sf::Color fillColour;
	bool pressed = false;
	bool hovered = false;
	int midPosX, midPosY, sizeX, sizeY;
public:
	//basic setters
	void setText(std::string text_) {text.setString(text_);}
	void setFillC(sf::Color color_) { fillColour = color_; Button.setFillColor(fillColour); }
	
	bool isPressed() { return pressed; }
	void resetPos(){text.setPosition(midPosX - text.getLocalBounds().width / 2, midPosY - text.getLocalBounds().height);}

	//basic game loop oriented functions
	void setup(int midPosX_, int midPosY_, int sizeX_, int sizeY_, sf::String string, sf::Color fillColour_ = sf::Color::White, sf::Color outlineColor = sf::Color::Black);
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
	void setFillC(sf::Color color_) { textField.setFillC(color_); }
	std::string getString() { return shown_string; }

	//clears text field
	void clearTextField(bool wasWrong);
	
	//basic game loop oriented functions
	void setup(int midPosX_, int midPosY_, int sizeX_, int sizeY_, sf::String string, bool onlyInts = false, sf::Color fillColour = sf::Color::White, sf::Color outlineColor = sf::Color::Black);
	void render(sf::RenderWindow* window_);
	void update(Input* input_);
};