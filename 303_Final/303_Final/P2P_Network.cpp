#include "P2P_Network.h"

void P2P_Network::setup() {
	
	peers.push_back(new Peer);

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
}


void P2P_Network::accept_TCP_new() {

	//accept new connection
	if (mylistener.accept(peers.back()->socket) != sf::Socket::Done)
	{
		//fail
	}
	else {
		
		//immediately (before we set blocking false) get the connected peer's port
		sf::Packet connectedPeerListenerPort;
		sf::Uint16 should_share_p, pport;
		peers.back()->socket.receive(connectedPeerListenerPort);
		connectedPeerListenerPort >> pport;
		connectedPeerListenerPort >> should_share_p;

		//set up the new peer's info
		peers.back()->IpAddress = peers.back()->socket.getRemoteAddress();
		peers.back()->TCP_localPort = peers.back()->socket.getLocalPort();
		peers.back()->TCP_remotePort = peers.back()->socket.getRemotePort();
		peers.back()->TCP_listener_Port = (unsigned short)pport; 
		peers.back()->socket.setBlocking(false);

		std::cout << "succesfully accepted Client " << peers.size() <<" at ip "<< peers.back()->IpAddress <<" with listener port "<< peers.back()->TCP_listener_Port << std::endl;

		//share peers with the new 
		if (should_share_p) {
		sharePeers();   //exchange peers
		}

		//set up another slot for a potential peer
		peers.push_back(new Peer);

	}


}

void P2P_Network::connect_TCP_to(sf::IpAddress hostIP, unsigned short port,bool sharePeers_) {
	
	//tries to connect

	if (peers.size() == 0) {
		return;
	}

	sf::Socket::Status status = peers.back()->socket.connect(hostIP, port);
	if (status != sf::Socket::Done)
	{
		std::cout << "[ ! ] Couldn't connect. [ ! ]\n";
	}
	else {
		//immediately share this app's listener port
		sf::Packet myListenerPortinfo;
		myListenerPortinfo << (sf::Uint16)mylistener.getLocalPort();
		myListenerPortinfo << (sf::Uint16)sharePeers_;


		peers.back()->IpAddress = peers.back()->socket.getRemoteAddress();
		peers.back()->TCP_localPort = peers.back()->socket.getLocalPort();
		peers.back()->TCP_remotePort = peers.back()->socket.getRemotePort();
		peers.back()->TCP_listener_Port = port;

		peers.back()->socket.send(myListenerPortinfo);


		peers.back()->socket.setBlocking(false);

		//set up another slot for a potential peer
		peers.push_back(new Peer);

		std::cout << "Succesfully connected to "<<hostIP <<"on port "<<port << std::endl;



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
				}
				else {
					std::cout << "SENT (as TCP) to client " << i + 1 << " succesfully" << std::endl;
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
	hdr_.game_elapsed_time = 0.0f;
	hdr_.information_type = (sf::Uint16)NW_INFO;
	hdr_.information_amount = (sf::Uint16)peers.size() - 1; //-1 so as not to send client its own data

	peerInfoPacket << hdr_;

	for (int i = 0; i < hdr_.information_amount; i++) { 
		peerNWinfo info;
		info.ipAddress = peers.at(i)->IpAddress.toInteger();
		info.listenerPort = (sf::Uint16)peers.at(i)->TCP_listener_Port;

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

void P2P_Network::decodePackets() {

	sf::Uint32 int32holder;
	char * charbbuffer = new char;
	sf::Uint16 nextDataHeader;
	peerNWinfo info_;
	//deal with header here
	
	header header_;
	sf::Packet packet_;

	bool keep_decoding = true;

	while (keep_decoding) {
		keep_decoding = false;//while stopper
		for (int i = 0; i < peers.size() - 1; i++) { //-1 to compensate for the last vector slot being a blank
			if (peers.at(i)->receivedPackets.size() > 0) {//if any of the peers still has something to send
				keep_decoding = true;//keep sending
				packet_ = peers.at(i)->receivedPackets.front();
				packet_ >> header_;
				for (int i = 0; i < header_.information_amount; i++) {
					switch (header_.information_type) {
					case NW_INFO:
						packet_ >> info_;
						connect_TCP_to(sf::IpAddress(info_.ipAddress), (unsigned short)info_.listenerPort, 0);
						break;
					case TEST:
						packet_ >> charbbuffer;
						std::cout << charbbuffer << std::endl;
						break;
					default:
						break;
					}
				}
				peers.at(i)->receivedPackets.pop();

			}
		}
	}
	//std::cout << "\ndecoding went well\n";
}



void P2P_Network::pushOutPacket_all(sf::Packet packet) {
	for (int i = 0; i < peers.size() - 1; i++) {
		peers.at(i)->outgoingPackets.push(packet);
	}
}