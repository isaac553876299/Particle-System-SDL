#include "ParticlesEngine.h"

ParticlesEngine::ParticlesEngine()
{

}

ParticlesEngine::~ParticlesEngine()
{

}

void ParticlesEngine::Step(float dt, SDL_Renderer* renderer)
{
	Update(dt);
	Draw(renderer);
}

void ParticlesEngine::Update(float dt)
{

}

void ParticlesEngine::Draw(SDL_Renderer* renderer)
{

}
