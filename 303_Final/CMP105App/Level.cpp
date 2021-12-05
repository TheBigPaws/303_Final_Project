#include "Level.h"

Level::Level(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;



	view1.reset(sf::FloatRect(0, 0, 1200, 675));

	mainMenu.setup(hwnd, in);
	lobby.setup(hwnd, in);
	game.setup(hwnd, in);

	test_pkt_hdr_.game_elapsed_time = 0.0f;
	test_pkt_hdr_.information_amount = 1;
	test_pkt_hdr_.information_type = 5;
	test_pkt_ << test_pkt_hdr_ << "test text";

	networkModule.setup();


}

Level::~Level()
{

}

void Level::decodePacket(sf::Packet packet) {

	sf::Uint32 int32holder;
	char* charbbuffer = new char;
	peerNWinfo info_;
	//deal with header here

	header header_;
	packet >> header_;
	for (int i = 0; i < header_.information_amount; i++) {
		switch (header_.information_type) {
		case NW_INFO:
			packet >> info_;
			if (networkModule.connect_TCP_to(sf::IpAddress(info_.ipAddress), (unsigned short)info_.listenerPort, 0)) {

			}
			break;
		case CHAT_MESSAGE:
			packet >> charbbuffer;
			if (gameState == LOBBY) {
				lobby.chat.addMessage(charbbuffer, sf::Color::White, header_.senderName);
			}
			//std::cout << charbbuffer << std::endl;
			break;
		default:
			break;
		}
	}
}

// handle user input
void Level::handleInput(float dt)
{
	if (input->isKeyDown(sf::Keyboard::Escape)) {
		window->close();
	}

	if (input->isKeyDown(sf::Keyboard::D)) {
		view1.move(100.0f * dt, 0.0f);		
	}

	if (input->isKeyDown(sf::Keyboard::A)) {
		view1.move(-100.0f * dt, 0.0f);
	}

	if (input->isKeyDown(sf::Keyboard::P)) {
		input->setKeyUp(sf::Keyboard::P);
		lobby.addPeer(networkModule.getMyInfo()->name, networkModule.getMyInfo()->IpAddress.toString(), std::to_string(networkModule.getMyInfo()->TCP_listener_Port));
	}
}

// Update game objects
void Level::update(float dt)
{
	switch (gameState) {
		case MAIN_MENU: //MAIN MENU UPDATE
			mainMenu.update(dt);
			if (mainMenu.attemptConnect) {//if player clicked enter on the connect screen
				mainMenu.attemptConnect = false;
				networkModule.setMyName(mainMenu.getEnteredName());

				if (networkModule.connect_TCP_to(sf::IpAddress(mainMenu.getEnteredIP()), (unsigned short)std::stoi(mainMenu.getEnteredPort()), true)) {
					mainMenu.goToLobby = true;
				}
				else {
					mainMenu.resetInput(true);

				}
			}
			if (mainMenu.goToLobby) {
				networkModule.setMyName(mainMenu.getEnteredName());
				lobby.addPeer(networkModule.getMyInfo()->name, networkModule.getMyInfo()->IpAddress.toString(), std::to_string(networkModule.getMyInfo()->TCP_listener_Port));
				gameState = LOBBY;
			}
		break;

		case LOBBY: //LOBBY CODE
			networkModule.accept_TCP_new();
			if (lobby.displayedPeerNr() != networkModule.getPeerCount()) {
				for (int i = 0; i < networkModule.getPeerCount(); i++) {
					lobby.addPeer(networkModule.getPeer(i)->name,networkModule.getPeer(i)->IpAddress.toString(), std::to_string(networkModule.getPeer(i)->TCP_listener_Port)); //display peers
				}
			}

			if (lobby.chat.sentSomething) {
				sf::Packet packet;
				header hdr_;
				hdr_.game_elapsed_time = 0.0f;
				hdr_.information_amount = 1;
				hdr_.information_type = CHAT_MESSAGE;
				hdr_.senderName = networkModule.getMyInfo()->name;
				packet << hdr_ << lobby.chat.sent_string;
				networkModule.pushOutPacket_all(packet);
				lobby.chat.sentSomething = false;
				lobby.chat.sent_string = "";
				networkModule.sendAll_TCP();
			}
			lobby.update(dt);
		break;

		case GAME:
			game.update(dt);
		break;

	}
	networkModule.receiveAll_TCP();
	if (networkModule.anyPacketsToRead()) {
		decodePacket(networkModule.getPacketToRead());
	}
}

// Render level
void Level::render()
{
	beginDraw();
	
	switch (gameState) {
		case MAIN_MENU:
			mainMenu.render();
		break;

		case LOBBY:
			lobby.render();
		break;

		case GAME:
			game.render();
		break;
	}

	
	endDraw();
}

// Begins rendering to the back buffer. Background colour set to light blue.
void Level::beginDraw()
{
	window->clear(sf::Color(100, 100, 100));
}

// Ends rendering to the back buffer, and swaps buffer to the screen.
void Level::endDraw()
{
	window->display();
}