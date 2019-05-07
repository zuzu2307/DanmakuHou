#include "enemy.h"

//std::vector<Bullet> enemy01BulletMove;


enemy::enemy(Vector2f size,Vector2f position, Texture* texture,  Vector2f imageCount, float switchTime):
	animation(texture, imageCount, switchTime)
{
	this->bulletTexture = bulletTexture;
	this->speed = speed;
	this->VIEW_HEIGHT = VIEW_HEIGHT;
	this->VIEW_WIDTH = VIEW_WIDTH;
	this->clock = clock;
	this->position = position;
	this->mode = mode;
	this->faceRight = faceRight;
	this->row = row;

	bodyenemy.setSize(size);
	bodyenemy.setOrigin(bodyenemy.getSize() / 2.0f);
	bodyenemy.setPosition(position);
	bodyenemy.setTexture(texture);
}


enemy::~enemy()
{
}


void enemy::Update(Vector2f velocity,bool faceRight, unsigned int row,float deltaTime)
{
	this->Move(velocity);
	animation.Update(row, deltaTime, faceRight);
	bodyenemy.setTextureRect(animation.uvRect);
}

void enemy::Draw(RenderWindow & window)
{
	window.draw(bodyenemy);
}
void enemy::Move(Vector2f velocity)
{
	bodyenemy.move(velocity);
}
