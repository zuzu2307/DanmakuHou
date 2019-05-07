#pragma once
#include"Player.h"

class enemy
{
public:
	enemy(Vector2f size,Vector2f position, Texture* texture, Vector2f imageCount, float switchTime);
	~enemy();

	void Update(Vector2f velocity,bool faceRight, unsigned int row, float deltaTime);
	void Draw(RenderWindow& window);
	
	Vector2f GetPosition() { return bodyenemy.getPosition(); }
	void Move(Vector2f velocity);
	Collider GetCollider() { return Collider(bodyenemy); }
	RectangleShape bodyenemy;
	

private:
	
	Animation animation;
	Texture *bulletTexture;
	unsigned int row;
	float speed;
	bool faceRight;
	float VIEW_HEIGHT;
	float VIEW_WIDTH;
	Time timeElapsed;
	Clock clock,clockT;
	float mode;
	
	Vector2f velocity;
	Vector2f position;

};

