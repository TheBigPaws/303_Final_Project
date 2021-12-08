#include "Bullet.h"
#include <iostream>
Bullet::Bullet(sf::Vector2f position_, sf::Vector2f direction_) {
	bullet = sf::CircleShape(10);
	bullet.setOutlineColor(sf::Color::Black);
	bullet.setOutlineThickness(3);
	position = position_;
	position.x -= bullet.getRadius();
	position.y -= bullet.getRadius();
	direction = direction_;
	bullet.setPosition(position);
	id = (sf::Uint16)(rand() % 65000); // 0 to 65000, because 65535 is max of uint16 

}

Bullet::Bullet(sf::Vector2f position_, sf::Vector2f direction_, sf::Uint16 id_) {
	bullet = sf::CircleShape(10);
	bullet.setOutlineColor(sf::Color::Black);
	bullet.setOutlineThickness(3);
	position = position_;
	position.x -= bullet.getRadius();
	position.y -= bullet.getRadius();
	direction = direction_;
	bullet.setPosition(position);
	id = id_;

}

void Bullet::update(float dt) {
	position += direction * dt * 700.0f; //100 is bullet speed

	bullet.setPosition(position);
}