#pragma once
#include "Screens_Base.h"
#include "UI_tools.h"
//#include "Framework/AudioManager.h"



class MainMenu : public Screens_Base
{
private:

	sf::Text gameTitletxt,connectPromptIP, connectPromptPort,enterNameText,promptExplT;
	bool showConnectPrompt = false;
	sf::RectangleShape connectPrompt;

	//coloured title
	int titleColours[3];
	int goalColours[3];
	sf::Clock clrclock;
	void updateTitleColours();

	//interactive elements
	TextField IPfield,PortField,NameField;
	Button connectButton,hostButton,connectCloseButton;

public:
	//cross-class communication variables
	bool goToLobby = false;
	bool attemptConnect = false;

	//basic game loop functions
	void setup(sf::RenderWindow* window_, Input* input_);
	void render();
	void update(float dt);

	//handles different error messages we can have from faulty connects
	void resetInput(bool wasWrong, int errorType) {
		IPfield.clearTextField(wasWrong);
		PortField.clearTextField(wasWrong);
		enterNameText.setFillColor(sf::Color::Red);
		if (errorType == 1) {
			enterNameText.setString("Name already used!");
		}
		else if (errorType == 2) {
			enterNameText.setString("Wrong IP Address/Port!");
		}
	}

	//input getters
	std::string getEnteredIP() {
		return IPfield.getString();
	}
	std::string getEnteredPort() {
		return PortField.getString();
	}
	std::string getEnteredName() {
		return NameField.getString();
	}
	

};