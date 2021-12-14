#include "Lobby.h"

void Lobby::setup(sf::RenderWindow* window_, Input* input_) {
	
	// sets up window, input and font
	Screens_Base::setup(window_, input_);

	//set up lobby UI
	rectangles.push_back(sf::RectangleShape(sf::Vector2f(window->getSize().x/3, window->getSize().x / 3)));
	rectangles.back().setPosition(window->getSize().x  - rectangles.back().getSize().x, window->getSize().y - rectangles.back().getSize().x);
	rectangles.back().setFillColor(sf::Color(60, 60, 60));

	sf::Vector2f exPeerPos = sf::Vector2f(rectangles.back().getPosition().x + rectangles.back().getSize().x / 2, rectangles.back().getPosition().y - 100);
	examplePeer.createCircle(exPeerPos,40,"NAME","IP ADDRESS","LISTENER PORT",&arialF);
	examplePeerText = sf::Text("Visualisation \n Explained", arialF, 40);
	setTextVal(&examplePeerText,exPeerPos - sf::Vector2f(0,100),40,sf::Color(200,200,200,255));
	examplePeerText.setStyle(sf::Text::Bold);

	GraphicVisualisationText = sf::Text("Connected Peers", arialF, 40);
	setTextVal(&GraphicVisualisationText, sf::Vector2f(exPeerPos.x, rectangles.back().getPosition().y-10), 30, sf::Color::White);

	chat.setup(window_, input_);
	chat.setPosSize(sf::Vector2f(150, window->getSize().y / 3 * 2), sf::Vector2f(300, window->getSize().y / 3 * 2));

	gameTimer = sf::Text("Start in 10.0 seconds",arialF,25);
	gameTimer.setPosition(window->getSize().x / 2 - gameTimer.getLocalBounds().width / 2, 20);

	readyButton.setup(sf::Vector2f(window->getSize().x / 2, gameTimer.getPosition().y + 100), sf::Vector2f(300, 100), "Not Ready!", 40, sf::Color::Red, sf::Color::White);
}


void Lobby::addPeer(std::string name, std::string IP_, std::string listPort) {

	//fix for doubling peers in lob by mistake
	for (int i = 0; i < peersInLob.size(); i++) {
		if (peersInLob.at(i).Name.getString() == name) {
			return;
		}
	}

	//find a random position on the connected Peers graphical visualization
	int posX, posY;
	bool keepPlacing = true;
	while (keepPlacing) {
		keepPlacing = false;
		posX = 30 + rectangles.at(0).getPosition().x + (rand() % ((int)rectangles.at(0).getSize().x - 30));
		posY = 30 + rectangles.at(0).getPosition().y + (rand() % ((int)rectangles.at(0).getSize().y - 30));

		for (int i = 0; i < peersInLob.size(); i++) {//checks if its overlapping w someone
			sf::Vector2f distance = sf::Vector2f(posX - peersInLob.at(i).circle_.getPosition().x, posY - peersInLob.at(i).circle_.getPosition().y);
			if (distance.x * distance.x + distance.y * distance.y < 10000) {
				keepPlacing = true;
			}
		}

	}

	graphicPeer protot;
	protot.createCircle(sf::Vector2f(posX, posY), 20, name, IP_, listPort, &arialF);

	//first added to lob display, i.e. me
	if (peersInLob.size() == 0) {//THE FIRST ADDED WILL ALWAYS BE USER'S INFO
		protot.createCircle(sf::Vector2f(rectangles.at(0).getPosition().x + rectangles.at(0).getSize().x/2, rectangles.at(0).getPosition().y + rectangles.at(0).getSize().y / 2), 35, name, IP_, listPort, &arialF);
		protot.circle_.setFillColor(sf::Color::White);

	}//add enemy peer
	else {

		//add ready text
		std::string readyString = name + " is not ready.";
		sf::Text rdyTextProt = sf::Text(readyString, arialF, 20);
		setTextVal(&rdyTextProt, sf::Vector2f(window->getSize().x / 2, 250 + playerReadinessText.size()*50), 20, sf::Color::Red, 3.0);
		playerReadinessText.push_back(rdyTextProt);

		//add chat message about join
		std::string join_msg = "Peer ";
		join_msg = join_msg + protot.Name.getString() + " has joined the lobby.";
		chat.addMessage(join_msg, sf::Color::Yellow, "LOBBY");
	}

	//ADD ALL THE CONNECTING LINES
	for (int i = 0; i < peersInLob.size(); i++) {
		graphicPeerConnectLine a;
		a.createConnectLine(protot, peersInLob.at(i));
		peersConnectLines.push_back(a);
	}
	peersInLob.push_back(protot);
	checkReadiness();
}


//classic render
void Lobby::render() {
	for (int i = 0; i < rectangles.size(); i++) {
		window->draw(rectangles.at(i));
	}

	for (int i = 0; i < peersConnectLines.size(); i++) {
		window->draw(peersConnectLines.at(i).line);
	}

	chat.render();

	for (int i = 0; i < peersInLob.size(); i++) {
		window->draw(peersInLob.at(i).circleOutline_);
		window->draw(peersInLob.at(i).circle_);
		window->draw(peersInLob.at(i).Name);
		window->draw(peersInLob.at(i).IP);
		window->draw(peersInLob.at(i).Port);
	}
	
	window->draw(examplePeer.circleOutline_);
	window->draw(examplePeer.circle_);
	window->draw(examplePeer.Name);
	window->draw(examplePeer.IP);
	window->draw(examplePeer.Port);
	window->draw(examplePeerText);
	window->draw(GraphicVisualisationText);

	for (int i = 0; i < playerReadinessText.size(); i++) {
		window->draw(playerReadinessText.at(i));
	}

	readyButton.render(window);

	if (countDownTimer != 10.0f) {
		window->draw(gameTimer);
	}
}


void Lobby::update(float dt) {
	chat.update();
	
	if (allReady && peersInLob.size() > 1) {//can count down game start (all ready and not just me in lobby)
		countDownTimer -= dt;
		if (countDownTimer <= 0.0f) {
			startGame = true;
		}
		std::string timeWText = "Start in ";
		timeWText += std::to_string(countDownTimer);
		timeWText += " seconds";
		gameTimer.setString(timeWText);
		gameTimer.setPosition(window->getSize().x / 2 - gameTimer.getLocalBounds().width / 2, gameTimer.getPosition().y);
	}

	if (readyButton.isPressed()) { //set my readiness
		input->setMouseLDown(false);
		if (readyButton.fillColour == sf::Color::Red) {
			peersInLob.front().ready = true;
			readyButton.setText("Ready!");
			readyButton.setColors(sf::Color::Green);
			
		}
		else {
			peersInLob.front().ready = false;
			readyButton.setText("Not Ready!");
			readyButton.setColors(sf::Color::Red);
		}
		checkReadiness();
	}
	readyButton.update(input);

}

void Lobby::disconnectPlayer(std::string name) {
	for (int i = 0; i < peersInLob.size(); i++) { //delete that player from lobby
		if (peersInLob.at(i).Name.getString() == name) {
			playerReadinessText.erase(playerReadinessText.begin() + i - 1);
			peersInLob.erase(peersInLob.begin() + i);
			break;
		}
	}

	for (int i = 0; i < peersConnectLines.size(); i++) { //remove all UI connecting lines to/from that player
		if (peersConnectLines.at(i).peer1_name == name || peersConnectLines.at(i).peer2_name == name) {
			peersConnectLines.erase(peersConnectLines.begin() + i);
			i--;
		}
	}

	//add chat message about join
	std::string join_msg = "Peer ";
	join_msg = join_msg + name + " has left the lobby.";
	chat.addMessage(join_msg, sf::Color::Yellow, "LOBBY");

}

//set the specified peer to ready or not ready and update the UI accordingly
void Lobby::setReady(std::string name,bool ready) {

	for (int i = 0; i < peersInLob.size(); i++) {
		if (peersInLob.at(i).Name.getString() == name) {
			peersInLob.at(i).ready = ready;
			if (ready && playerReadinessText.at(i - 1).getFillColor() == sf::Color::Red){
				playerReadinessText.at(i - 1).setString(std::string(name + " is ready."));
				playerReadinessText.at(i - 1).setFillColor(sf::Color::Green);
			}
			if (!ready && playerReadinessText.at(i - 1).getFillColor() == sf::Color::Green) {
				playerReadinessText.at(i - 1).setString(std::string(name + " is not ready."));
				playerReadinessText.at(i - 1).setFillColor(sf::Color::Red);
			}
			break;
		}
	}


	checkReadiness();
}


//makes sure if all connected players are ready
void Lobby::checkReadiness() {
	bool readyCheck = true;

	for (int i = 0; i < peersInLob.size(); i++) {
		if (!peersInLob.at(i).ready) {
			readyCheck = false;
			break;
		}
	}

	allReady = readyCheck;
}