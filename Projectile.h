#pragma once
#include"Player.h"
class Projectile
{
public:
	Projectile(Texture *texture);
	~Projectile();
	RectangleShape bullets;
private:
	
	Player *player;

};

