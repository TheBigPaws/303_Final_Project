#include "P2P_Network.h"

void P2P_Network::setup() {

	//if (udpSocket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
	//{
	//	//error
	//}
	//else {
	//	std::cout << "connected locally to port " << udpSocket.getLocalPort() << std::endl;
	//}
	//udpSocket.setBlocking(false);
	
	peers.push_back(new Peer);
	// bind the listener to a port
	if (listener.listen(sf::Socket::AnyPort) != sf::Socket::Done)
	{
		std::cout << "error setting up listener\n";
	}
	else {
		std::cout << "succesfully set up listener to port " << listener.getLocalPort() << std::endl;
	}
	listener.setBlocking(false);

	//info
	std::cout << "This device's local IP address is " << sf::IpAddress::getLocalAddress() << std::endl;
	std::cout << "This device's public IP address is " << sf::IpAddress::getPublicAddress() << std::endl;
	std::cout << "\n*** Setup complete ***\n" << std::endl;
}
//
//void P2P_Network::send_UDP() {
//	for (int i = 0; i < TCP_connections.size(); i++) {
//		sf::IpAddress recepientIP = TCP_connections.at(i)->getRemoteAddress();
//		unsigned short recepientPort = TCP_connections.at(i)->getRemotePort();
//
//		std::cout << "sending udp to " << recepientIP.toString() << " on port " << recepientPort << std::endl;
//
//		if (udpSocket.send(outgoingPacket, recepientIP, recepientPort) != sf::Socket::Done)
//		{
//			std::cout << "Failed to send UDP message to client " << std::endl;
//		}
//		else {
//			std::cout << "succesfully sent (as UDP) to client " << std::endl;
//			//deal with packet
//		}
//	}
//}
//void P2P_Network::receive_UDP() {
//	for (int i = 0; i < TCP_connections.size(); i++) {
//
//		sf::IpAddress senderIP = TCP_connections.at(i)->getRemoteAddress();
//		unsigned short senderPort = TCP_connections.at(i)->getRemotePort();
//
//		if (udpSocket.receive(incomingPacket, senderIP, senderPort) != sf::Socket::Done)
//		{
//			//error
//		}
//		else {
//			std::cout << "succesfully received (as UDP) from client " << std::endl;
//			//deal with packet
//		}
//	}
//}

bool P2P_Network::accept_TCP_new() {

	//accept new connection
	if (listener.accept(peers.back()->socket) != sf::Socket::Done)
	{
		return false;
	}
	else {
		
		//immediately (before we set blocking false) get the connected peer's port
		sf::Packet connectedPeerListenerPort;
		peers.back()->socket.receive(connectedPeerListenerPort);
		sf::Uint16 pport;
		connectedPeerListenerPort >> pport;

		//set up the new peer's info
		peers.back()->IpAddress = peers.back()->socket.getRemoteAddress();
		peers.back()->localPort = peers.back()->socket.getLocalPort();
		peers.back()->remotePort = peers.back()->socket.getRemotePort();
		peers.back()->TCP_listener_Port = (unsigned short)pport; //necessary for sharing with peers if they want to connect to them
		peers.back()->socket.setBlocking(false);

		std::cout << "succesfully accepted Client " << peers.size() <<" at ip "<< peers.back()->IpAddress <<"with listener port "<<pport << std::endl;

		//share peers with the new 
		sharePeers();   //exchange peers

		//set up another slot for a potential peer
		peers.push_back(new Peer);

		return true;
	}


}

bool P2P_Network::connect_TCP_to(sf::IpAddress hostIP, unsigned short port) {
	
	//tries to connect

	sf::Socket::Status status = peers.back()->socket.connect(hostIP, port);
	if (status != sf::Socket::Done)
	{
		std::cout << "[ ! ] Couldn't connect. [ ! ]\n";
		return false;
	}
	else {
		//immediately share this app's listener port
		sf::Packet myListenerPortinfo;
		myListenerPortinfo << (sf::Uint16)listener.getLocalPort();
		peers.back()->socket.send(myListenerPortinfo);


		peers.back()->socket.setBlocking(false);

		//set up another slot for a potential peer
		peers.push_back(new Peer);

		std::cout << "Succesfully connected to "<<hostIP<< " on list port "<<port<<std::endl;

		return true;
	}
}

void P2P_Network::receiveAll_TCP() {
	if (peers.size() == 0) { return; }
	sf::Packet tempPacket;

	for (int i = 0; i < peers.size() - 1; i++) { //-1 to compensate for the last vector slot being a blank
		if (peers.at(i)->socket.receive(tempPacket) != sf::Socket::Done)
		{
			//std::cout << "error in receive from client" << i + 1 << std::endl;
		}
		else {
			std::cout << "     RECEIVED sucessfully (as TCP) from client " << i+1 << std::endl;
			//deal with packet
			//peers.at(i)->receivedPackets.push(incomingPacket);

			decodePacket(tempPacket);

		}
	}
	
}

void P2P_Network::sendAll_TCP(sf::Packet packet) {
	if (peers.size() == 0) { return; }
	for (int i = 0; i < peers.size() - 1; i++) { //-1 to compensate for the last vector slot being a blank
		if (peers.at(i)->socket.send(packet) != sf::Socket::Done)
		{
			std::cout << "error in send to client" << i + 1 << std::endl;
		}
		else {
			std::cout << "SENT (as TCP) to client "<< i + 1 << " succesfully" <<  std::endl;
			
		}
	}
}

//function for the new connectee to get all the other connects
void P2P_Network::sharePeers() {

	std::cout << "Trying to share peers...\n ";
	sf::Packet peerInfoPacket;
	sf::Uint16 nextData;

	for (int i = 0; i < peers.size() - 1; i++) { //-1 so as not to send client its own data

		peerNWinfo peerInfo;
		peerInfo.ipAddress = peers.at(i)->IpAddress.toInteger();

		nextData = NW_INFO;//next data is IP
		peerInfo.listenerPort = peers.at(i)->TCP_listener_Port;

		peerInfoPacket << nextData << peerInfo;

	}


	if (peers.back()->socket.send(peerInfoPacket) != sf::Socket::Done)
	{
		std::cout << "error in sharing existing peers with new peer";
	}
	else {
		std::cout << "success sharing peers with new peer.";
	}



}

void P2P_Network::decodePacket(sf::Packet packet) {

	sf::Uint32 int32holder;
	sf::Uint16 nextDataHeader;
	char * charbbuffer = new char;
	bool alreadyKnown = false;

	//deal with header here


	while (!packet.endOfPacket()) { //take data until there's no left
	
		packet >> nextDataHeader;//looks what data comes next (how to deal with it)
	
		switch (nextDataHeader) //what's the next data type
		{
		case 6: //NW info
			
			peerNWinfo nwinfo_holder;

			packet >> nwinfo_holder;

			for (int i = 0; i < peers.size() - 1; i++) {
				if (peers.at(i)->TCP_listener_Port == nwinfo_holder.listenerPort && peers.at(i)->IpAddress == sf::IpAddress(nwinfo_holder.ipAddress)) {
					alreadyKnown = true;
					break;
				}
			}

			std::cout << "connecting new peer\n";
			connect_TCP_to(sf::IpAddress(nwinfo_holder.ipAddress), (unsigned short)nwinfo_holder.listenerPort);

			break;
		case 1:
			std::cout << "sent " << std::endl;
			break;
		case 0:
			std::cout << "zero " << std::endl;
			break;
		default:
			return;
			break;
		}
	
	}

	std::cout << "\ndecoding went well\n";
}

