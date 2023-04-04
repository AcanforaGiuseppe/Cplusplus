#include "Player.h"

Player::Player::Player()
{
	position = Vector2::Vec2(0, 0);
	number_of_lifes = 1;
	speed = 1;
	number_of_bombs = 1;
	score = 0;
}

void Player::Player::Update()
{
	texture->position = position;
}

Player::Player::~Player()
{
	delete texture;
}