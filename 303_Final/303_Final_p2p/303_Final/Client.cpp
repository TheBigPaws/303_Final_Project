#include "Client.h"


//converts a string into char[30], filled with '-' for empty slots
void Client::convertToData(std::string message) {
	for (int i = 0; i < 30; i++) {
		if (i < message.length()) {
			data[i] = message[i];
		}
		else {
			data[i] = '-';
		}
	}
}

//CLIENT functions. c_ prefix
void Client::connectToHost_TCP(std::string hostIP) {
	std::cout << "Connecting to server...\n";
	//socket.setBlocking(false);
	//tries to connect 5 times before it gives up
	for (int i = 0; i < 5; i++) {
		sf::Socket::Status status = socket.connect(sf::IpAddress(hostIP), 53000, sf::seconds(connect_attempt_time));
		if (status != sf::Socket::Done)
		{
			std::cout << "[ ! ] Couldn't connect to server. [ ! ]\n";
			if (i != 4) {
				std::cout << "Trying again...\n";
			}
			else {
				std::cout << "Connecting Unsuccessful\n";
			}
		}
		else {
			std::cout << "Succesfully connected to server!\n";
			i = 5;
		}
	}
}

void Client::sendToHost_TCP(std::string message) {

	convertToData(message);

	// TCP socket SEND:
	if (socket.send(data, 30) != sf::Socket::Done)
	{
		std::cout << "[ ! ] Failed to send message [ ! ]\n";
	}
	else {
		std::cout << "Succesfully sent message\n";
	}

}