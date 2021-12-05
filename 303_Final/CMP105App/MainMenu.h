#pragma once
#include "HelperFunctions.h"
#include "Framework/Input.h"
//#include "Framework/AudioManager.h"



class MainMenu :public GameObject
{
private:
	sf::RenderWindow* window;
	Input* input;
	sf::Font arialF;

	sf::Text gameTitletxt,connectPromptIP, connectPromptPort,enterNameText;
	bool showConnectPrompt = false;
	sf::RectangleShape connectPrompt;

	//colored title
	int titleColours[3];
	int goalColours[3];
	sf::Clock clrclock;
	void updateTitleColours();

	//interactive elements
	TextField IPfield,PortField,NameField;
	Button connectButton,hostButton,connectCloseButton;
public:
	void setup(sf::RenderWindow* window_, Input* input_);
	void render();
	void update(float dt);

	bool goToLobby = false;
	bool attemptConnect = false;

	std::string getEnteredIP() {
		return IPfield.getString();
	}
	std::string getEnteredPort() {
		return PortField.getString();
	}
	std::string getEnteredName() {
		return NameField.getString();
	}
	void resetInput(bool wasWrong) {
		IPfield.clearTextField(wasWrong);
		PortField.clearTextField(wasWrong);
	}

};