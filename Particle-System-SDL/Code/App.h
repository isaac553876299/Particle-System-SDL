#ifndef _APP_H_
#define _APP_H_

#include <iostream>
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
	Timer() { Start(); };
	void Start() { time = SDL_GetTicks(); };
	inline Uint32 msRead() { return (SDL_GetTicks() - time); };
	inline float sRead() { return float((SDL_GetTicks() - time) / 1000.f); };
};

class App
{
public:
	bool active;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	float dt;
	Timer timer;

	Uint32 seconds;
	int fpsCount;
	int fps;

	//KeyStates::IDLE_0,DOWN_1,REPEAT_2,UP_3
	Mouse mouse;
	int keyboard[200];

	App();
	~App();
	void Input();
	void Update();
	void Draw();
	bool Running();
};

#endif
