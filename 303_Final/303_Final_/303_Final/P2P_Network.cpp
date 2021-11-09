#include "P2P_Network.h"

void P2P_Network::setup() {

	//test packet shit
	outgoingPacket << "banana";

	//SET UP THIS MACHINE'S UDP PORT -------------------------------

	myUDPport = sf::Socket::AnyPort;
	//myIpAdress = sf::IpAddress::getPublicAddress(); WILL BE USED IN FINAL VERSION
	myIpAdress = sf::IpAddress::getLocalAddress(); //used rn for local build

	if (udpSocket.bind(myUDPport) != sf::Socket::Done)
	{
		//error
	}
	else {
		std::cout << "connected locally to port "<<myUDPport<<std::endl;
	}
	udpSocket.setBlocking(false);

	//SET UP THIS MACHINE'S LISTENER -------------------------------

	// bind the listener to a port
	if (listener.listen(sf::Socket::AnyPort) != sf::Socket::Done)
	{
		std::cout << "error setting up listener\n";
	}
	else {
		std::cout << "succesfully set up listener to port " << listener.getLocalPort() << std::endl;
	}
	listener.setBlocking(false);
	std::cout << "This device's local IP address is " << sf::IpAddress::getLocalAddress() << std::endl;
	std::cout << "This device's public IP address is " << sf::IpAddress::getPublicAddress() << std::endl;
}

//sends the constructed message to all connections and clears the packet
void P2P_Network::send_UDP() {

	for (int i = 0; i < TCP_connections.size(); i++) {

		sf::IpAddress recepientIP = TCP_connections.at(i)->getRemoteAddress();
		unsigned short recepientPort = TCP_connections.at(i)->getRemotePort();

		if (udpSocket.send(outgoingPacket, recepientIP, recepientPort) != sf::Socket::Done)
		{
			//error
		}
		else {
			std::cout << "succesfully sent (as UDP) to client " << i << std::endl;
			//deal with packet
		}
	}
	//outgoingPacket.clear();
}

void P2P_Network::send_TCP() {

	for (int i = 0; i < TCP_connections.size(); i++) {
		// TCP socket SEND:
		if (TCP_connections.at(i)->send(outgoingPacket) != sf::Socket::Done)
		{
			std::cout << "[ ! ] Failed to send message [ ! ]\n";
		}
		else {
			std::cout << "succesfully sent (as TCP) to client " << i << std::endl;
		}
	}
	//outgoingPacket.clear();

}

void P2P_Network::receive_UDP(int senderIndex) {
	incomingPacket.clear();

	sf::IpAddress senderIP = TCP_connections.at(senderIndex)->getRemoteAddress();
	unsigned short senderPort = TCP_connections.at(senderIndex)->getRemotePort();

	if (udpSocket.receive(incomingPacket,senderIP, senderPort) != sf::Socket::Done)
	{
		//error
	}
	else {
		std::cout << "succesfully received (as UDP) from client " << senderIndex << std::endl;
		//deal with packet
	}
}

void P2P_Network::receive_TCP(int senderIndex) {
	incomingPacket.clear();

	if (TCP_connections.at(senderIndex)->receive(incomingPacket) != sf::Socket::Done)
	{
		//std::cout << "error in receive\n";
	}
	else {
		std::cout << "succesfully received (as TCP) from client " << senderIndex <<std::endl;
		//deal with packet
	}
}

void P2P_Network::acceptClient_TCP() {

	sf::TcpSocket* protot = new sf::TcpSocket;

	//accept new connection
	if (listener.accept(*protot) != sf::Socket::Done)
	{
		//std::cout << "error accepting new connection\n";
	}
	else {
		TCP_connections.push_back(protot);

		if (TCP_connections.size() >= 2) {
			//Send data about other peers. Structure:
		// (bool continue 1) / (Ipadress) / (port) ... (bool continue 0
			sf::Packet otherConnections;
			sf::Uint16 nextCon = 1;
			for (int i = 0; i < TCP_connections.size() - 1; i++) {
				otherConnections << nextCon << TCP_connections.at(i)->getRemoteAddress().toString() << TCP_connections.at(i)->getRemotePort();
			}
			nextCon = 0;
			otherConnections << nextCon;
			TCP_connections.back()->setBlocking(true);
			TCP_connections.back()->send(otherConnections);
			std::cout << "succesfully sent thru data of other clients\n";
		}
		
		TCP_connections.back()->setBlocking(false);

		std::cout << "succesfully accepted Client to slot " << TCP_connections.size() << ".\n";

	}
}

void P2P_Network::connect_TCP(sf::IpAddress clientIP, unsigned short clientPort) {
	std::cout << "Connecting to server...\n";

	sf::TcpSocket* protot = new sf::TcpSocket;

	//tries to connect 5 times before it gives up
	for (int i = 0; i < 3; i++) {
		sf::Socket::Status status = protot->connect(clientIP, clientPort, sf::seconds(5)); //tries to connect every 5 seconds, 3 times
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
			TCP_connections.push_back(protot);

			TCP_connections.back()->setBlocking(true);
			TCP_connections.back()->receive(incomingPacket);

			sf::Uint16 nextCon;
			std::string ipAddr;
			unsigned short port_;

			while (true) {
				incomingPacket >> nextCon;

				if (nextCon == 1) {
					std::cout << "binding other client" << std::endl;
					incomingPacket >> ipAddr >> port_;

					bool newConnection = true;

					for (int i = 0; i < TCP_connections.size(); i++) {

					}
				}
				else { std::cout << "bound all"; break; }
			}
			TCP_connections.back()->setBlocking(false);
			break;
		}
	}
}

void P2P_Network::update() {

	acceptClient_TCP();

	for (int i = 0; i < TCP_connections.size(); i++) {
		receive_TCP(i);
		receive_UDP(i);
	}
	
}
