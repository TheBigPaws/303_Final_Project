#include "Bullet.h"
#include <iostream>


//Bullet constructor shooting my bullets - assign them a random id
Bullet::Bullet(sf::Vector2f position_, sf::Vector2f direction_) {
	bullet = constructSphere(position, 10.0f, sf::Color::Yellow, 3);
	bullet.setOutlineThickness(3);
	position = position_;
	direction = direction_;
	id = (sf::Uint16)(rand() % 65000); // 0 to 65000, because 65535 is max of uint16 

}

//Bullet constructor for instantiating enemy bullets
Bullet::Bullet(sf::Vector2f position_, sf::Vector2f direction_, sf::Uint16 id_) {

	bullet = constructSphere(position, 10.0f, sf::Color::Yellow, 3);
	bullet.setOutlineThickness(3);
	position = position_;
	direction = direction_;
	id = id_;

}

void Bullet::update(float dt) {
	position += direction * dt * 700.0f; //700 is bullet speed

	bullet.setPosition(position);
}