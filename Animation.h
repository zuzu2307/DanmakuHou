#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
class Animation
{
public:
	Animation(Texture* texture, Vector2f imageCount, float switchTime);
	~Animation();

	void Update(int row, float deltaTime, bool faceRight);

public:
	IntRect uvRect;


private:
	Vector2f imageCount;
	Vector2u currentImage;

	float totalTime;
	float switchTime;
};

