#include "UI_tools.h"

// ------------------------------------------------------BUTTON CODE------------------------------


void Button::setup(sf::Vector2f position_, sf::Vector2f size_, sf::String string, int fontSize, sf::Color fillColour_, sf::Color outlineColour_) {

	font_.loadFromFile("font/arial.ttf");
	//store variables
	position = position_;
	size = size_;
	fillColour = fillColour_;
	outlineColour = outlineColour_;
	//create button rects
	Button = constructRectangle(position_, size_,fillColour,5.0f,outlineColour);

	//set up text on the button
	text = sf::Text(string, font_);
	setTextVal(&text, position_, fontSize, sf::Color(255 - fillColour.r, 255 - fillColour.g, 255 - fillColour.b, fillColour.a));

}

void Button::setColors(sf::Color fillColour_, bool reset) {
	if (reset) {
		Button.setFillColor(fillColour_);
		return;
	}
	fillColour = fillColour_;
	Button.setFillColor(fillColour);
}


void Button::render(sf::RenderWindow* window_) {
	window_->draw(Button);
	window_->draw(text);
}

void Button::update(Input* input_) {
	//check if the mouse is hovering over it
	if (input_->getMouseX() < Button.getPosition().x + size.x && input_->getMouseX() > Button.getPosition().x && input_->getMouseY() < Button.getPosition().y + size.y && input_->getMouseY() > Button.getPosition().y) {
		//apply gray filter to indicate it's hovered over
		if (!hovered) {
			Button.setFillColor(sf::Color(fillColour.r / 2, fillColour.g / 2, fillColour.b / 2, fillColour.a));
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
		textField.setColors(sf::Color::Red);
	}
	else {
		textField.setColors(textField.fillColour, true);
	}
	shown_string = "";
	textField.setText(shown_string);
}

//basic setup
void TextField::setup(sf::Vector2f position_, sf::Vector2f size_, int fontSize, bool onlyInts, sf::Color fillColour, sf::Color outlineColor) {
	textField.setup(position_,size_, shown_string, fontSize, fillColour, outlineColor);
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
		
		for (int i = 0; i <= 50; i++) {//look if the user pressed any numbers
			if (input_->isKeyDown(i)) {
				input_->setKeyUp(i);
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
				textField.setText(shown_string,sf::Color::White);
				textField.textResetPos();
				break;
			}
		}
		if (input_->isKeyDown(sf::Keyboard::Space)) { //spaces
			input_->setKeyUp(sf::Keyboard::Space);
			shown_string += " ";
			textField.setText(shown_string, sf::Color::White);
			textField.textResetPos();
		}
		if (input_->isKeyDown(sf::Keyboard::BackSpace)) { //cleear text field of selected text
			input_->setKeyUp(sf::Keyboard::BackSpace);
			clearTextField(false);

		}
	}
}
