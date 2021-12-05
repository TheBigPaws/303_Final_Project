#include "HelperFunctions.h"

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

void Button::setColors(sf::Color fillColour, sf::Color outlineColour,bool reset) { 
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
		textField.setColors(sf::Color::Red,textField.outlineColour_);
	}
	else {
		textField.setColors(sf::Color::White,sf::Color::White,true);
	}
	shown_string = "";
	textField.setText(shown_string);
}

//basic setup
void TextField::setup(int midPosX_, int midPosY_, int sizeX_, int sizeY_, int fontSize, bool onlyInts, sf::Color fillColour, sf::Color outlineColor) {
	textField.setup(midPosX_, midPosY_, sizeX_, sizeY_, "",fontSize,fillColour,outlineColor);
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

// ------------------------------------------------------Chat CODE------------------------------


//basic game loop oriented functions
void Chat::setup(int midPosX_, int midPosY_, int sizeX_, int sizeY_) {
	font_.loadFromFile("font/arial.ttf");
	chatBG = sf::RectangleShape(sf::Vector2f(sizeX_, sizeY_));
	chatBG.setFillColor(sf::Color::Black);
	chatBG.setPosition(midPosX_ - sizeX_ / 2, midPosY_ - sizeY_ / 2);
	textEntryField.setup(midPosX_, midPosY_ + sizeY_/2 - 15, sizeX_, 30,15, false, sf::Color::Black, sf::Color::White);

}

void Chat::render(sf::RenderWindow* window_) {
	window_->draw(chatBG);
	textEntryField.render(window_);
	for (int i = bottomChatMsgIndex; i < chatMessages.size(); i++) {
		if (chatMessages.at(i).getPosition().y > chatBG.getPosition().y && chatMessages.at(i).getPosition().y < chatBG.getPosition().y + chatBG.getSize().y - 30) { //chat is outside the console show range
		window_->draw(chatMessages.at(i));
		}
	}

}

void Chat::update(Input* input_) {
	textEntryField.update(input_);

	if (textEntryField.isSelected()) {
		if (input_->isKeyDown(sf::Keyboard::Enter)) {
			if(textEntryField.getString() != ""){
				input_->setKeyUp(sf::Keyboard::Enter);
				sent_string = textEntryField.getString();
				addMessage(sent_string);
				sentSomething = true;
				textEntryField.clearTextField(false);
			}
		}
	}

}

void Chat::addMessage(std::string message, sf::Color msgColor, std::string senderName) {

	//get a local time stamp
	auto timenow =
		std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	char temp_str[26];
	ctime_s(temp_str, sizeof temp_str, &timenow);
	// ^ date gets copied too, but we only want time for output [11] - 19
	std::string str = "[";
	for (int i = 0; i < 5; i++) {
		str += temp_str[i + 11];
	}
	str += "] " + senderName + ": ";

	//set the string with the time stamp
	std::string textMsg = str + message;
	sf::Text protot = sf::Text(textMsg, font_, 15);
	protot.setFillColor(msgColor);

	//
	if (chatMessages.size() == 0) {
		protot.setPosition(10, chatBG.getPosition().y + chatBG.getSize().y - 50 - protot.getLocalBounds().height);
	}
	else {

		for (int i = 0; i < chatMessages.size(); i++) {
			chatMessages.at(i).move(0,-chatMessages.at(i).getLocalBounds().height - 10);
		}
		protot.setPosition(10, chatMessages.back().getPosition().y + 10 + protot.getLocalBounds().height);
	}



	chatMessages.push_back(protot);
}