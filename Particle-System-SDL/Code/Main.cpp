#include <iostream>
#include <time.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "pugixml.hpp"

#include "ParticlesEngine.h"

#define FPS 60

#define RELEASE(x) { delete x; x = nullptr; }
#define RELEASE_ARRAY(x) { delete[] x; x = nullptr; }

struct Timer
{
	Uint32 time;
	Timer() { Start(); };
	void Start() { time = SDL_GetTicks(); };
	Uint32 msRead() { return (SDL_GetTicks() - time); };
	float sRead() { return float((SDL_GetTicks() - time) / 1000.f); };
};

void DrawFont(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, int x, int y, float s, const char* text)
{
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);//TTF_RenderText_Shaded
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_Rect rect{ x,y,0,0 };
	SDL_QueryTexture(textTexture, 0, 0, &rect.w, &rect.h);
	rect.w *= s;
	rect.h *= s;
	SDL_RenderCopy(renderer, textTexture, 0, &rect);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}

int main(int argc, char** argv)
{
	bool active = true;

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	SDL_Window* window = SDL_CreateWindow("Particle Engine Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	float dt;
	Timer timer;
	Uint32 seconds = SDL_GetTicks();
	int fpsCount = 0;
	int fps = 0;

	int* mouse = (int*)calloc(4, sizeof(int));
	int* keyboard = (int*)calloc(200, sizeof(int));

	ParticleSystem* particleSystem = new ParticleSystem;

	while (active)
	{
		if (mouse[2] == 1) mouse[2] = 2;
		if (mouse[3] == 1) mouse[3] = 2;
		if (mouse[2] == 3) mouse[2] = 0;
		if (mouse[3] == 3) mouse[3] = 0;

		SDL_GetMouseState(&mouse[0], &mouse[1]);

		SDL_Event event;
		while (SDL_PollEvent(&event))
			switch (event.type)
			{
			case SDL_QUIT:
				active = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) mouse[2] = 1;
				if (event.button.button == SDL_BUTTON_RIGHT) mouse[3] = 1;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) mouse[2] = 3;
				if (event.button.button == SDL_BUTTON_RIGHT) mouse[3] = 3;
				break;
			}

		const Uint8* keys = SDL_GetKeyboardState(0);

		if (keys[SDL_SCANCODE_ESCAPE]) active = false;

		int keyMap[2][4] = { 0,3,3,0,1,2,2,1 };
		for (int i = 0; i < 200; ++i) keyboard[i] = keyMap[(int)(keys[i])][keyboard[i]];

		dt = timer.sRead();
		timer.Start();
		if (((1000 / FPS) - dt) > 0) SDL_Delay((1000 / FPS) - dt);

		++fpsCount;
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

		particleSystem->Update(dt, mouse, keyboard);

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		particleSystem->Draw(renderer);

		SDL_Texture* texture = IMG_LoadTexture(renderer, "Assets/Textures/54975857_p0.png");
		SDL_RenderCopy(renderer, texture, 0, 0);
		SDL_DestroyTexture(texture);

		TTF_Font* font = TTF_OpenFont("Assets/Fonts/Kurale-Regular.ttf", 72);
		DrawFont(renderer, font, { 255,0,0,255 }, 100, 100, 0.5f, "hello 3.1416");
		TTF_CloseFont(font);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}
