#include "MainMenu.h"
#include <string>

void MainMenu::setup(sf::RenderWindow* window_, Input* input_){

	window = window_; input = input_;
	
	arialF.loadFromFile("font/arial.ttf");

	gameTitletxt = sf::Text("Tank Capture Game",arialF);
	connectPromptIP = gameTitletxt;
	gameTitletxt.setCharacterSize(30);
	gameTitletxt.setStyle(sf::Text::Bold);
	gameTitletxt.setOutlineThickness(5);
	gameTitletxt.setPosition(window->getSize().x / 2 - gameTitletxt.getLocalBounds().width/2, 50);

	connectPromptIP.setCharacterSize(25);
	connectPromptIP.setStyle(sf::Text::Bold);
	connectPromptIP.setStyle(sf::Text::Italic);
	connectPromptIP.setFillColor(sf::Color::Black);
	connectPromptPort = connectPromptIP;

	connectPromptIP.setString("IP:");
	connectPromptPort.setString("Port:");

	connectPromptIP.setPosition(window->getSize().x / 2 - 150 - connectPromptIP.getLocalBounds().width, window->getSize().y / 2-40);
	connectPromptPort.setPosition(window->getSize().x / 2 - 150 - connectPromptPort.getLocalBounds().width, window->getSize().y / 2+40);

	IPfield.setup(window->getSize().x / 2, window->getSize().y / 2 - 40, 300, 50, "");
	PortField.setup(window->getSize().x / 2, window->getSize().y / 2 + 40, 300, 50, "",true);

	connectPrompt = sf::RectangleShape(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2));
	connectPrompt.setPosition(window->getSize().x / 4, window->getSize().y / 4);
	connectPrompt.setFillColor(sf::Color::White);

	hostButton.setup(window->getSize().x / 2, window->getSize().y / 4, 200, 50, "Host");
	connectButton.setup(window->getSize().x / 2, window->getSize().y / 4 + 100, 200, 50, "Connect");
	connectCloseButton.setup(window->getSize().x / 4 * 3, window->getSize().y / 4, 40, 40, "X",sf::Color::Red);

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
				IPfield.setFillC(sf::Color::Red);
				PortField.setFillC(sf::Color::Red);
			}
		}

		if (connectCloseButton.isPressed()) {//close connect prompt
			showConnectPrompt = false;
		}
	}else{
		hostButton.update(input);
		connectButton.update(input);
		if (connectButton.isPressed()) {
			showConnectPrompt = true;
		}
		else if (hostButton.isPressed()) {
			goToLobby = true;
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