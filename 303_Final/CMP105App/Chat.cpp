#include "Chat.h"


void Chat::setup(sf::RenderWindow* window_, Input* input_) {
	Screens_Base::setup(window_, input_);
}

//basic game loop oriented functions
void Chat::setPosSize(sf::Vector2f position, sf::Vector2f size) {
	chatBG = sf::RectangleShape(size);
	chatBG.setFillColor(sf::Color::Black);
	chatBG.setPosition(position - size / 2.0f);
	textEntryField.setup(position + sf::Vector2f(0, size.y / 2 - 15), sf::Vector2f(size.x, 30), 15, false);

}

void Chat::render() {
	window->draw(chatBG);
	textEntryField.render(window);
	for (int i = bottomChatMsgIndex; i < chatMessages.size(); i++) {
		//only render messages that fit in the chat box
		if (chatMessages.at(i).getPosition().y > chatBG.getPosition().y && chatMessages.at(i).getPosition().y < chatBG.getPosition().y + chatBG.getSize().y - 30) { //chat is outside the console show range
			window->draw(chatMessages.at(i)); 
		}
	}

}

void Chat::update() {
	textEntryField.update(input);

	if (textEntryField.isSelected()) {
		if (input->isKeyDown(sf::Keyboard::Enter)) {
			if (textEntryField.getString() != "") {
				input->setKeyUp(sf::Keyboard::Enter);
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
	sf::Text protot = sf::Text(textMsg, arialF, 15);
	protot.setFillColor(msgColor);

	//push a new message
	if (chatMessages.size() == 0) {
		protot.setPosition(10, chatBG.getPosition().y + chatBG.getSize().y - 50 - protot.getLocalBounds().height);
	}
	else { //if there are some, move them all up first

		for (int i = 0; i < chatMessages.size(); i++) {
			chatMessages.at(i).move(0, -chatMessages.at(i).getLocalBounds().height - 10);
		}
		protot.setPosition(10, chatMessages.back().getPosition().y + 10 + protot.getLocalBounds().height);
	}



	chatMessages.push_back(protot);
}