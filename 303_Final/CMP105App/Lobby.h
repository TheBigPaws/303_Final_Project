#pragma once
#include "HelperFunctions.h"
#include "Framework/Input.h"

//struct holding display  info of peer
struct graphicPeer {
	sf::Text Name,IP,Port;
	sf::CircleShape circle_, circleOutline_;
	int positionX, positionY;
	float radius;
	void createCircle(int posX,int posY, float radius_,std::string name,std::string IpAddress, std::string ListenerPort, sf::Font * font) {
		positionX = posX;
		positionY = posY;
		radius = radius_;

		IP = sf::Text(IpAddress, *font);
		IP.setCharacterSize(15);
		IP.setFillColor(sf::Color::Black);
		IP.setOutlineThickness(2);
		IP.setOutlineColor(sf::Color::White);
		IP.setPosition(posX - IP.getLocalBounds().width / 2, posY - IP.getLocalBounds().height / 2);

		Name = IP;
		Name.setString(name);
		Name.setCharacterSize(20);
		Name.setPosition(posX - Name.getLocalBounds().width / 2, posY - Name.getLocalBounds().height / 2 - IP.getLocalBounds().height-5);


		Port = Name;
		Port.setString(ListenerPort);
		Port.setPosition(posX - Port.getLocalBounds().width / 2, posY - Name.getLocalBounds().height / 2 + IP.getLocalBounds().height+5);

		circleOutline_ = sf::CircleShape(radius_);
		circleOutline_.setPosition(posX - radius_, posY - radius_);
		circleOutline_.setFillColor(sf::Color::White);

		//-10 is the thickness of outline
		circle_ = sf::CircleShape(radius_ - 10);
		circle_.setPosition(posX - circle_.getRadius(), posY - circle_.getRadius());
		circle_.setFillColor(sf::Color(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200, 255));
	}
};

struct graphicPeerConnectLine {
	std::string peer1_name,peer2_name;
	sf::RectangleShape line;
	void createConnectLine(graphicPeer peer1, graphicPeer peer2) { //connects p1 TO p2

		peer1_name = peer1.Name.getString();
		peer2_name = peer2.Name.getString();

		sf::Vector2f protToPeer = sf::Vector2f(peer2.positionX - peer1.positionX, peer2.positionY - peer1.positionY);
		line = sf::RectangleShape(sf::Vector2f(sqrt(protToPeer.x * protToPeer.x + protToPeer.y * protToPeer.y), 5));
		line.setPosition(peer1.positionX, peer1.positionY);
		line.rotate(atan(protToPeer.y / protToPeer.x) * 57.32);
		if (protToPeer.x < 0) {
			line.rotate(180);
			int colorR = 50 + rand() % 200;
			int colorG = 50 + rand() % 200;
			int colorB = 50 + rand() % 200;
			line.setFillColor(sf::Color(colorR, colorG, colorB, 255));
		}
	}
};

class Lobby :public GameObject
{
private:
	sf::RenderWindow* window;
	Input* input;
	sf::Font arialF;
	std::vector<graphicPeer> peersInLob;
	std::vector<graphicPeerConnectLine> peersConnectLines;
	std::vector<sf::RectangleShape> rectangles;
	std::vector<sf::Text> texts;
	sf::Text gameTimer;
	Button startCountDown, startNow;
	float countDownTimer = 30.0f;
	bool buttonPressed = false;
public:
	Chat chat;
	bool startGame = false;

	void setup(sf::RenderWindow* window_, Input* input_);
	void render();
	void update(float dt);

	void addPeer(std::string name, std::string IP_, std::string listPort);
	int displayedPeerNr() { return peersInLob.size() - 1; }

	void disconnectPlayer(std::string name);

};