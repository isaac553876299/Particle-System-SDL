#include "ParticlesEngine.h"

ParticlesEngineManager::ParticlesEngineManager(int maxe)
{
	maxEmitters = maxe;
}

ParticlesEngineManager::~ParticlesEngineManager()
{

}

void ParticlesEngineManager::AddEmitter(int maxp, iVec2 pos)
{
	for (int i = 0; i < maxEmitters; i++)
	{
		if (emitters[i] == nullptr)
		{
			emitters[i] = new Emitter(maxp, pos);
			break;
		}
	}
}

void ParticlesEngineManager::Step(Mouse mouse, float dt)
{
	Input(mouse);
	Update(dt);
}

void ParticlesEngineManager::Input(Mouse mouse)
{
	if (mouse.stateL == 1)
	{
		AddEmitter(10, { mouse.x,mouse.y });
	}
}

void ParticlesEngineManager::Update(float dt)
{
	for (int i = 0; i < maxEmitters; i++)
	{
		if (emitters[i] != nullptr)
		{
			emitters[i]->Update(dt);
		}
	}
}

void ParticlesEngineManager::Draw(SDL_Renderer* renderer)
{
	for (int i = 0; i < maxEmitters; i++)
	{
		if (emitters[i] == nullptr)
		{
			emitters[i]->Draw(renderer);
		}
	}
}
