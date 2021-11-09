#include "NetworkExtensionBase.h"


////converts a string into char[30], filled with '-' for empty slots
//void Client::convertToData(std::string message) {
//	for (int i = 0; i < 30; i++) {
//		if (i < message.length()) {
//			data[i] = message[i];
//		}
//		else {
//			data[i] = '-';
//		}
//	}
//}


//binds UDP to a specific port
void NetworkExtensionBase::bind_UDP(int port) {
	UDPport = port;
	if (udpSocket.bind(UDPport) != sf::Socket::Done)
	{
		//error
	}
	else {
		std::cout << "connected client succesfully\n";
	}
	udpSocket.setBlocking(false);

}


void NetworkExtensionBase::receive_UDP_(sf::IpAddress sender) {
	incomingPacket.clear();

	if (udpSocket.receive(incomingPacket, sender, UDPport) != sf::Socket::Done)
	{
		//error
	}
	else {
		std::cout << "Received from " << sender << " (As UDP) on port " << UDPport << std::endl;
		//deal with packet
	}
}

//sends the constructed message and clears the packet
void NetworkExtensionBase::send_UDP_(sf::IpAddress recepient_IP, unsigned short sender_port) {

	udpSocket.send(outgoingPacket, recepient_IP, sender_port);
	outgoingPacket.clear();
}



//overloads for constructing a packet
void NetworkExtensionBase::addToPacket(float float_) { outgoingPacket << float_; }
//overloads for constructing a packet
void NetworkExtensionBase::addToPacket(sf::Uint16 integer) { outgoingPacket << integer; }
//overloads for constructing a packet
void NetworkExtensionBase::addToPacket(std::string string_) { outgoingPacket << string_; }
//overloads for constructing a packet
void NetworkExtensionBase::addToPacket(Data_Header header) { outgoingPacket << (sf::Uint16)header; }
//overloads for constructing a packet
//void Client::addToPacket(playerInformation PlayerInformation) { outgoingPacket << PlayerInformation; }

void NetworkExtensionBase::decodePacket() {

	sf::Uint16 msg_header;

	incomingPacket >> msg_header;
	while (msg_header != MESSAGE_END) {
		switch (msg_header) {
		case PLAYER_POSITION:

			break;
		case PLAYER_ANIMATION_STAGE:

			break;
		case PARTICLE_POSITION:

			break;
		case PARTICLE_ROTATION:

			break;
		}
		incomingPacket >> msg_header;
	}


}
