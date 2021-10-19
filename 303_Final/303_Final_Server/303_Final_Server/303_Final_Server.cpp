#include <iostream>
#include <string>
#include <SFML/Network.hpp>
#include <vector>

struct EnviromentObject {
public:
	std::string texture;
	float position[2];
	float size[2];
	bool doesCollide;
};

std::vector<EnviromentObject> envObjects;

int main()
{
	


	//SERVER
	sf::TcpListener listener;

	// bind the listener to a port
	if (listener.listen(53000) != sf::Socket::Done)
	{
		std::cout << "error setting up listener\n";
	}
	else {
		std::cout << "succesfully set up listener\n";
	}

	// accept a new connection
	sf::TcpSocket client;
	if (listener.accept(client) != sf::Socket::Done)
	{
		std::cout << "error accepting new connection\n";
	}
	else {
		std::cout << "succesfully accepted Client\n";
	}

	//TCP socket RECEIVE
	char data[6];
	std::size_t received;

	// TCP socket:
	if (client.receive(data, 6, received) != sf::Socket::Done)
	{
		std::cout << "error in receive\n";
	}
	else {
		std::cout << "succesfully received\n";
	}
	
	std::cout << "Received " << received << " bytes" << std::endl;
}

void setupEnv() {
	EnviromentObject EO;

	EO.doesCollide = false;
	EO.position[0] = -5.0f;
	EO.position[1] = -5.0f;

	EO.size[0] = 5.0f;
	EO.size[1] = 5.0f;

	EO.texture = "wood_floor";

	envObjects.push_back(EO);
}
