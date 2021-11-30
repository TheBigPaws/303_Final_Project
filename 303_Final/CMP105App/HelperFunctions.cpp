#include "HelperFunctions.h"

void Button::setup(int midPosX_, int midPosY_, int sizeX_, int sizeY_, sf::String string, sf::Color fillColour_, sf::Color outlineColor) {
	
	//store variables
	midPosX = midPosX_;
	midPosY = midPosY_;
	sizeX = sizeX_;
	sizeY = sizeY_;
	fillColour = fillColour_;

	//create button rects
	Button = constructRectangle(midPosX, midPosY, sizeX - 10, sizeY - 10, fillColour);
	ButtonOutline = constructRectangle(midPosX, midPosY, sizeX, sizeY, outlineColor);

	//set up text on the button
	font_.loadFromFile("font/arial.ttf");
	text = sf::Text(string, font_);
	text.setFillColor(sf::Color(255 - fillColour.r, 255 - fillColour.g, 255 - fillColour.b, fillColour.a));
	text.setCharacterSize(30);
	text.setPosition(midPosX - text.getLocalBounds().width / 2, midPosY - text.getLocalBounds().height);
}

void Button::render(sf::RenderWindow* window_) {
	window_->draw(ButtonOutline);
	window_->draw(Button);
	window_->draw(text);
}

void Button::update(Input* input_) {
	//check if the mouse is hovering over it
	if (input_->getMouseX() < ButtonOutline.getPosition().x + sizeX && input_->getMouseX() > ButtonOutline.getPosition().x && input_->getMouseY() < ButtonOutline.getPosition().y + sizeY && input_->getMouseY() > ButtonOutline.getPosition().y) {
		if (!hovered) {//apply gray filter to indicate it's hovered over
			Button.setFillColor(sf::Color(Button.getFillColor().r / 2, Button.getFillColor().g / 2, Button.getFillColor().b / 2, Button.getFillColor().a));
			hovered = true;
		}
		if (input_->isMouseLDown()) {//button is pressed
			pressed = true;
		}
		else { pressed = false; }//reset pressed var
	}
	else {//reset vars
		if (pressed) {
			pressed = false;
		}
		if (hovered) {
			hovered = false;
			Button.setFillColor(fillColour);
		}
	}
}


// ------------------------------------------------------TEXT FIELD CODE------------------------------

//clear text field
void TextField::clearTextField(bool wasWrong) {
	if (wasWrong) {
		textField.setFillC(sf::Color::Red);
	}
	else {
		textField.setFillC(sf::Color::White);
	}
	shown_string = "";
	textField.setText(shown_string);
}

//basic setup
void TextField::setup(int midPosX_, int midPosY_, int sizeX_, int sizeY_, sf::String string, bool onlyInts, sf::Color fillColour, sf::Color outlineColor) {
	textField.setup(midPosX_, midPosY_, sizeX_, sizeY_, "banana");
	textField.setText(shown_string);
	onlyIntsAllowed = onlyInts;
}

//render
void TextField::render(sf::RenderWindow* window_) {
	textField.render(window_);
}

//update
void TextField::update(Input* input_) {
	textField.update(input_);
	if (input_->isMouseLDown()) {
		if (textField.isPressed()) {//if pressed inside text, select it
			selected = true;
			textField.setFillC(sf::Color::Green);
		}
		else { selected = false; textField.setFillC(sf::Color::White); }//if clicked anywhere else,unselect
	}

	if (selected) {
		//keyPressed = false;
		for (int i = 0; i <= 50; i++) {//look if the user pressed any numbers
			if (input_->isKeyDown(i)) {
				input_->setKeyUp(i);
				//keyPressed = true;
				char charToAdd;
				if (i <= 25 && !onlyIntsAllowed) {
					charToAdd = 65 + i; // add int
					shown_string += charToAdd;

				}
				else if (i <= 35 && i > 25) {
					charToAdd = 22 + i; // ints
					shown_string += charToAdd;

				}
				else if (i == 50 && !onlyIntsAllowed) {// DOT
					charToAdd = 46;
					shown_string += charToAdd;

				}
				textField.setText(shown_string);
				textField.resetPos();
				break;
			}
		}
		if (input_->isKeyDown(sf::Keyboard::BackSpace)) { //cleear text field of selected text
			input_->setKeyUp(sf::Keyboard::BackSpace);
			clearTextField(false);

		}
	}
}