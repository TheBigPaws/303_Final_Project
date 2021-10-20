#include "Server.h"

//HOST functions. h_ prefix
void Server::setupListener_TCP() {
	// bind the listener to a port
	if (listener.listen(53000) != sf::Socket::Done)
	{
		std::cout << "error setting up listener\n";
	}
	else {
		std::cout << "succesfully set up listener\n";
	}
	listener.setBlocking(false);
	//clients.push_back(client);
	//client.setBlocking(false);
}

void Server::acceptClient_TCP() {
	//accept new connection
	if (listener.accept(client) != sf::Socket::Done)
	{
		std::cout << "error accepting new connection\n";
	}
	else {
		std::cout << "succesfully accepted Client\n";
		//clients.push_back(client);
	}
}

void Server::receive_TCP() {

	//TCP socket RECEIVE
	char data[30];
	std::size_t received;


	for (int i = 0; i < 1; i++) {
		// TCP socket:
		if (client.receive(data, 30, received) != sf::Socket::Done)
		{
			std::cout << "error in receive\n";
		}
		else {
			std::cout << "succesfully received\n";
			std::cout << "Message is:\n";
			std::cout << data << std::endl;
		}

	}
	
	std::cout << "Received " << received << " bytes" << std::endl;
}
