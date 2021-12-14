#include "MainMenu.h"
#include <string>

void MainMenu::setup(sf::RenderWindow* window_, Input* input_){

	// sets up window, input and font
	Screens_Base::setup(window_, input_);

	//set up title text
	gameTitletxt = sf::Text("Tank Capture Game",arialF);
	setTextVal(&gameTitletxt, sf::Vector2f(window->getSize().x / 2, 50), 30,sf::Color::Black,5);
	gameTitletxt.setStyle(sf::Text::Bold);

	//set up connect prompt UI
	connectPrompt = constructRectangle(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2), sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2));

	promptExplT = sf::Text("For local multiplayer, enter 1 as IP", arialF);
	setTextVal(&promptExplT, sf::Vector2f(window->getSize().x / 2, connectPrompt.getPosition().y + 50), 25,sf::Color(150,150,150,255));


	connectPromptIP = sf::Text("IP:", arialF);
	setTextVal(&connectPromptIP, sf::Vector2f(window->getSize().x / 2 - 150 - connectPromptIP.getLocalBounds().width, window->getSize().y / 2 - 40), 25);
	connectPromptIP.setStyle(sf::Text::Italic);

	connectPromptPort = connectPromptIP;
	connectPromptPort.setString("Port:");
	connectPromptPort.setPosition(window->getSize().x / 2 - 150 - connectPromptPort.getLocalBounds().width, window->getSize().y / 2+40);

	enterNameText = connectPromptPort;
	enterNameText.setString("Enter Name:");
	enterNameText.setPosition(window->getSize().x / 2 - enterNameText.getLocalBounds().width/2, window->getSize().y - 100);

	//setup Buttons
	hostButton.setup(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 4), sf::Vector2f(200, 50), "Host",30);
	connectButton.setup(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 4 + 100), sf::Vector2f(200, 50), "Connect",30);
	connectCloseButton.setup(sf::Vector2f(window->getSize().x / 4 * 3, window->getSize().y / 4), sf::Vector2f(40, 40), "X",20,sf::Color::Red);


	//set up text Fields
	IPfield.setup(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2 - 40), sf::Vector2f(300, 50), 25);
	PortField.setup(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2 + 40), sf::Vector2f(300, 50), 25, true);
	NameField.setup(sf::Vector2f(window->getSize().x / 2, window->getSize().y - 30), sf::Vector2f(500, 60), 26, false, sf::Color(217, 217, 247));
}

void MainMenu::render() {
	window->draw(gameTitletxt);
	hostButton.render(window);
	connectButton.render(window);
	if (showConnectPrompt) {//connect prompt is open so render it
		window->draw(connectPrompt);
		window->draw(connectPromptIP);
		window->draw(connectPromptPort);
		window->draw(promptExplT);
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
		//update conmmand prompt
		hostButton.update(input);
		connectButton.update(input);
		NameField.update(input);

		if (connectButton.isPressed() || hostButton.isPressed()) { //only let me go to connect/lobby if i set a name
			if (NameField.getString() == "") {
				NameField.clearTextField(true);
				enterNameText.setString("Name can't be blank!");
				enterNameText.setFillColor(sf::Color::Red);
			}
			else {
				if (connectButton.isPressed()) {
					showConnectPrompt = true;
				}else{ goToLobby = true; }
			}
		}

	}

}

void MainMenu::updateTitleColours() {

	//every 5 milliseconds move colours closer to goal
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

	}
	//highlight of the selected text
	gameTitletxt.setOutlineColor(sf::Color(titleColours[0], titleColours[1], titleColours[2]));
	gameTitletxt.setFillColor(sf::Color(255-titleColours[0], 255-titleColours[1], 255-titleColours[2]));
}