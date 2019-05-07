#include "Projectile.h"



Projectile::Projectile(Texture *texture)
{
	//bullet
	bullets.setSize(Vector2f(10.0f, 15.0f));
	bullets.setOrigin(bullets.getSize() / 2.0f);
	bullets.setPosition(player->GetPosition());
	bullets.setTexture(texture);
}


Projectile::~Projectile()
{
}
