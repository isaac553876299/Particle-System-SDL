#ifndef _PARTICLESENGINE_H_
#define _PARTICLESENGINE_H_

#include <time.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "pugixml.hpp"
#include "List.h"

#define RELEASE(x) { delete x; x = nullptr; }
#define RELEASE_ARRAY(x) { delete[] x; x = nullptr; }

struct iVec2 { int x, y; };

struct fVec2 { float x, y; };

struct Particle
{
	int lifetime;
	iVec2 position;
	iVec2 velocity;
};

enum class EmitterType
{
	SPARKLES,
	RAIN,
	SNOW,
	FIRE,
	SMOKE,
	FIREWORKS,
};

class Emitter
{
public:

	bool active;
	iVec2 position;
	int maxParticles;
	EmitterType type;
	Particle* particles;

	Emitter()
	{
		active = false;
	}

	~Emitter()
	{
		RELEASE_ARRAY(particles);
	}

	void Init(EmitterType _type, iVec2 _position, int _maxParticles)
	{
		active = true;

		type = _type;
		position = _position;
		maxParticles = _maxParticles;

		particles = new Particle[maxParticles];
		for (int i = 0; i < maxParticles; i++)
		{
			particles[i] = addParticle();
		}
	}

	Particle addParticle()
	{
		Particle p;

		switch (type)
		{
		case EmitterType::SPARKLES:
			p = { (rand() % 30) + 60,position,{((rand() % 20) - 10),((rand() % 20) - 10)} };
			break;
		case EmitterType::RAIN:
			p = { (rand() % 30) + 60,{position.x + (rand() % 200) - 100,position.y},{0,(rand() % 5) + 10} };
			break;
		case EmitterType::SNOW:
			p = { (rand() % 30) + 60,{position.x + (rand() % 200) - 100,position.y},{(rand() % 3) - 1,(rand() % 3) + 1} };
			break;
		case EmitterType::FIRE:

			break;
		case EmitterType::SMOKE:

			break;
		case EmitterType::FIREWORKS:

			break;
		default:
			break;
		}

		return p;
	}

	void Update(float dt)
	{
		for (int i = 0; i < maxParticles; i++)
		{
			if (particles[i].lifetime == 0)
				particles[i] = addParticle();

			particles[i].lifetime--;

			switch (type)
			{
			case EmitterType::SPARKLES:
				particles[i].position.x += particles[i].velocity.x;
				particles[i].position.y += particles[i].velocity.y;
				break;
			case EmitterType::RAIN:
				particles[i].position.x += particles[i].velocity.x;
				particles[i].position.y += particles[i].velocity.y;
				break;
			case EmitterType::SNOW:
				particles[i].position.x += particles[i].velocity.x;
				particles[i].position.y += particles[i].velocity.y;
				break;
			case EmitterType::FIRE:
				particles[i].position.x += particles[i].velocity.x;
				particles[i].position.y += particles[i].velocity.y;
				break;
			case EmitterType::SMOKE:

				break;
			case EmitterType::FIREWORKS:

				break;
			default:
				break;
			}
		}
	}

	void Draw(SDL_Renderer* renderer, bool debugDraw)
	{
		for (int i = 0; i < maxParticles; i++)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderDrawPoint(renderer, particles[i].position.x, particles[i].position.y);
			SDL_Rect particleRect{ particles[i].position.x - 2, particles[i].position.y - 2,5,5 };
			SDL_RenderDrawRect(renderer, &particleRect);

			/*if (debugdraw)
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
				SDL_RenderDrawLine(renderer, particles[i].position.x, particles[i].position.y, particles[i].position.x + particles[i].velocity.x * 10, particles[i].position.y + particles[i].velocity.y * 10);
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				SDL_Rect r2{ particles[i].position.x - particles[i].lifetime / 2,particles[i].position.y - particles[i].lifetime / 2,particles[i].lifetime,particles[i].lifetime };
				SDL_RenderDrawRect(renderer, &r2);

			}*/
		}
		if (debugDraw)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawLine(renderer, position.x - 10, position.y, position.x + 10, position.y);
			SDL_RenderDrawLine(renderer, position.x, position.y - 10, position.x, position.y + 10);
		}
	}

};

class ParticleSystem
{
public:

	List<Emitter*>* emitters;
	bool debugDraw = false;

	ParticleSystem()
	{
		srand(time(0));
	}

	~ParticleSystem()
	{
		RELEASE(emitters);
	}

	void AddEmitter(EmitterType t, iVec2 p, int m)
	{
		Emitter emitter;
		emitter.Init(t, p, m);
		emitters->Add(&emitter);
	}

	void Update(float dt, int* mouse, int* keyboard)
	{
		if (keyboard[SDL_SCANCODE_1] == 1) AddEmitter(EmitterType::SPARKLES, { mouse[0],mouse[1] }, 10);

		if (keyboard[SDL_SCANCODE_2] == 1) AddEmitter(EmitterType::RAIN, { mouse[0],mouse[1] }, 10);

		if (keyboard[SDL_SCANCODE_3] == 1) AddEmitter(EmitterType::SNOW, { mouse[0],mouse[1] }, 10);

		if (keyboard[SDL_SCANCODE_4] == 1) AddEmitter(EmitterType::FIRE, { mouse[0],mouse[1] }, 10);

		if (keyboard[SDL_SCANCODE_5] == 1) AddEmitter(EmitterType::SMOKE, { mouse[0],mouse[1] }, 10);

		if (keyboard[SDL_SCANCODE_6] == 1) AddEmitter(EmitterType::FIREWORKS, { mouse[0],mouse[1] }, 10);

		debugDraw = bool(keyboard[SDL_SCANCODE_D] == 2);
		//if (keyboard[SDL_SCANCODE_D] == 1) debugDraw = !debugDraw;

		for (ListItem<Emitter*>* emitter = emitters->start; emitter; emitter = emitter->next) emitter->data->Update(dt);
	}

	void Draw(SDL_Renderer* renderer)
	{
		for (ListItem<Emitter*>* emitter = emitters->start; emitter; emitter = emitter->next) emitter->data->Draw(renderer, debugDraw);
	}

};

#endif
