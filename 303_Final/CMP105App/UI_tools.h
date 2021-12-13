#pragma once
#include "HelperFunctions.h"
#include "Framework/Input.h"
//not needed 
class UI_tools
{
};

//BUTTON CLASS
class Button {
private:
	sf::Font font_;
	sf::Text text;
	sf::RectangleShape Button;
	bool pressed = false;
	bool hovered = false;
public:
	sf::Vector2f position, size;
	sf::Color fillColour, outlineColour;
	//basic setters
	void setText(std::string text_) { text.setString(text_); text.setFillColor(sf::Color(255 - fillColour.r, 255 - fillColour.g, 255 - fillColour.b, 255)); }
	void setColors(sf::Color fillColour, bool reset = false);
	void textResetPos() { text.setPosition(position.x - text.getLocalBounds().width / 2, position.y - text.getLocalBounds().height); }

	//basic getters
	bool isPressed() { return pressed; }
	bool isHovered() { return hovered; }

	//basic game loop oriented functions
	void setup(sf::Vector2f position_, sf::Vector2f size_, sf::String string, int fontSize, sf::Color fillColour_ = sf::Color::White, sf::Color outlineColour_ = sf::Color::Black);
	void render(sf::RenderWindow* window_);
	void update(Input* input_);

};

class TextField {
private:
	bool onlyIntsAllowed = false;
	bool selected = false;
	std::string shown_string = "";
	Button textField;
	bool keyPressed = false;
public:
	//basic get & set funcs
	std::string getString() { return shown_string; }
	bool isSelected() { return selected; }

	//clears text field
	void clearTextField(bool wasWrong);

	//basic game loop oriented functions
	void setup(sf::Vector2f position_, sf::Vector2f size_, int fontSize, bool onlyInts = false, sf::Color fillColour = sf::Color::White, sf::Color outlineColor = sf::Color::Black);
	void render(sf::RenderWindow* window_);
	void update(Input* input_);
};

 

//struct holding display  info of peer
struct graphicPeer {
	sf::Text Name, IP, Port;
	sf::CircleShape circle_, circleOutline_;
	sf::Vector2f position;
	float radius;
	bool ready = false;
	void createCircle(sf::Vector2f position_, float radius_, std::string name, std::string IpAddress, std::string ListenerPort, sf::Font* font) {
		position = position_;
		radius = radius_;



		//IP = constructText(position, 15, IpAddress, *font, sf::Color::Black, 2.0);
		IP = sf::Text(IpAddress, *font,15);
		setTextVal(&IP, position,15, sf::Color::Black, 2.0f);

		//Name = constructText(position - sf::Vector2f(0, IP.getLocalBounds().height + 5), 20, name, *font, sf::Color::Black, 2.0);
		Name = sf::Text(name,*font,20);
		setTextVal(&Name, position - sf::Vector2f(0, IP.getLocalBounds().height + 5), 20 , sf::Color::Black, 2.0f);

		//Port = constructText(position + sf::Vector2f(0, IP.getLocalBounds().height + 5), 15, ListenerPort, *font, sf::Color::Black, 2.0);
		Port = sf::Text(ListenerPort, *font, 15);
		setTextVal(&Port, position + sf::Vector2f(0, IP.getLocalBounds().height + 5), 15, sf::Color::Black, 2.0f);

		circle_ = constructSphere(position, radius_, sf::Color(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200, 255), 5.0f, sf::Color::White);
	}
};

struct graphicPeerConnectLine {
	std::string peer1_name, peer2_name;
	sf::RectangleShape line;
	void createConnectLine(graphicPeer peer1, graphicPeer peer2) { //connects p1 TO p2

		peer1_name = peer1.Name.getString();
		peer2_name = peer2.Name.getString();

		sf::Vector2f protToPeer = peer2.position - peer1.position;
		line = sf::RectangleShape(sf::Vector2f(sqrt(protToPeer.x * protToPeer.x + protToPeer.y * protToPeer.y), 5));
		line.setPosition(peer1.position.x, peer1.position.y);
		line.rotate(atan(protToPeer.y / protToPeer.x) * 57.32);
		if (protToPeer.x < 0) {
			line.rotate(180);
		}
	}
};