#include "UI_tools.h"


void Button::setup(int midPosX_, int midPosY_, int sizeX_, int sizeY_, sf::String string, int fontSize, sf::Color fillColour, sf::Color outlineColour) {

	//store variables
	midPosX = midPosX_;
	midPosY = midPosY_;
	sizeX = sizeX_;
	sizeY = sizeY_;
	fillColour_ = fillColour;
	outlineColour_ = outlineColour;
	//create button rects
	Button = constructRectangle(midPosX, midPosY, sizeX - 10, sizeY - 10, fillColour);
	ButtonOutline = constructRectangle(midPosX, midPosY, sizeX, sizeY, outlineColour);

	//set up text on the button
	font_.loadFromFile("font/arial.ttf");
	text = sf::Text(string, font_);
	text.setFillColor(sf::Color(255 - fillColour.r, 255 - fillColour.g, 255 - fillColour.b, fillColour.a));
	text.setCharacterSize(fontSize);
	text.setPosition(midPosX - text.getLocalBounds().width / 2, midPosY - text.getLocalBounds().height);
}

void Button::setColors(sf::Color fillColour, sf::Color outlineColour, bool reset) {
	if (reset) {
		Button.setFillColor(fillColour_);
		ButtonOutline.setFillColor(outlineColour_);
		return;
	}
	fillColour_ = fillColour;
	outlineColour_ = outlineColour;
	Button.setFillColor(fillColour);
	ButtonOutline.setFillColor(outlineColour);
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
			Button.setFillColor(sf::Color(fillColour_.r / 2, fillColour_.g / 2, fillColour_.b / 2, fillColour_.a));
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
			Button.setFillColor(fillColour_);
		}
	}
}


// ------------------------------------------------------TEXT FIELD CODE------------------------------

//clear text field
void TextField::clearTextField(bool wasWrong) {
	if (wasWrong) {
		textField.setColors(sf::Color::Red, textField.outlineColour_);
	}
	else {
		textField.setColors(sf::Color::White, sf::Color::White, true);
	}
	shown_string = "";
	textField.setText(shown_string);
}

//basic setup
void TextField::setup(int midPosX_, int midPosY_, int sizeX_, int sizeY_, int fontSize, bool onlyInts, sf::Color fillColour, sf::Color outlineColor) {
	textField.setup(midPosX_, midPosY_, sizeX_, sizeY_, "", fontSize, fillColour, outlineColor);
	textField.setText(shown_string);
	onlyIntsAllowed = onlyInts;
}

//render
void TextField::render(sf::RenderWindow* window_) {
	if (selected) {
		shown_string += '_';
		textField.setText(shown_string);
	}
	textField.render(window_);
	if (selected) {
		shown_string.pop_back();
		textField.setText(shown_string);
	}
}

//update
void TextField::update(Input* input_) {
	textField.update(input_);
	if (input_->isMouseLDown()) {
		if (textField.isPressed()) {//if pressed inside text, select it
			if (!selected) {
				selected = true;
			}
		}
		else {
			if (selected) {
				selected = false;
			}
		}
	}

	if (selected) {
		//keyPressed = false;
		for (int i = 0; i <= 50; i++) {//look if the user pressed any numbers
			if (input_->isKeyDown(i)) {
				input_->setKeyUp(i);
				//keyPressed = true;
				char charToAdd;
				if (i <= 25 && !onlyIntsAllowed) {
					charToAdd = 65 + i; // add chars
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
				textField.textResetPos();
				break;
			}
		}
		if (input_->isKeyDown(sf::Keyboard::BackSpace)) { //cleear text field of selected text
			input_->setKeyUp(sf::Keyboard::BackSpace);
			clearTextField(false);

		}
	}
}
