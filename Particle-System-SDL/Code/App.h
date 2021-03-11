#ifndef _APP_H_
#define _APP_H_

#include "Utils.h"
#include "ParticlesEngine.h"

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

	Manager* engine = nullptr;

	App()
	{
		active = true;
		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_CreateWindowAndRenderer(WWIDTH, WHEIGHT, 0, &window, &renderer);
		srand(time(0));

		mouse = { 0,0,0,0,0,0 };

		for (auto i : keyboard) i = 0;
		for (int i = 0; i < 200; i++) keyboard[i] = 0;

		seconds = SDL_GetTicks();
		fpsCount = 0;
		fps = 0;

		engine = new Manager(5);
	}

	~App()
	{

		SDL_Quit();
	}

	bool Running()
	{
		Input();
		Update();
		Draw();
		return active;
	}

	void Input()
	{
		if (mouse.stateL == 1) mouse.stateL = 2;
		if (mouse.stateR == 1) mouse.stateR = 2;
		if (mouse.stateL == 3) mouse.stateL = 0;
		if (mouse.stateR == 3) mouse.stateR = 0;

		SDL_GetMouseState(&mouse.x, &mouse.y);

		SDL_Event event;
		while (SDL_PollEvent(&event))
			switch (event.type)
			{
			case SDL_QUIT:
				active = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) mouse.stateL = 1;
				if (event.button.button == SDL_BUTTON_RIGHT) mouse.stateR = 1;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) mouse.stateL = 3;
				if (event.button.button == SDL_BUTTON_RIGHT) mouse.stateR = 3;
				break;
			}

		const Uint8* keyboardState = SDL_GetKeyboardState(0);

		if (keyboardState[SDL_SCANCODE_ESCAPE])
			active = false;

		for (int i = 0; i < 200; i++)
		{
			if (keyboardState[i]) keyboard[i] = (keyboard[i] == 1 || keyboard[i] == 2) ? 2 : 1;
			else keyboard[i] = (keyboard[i] == 1 || keyboard[i] == 2) ? 3 : 0;
		}
	}

	void Update()
	{
		dt = timer.sRead();
		timer.Start();
		if (dt < _dt) SDL_Delay(_dt - dt);
		//if (_dt - dt > 0) SDL_Delay(_dt - dt);

		fpsCount++;
		if (SDL_GetTicks() - seconds > 1000)
		{
			seconds = SDL_GetTicks();
			fps = fpsCount;
			fpsCount = 0;
		}

		static char title[256];
		sprintf_s(title, 256, " | fps: %d | dt: %.3f | ",
			fps, dt);
		SDL_SetWindowTitle(window, title);

		engine->Input(mouse, keyboard);
		engine->Update(dt);
	}

	void Draw()
	{
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		engine->Draw(renderer);

		SDL_RenderPresent(renderer);
	}

};

#endif
