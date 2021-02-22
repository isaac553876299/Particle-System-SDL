#ifndef _PARTICLESENGINE_H_
#define _PARTICLESENGINE_H_

#include "Utils.h"

struct Particle { int x, y, vx, vy; };

class ParticlesEngine
{
public:

	ParticlesEngine()
	{

	}
	~ParticlesEngine()
	{

	}
	void Step(Mouse mouse, float dt, SDL_Renderer* renderer)
	{
		Input(mouse);
		Update(dt);
		Draw(renderer);
	}

	void Input(Mouse mouse)
	{

	}

	void Update(float dt)
	{

	}

	void Draw(SDL_Renderer* renderer)
	{

	}
};

#endif
