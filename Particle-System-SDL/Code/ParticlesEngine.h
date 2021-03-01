#ifndef _PARTICLESENGINE_H_
#define _PARTICLESENGINE_H_

#include "Utils.h"

struct iVec2 { int x, y; };

struct fVec2 { float x, y; };

struct Particle { float lifetime; iVec2 position, velocity; };

class Emitter
{
public:

	iVec2 position;
	int maxParticles;
	Particle* particles = nullptr;

	Emitter(iVec2 pos, int maxp) : position(pos), maxParticles(maxp)
	{
		particles = new Particle[maxParticles];
		for (int i = 0; i < maxParticles; i++)
			particles[i] = { 60,position,{ ((rand() % 20) - 10),((rand() % 20) - 10) } };
	}

	~Emitter()
	{
		RELEASE_ARRAY(particles);
	}

	//RandomParticle
	//{ 60,position,{ ((rand() % 20) - 10),((rand() % 20) - 10) } };

	void Update(float dt)
	{
		for (int i = 0; i < maxParticles; i++)
		{
			if (particles[i].lifetime == 0)
				particles[i] = { 60,position,{ ((rand() % 20) - 10),((rand() % 20) - 10) } };
			
			particles[i].lifetime--;
			particles[i].position.x += particles[i].velocity.x;
			particles[i].position.y += particles[i].velocity.y;
		}
	}

	void Draw(SDL_Renderer* renderer, bool debugdraw)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for (int i = 0; i < maxParticles; i++)
		{
			//SDL_RenderDrawPoint(renderer, particles[i].position.x, particles[i].position.y);
			SDL_Rect r{ particles[i].position.x - 1, particles[i].position.y - 1,3,3 };
			SDL_RenderDrawRect(renderer, &r);
			if (debugdraw)
			{
				SDL_RenderDrawLine(renderer, position.x, position.y, particles[i].position.x, particles[i].position.y);
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
			AddEmitter({ mouse.x,mouse.y }, 10);
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
