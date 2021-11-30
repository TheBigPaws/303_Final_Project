#include "Level.h"

Level::Level(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;



	view1.reset(sf::FloatRect(0, 0, 1200, 675));

	mainMenu.setup(hwnd, in);
	lobby.setup(hwnd, in);

	test_pkt_hdr_.game_elapsed_time = 0.0f;
	test_pkt_hdr_.information_amount = 1;
	test_pkt_hdr_.information_type = 5;
	test_pkt_ << test_pkt_hdr_ << "test text";

	networkModule.setup();


}

Level::~Level()
{

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
		lobby.addPeer("testip", std::to_string(rand() % 500));
	}
}

// Update game objects
void Level::update(float dt)
{
	switch (gameState) {
		case MAIN_MENU: //MAIN MENU UPDATE
			mainMenu.update(dt);
			if (mainMenu.attemptConnect) {//if player clicked enter on the connect screen
				if (networkModule.connect_TCP_to(sf::IpAddress(mainMenu.getEnteredIP()), (unsigned short)std::stoi(mainMenu.getEnteredPort()), true)) {
					mainMenu.attemptConnect = false;
					mainMenu.goToLobby = true;
					
				}
				else {
					mainMenu.attemptConnect = false;
					mainMenu.resetInput(1);

				}
			}
			if (mainMenu.goToLobby) {
				lobby.addPeer(networkModule.getMyIP(), networkModule.getMyPort());
				gameState = LOBBY;
			}
		break;

		case LOBBY: //LOBBY CODE
			networkModule.accept_TCP_new();
			for (int i = 0; i < networkModule.getPeerCount(); i++) {
				lobby.addPeer(networkModule.getPeerIP(i), networkModule.getPeerPort(i)); //display peers
			}
			lobby.update(dt);
		break;
	}
	networkModule.receiveAll_TCP();
	networkModule.decodePackets();
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
	}

	
	endDraw();
}

// Begins rendering to the back buffer. Background colour set to light blue.
void Level::beginDraw()
{
	window->clear(sf::Color(100, 149, 237));
}

// Ends rendering to the back buffer, and swaps buffer to the screen.
void Level::endDraw()
{
	window->display();
}