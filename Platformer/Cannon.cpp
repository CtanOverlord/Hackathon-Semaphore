#include "Cannon.h"
static const float SCALE = 30.f;

Cannon::Cannon(float x, float y, b2World& World, Render* renderer, float d)
{
	initX = x;
	initY = y;

	dir = d;

	std::string basepath(SDL_GetBasePath());
	std::string imagePath;

	if (dir == 1)
	{
		imagePath = basepath + "cannon32.bmp";
	}
	else
	{
		imagePath = basepath + "cannon32.bmp";
	}

	sprite = SDL_LoadBMP(imagePath.c_str());

	if (spriteRect == NULL)
	{
		spriteRect = renderer->AddSurfaceToRenderer(sprite, initX, initY, 0.5f);
	}

	else
	{

	}

	fireballs.push_back(new Fireball(230, y - 5, World, renderer, dir));
	fireballs.push_back(new Fireball(660, y - 5, World, renderer, dir));
	//fireballs.push_back(new Fireball(858, y - 5, World, renderer, dir));
	//fireballs.push_back(new Fireball(0, y - 5, World, renderer, dir));

}

void Cannon::Update()
{
	for (int i = 0; i < fireballs.size(); i++)
	{
		fireballs.at(i)->Update(dir);
		//fireballs.at(i)->CheckLife();
	}
}