#include "Player.h"
#include<iostream>

Player::Player(Vector2f size,Texture* BulletTexture,Texture* texture, Vector2f imageCount, float switchTime, float speed, float VIEW_HEIGHT,	float VIEW_WIDTH,Clock clock) :
	animation(texture,imageCount,switchTime)
{
	this->speed = speed;
	this->BulletTexture = BulletTexture;
	this->VIEW_HEIGHT = VIEW_HEIGHT;
	this->VIEW_WIDTH = VIEW_WIDTH;
	this->clock = clock;
	row = 0;
	faceRight = true;
	//player
	playerbody.setSize(Vector2f(40.0f, 70.0f));
	playerbody.setOrigin(playerbody.getSize() / 2.0f);
	playerbody.setPosition(550, VIEW_HEIGHT - playerbody.getTextureRect().height-45.0f);
	playerbody.setTexture(texture);

}


Player::~Player()
{
}

void Player::Update(bool canshoot, float deltaTime, float VIEW_HEIGHT, float VIEW_WIDTH, RenderWindow & window, Time timeElapsed)
{
	velocity.x = 0.0f;   //velocity.x *= 0.5f; slow stop
	velocity.y = 0.0f;
	this->timeElapsed = timeElapsed;
	if (canshoot)
	{
		this->Movement();
	}
	this->Border();
	animation.Update(row, deltaTime, faceRight);
	playerbody.setTextureRect(animation.uvRect);
	playerbody.move(velocity*deltaTime);
}

void Player::Draw(RenderWindow & window)
{
	window.draw(playerbody);
}

void Player::OnCollision(Vector2f direction)
{
	if (direction.x < 0.0f)
	{
		//Collision on the left
		velocity.x = 0.0f;
	}
	else if (direction.x > 0.0f)
	{
		//Collision on the Right
		velocity.x = 0.0f;
	}
	if (direction.y < 0.0f)
	{
		//Collision on the bottom
		velocity.y = 0.0f;
	}
	else if (direction.y > 0.0f)
	{
		//Collision on the top
		velocity.y = 0.0f;
	}
}

void Player::Movement()
{
	if (Keyboard::isKeyPressed(Keyboard::LControl))
	{
		speed = 125;
		slow = false;
	}
	if (Keyboard::isKeyPressed(Keyboard::LShift))
	{
		speed = 300;
		slow = true;
	}
	if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
	{
		velocity.x -= speed;
	}

	if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
	{
		velocity.x += speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::Up)||Keyboard::isKeyPressed(Keyboard::W))
	{
		velocity.y -= speed*1.5;
	}

	if (Keyboard::isKeyPressed(Keyboard::Down)|| Keyboard::isKeyPressed(Keyboard::S))
	{
		velocity.y += speed*1.5;
	}
	
	//move
	if (velocity.x == 0)
	{
		row = 0;
	}
	else
	{
		
		if (velocity.x > 0)
		{
			row = 2;
		}
		else
		{
			row = 1;
		}
	}
	//move
}

void Player::Border()
{
	//border
	if (playerbody.getPosition().x <= 115.0f) //Left
	{
		playerbody.setPosition(sf::Vector2f(115.0f, playerbody.getPosition().y));
	}
	if (playerbody.getPosition().y <= 45.0f) //Top
	{
		playerbody.setPosition(sf::Vector2f(playerbody.getPosition().x, 45.0f));
	}
	if (playerbody.getPosition().x >= 1000 -20) //Right
	{
		playerbody.setPosition(sf::Vector2f(1000 - 20, playerbody.getPosition().y));
	}
	if (playerbody.getPosition().y >= 1000 +45.0f) //bottom
	{
		playerbody.setPosition(sf::Vector2f(playerbody.getPosition().x, 1000 + 45.0f));
	}
	//BORDER
}





