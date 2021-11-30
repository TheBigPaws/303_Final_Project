#include "Lobby.h"

void Lobby::setup(sf::RenderWindow* window_, Input* input_) {
	window = window_; input = input_;
	arialF.loadFromFile("font/arial.ttf");
	
	rectangles.push_back(sf::RectangleShape(sf::Vector2f(window->getSize().x, window->getSize().y*0.8f)));
	rectangles.back().setPosition(0, window->getSize().y - rectangles.back().getLocalBounds().height);
	rectangles.back().setFillColor(sf::Color::Red);
}


void Lobby::addPeer(std::string IP_, std::string listPort) {

	for (int i = 0; i < peersInLob.size(); i++) {
		if (peersInLob.at(i).listPortT.getString() == listPort) {
			return;//if added peer is alrea
		}
	}



	int posX, posY;

	bool keepPlacing = true;

	while (keepPlacing) {
		keepPlacing = false;
		posX = 50 + rand() % (window->getSize().x - 100);
		posY = rectangles.at(0).getPosition().y + 50 + rand() % ((int)rectangles.at(0).getSize().y-50);

		for (int i = 0; i < peersInLob.size(); i++) {//checks if its overlapping w someone
			sf::Vector2f distance = sf::Vector2f(posX - peersInLob.at(i).circle_.getPosition().x, posY - peersInLob.at(i).circle_.getPosition().y);
			if (distance.x * distance.x + distance.y * distance.y < 10000) {
				keepPlacing = true;
			}
		}

	}

	graphicPeer protot;

	protot.positionX = posX;
	protot.positionY = posY;

	protot.remoteIPT = sf::Text(IP_, arialF);
	protot.remoteIPT.setCharacterSize(20);
	protot.remoteIPT.setPosition(posX - protot.remoteIPT.getLocalBounds().width / 2, posY - 10);
	
	protot.listPortT = sf::Text(listPort, arialF);
	protot.listPortT.setCharacterSize(20);
	protot.listPortT.setPosition(posX - protot.listPortT.getLocalBounds().width / 2, posY + 20);

	protot.circleOutline_ = sf::CircleShape(protot.remoteIPT.getLocalBounds().width / 2 + 10);
	protot.circleOutline_.setPosition(posX - protot.circleOutline_.getRadius(), posY - protot.circleOutline_.getRadius());
	protot.circleOutline_.setFillColor(sf::Color::White);

	protot.circle_ = sf::CircleShape(protot.circleOutline_.getRadius() - 10);
	protot.circle_.setPosition(posX - protot.circle_.getRadius(), posY - protot.circle_.getRadius());
	protot.circle_.setFillColor(sf::Color(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200, 255));





	if (peersInLob.size() == 0) {//THE FIRST ADDED WILL ALWAYS BE USER'S INFO
		protot.circle_.setFillColor(sf::Color::White);
		protot.circleOutline_.setPosition(window->getSize().x / 2 - protot.circleOutline_.getRadius(), window->getSize().y / 2 - protot.circleOutline_.getRadius());
		protot.circle_.setPosition(window->getSize().x / 2 - protot.circle_.getRadius(), window->getSize().y / 2 - protot.circle_.getRadius());
		protot.remoteIPT.setPosition(window->getSize().x / 2 - protot.remoteIPT.getLocalBounds().width / 2, window->getSize().y / 2 - 10);
		protot.listPortT.setPosition(window->getSize().x / 2 - protot.listPortT.getLocalBounds().width / 2, window->getSize().y / 2 + 20);
		protot.remoteIPT.setFillColor(sf::Color::Black);
		protot.listPortT.setFillColor(sf::Color::Black);

		protot.positionX = window->getSize().x / 2;
		protot.positionY = window->getSize().y / 2;
	}

	for (int i = 0; i < peersInLob.size(); i++) {//ADD ALL THE CONNECTING LINES
		sf::Vector2f protToPeer = sf::Vector2f(peersInLob.at(i).positionX - posX, peersInLob.at(i).positionY - posY);
		rectangles.push_back(sf::RectangleShape(sf::Vector2f(sqrt(protToPeer.x * protToPeer.x + protToPeer.y * protToPeer.y), 5)));
		rectangles.back().setPosition(posX,posY);
		rectangles.back().rotate(atan(protToPeer.y/protToPeer.x)* 57.32);
		if (protToPeer.x < 0) {
			rectangles.back().rotate(180);
			int colorR = 50 + rand() % 200;
			int colorG = 50 + rand() % 200;
			int colorB = 50 + rand() % 200;
			rectangles.back().setFillColor(sf::Color(colorR, colorG, colorB, 255));
		}
	}
	peersInLob.push_back(protot);
}


void Lobby::render() {
	for (int i = 0; i < rectangles.size(); i++) {
		window->draw(rectangles.at(i));
	}

	for (int i = 0; i < peersInLob.size(); i++) {
		window->draw(peersInLob.at(i).circleOutline_);
		window->draw(peersInLob.at(i).circle_);
		window->draw(peersInLob.at(i).remoteIPT);
		window->draw(peersInLob.at(i).listPortT);
	}
}


void Lobby::update(float dt) {

}