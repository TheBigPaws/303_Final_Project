#include <iostream>
#include <string>
#include <SFML/Network.hpp>
int main()
{
	//SERVER
	/*sf::TcpListener listener;

	// bind the listener to a port
	if (listener.listen(53000) != sf::Socket::Done)
	{
		std::cout << "error setting up listener\n";
	}
	else {
		std::cout << "succesfully set up listener\n";
	}*/

	//CLIENT
	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect(sf::IpAddress::getLocalAddress(), 53000);
	if (status != sf::Socket::Done)
	{
		// error...
	}
	else {
		std::cout << "succesfully connected client\n";
	}

	// accept a new connection
	/*sf::TcpSocket client;
	if (listener.accept(client) != sf::Socket::Done)
	{
		std::cout << "error accepting new connection\n";
	}
	else {
		std::cout << "succesfully accepted Client\n";
	}*/

	

	// TCP socket SEND:
	if (socket.send("message", 6) != sf::Socket::Done)
	{
		std::cout << "error in send\n";
	}
	else {
		std::cout << "succesfully sent message\n";
	}

	//TCP socket RECEIVE
	/*char data[6];
	std::size_t received;

	// TCP socket:
	if (client.receive(data, 6, received) != sf::Socket::Done)
	{
		std::cout << "error in receive\n";
	}
	else {
		std::cout << "succesfully received\n";
	}

	std::cout << "Received " << received << " bytes" << std::endl;*/	
}
