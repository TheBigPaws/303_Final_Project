#include "Chat.h"

//basic game loop oriented functions
void Chat::setup(int midPosX_, int midPosY_, int sizeX_, int sizeY_) {
	font_.loadFromFile("font/arial.ttf");
	chatBG = sf::RectangleShape(sf::Vector2f(sizeX_, sizeY_));
	chatBG.setFillColor(sf::Color::Black);
	chatBG.setPosition(midPosX_ - sizeX_ / 2, midPosY_ - sizeY_ / 2);
	textEntryField.setup(midPosX_, midPosY_ + sizeY_ / 2 - 15, sizeX_, 30, 15, false, sf::Color::Black, sf::Color::White);

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
			if (textEntryField.getString() != "") {
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
			chatMessages.at(i).move(0, -chatMessages.at(i).getLocalBounds().height - 10);
		}
		protot.setPosition(10, chatMessages.back().getPosition().y + 10 + protot.getLocalBounds().height);
	}



	chatMessages.push_back(protot);
}