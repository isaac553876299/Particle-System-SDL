#include "App.h"

App::App()
{
	active = true;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(WWIDTH, WHEIGHT, 0, &window, &renderer);

	mouse = { 0,0,0,0,0,0 };

	for (auto i : keyboard) i = 0;
	for (int i = 0; i < 200; i++) keyboard[i] = 0;

	seconds = SDL_GetTicks();
	fpsCount = 0;
	fps = 0;
}

App::~App()
{

	SDL_Quit();
}

bool App::Running()
{
	Input();
	Update();
	Draw();
	return active;
}

void App::Input()
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
		if (keyboard[i] == 3) keyboard[i] = 0;
		if (keyboardState[i])
		{
			if (keyboard[i] == 0 || keyboard[i] == 3) keyboard[i] = 1;
			if (keyboard[i] == 1) keyboard[i] = 2;
		}
		else if (keyboard[i] != 0) keyboard[i] = 3;
	}
}

void App::Update()
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

	//updates

	static char title[256];
	sprintf_s(title, 256, "fps(%d) dt(%.4f)",
		fps, dt);
	SDL_SetWindowTitle(window, title);
}

void App::Draw()
{
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	//draws

	SDL_RenderPresent(renderer);
}
