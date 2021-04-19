#include <iostream>
#include <time.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "pugixml.hpp"

#include "ParticlesEngine.h"

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

	float scale = 1.0f;
	float camerax = 0.0f;
	float cameray = 0.0f;

	float dt;
	Timer timer;
	Uint32 seconds = SDL_GetTicks();
	int fpsCount = 0;
	int fps = 0;

	int* mouse = (int*)calloc(4, sizeof(int));
	int* keyboard = (int*)calloc(200, sizeof(int));

	ParticleSystem* particleSystem = new ParticleSystem;

	TTF_Font* font = TTF_OpenFont("Assets/Fonts/Kurale-Regular.ttf", 72);



	while (active)
	{
		dt = timer.sRead();
		timer.Start();
		if (dt < 1000 / 60) SDL_Delay(1000 / 60 - dt);

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
				if (event.button.button == SDL_BUTTON_MIDDLE) scale = 1.0f;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) mouse[2] = 3;
				if (event.button.button == SDL_BUTTON_RIGHT) mouse[3] = 3;
				break;
			case SDL_MOUSEWHEEL:
				if (event.wheel.y > 0) scale += 0.1f;
				else if (event.wheel.y < 0) scale -= 0.1f;
				break;
			}

		const Uint8* keys = SDL_GetKeyboardState(0);

		if (keys[SDL_SCANCODE_ESCAPE]) active = false;

		int keyMap[2][4] = { 0,3,3,0,1,2,2,1 };
		for (int i = 0; i < 200; ++i) keyboard[i] = keyMap[(int)(keys[i])][keyboard[i]];

		if (keyboard[SDL_SCANCODE_R] == 1) scale = 1.0f;
		SDL_RenderSetScale(renderer, scale, scale);
		if (mouse[3] == 2)
		{
			camerax += (mouse[0] - camerax) * dt;
			cameray += (mouse[1] - cameray) * dt;
			SDL_Rect viewport{ camerax-640,cameray-360,1280,720 };
			SDL_RenderSetViewport(renderer, &viewport);
		}

		particleSystem->Update(dt, mouse, keyboard);

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		static char debug[256];
		sprintf_s(debug, 256, "Number of emitters: %d", particleSystem->CountEmitters());
		DrawFont(renderer, font, { 255,0,0,255 }, 20, 10, 0.5f, debug);
		sprintf_s(debug, 256, "Number of particles: %d", particleSystem->CountParticles());
		DrawFont(renderer, font, { 255,0,0,255 }, 20, 50, 0.5f, debug);

		particleSystem->Draw(renderer);

		SDL_RenderPresent(renderer);
	}

	TTF_CloseFont(font);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}
