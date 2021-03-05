#ifndef _PARTICLESENGINE_H_
#define _PARTICLESENGINE_H_

#include "Utils.h"

struct iVec2 { int x, y; };

struct fVec2 { float x, y; };

struct Particle { int lifetime; iVec2 position, velocity; };

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

	iVec2 position;
	int maxParticles;
	EmitterType type;
	Particle* particles = nullptr;

	Emitter()
	{

	}

	Emitter(EmitterType _type, iVec2 _position, int _maxParticles) : type(_type), position(_position), maxParticles(_maxParticles)
	{
		particles = new Particle[maxParticles];
		for (int i = 0; i < maxParticles; i++)
		{
			particles[i] = addParticle();
		}
	}

	~Emitter()
	{
		RELEASE_ARRAY(particles);
	}

	/*
	switch (type)
		{
		case EmitterType::SPARKLES:

			break;
		case EmitterType::RAIN:

			break;
		case EmitterType::SNOW:

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
	*/

	Particle addParticle()
	{
		Particle p;

		switch (type)
		{
		case EmitterType::SPARKLES:
			p = { (rand() % 30) + 60,position,{ ((rand() % 20) - 10),((rand() % 20) - 10) } };
			break;
		case EmitterType::RAIN:
			p = { (rand() % 30) + 60,position,{ ((rand() % 20) - 10),((rand() % 20) - 10) } };
			break;
		case EmitterType::SNOW:

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
			particles[i].position.x += particles[i].velocity.x;
			particles[i].position.y += particles[i].velocity.y;
		}
	}

	void Draw(SDL_Renderer* renderer, bool debugdraw)
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
		if (debugdraw)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawLine(renderer, 0, position.y, WWIDTH, position.y);
			SDL_RenderDrawLine(renderer, position.x, 0, position.x, WHEIGHT);
		}
	}

};

class Manager
{
public:

	int maxEmitters;
	Emitter* emitters = nullptr;
	bool debugdraw = false;

	Manager(int maxe) : maxEmitters(maxe)
	{
		
	}

	~Manager()
	{
		RELEASE_ARRAY(emitters);
	}

	void AddEmitter(EmitterType t, iVec2 p, int m)
	{
		RELEASE(emitters);
		emitters = new Emitter(t, p, m);
	}

	void Input(Mouse mouse, int* keyboard)
	{
		if (keyboard[SDL_SCANCODE_1] == 1)
			AddEmitter(EmitterType::SPARKLES, { mouse.x,mouse.y }, 10);

		if (keyboard[SDL_SCANCODE_2] == 1)
			AddEmitter(EmitterType::RAIN, { mouse.x,mouse.y }, 10);

		if (keyboard[SDL_SCANCODE_3] == 1)
			AddEmitter(EmitterType::SNOW, { mouse.x,mouse.y }, 10);

		if (keyboard[SDL_SCANCODE_4] == 1)
			AddEmitter(EmitterType::FIRE, { mouse.x,mouse.y }, 10);

		if (keyboard[SDL_SCANCODE_5] == 1)
			AddEmitter(EmitterType::SMOKE, { mouse.x,mouse.y }, 10);

		if (keyboard[SDL_SCANCODE_6] == 1)
			AddEmitter(EmitterType::FIREWORKS, { mouse.x,mouse.y }, 10);

		debugdraw = bool(keyboard[SDL_SCANCODE_D] == 2);
	}

	void Update(float dt)
	{
		if (emitters != nullptr)
			emitters->Update(dt);
	}

	void Draw(SDL_Renderer* renderer)
	{
		if (emitters != nullptr)
			emitters->Draw(renderer, debugdraw);
	}

};

#endif
