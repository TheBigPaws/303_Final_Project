#include "Lobby.h"

void Lobby::setup(sf::RenderWindow* window_, Input* input_) {
	
	// sets up window, input and font
	Screens_Base::setup(window_, input_);


	rectangles.push_back(sf::RectangleShape(sf::Vector2f(window->getSize().x/3, window->getSize().x / 3)));
	rectangles.back().setPosition((float)window->getSize().x  - rectangles.back().getSize().x, window->getSize().y - rectangles.back().getSize().x);
	rectangles.back().setFillColor(sf::Color(60, 60, 60));

	chat.setup(window_, input_);
	chat.setPosSize(sf::Vector2f(150, window->getSize().y / 3 * 2), sf::Vector2f(300, window->getSize().y / 3 * 2));

	gameTimer = sf::Text("Start in 10.0 seconds",arialF,25);
	gameTimer.setPosition(window->getSize().x / 2 - gameTimer.getLocalBounds().width / 2, 20);

	readyButton.setup(sf::Vector2f(window->getSize().x / 2, gameTimer.getPosition().y + 100), sf::Vector2f(300, 100), "Not Ready!", 40, sf::Color::Red, sf::Color::White);
}


void Lobby::addPeer(std::string name, std::string IP_, std::string listPort) {

	for (int i = 0; i < peersInLob.size(); i++) {
		if (peersInLob.at(i).Name.getString() == name) {
			return;//if added peer is already there
		}
	}



	int posX, posY;

	bool keepPlacing = true;

	while (keepPlacing) {
		keepPlacing = false;
		//rectangles.front is the p2p display bound
		//    circ rad              pos of space to display
		posX = 20 + rectangles.at(0).getPosition().x + (rand() % ((int)rectangles.at(0).getSize().x - 20));
		posY = 20 + rectangles.at(0).getPosition().y + (rand() % ((int)rectangles.at(0).getSize().y - 20));

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

		//add ready message
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
		//sf::Vector2f protToPeer = sf::Vector2f(peersInLob.at(i).positionX - posX, peersInLob.at(i).positionY - posY);
		//rectangles.push_back(sf::RectangleShape(sf::Vector2f(sqrt(protToPeer.x * protToPeer.x + protToPeer.y * protToPeer.y), 5)));
		//rectangles.back().setPosition(posX,posY);
		//rectangles.back().rotate(atan(protToPeer.y/protToPeer.x)* 57.32);
		//if (protToPeer.x < 0) {
		//	rectangles.back().rotate(180);
		//	int colorR = 50 + rand() % 200;
		//	int colorG = 50 + rand() % 200;
		//	int colorB = 50 + rand() % 200;
		//	rectangles.back().setFillColor(sf::Color(colorR, colorG, colorB, 255));
		//}
		graphicPeerConnectLine a;
		a.createConnectLine(protot, peersInLob.at(i));
		peersConnectLines.push_back(a);
	}
	peersInLob.push_back(protot);
	checkReadiness();
}


void Lobby::render() {
	for (int i = 0; i < rectangles.size(); i++) {
		window->draw(rectangles.at(i));
	}

	for (int i = 0; i < peersConnectLines.size(); i++) {
		window->draw(peersConnectLines.at(i).line);
	}

	for (int i = 0; i < texts.size(); i++) {
		window->draw(texts.at(i));
	}

	chat.render();

	for (int i = 0; i < peersInLob.size(); i++) {
		window->draw(peersInLob.at(i).circleOutline_);
		window->draw(peersInLob.at(i).circle_);
		window->draw(peersInLob.at(i).Name);
		window->draw(peersInLob.at(i).IP);
		window->draw(peersInLob.at(i).Port);
	}
	
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

	if (countDownTimer <= 0.0f) {
		startGame = true;
	}
	if (allReady && peersInLob.size() > 1) {
		countDownTimer -= dt;

		std::string timeWText = "Start in ";
		timeWText += std::to_string(countDownTimer);
		timeWText += " seconds";
		gameTimer.setString(timeWText);
		gameTimer.setPosition(window->getSize().x / 2 - gameTimer.getLocalBounds().width / 2, gameTimer.getPosition().y);
	}

	if (readyButton.isPressed()) {
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
	for (int i = 0; i < peersInLob.size(); i++) {
		if (peersInLob.at(i).Name.getString() == name) {
			playerReadinessText.erase(playerReadinessText.begin() + i - 1);
			peersInLob.erase(peersInLob.begin() + i);
			break;
		}
	}

	for (int i = 0; i < peersConnectLines.size(); i++) {
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

void Lobby::setReady(std::string name,bool ready) {

	for (int i = 0; i < peersInLob.size(); i++) {
		if (peersInLob.at(i).Name.getString() == name) {
			peersInLob.at(i).ready = ready;
			if (ready && playerReadinessText.at(i - 1).getFillColor() == sf::Color::Red){
				playerReadinessText.at(i - 1).setString(std::string(name + "is ready."));
				playerReadinessText.at(i - 1).setFillColor(sf::Color::Green);
			}
			if (!ready && playerReadinessText.at(i - 1).getFillColor() == sf::Color::Green) {
				playerReadinessText.at(i - 1).setString(std::string(name + "is not ready."));
				playerReadinessText.at(i - 1).setFillColor(sf::Color::Red);
			}
			break;
		}
	}


	checkReadiness();
}


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