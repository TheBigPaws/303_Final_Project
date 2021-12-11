#include "P2P_Network.h"

void P2P_Network::setup() {
	if (isSetup) {
		return;
	}
	isSetup = true;
	//peers.push_back(new Peer);

	// bind the listener to a port
	if (mylistener.listen(sf::Socket::AnyPort) != sf::Socket::Done)
	{
		std::cout << "error setting up listener\n";
	}
	else {
		std::cout << "succesfully set up listener to port " << mylistener.getLocalPort() << std::endl;
	}
	mylistener.setBlocking(false);

	//info
	std::cout << "This device's local IP address is " << sf::IpAddress::getLocalAddress() << std::endl;
	std::cout << "This device's public IP address is " << sf::IpAddress::getPublicAddress() << std::endl;
	std::cout << "\n*** Setup complete ***\n\n" << std::endl;

	myInfo.IpAddress = sf::IpAddress::getLocalAddress();
	myInfo.TCP_listener_Port = mylistener.getLocalPort();

}


bool P2P_Network::accept_TCP_new() {

	//accept new connection
	if (mylistener.accept(peers.back()->socket) != sf::Socket::Done)
	{
		//fail
		return false;
	}
	else {
		
		//immediately (before we set blocking false) get the connected peer's port
		sf::Packet connectedPeerInfo;
		sf::Uint16 should_share_p, pport;
		std::string acceptedPeerName;
		peers.back()->socket.receive(connectedPeerInfo);
		connectedPeerInfo >> pport;
		connectedPeerInfo >> acceptedPeerName;
		connectedPeerInfo >> should_share_p;

		//set up the new peer's info
		peers.back()->IpAddress = peers.back()->socket.getRemoteAddress();
		peers.back()->TCP_localPort = peers.back()->socket.getLocalPort();
		peers.back()->TCP_remotePort = peers.back()->socket.getRemotePort();
		peers.back()->name = acceptedPeerName;
		peers.back()->TCP_listener_Port = (unsigned short)pport; 

		sf::Packet myName;
		myName << myInfo.name;
		peers.back()->socket.send(myName);

		peers.back()->socket.setBlocking(false);
		std::cout << "succesfully accepted Client " << peers.size() <<" at ip "<< peers.back()->IpAddress <<" with listener port "<< peers.back()->TCP_listener_Port << std::endl;

		//share peers with the new 
		if (should_share_p) {
		sharePeers();   //exchange peers
		}

		//set up another slot for a potential peer
		peers.push_back(new Peer);
		return true;
	}

	return false;
}

bool P2P_Network::connect_TCP_to(sf::IpAddress hostIP, unsigned short port,bool sharePeers_) {
	
	//tries to connect

	if (peers.size() == 0) {
		return false;
	}

	sf::Socket::Status status = peers.back()->socket.connect(hostIP, port);
	if (status != sf::Socket::Done)
	{
		std::cout << "[ ! ] Couldn't connect. [ ! ]\n";
		return false;
	}
	else {
		//peers.back()->socket.setBlocking(false);
		//immediately share this app's listener port
		sf::Packet myListenerPortinfo;
		myListenerPortinfo << (sf::Uint16)mylistener.getLocalPort();
		myListenerPortinfo << myInfo.name;
		myListenerPortinfo << (sf::Uint16)sharePeers_;


		peers.back()->IpAddress = peers.back()->socket.getRemoteAddress();
		peers.back()->TCP_localPort = peers.back()->socket.getLocalPort();
		peers.back()->TCP_remotePort = peers.back()->socket.getRemotePort();
		peers.back()->TCP_listener_Port = port;

		peers.back()->socket.send(myListenerPortinfo);

		sf::Packet namePacket;

		peers.back()->socket.receive(namePacket);
		std::string theirName;

		namePacket >> theirName;
		peers.back()->name = theirName;

		peers.back()->socket.setBlocking(false);

		//set up another slot for a potential peer
		peers.push_back(new Peer);

		std::cout << "Succesfully connected to "<<hostIP <<"on port "<<port << " whith name "<< theirName << std::endl;

		return true;

	}
	return false;
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
			//std::cout << "     RECEIVED sucessfully (as TCP) from client " << i+1 << std::endl;

			peers.at(i)->receivedPackets.push(tempPacket);

		}
	}
}

void P2P_Network::sendAll_TCP() {
	if (peers.size() <= 1) { return; } //one is bad cause it's the empty peer
	bool keep_sending = true;


	while (keep_sending) {
		keep_sending = false;//while stopper
		for (int i = 0; i < peers.size() - 1; i++) { //-1 to compensate for the last vector slot being a blank
			if (peers.at(i)->outgoingPackets.size() > 0) {//if any of the peers still has something to send
				keep_sending = true;//keep sending
				if (peers.at(i)->socket.send(peers.at(i)->outgoingPackets.front()) != sf::Socket::Done)
				{
					std::cout << "error in send to client" << i + 1 << std::endl;
					someoneDisconnected = true;
					disconnectedName = peers.at(i)->name;
					peers.erase(peers.begin() + i);
					break;
				}
				else {
					//std::cout << "SENT (as TCP) to client " << i + 1 << " succesfully" << std::endl;
					peers.at(i)->outgoingPackets.pop();
				}
			}
		}
	}
}

//function for the new connectee to get all the other connects
void P2P_Network::sharePeers() {

	if (peers.size() == 1) {//there's nothing to share
		return;
	}
	sf::Packet peerInfoPacket;
	header hdr_;
	hdr_.senderName = myName;
	hdr_.game_elapsed_time = 0.0f;
	hdr_.information_type = (sf::Uint16)NW_INFO;
	hdr_.information_amount = (sf::Uint16)peers.size() - 1; //-1 so as not to send client its own data

	peerInfoPacket << hdr_;

	for (int i = 0; i < hdr_.information_amount; i++) { 
		peerNWinfo info;
		info.ipAddress = peers.at(i)->IpAddress.toInteger();
		info.listenerPort = (sf::Uint16)peers.at(i)->TCP_listener_Port;
		info.name = peers.at(i)->name;

		peerInfoPacket << info;
		std::cout << "sharing data about a peer with IP "<<peers.at(i)->IpAddress << " with listener port " << peers.at(i)->TCP_listener_Port << std::endl;
	}

	if (peers.back()->socket.send(peerInfoPacket) != sf::Socket::Done)
	{
		std::cout << "error in sharing existing peers with new peer\n";
	}
	else {
		std::cout << "success sharing peers with new peer.\n";
	}

}

bool P2P_Network::anyPacketsToRead() {
	for (int i = 0; i < peers.size() - 1; i++) { //-1 to compensate for the last vector slot being a blank
		if (peers.at(i)->receivedPackets.size() > 0) {//if any of the peers still has something to send
			return true;
		}
	}
	return false;
}


sf::Packet P2P_Network::getPacketToRead() {
	sf::Packet retPacket;
	for (int i = 0; i < peers.size() - 1; i++) { //-1 to compensate for the last vector slot being a blank
		if (peers.at(i)->receivedPackets.size() > 0) {//if any of the peers still has something to send
			retPacket = peers.at(i)->receivedPackets.front();
			peers.at(i)->receivedPackets.pop();
			break;
		}
	}
	return retPacket;
}

void P2P_Network::pushOutPacket_all(sf::Packet packet) {
	for (int i = 0; i < peers.size() - 1; i++) {
		peers.at(i)->outgoingPackets.push(packet);
	}
}