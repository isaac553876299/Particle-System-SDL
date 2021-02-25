#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <time.h>
#include "SDL.h"

#define RELEASE(x) { delete x; x = nullptr; }
#define RELEASE_ARRAY(x) { delete[] x; x = nullptr; }

#define WWIDTH 1280
#define WHEIGHT 720

#define FPS 60
#define _dt (1000/FPS)

struct Mouse { int x, y, offsx, offsy, stateL, stateR; };

struct Timer
{
	Uint32 time;

	Timer()
	{ Start(); };

	void Start()
	{ time = SDL_GetTicks(); };

	inline Uint32 msRead()
	{ return (SDL_GetTicks() - time); };

	inline float sRead()
	{ return float((SDL_GetTicks() - time) / 1000.f); };
};

#endif
