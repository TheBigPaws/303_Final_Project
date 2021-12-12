#include "MainMenu.h"
#include <string>

void MainMenu::setup(sf::RenderWindow* window_, Input* input_){

	// sets up window, input and font
	Screens_Base::setup(window_, input_);

	IPfield.setup(window->getSize().x / 2, window->getSize().y / 2 - 40, 300, 50, 25);
	PortField.setup(window->getSize().x / 2, window->getSize().y / 2 + 40, 300, 50, 25, true);
	NameField.setup(window->getSize().x / 2, window->getSize().y - 30, 500, 60, 26, false, sf::Color(217, 217, 247));

	gameTitletxt = sf::Text("Tank Capture Game",arialF);
	gameTitletxt.setCharacterSize(30);
	gameTitletxt.setStyle(sf::Text::Bold);
	gameTitletxt.setPosition(window->getSize().x / 2 - gameTitletxt.getLocalBounds().width/2, 50);

	connectPrompt = sf::RectangleShape(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2));
	connectPrompt.setPosition(window->getSize().x / 4, window->getSize().y / 4);
	connectPrompt.setFillColor(sf::Color::White);

	connectPromptIP = gameTitletxt;
	gameTitletxt.setOutlineThickness(5);

	connectPromptIP.setCharacterSize(25);
	connectPromptIP.setStyle(sf::Text::Italic);
	connectPromptIP.setFillColor(sf::Color::Black);
	connectPromptIP.setString("IP:");
	connectPromptIP.setPosition(window->getSize().x / 2 - 150 - connectPromptIP.getLocalBounds().width, window->getSize().y / 2-40);


	connectPromptPort = connectPromptIP;
	connectPromptPort.setString("Port:");
	connectPromptPort.setPosition(window->getSize().x / 2 - 150 - connectPromptPort.getLocalBounds().width, window->getSize().y / 2+40);

	enterNameText = connectPromptPort;
	enterNameText.setString("Enter Name:");
	enterNameText.setPosition(window->getSize().x / 2 - enterNameText.getLocalBounds().width/2, window->getSize().y - 100);


	hostButton.setup(window->getSize().x / 2, window->getSize().y / 4, 200, 50, "Host",30);
	connectButton.setup(window->getSize().x / 2, window->getSize().y / 4 + 100, 200, 50, "Connect",30);
	connectCloseButton.setup(window->getSize().x / 4 * 3, window->getSize().y / 4, 40, 40, "X",20,sf::Color::Red);

}

void MainMenu::render() {
	window->draw(gameTitletxt);
	hostButton.render(window);
	connectButton.render(window);
	if (showConnectPrompt) {
		window->draw(connectPrompt);
		window->draw(connectPromptIP);
		window->draw(connectPromptPort);
		connectCloseButton.render(window);
		IPfield.render(window);
		PortField.render(window);
	}

	window->draw(enterNameText);
	NameField.render(window);
}
void MainMenu::update(float dt) {
	updateTitleColours();
	
	if (showConnectPrompt) { //if the connect prompt is open
		connectCloseButton.update(input);
		IPfield.update(input);
		PortField.update(input);

		if (input->isKeyDown(sf::Keyboard::Enter)){//if user presses enter
			input->setKeyUp(sf::Keyboard::Enter);
			if (IPfield.getString() != "" && PortField.getString() != "") {//try to connect)
				attemptConnect = true;
			}
			else { //usered entered blanks
				IPfield.clearTextField(true);
				PortField.clearTextField(true);
			}
		}

		if (connectCloseButton.isPressed()) {//close connect prompt
			showConnectPrompt = false;
		}
	}else{
		hostButton.update(input);
		connectButton.update(input);
		NameField.update(input);
		if (connectButton.isPressed()) {
			if (NameField.getString() == "ajajajaj") {
				NameField.clearTextField(true);
				enterNameText.setString("Name can't be blank!");
				enterNameText.setFillColor(sf::Color::Red);
			}
			else { showConnectPrompt = true; }
		}
		else if (hostButton.isPressed()) {
			if (NameField.getString() == "") {
				NameField.clearTextField(true);
				enterNameText.setString("Name can't be blank!");
				enterNameText.setFillColor(sf::Color::Red);
			}
			else { goToLobby = true; }
		}
	}

}

void MainMenu::updateTitleColours() {
	if (clrclock.getElapsedTime().asMilliseconds() > 5) {
		for (int i = 0; i < 3; i++) {
			if (titleColours[i] > goalColours[i]) {
				titleColours[i]--;
			}
			else if (titleColours[i] < goalColours[i]) { titleColours[i]++; }
		}
		clrclock.restart();
	}
	//if the color reaches the goal color, determine a new goalcolor
	if (titleColours[0] == goalColours[0] && titleColours[1] == goalColours[1] && titleColours[2] == goalColours[2]) {
		goalColours[0] = 56 + rand() % 200;
		goalColours[1] = 56 + rand() % 200;
		goalColours[2] = 56 + rand() % 200;
		//text.at(0).setFillColor(sf::Color(goalColours[0], goalColours[1], goalColours[2]));

	}
	//highlight of the selected text
	gameTitletxt.setOutlineColor(sf::Color(titleColours[0], titleColours[1], titleColours[2]));
	gameTitletxt.setFillColor(sf::Color(255-titleColours[0], 255-titleColours[1], 255-titleColours[2]));
}