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
	iVec2 position;
	iVec2 velocity;
	float lifetime;
};

class ParticlesEngine
{
public:



	ParticlesEngine();

	~ParticlesEngine();

	void Step(float dt, SDL_Renderer* renderer);

	void Update(float dt);

	void Draw(SDL_Renderer* renderer);

};

#endif
