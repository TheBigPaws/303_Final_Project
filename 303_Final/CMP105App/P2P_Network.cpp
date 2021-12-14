#include "P2P_Network.h"

void P2P_Network::setup() {
	
	//std::cout << "This device's public IP address is " << sf::IpAddress::getPublicAddress() << std::endl;


	// setup listener to any free port - safer than assigning a constant one
	if (mylistener.listen(sf::Socket::AnyPort) != sf::Socket::Done)
	{
		std::cout << "error setting up listener\n";
	}
	else {
		std::cout << "succesfully set up listener to port " << mylistener.getLocalPort() << std::endl;
	}
	mylistener.setBlocking(false);

	//debug info
	std::cout << "This device's local IP address is " << sf::IpAddress::getLocalAddress() << std::endl;
	std::cout << "\n*** Setup complete ***\n\n" << std::endl;

	//set up my info
	myInfo.IpAddress = sf::IpAddress::getPublicAddress();
	std::cout << "This device's public IP address is " << myInfo.IpAddress.toString() << std::endl;
	myInfo.TCP_listener_Port = mylistener.getLocalPort();

}

//returns bool indicating whether someone just connected succesfully 
bool P2P_Network::accept_TCP_new() {

	//accept new connection
	if (mylistener.accept(peers.back()->socket) != sf::Socket::Done)
	{
		//fail
		return false;
	}
	else {
		
		//immediately (before we set blocking false) receive the connected peer's info
		sf::Packet connectedPeerInfo;
		sf::Uint16 should_share_p, pport;
		std::string acceptedPeerName;
		peers.back()->socket.receive(connectedPeerInfo);
		connectedPeerInfo >> pport;
		connectedPeerInfo >> acceptedPeerName;
		connectedPeerInfo >> should_share_p;

		//set up the new peer's info
		peers.back()->IpAddress = peers.back()->socket.getRemoteAddress();
		peers.back()->name = acceptedPeerName;
		peers.back()->TCP_listener_Port = (unsigned short)pport; 

		
		sf::Packet response;

		//check if the name is already being used by someone else
		bool nameIsUsed = false;
		for (int i = 0; i < peers.size() - 1; i++) {
			if (peers.at(i)->name == acceptedPeerName) {
				nameIsUsed = true;
				break;
			}
		}

		//check if the name is already being used by me
		if (myInfo.name == acceptedPeerName) {
			nameIsUsed = true;
		}

		//send back the response
		response << myInfo.name << nameIsUsed;
		peers.back()->socket.send(response);

		
		if (!nameIsUsed) {//finish setting up new client with original name
			peers.back()->socket.setBlocking(false);
			std::cout << "succesfully accepted " << peers.back()->name << " at ip " << peers.back()->IpAddress << " with listener port " << peers.back()->TCP_listener_Port << std::endl;

			//share peers with the new 
			if (should_share_p) {
				sharePeers();   //exchange peers
			}

			//set up another slot for a potential peer
			peers.push_back(new Peer);
			return true;
		}else{//name was used, cut the connection
			std::cout << " Unsuccesfull connect from ip " << peers.back()->IpAddress << " due to same name alrready used." << std::endl;

			//delete connection to that client
			peers.pop_back();
			//set up another slot for a potential peer
			peers.push_back(new Peer);
			return false;//didn't accept new client succesfully - same name
		}
		
	}

	return false;
}

//return whether our connecting was succesul
connectResult P2P_Network::connect_TCP_to(sf::IpAddress hostIP, unsigned short port,bool sharePeers_) {
	

	sf::Socket::Status status = peers.back()->socket.connect(hostIP, port);
	if (status != sf::Socket::Done)
	{
		std::cout << "[ ! ] Couldn't connect. [ ! ]\n";
		return WRONG_INPUT;
	}
	else {
		//set up info about peer to whom we conneected
		peers.back()->IpAddress = peers.back()->socket.getRemoteAddress();
		peers.back()->TCP_listener_Port = port;

		//immediately share the our information
		sf::Packet myPeerInfo;

		myPeerInfo << (sf::Uint16)mylistener.getLocalPort();
		myPeerInfo << myInfo.name;
		myPeerInfo << (sf::Uint16)sharePeers_;

		peers.back()->socket.send(myPeerInfo);

		//get a response about the originality of our name
		sf::Packet response;
		peers.back()->socket.receive(response);
		
		std::string theirName;
		bool nameUsed;

		response >> theirName >> nameUsed;

		if (!nameUsed) {//my name is OK, finish connecting

			std::cout << "Succesfully connected to " << hostIP << "on port " << port << " whith name " << theirName << std::endl;

			peers.back()->name = theirName;
			peers.back()->socket.setBlocking(false);

			//set up another slot for a potential peer
			peers.push_back(new Peer);

			return SUCCESS;
		}
		else {//my name is already used,

			std::cout << "Name was already used" << std::endl;

			//delete connection to that client
			peers.pop_back();
			//set up another slot for a potential peer
			peers.push_back(new Peer);
			return NAME_ALREADY_USED; //we weren't succesful in connecting
		}

	}
	return WRONG_INPUT;
}

void P2P_Network::receiveAll_TCP() {

	sf::Packet tempPacket;

	for (int i = 0; i < peers.size() - 1; i++) { //-1 to compensate for the last vector slot being a blank
		if (peers.at(i)->socket.receive(tempPacket) == sf::Socket::Done){

			//push the receivedPacket into receivedQ of the respective Peer
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
			
			if (peers.at(i)->outgoingPackets.size() > 0) {//if this peer still has something to send
				keep_sending = true;//keep sending
				if (peers.at(i)->socket.send(peers.at(i)->outgoingPackets.front()) != sf::Socket::Done)
				{
					//if we couldn't send, means the connection was lost
					std::cout << "error in send to client" << i + 1 << std::endl;
					
					//delete connection with that client
					someoneDisconnected = true;
					disconnectedName = peers.at(i)->name;
					peers.erase(peers.begin() + i);
					break;
				}
				else {
					peers.at(i)->outgoingPackets.pop();
				}
			}
		}
	}
}

//function that shares my peers with the person connecting to me
void P2P_Network::sharePeers() {

	if (peers.size() == 1) {//there's nothing to share (1 is always blank)
		return;
	}

	sf::Packet peerInfoPacket;
	header hdr_;
	hdr_.senderName = myInfo.name;
	hdr_.game_elapsed_time = 0.0f;
	hdr_.information_type = (sf::Uint16)NW_INFO;
	hdr_.information_amount = (sf::Uint16)peers.size() - 1; //-1 so as not to send client its own data

	peerInfoPacket << hdr_; //put in the header 

	//add all connected pees' infos to the packet
	for (int i = 0; i < hdr_.information_amount; i++) { 
		
		peerNWinfo info; //stream overloaded struct that's passable to packets
		info.ipAddress = peers.at(i)->IpAddress.toInteger();
		info.listenerPort = (sf::Uint16)peers.at(i)->TCP_listener_Port;
		info.name = peers.at(i)->name;

		peerInfoPacket << info;
	}


	//send the peer data to the new connectee (always at the back of the vector)
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

//push a packet to everyone's send queue
void P2P_Network::pushOutPacket_all(sf::Packet packet) {
	for (int i = 0; i < peers.size() - 1; i++) {
		peers.at(i)->outgoingPackets.push(packet);
	}
}

//only push the send packet to specified player - private information send
void P2P_Network::pushOutPacket(std::string name, sf::Packet packet) {
	for (int i = 0; i < peers.size() - 1; i++) {
		if(peers.at(i)->name == name){
			peers.at(i)->outgoingPackets.push(packet);
			break;
		}
	}
}
