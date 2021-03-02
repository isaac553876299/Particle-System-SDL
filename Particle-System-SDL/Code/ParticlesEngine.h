#ifndef _PARTICLESENGINE_H_
#define _PARTICLESENGINE_H_

#include "Utils.h"

struct iVec2 { int x, y; };

struct fVec2 { float x, y; };

struct Particle { int lifetime; iVec2 position, velocity; };// int pathSize; iVec2* trace; };

enum class EmitterType
{
	SPARKLES,
	RAIN,
	SNOW,
	FIRE,
	FIREWORKS,
};

class Emitter
{
public:

	iVec2 position;
	int maxParticles;
	Particle* particles = nullptr;

	Emitter()
	{

	}

	Emitter(iVec2 pos, int maxp) : position(pos), maxParticles(maxp)
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

	Particle addParticle()
	{
		Particle p;
		p = { (rand() % 30) + 60,position,{ ((rand() % 20) - 10),((rand() % 20) - 10) } };
		//p.pathSize = p.lifetime;
		//p.trace = new iVec2[p.pathSize];
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
			SDL_Rect r1{ particles[i].position.x - 2, particles[i].position.y - 2,5,5 };
			SDL_RenderDrawRect(renderer, &r1);
			if (debugdraw)
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
				SDL_RenderDrawLine(renderer, particles[i].position.x, particles[i].position.y, particles[i].position.x + particles[i].velocity.x * 10, particles[i].position.y + particles[i].velocity.y * 10);
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				SDL_Rect r2{ particles[i].position.x - particles[i].lifetime / 2,particles[i].position.y - particles[i].lifetime / 2,particles[i].lifetime,particles[i].lifetime };
				SDL_RenderDrawRect(renderer, &r2);

			}
		}
		if (debugdraw)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawLine(renderer, 0, position.y, WWIDTH, position.y);
			SDL_RenderDrawLine(renderer, position.x, 0, position.x, WHEIGHT);
		}
	}

};

class Sparkles : public Emitter
{
public:
	Sparkles()
	{

	}
	~Sparkles()
	{

	}
	void Update(float dt)
	{

	}
	void Draw(SDL_Renderer* renderer, bool debugdraw)
	{

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

	}

	void AddEmitter(iVec2 pos, int maxp)
	{
		RELEASE(emitters);
		emitters = new Emitter(pos, maxp);
	}

	void Input(Mouse mouse, int keyboard[200])
	{
		if (mouse.stateL == 1)
			AddEmitter({ mouse.x,mouse.y }, 1000);
		debugdraw = bool(keyboard[SDL_SCANCODE_D] == 2);
	}

	void Update(float dt)
	{
		if(emitters!=nullptr)
			emitters->Update(dt);
	}

	void Draw(SDL_Renderer* renderer)
	{
		if (emitters != nullptr)
			emitters->Draw(renderer, debugdraw);
	}

};

#endif
