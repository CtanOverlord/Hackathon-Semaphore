#include "Fireball.h"
static const float SCALE = 30.f;

int UpdateReset(void * data);
int Worker(void * data);

Fireball::Fireball(float x, float y, b2World& World, Render* renderer, float d)
{
	world = &World;

	initX = x;
	initY = y;

	dir = d;

	std::string basepath(SDL_GetBasePath());

	std::string imagePath;

	timer = 2;

	if (dir == 2)
	{
		imagePath = basepath + "spearL2.bmp";
	}
	else
	{
		imagePath = basepath + "spearR2.bmp";
	}
	sprite = SDL_LoadBMP(imagePath.c_str());

	if (spriteRect == NULL)
	{
		spriteRect = renderer->AddSurfaceToRenderer(sprite, x, y, 0.5f);
	}
	
	else
	{

	}
		
	CreateBody();

	threading = true;

	//threading = false;
	
	if (threading == true)
	{
		resetThread = SDL_CreateThread(Worker, "ResetThread", (void *) this);
	}
}

int UpdateReset(void *data)
{
	while (true) {
		if (SDL_mutexP(mutex) == -1)
		{
			std::cout << "Spinning" << std::endl;
		}
		else
		{
			std::cout << "Locked " << std::endl << (int)SDL_ThreadID() << std::endl;
			static_cast<Fireball*>(data)->CheckLife();

			if (SDL_mutexV(mutex) == -1)
			{
				std::cout << "Failed Spinning" << std::endl;
			}
			std::cout << "Unlocked" << std::endl;
			SDL_Delay(100);
		}
	}

	return 0;
}

int Worker(void* data)
{
	while (true) {
		//Pre thread random seeding
		srand(SDL_GetTicks());

		//SDL_Delay(100);

		//Lock 
		SDL_SemWait(dataLock);

		//"Work" 

		static_cast<Fireball*>(data)->CheckCall();

		//Unlock 
		SDL_SemPost(dataLock);

		//if (SDL_mutexP(mutex) == -1)
		//{
		//	//std::cout << "Spinning" << std::endl;
		//}
		//else
		//{
		//	//std::cout << "Locked " << std::endl << (int)SDL_ThreadID() << std::endl;

		//	//Lock 
		//	SDL_SemWait(dataLock);

		//	//"Work" 

		//	static_cast<Fireball*>(data)->CheckCall();

		//	//Unlock 
		//	SDL_SemPost(dataLock);

		//	if (SDL_mutexV(mutex) == -1)
		//	{
		//		//std::cout << "Failed Spinning" << std::endl;
		//	}
		//	//std::cout << "Unlocked" << std::endl;
		//	SDL_Delay(100);
		//}

	}

	return 0;
}

void Fireball::RemoveSurface()
{
}

void Fireball::CreateBody()
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(initX / SCALE, initY / SCALE);
	BodyDef.type = b2_dynamicBody;
	BodyDef.userData = "fireball";
	b2Body* Body = world->CreateBody(&BodyDef);
	Body->SetFixedRotation(true);
	Body->SetSleepingAllowed(false);
	Body->SetGravityScale(0.0f);

	b2PolygonShape Shape;
	Shape.SetAsBox(spriteRect->w /2 / SCALE, spriteRect->h / 2 / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 10.0f;
	FixtureDef.friction = 0.f;
	FixtureDef.shape = &Shape;
	FixtureDef.isSensor = true;
	Body->CreateFixture(&FixtureDef);

	fireballBody = Body;
}

void Fireball::Update(int dir)
{
	if (spriteRect != NULL)
	{
		if (dir == 2)
		{
			//fireballBody->SetTransform(b2Vec2(fireballBody->GetPosition().x - 0.0030f, fireballBody->GetPosition().y), 0);

			if (timer == 0)
			{
				spriteRect->x = spriteRect->x - 1.0f;
				timer = 4;
			}
			timer--;
		}
		if (dir == 1)
		{
			//fireballBody->SetTransform(b2Vec2(fireballBody->GetPosition().x + 0.0030f, fireballBody->GetPosition().y), 0);

			if (timer == 0)
			{
				spriteRect->x = spriteRect->x + 1.0f;
				timer = 4;
			}
			timer--;
		}

		//spriteRect->x = (fireballBody->GetPosition().x) * SCALE + 20;
		//spriteRect->y = (fireballBody->GetPosition().y) * SCALE + 20;

		fireballBody->SetTransform(b2Vec2(spriteRect->x / SCALE, spriteRect->y / SCALE), 0);
	}
	
	if (threading == false)
	{
		CheckCall();
	}
}

bool Fireball::CheckCollision(SDL_Rect* playerRect)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = spriteRect->x;
	rightA = spriteRect->x + spriteRect->w;
	topA = spriteRect->y;
	bottomA = spriteRect->y + spriteRect->h;

	//Calculate the sides of rect B
	leftB = playerRect->x;
	rightB = playerRect->x + playerRect->w;
	topB = playerRect->y;
	bottomB = playerRect->y + playerRect->h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

void Fireball::CheckCall()
{
	CheckLife();
	for (int i = 0; i < 300; i++)
	{
		for (int j = 0; j < 300; j++)
		{

		}
	}
}

void Fireball::CheckLife()
{
	if (spriteRect->x < 150 || spriteRect->x > 1010)
	{
		if (dir == 2)
		{
			//fireballBody->SetTransform(b2Vec2((spriteRect->x + 800) / SCALE, (spriteRect->y - 20) / SCALE), 0); // new Fireball(825, 379, World, renderer);
			spriteRect->x = spriteRect->x + 800;
			//spriteRect->x = initX;
		}
		if (dir == 1)
		{
			//fireballBody->SetTransform(b2Vec2((spriteRect->x - 800) / SCALE, (spriteRect->y - 20) / SCALE), 0); // new Fireball(825, 379, World, renderer);
			spriteRect->x = spriteRect->x - 800;
			//spriteRect->x = initX;
		}
		//return true;
	}
	//else
		//return false;
}
