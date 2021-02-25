#ifndef _PARTICLESENGINE_H_
#define _PARTICLESENGINE_H_

#include "Utils.h"

struct iVec2
{
	int x, y;
};

struct fVec2
{
	float x, y;
};

struct Particle
{
	iVec2 initPosition;
	iVec2 position;
	iVec2 velocity;
	float lifetime;
};

class Emitter
{
public:

	iVec2 position;
	int maxParticles;
	Particle* particles = nullptr;

	Emitter(int maxp, iVec2 pos) : maxParticles(maxp)
	{
		particles = new Particle[maxParticles];
		for (int i = 0; i < maxParticles; i++)
		{
			particles[i] = randd(pos);
		}
	}

	Particle randd(iVec2 pos)
	{
		iVec2 vel{ rand() % 10,rand() % 10 };
		vel.x *= (rand() % 1 == 0) ? 1 : -1;
		vel.y *= (rand() % 1 == 0) ? 1 : -1;
		Particle p{ pos,pos,vel,60 };
		return p;
	}

	~Emitter()
	{
		RELEASE_ARRAY(particles);
	}

	void Update(float dt)
	{
		for (int i = 0; i < maxParticles; i++)
		{
			if (particles[i].lifetime == 0)
			{
				particles[i].lifetime = 60;
				particles[i] = randd(particles[i].initPosition);
			}
			particles[i].position.x += particles[i].velocity.x;
			particles[i].position.y += particles[i].velocity.y;
			particles[i].lifetime--;
		}
	}

	void Draw(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for (int i = 0; i < maxParticles; i++)
		{
			SDL_RenderDrawPoint(renderer, particles[i].position.x, particles[i].position.y);
		}
	}

};

class ParticlesEngineManager
{
public:

	int maxEmitters;
	Emitter* emitters[2];

	ParticlesEngineManager(int maxe);

	~ParticlesEngineManager();

	void AddEmitter(int maxp, iVec2 pos);

	void Step(Mouse mouse, float dt);

	void Input(Mouse mouse);

	void Update(float dt);

	void Draw(SDL_Renderer* renderer);

};

#endif
