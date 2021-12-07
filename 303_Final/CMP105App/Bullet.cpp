#include "Bullet.h"

Bullet::Bullet(sf::Vector2f position_, sf::Vector2f direction_) {
	bullet = sf::CircleShape(10);
	bullet.setOutlineColor(sf::Color::Black);
	bullet.setOutlineThickness(3);
	position = position_;
	position.x -= bullet.getRadius();
	position.y -= bullet.getRadius();
	direction = direction_;
	bullet.setPosition(position);
	id = rand() % 1000000; // 0 to million 
}

void Bullet::update(float dt) {
	position += direction * dt * 700.0f; //100 is bullet speed

	bullet.setPosition(position);
}