#include "ThreadingGlobals.h"

SDL_mutex* mutex = SDL_CreateMutex();

SDL_sem* dataLock = SDL_CreateSemaphore(2);