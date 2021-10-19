#include <iostream>
#include <string>
#include <SFML/Network.hpp>


int main()
{
	int connect_attempt_time = 10;
	sf::TcpSocket socket;

	std::cout << "Connecting to server...\n";

	//tries to connect 3 times before it gives up
	for (int i = 0; i < 3; i++) {
		sf::Socket::Status status = socket.connect(sf::IpAddress::getLocalAddress(), 53000, sf::seconds(connect_attempt_time));
		if (status != sf::Socket::Done)
		{
			std::cout << "[ ! ] Couldn't connect to server. [ ! ]\n";
			if (i != 2) {
				std::cout << "Trying again...\n";
			}
			else {
				std::cout << "Connecting Unsuccessful\n";
			}
		}
		else {
			std::cout << "Succesfully connected to server!\n";
			break;
		}
	}

	std::cout << "enter a message to send thru \n";
	std::string outgoing_message;

	std::cin >> outgoing_message;
	
	char data[30];
	for (int i = 0; i < 30; i++) {
		if (i < outgoing_message.length()) {
			data[i] = outgoing_message[i];
		}
		else {
			data[i] = '-';
		}
	}

	// TCP socket SEND:
	if (socket.send(data, 30) != sf::Socket::Done)
	{
		std::cout << "[ ! ] Failed to send message [ ! ]\n";
	}
	else {
		std::cout << "Succesfully sent message\n";
	}
	
}
