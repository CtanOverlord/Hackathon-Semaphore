/*
* Copyright (c) 2006-2007 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include <Box2D/Box2D.h>
#include <stdio.h>
#include <SDL.h>
#include <iostream>
#include <SDL_ttf.h>
#include <InputHandler.h>       // Added by Alison
#include <Render.h> //Added by Jonathan
#include <Player.h>
#include <Level.h>
#include <vector>
#include <Fireball.h>
#include <MenuScene.h>
#include <Cannon.h>
#include <Button.h>
#include <Door.h>
#include <SDL_timer.h>

#define FRAME_VALUES 10

// An array to store frame times:
Uint32 frametimes[FRAME_VALUES];

// Last calculated SDL_GetTicks
Uint32 frametimelast;

// total frames rendered
Uint32 framecount;

// the value you want
float framespersecond;

// This function gets called once on startup.
void fpsinit() {

	// Set all frame times to 0ms.
	memset(frametimes, 0, sizeof(frametimes));
	framecount = 0;
	framespersecond = 0;
	frametimelast = SDL_GetTicks();

}

void fpsthink() {

	Uint32 frametimesindex;
	Uint32 getticks;
	Uint32 count;
	Uint32 i;

	// frametimesindex is the position in the array. It ranges from 0 to FRAME_VALUES.
	// This value rotates back to 0 after it hits FRAME_VALUES.
	frametimesindex = framecount % FRAME_VALUES;

	// store the current time
	getticks = SDL_GetTicks();

	// save the frame time value
	frametimes[frametimesindex] = getticks - frametimelast;

	// save the last frame time for the next fpsthink
	frametimelast = getticks;

	// increment the frame count
	framecount++;

	// Work out the current framerate

	// The code below could be moved into another function if you don't need the value every frame.

	// I've included a test to see if the whole array has been written to or not. This will stop
	// strange values on the first few (FRAME_VALUES) frames.
	if (framecount < FRAME_VALUES) {

		count = framecount;

	}
	else {

		count = FRAME_VALUES;

	}

	// add up all the values and divide to get the average frame time.
	framespersecond = 0;
	for (i = 0; i < count; i++) {

		framespersecond += frametimes[i];

	}

	framespersecond /= count;

	// now to make it an actual frames per second value...
	framespersecond = 1000.f / framespersecond;

}

static const float SCALE = 30.f;

void logSDLError(const std::string &msg) {
	//std::cout << msg << " error: " << SDL_GetError() << std::endl;
}



int QuitWithError(const std::string &msg) {
	//std::cout << msg << " error: " << SDL_GetError() << std::endl;
	SDL_Delay(1000); //give user a chance to read error
	SDL_Quit();
	return 1;
}


/**
* Render the message we want to display to a texture for drawing
* @param message The message we want to display
* @param fontFile The font we want to use to render the text
* @param color The color we want the text to be
* @param fontSize The size we want the font to be
* @param renderer The renderer to load the texture in
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
*/

std::vector<b2Vec2> timeArray(60);

int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;
InputHandler inputHandler = InputHandler();

class Physics
{
public:
	unsigned int update(void* data)
	{
		//DEBUG_MSG("Physics Updating");
		return 0;
	}
};

class ProcessRespawn
{
public:
	static int run(void* data)
	{
		//Physics physics;
		unsigned int result = 0;
		//DEBUG_MSG("Process Physics Running");

		Uint32 update, elapsed = 0;
		update = 2000;

		while (true)
		{
			elapsed = SDL_GetTicks();

			if (elapsed >= update)
			{
				if (SDL_mutexP(mutex) == -1)
				{
					//DEBUG_MSG("Process Physics Spinning");
				}
				//DEBUG_MSG("Process Physics Locked Mutex");
				//result = physics.update(data);

				if (SDL_mutexV(mutex) == -1)
				{
					//DEBUG_MSG("Unlock failed Spinning");
				}
				//DEBUG_MSG("Process Physics Unlocked Mutex");
				elapsed = 0;
			}
		}
		return result;
	}
};

int main(int, char**) {


	//initialise SDL system and the video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		QuitWithError("SDL_Init Error: ");

	}

	//Create a Window
	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		QuitWithError("SDL_CreateWindow Error: ");

	}

	//SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);

	Mix_Music *music = NULL;
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1;
	std::string basepath(SDL_GetBasePath());
	music = Mix_LoadMUS((basepath + "background.wav").c_str());
	Mix_PlayMusic(music, -1);

	Render* renderer = new Render(win);

	//Main loop flag
	bool quit = false;
	b2Vec2 Gravity(0.f, 0.0098f);
	b2World World(Gravity);

	string bgPath = basepath + "background22.bmp";
	SDL_Surface* back = SDL_LoadBMP(bgPath.c_str());
	SDL_Rect* backGroundRect = renderer->AddSurfaceToRenderer(back, 0, -600, 1.0f);

	Button button = Button(-200, -50, World, renderer);
	Door door = Door(600, -100, renderer);

	Level level = Level(World, renderer);
	Player player = Player(100, 500, World, renderer);

	vector<Cannon*> cannons;
	cannons.push_back(new Cannon(170, 350, World, renderer, 1));
	cannons.push_back(new Cannon(1010, 50, World, renderer, 2));
	cannons.push_back(new Cannon(170, 450, World, renderer, 1));
	cannons.push_back(new Cannon(1010, 150, World, renderer, 2));
	cannons.push_back(new Cannon(170, 550, World, renderer, 1));
	cannons.push_back(new Cannon(1010, 250, World, renderer, 2));

	MenuScene* menu = new MenuScene(1200, 100, renderer);
	SDL_Event e;

	//thread t1(&Process::run, Process((*(game)))); //Passing references
	//t1.detach(); //detaches from SDL mainline

	float prevTicks = SDL_GetTicks();
	float currentTicks = SDL_GetTicks();
	float FPS = 0;

	int fpsTimer = 0;

	SDL_Init(0);

	//game loop
	while (!quit) {

		World.Step(1 / 60.f, 6, 2);
		while (SDL_PollEvent(&e) != 0) {
			if (inputHandler.CheckInput(SDLK_ESCAPE, e)) {
				quit = true;
			}
		}
		if (menu->playBool == false && menu->quitBool == false) {
			renderer->DrawMenuScene();
			menu->Update(renderer);
		}
		
		if (menu->playBool == true)
		{
			//PLAY GAME STATE
			int dir = player.Move(inputHandler, e);

			for (int i = 0; i < cannons.size(); i++)
			{
				cannons.at(i)->Update();
			}

			SDL_Rect rec(player.spriteRect);
			rec.y = player.GetY();

			for (int j = 0; j < cannons.size(); j++)
			{
				for (int i = 0; i < cannons.at(j)->fireballs.size(); i++)
				{
					if (cannons.at(j)->fireballs.at(i)->CheckCollision(&rec) == true)
					{
						std::cout << "Collision Detected!" << std::endl;
						player.Respawn();
						button.setOnce(false);
						button.buttonBody->SetTransform(b2Vec2(880 / SCALE, 39 / SCALE), 0);
						door.spriteRect->x = -1000;
						door.spriteRect->y = -1000;
						player.prevPosX.clear();
						player.prevPosY.clear();
						player.count = 0;
					}
				}
			}

			button.Update();
			
			if (button.CheckCollision(&rec) == true)
			{
				std::cout << "Collision Detected!" << std::endl;
				button.collision = true;
				button.spriteRect->x = -2000;
				button.spriteRect->y = -2000;
				button.buttonBody->SetTransform(b2Vec2(-2000, -2000), 0);
				//door.Draw(renderer);
			}
			if (door.CheckCollision(&rec) == true)
			{
				button.buttonBody->SetTransform(b2Vec2(880 / SCALE, 39/ SCALE), 0);
				std::cout << "Collision Detected!" << std::endl;
				player.Respawn();
				button.setOnce(false);
				door.spriteRect->x = -1000;
				door.spriteRect->y = -1000;
				player.prevPosX.clear();
				player.prevPosY.clear();
				player.count = 0;
				player.Respawn();
				menu->playBool = false;
				menu->quitBool = false;
				menu->backGroundRect->x = 0;
				menu->current = 0;
				button.collision = false;
			}
			if (button.collision == false)
			{
				door.DrawCage(renderer);
			}
			if (button.collision == true)
			{
				door.DrawNoCage(renderer);
			}

			int ticks = SDL_GetTicks();
			int seconds = ticks / 50;
			int sprite = seconds % 8;
			renderer->Update(player.srcRect, player.dstRect, *player.LeftTexture, *player.RightTexture, *player.StandTexture, sprite, dir, player.moving, player.GetY());
			player.dstRect.w = player.spriteRect.w;
			player.dstRect.h = player.spriteRect.h;
			player.dstRect.x = player.spriteRect.x;
			player.dstRect.y = player.spriteRect.y + 5;
		}

		if (menu->quitBool == true)
		{
			quit = true;
		}

		fpsthink();

		if (fpsTimer == 60)
		{
			printf("%f\n", framespersecond);
			fpsTimer = 0;
		}

		fpsTimer++;
	}
	SDL_DestroyRenderer(renderer->ren);
	SDL_DestroyWindow(win);
	SDL_DestroyMutex(mutex);

	SDL_Quit();
	return 0;
}

