//#include <SDL.h>
//#include <SDL_image.h>
//#include <SDL_thread.h>
//#include <SDL_timer.h>
//#include <iostream>
//#include <Game.h>
//#include <thread>
//#include <string>
//
//using namespace std;
//
//static SDL_mutex* mutex = SDL_CreateMutex();
//
//class Physics
//{
//public:
//	unsigned int update(void* data)
//	{
//		DEBUG_MSG("Physics Updating");
//		return 0;
//	}
//};
//
//class ArtificialIntelligence
//{
//public:
//	unsigned int update(void* data)
//	{
//		DEBUG_MSG("Artificial Intelligence Updating");
//		return 0;
//	}
//};
//
//class Process
//{
//public:
//	Process() {}
//	Process(const Game& game) : m_Game(game)
//	{
//
//	}
//	~Process() {};
//	void run()
//	{
//		DEBUG_MSG("Thread Running");
//		while (m_Game.IsRunning())
//		{
//			//DEBUG_MSG(".");
//		}
//	}
//private:
//	Game m_Game;
//};
//
//class ProcessPhysics
//{
//public:
//	static int run(void* data)
//	{
//		Physics physics;
//		unsigned int result = 0;
//		DEBUG_MSG("Process Physics Running");
//
//		Uint32 update, elapsed = 0;
//		update = 2000;
//
//		while (true)
//		{
//			elapsed = SDL_GetTicks();
//
//			if (elapsed >= update)
//			{
//				if (SDL_mutexP(mutex) == -1)
//				{
//					DEBUG_MSG("Process Physics Spinning");
//				}
//				DEBUG_MSG("Process Physics Locked Mutex");
//				result = physics.update(data);
//
//				if (SDL_mutexV(mutex) == -1)
//				{
//					DEBUG_MSG("Unlock failed Spinning");
//				}
//				DEBUG_MSG("Process Physics Unlocked Mutex");
//				elapsed = 0;
//			}
//		}
//		return result;
//	}
//};
//
//class ProcessArtificialIntelligence
//{
//public:
//	static int run(void* data)
//	{
//		ArtificialIntelligence ai;
//		unsigned int result = 0;
//		DEBUG_MSG("Process Artificial Intelligence Running");
//
//		Uint32 update, elapsed = 0;
//		update = 1000;
//
//		while (true)
//		{
//			elapsed = SDL_GetTicks();
//
//			if (elapsed >= update)
//			{
//				if (SDL_mutexP(mutex) == -1)
//				{
//					DEBUG_MSG("Process Artificial Intelligence Spinning");
//				}
//				DEBUG_MSG("Process Artificial Intelligence Locked Mutex");
//				result = ai.update(data);
//
//				if (SDL_mutexV(mutex) == -1)
//				{
//					DEBUG_MSG("Unlock failed Spinning");
//				}
//				DEBUG_MSG("Process Artificial Intelligence Unlocked Mutex");
//				elapsed = 0;
//			}
//		}
//		return result;
//	}
//};
//
//int main(int argc, char** argv) {
//
//	DEBUG_MSG("Game Object Created");
//
//	Game* game = new Game();
//
//	//Adjust screen positions as needed
//	game->Initialize("DGPP Skelatol", 300, 100, 800, 600, SDL_WINDOW_INPUT_FOCUS);
//	DEBUG_MSG("Game Initialised");
//
//	game->LoadContent();
//
//	thread t1(&Process::run, Process((*(game)))); //Passing references
//	t1.detach(); //detaches from SDL mainline
//
//	SDL_Thread* sdlThreadPhysics;
//	sdlThreadPhysics = SDL_CreateThread(&ProcessPhysics::run, "Physics Thread", game);
//
//
//	SDL_Thread* sdlThreadPhysicsAI;
//	sdlThreadPhysicsAI = SDL_CreateThread(&ProcessArtificialIntelligence::run, "AI Thread", game);
//
//	DEBUG_MSG("Game Loop Starting......");
//	while (game->IsRunning())
//	{
//		game->HandleEvents();
//		game->Update();
//		game->Render();
//	}
//
//	DEBUG_MSG("Calling Cleanup");
//	game->CleanUp();
//	game->UnloadContent();
//
//	SDL_DestroyMutex(mutex);
//
//	return 0;
//}