#include "P2P_Network.h"

void P2P_Network::setup() {

	//test packet shit
	outgoingPacket << "banana";

	//SET UP THIS MACHINE'S UDP PORT -------------------------------

	//myIpAdress = sf::IpAddress::getPublicAddress(); WILL BE USED IN FINAL VERSION
	myIpAdress = sf::IpAddress::getLocalAddress(); //used rn for local build

	if (udpSocket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
	{
		//error
	}
	else {
		std::cout << "connected locally to port "<<udpSocket.getLocalPort()<<std::endl;
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

		std::cout << "sending udp to " << recepientIP.toString() << " on port " << recepientPort << std::endl;

		if (udpSocket.send(outgoingPacket, recepientIP, recepientPort) != sf::Socket::Done)
		{
			std::cout << "Failed to send UDP message to client " << i << std::endl;
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
			std::cout << "Failed to send UDP message to client " << i << std::endl;
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
		//TCP_connections.back()->setBlocking(false);
		std::cout << "succesfully accepted Client to slot " << TCP_connections.size() - 1 << ".\n";
		//sf::Packet otherConnections;
		//sf::Uint16 nextCon = 1;
		//
		//if (TCP_connections.size() > 1) { //if there are more peers
		//	//Send data about other peers. Structure:
		//	// (bool continue 1) / (Ipadress) / (port) ... (bool continue 0
		//	for (int i = 0; i < TCP_connections.size() - 1; i++) {//appends all the IP addresses and ports of all except last(this) element
		//		otherConnections << nextCon << TCP_connections.at(i)->getRemoteAddress().toString() << TCP_connections.at(i)->getRemotePort();
		//	}
		//	
		//	TCP_connections.back()->setBlocking(true);
		//	std::cout << "succesfully sent thru data of other clients\n";
		//}
		//nextCon = 0;
		//otherConnections << nextCon;
		//TCP_connections.back()->send(otherConnections);
		TCP_connections.back()->setBlocking(false);

	}
}

void P2P_Network::connect_TCP(sf::IpAddress clientIP, unsigned short clientPort) {
	std::cout << "Connecting to server...\n";

	sf::TcpSocket* protot = new sf::TcpSocket;

	//tries to connect
	sf::Socket::Status status = protot->connect(clientIP, clientPort); 
	if (status != sf::Socket::Done)
	{
		std::cout << "[ ! ] Couldn't connect to server. [ ! ]\n";
	}
	else {
		std::cout << "Succesfully connected to server!\n";
		TCP_connections.push_back(protot);

		//TCP_connections.back()->receive(incomingPacket); //get data about other connections
		TCP_connections.back()->setBlocking(false);
		//
		//sf::Uint16 nextCon;//connect to others
		//std::string ipAddr;
		//unsigned short port_;
		//
		//incomingPacket >> nextCon;
		//
		//while (nextCon == 1) {
		//	incomingPacket >> ipAddr >> port_;
		//	sf::TcpSocket* protot_ = new sf::TcpSocket;
		//	TCP_connections.push_back(protot_);
		//	TCP_connections.back()->connect(ipAddr, port_);
		//	TCP_connections.back()->setBlocking(false);
		//	incomingPacket >> nextCon;
		//	std::cout << "added con\n";
		//}

		
	}
}


void P2P_Network::update() {

	acceptClient_TCP();

	for (int i = 0; i < TCP_connections.size(); i++) {
		receive_UDP(i);
		receive_TCP(i);
	}
	
}
