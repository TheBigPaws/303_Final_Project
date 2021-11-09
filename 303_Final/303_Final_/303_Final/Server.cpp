#include "Server.h"



//HOST functions. h_ prefix
void Server::setup_TCP() {
	// bind the listener to a port
	if (listener.listen(53000) != sf::Socket::Done)
	{
		std::cout << "error setting up listener\n";
	}
	else {
		std::cout << "succesfully set up listener\n";
	}
	listener.setBlocking(false);
	
}


void Server::acceptClient_TCP() {

	sf::TcpSocket * protot = new sf::TcpSocket;

	//accept new connection
	if (listener.accept(*protot) != sf::Socket::Done)
	{
		//std::cout << "error accepting new connection\n";
	}
	else {
		clients_.push_back(protot);
		clients_.back()->setBlocking(false);

		std::cout << "succesfully accepted Client to slot " << clients_.size() << ".\n";

	}
}





void Server::update() {

	acceptClient_TCP();

	receive_TCP();

	receive_UDP();
}

void Server::setup(int UDPport) {

	setup_TCP();

	setup_UDP(UDPport);
}


void Server::send_UDP() {

}
void Server::receive_UDP() {

}
void Server::send_TCP() {

}
void Server::receive_TCP() {
	for (int i = 0; i < clients_.size(); i++) {
		receive_TCP_(clients_.at(i));
	}
}
