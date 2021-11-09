#include "Client.h"

void Client::connectToHost_TCP(sf::IpAddress hostIP) {
	serverIP = hostIP;
	std::cout << "Connecting to server...\n";
	//socket.setBlocking(false);
	//tries to connect 5 times before it gives up
	for (int i = 0; i < 5; i++) {
		sf::Socket::Status status = socket.connect(serverIP, 53000, sf::seconds(connect_attempt_time));
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

void Client::send_UDP() {
	send_UDP_(serverIP, serverPort);
}

void Client::receive_UDP() {
	receive_UDP_(serverIP);
}


void Client::receive_TCP() {
	incomingPacket.clear();

	if (sender_socket.receive(incomingPacket) != sf::Socket::Done)
	{
		//std::cout << "error in receive\n";
	}
	else {
		std::cout << "succesfully received (as TCP) from client\n";
		//deal with packet
	}
}


void Client::send_TCP(sf::TcpSocket& recepient_socket) {


	// TCP socket SEND:
	if (recepient_socket.send(outgoingPacket) != sf::Socket::Done)
	{
		std::cout << "[ ! ] Failed to send message [ ! ]\n";
	}
	else {
		std::cout << "Succesfully sent message\n";
	}
	outgoingPacket.clear();

}