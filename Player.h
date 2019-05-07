#pragma once
#include<SFML/Graphics.hpp>
#include "Animation.h"
#include"Collider.h"
#include<iostream>
#include"enemy.h"

using namespace sf;
class Bullet
{
public:
	RectangleShape shape;
	Vector2f currVelocity;
	float maxSpeed;
	Bullet(Texture *BulletTexture, Vector2f size)
	{
		shape.setSize(size);
		shape.setOrigin(shape.getSize() / 2.0f);
		shape.setTexture(BulletTexture);
	}
	Collider GetCollider() { return Collider(shape); }
	Vector2f GetPosition() { return shape.getPosition(); }

};
class Item
{
public:
	RectangleShape Itemshape;
	Vector2f ItemsVelocity;
	float maxSpeed;
	Item( Vector2f size)
	{
		Itemshape.setSize(size);
		Itemshape.setOrigin(Itemshape.getSize() / 2.0f);
		
	}
	Collider GetCollider() { return Collider(Itemshape); }
	Vector2f GetPosition() { return Itemshape.getPosition(); }

};
class Player
{
public:
	Player(Vector2f size,Texture* BulletTexture,Texture* texture, Vector2f imageCount, float switchTime, float speed, float VIEW_HEIGHT, float VIEW_WIDTH, Clock clock);
	~Player();

	void Update(bool canshoot, float deltaTime, float VIEW_HEIGHT,	float VIEW_WIDTH, RenderWindow & window, Time timeElapsed);
	void Draw(RenderWindow& window);
	void OnCollision(Vector2f direction);
	void Movement();
	void Border();
	RectangleShape playerbody;

	Vector2f GetPosition() { return playerbody.getPosition(); }
	Collider GetCollider() { return Collider(playerbody); }
	


private:

	Animation animation;
	
	float speed;
	float bulletspeed;
	bool faceRight;
	unsigned int row;
	bool slow = false;
	Texture *BulletTexture;
	float VIEW_HEIGHT;
	float VIEW_WIDTH;
    Time timeElapsed;
	Clock clock;
	//Bullet b1;

	

	Vector2f velocity;
	bool attack=false;
};

