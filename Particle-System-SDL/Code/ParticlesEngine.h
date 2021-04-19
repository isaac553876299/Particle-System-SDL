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

struct Particle
{
	int lifetime, lifespan;
	int x, y;
	int vx, vy;
};

struct ParticleProperties
{
	float min_lifespan, max_lifespan;
	float min_vx, max_vx;
	float min_vy, max_vy;
	float center_x, center_y, gravity_ax, gravity_ay;
	float min_x, max_x, min_y, max_y, w, h;
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
	int center_x, center_y;
	ParticleProperties properties;
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

	void Init(EmitterType _type, int _x, int _y, int _maxParticles, pugi::xml_node config)
	{
		active = true;

		type = _type;
		center_x = _x;
		center_y = _y;
		maxParticles = _maxParticles;

		switch (type)
		{
		case EmitterType::SPARKLES: config = config.child("Sparkles"); break;
		case EmitterType::RAIN: config = config.child("Rain"); break;
		case EmitterType::SNOW: config = config.child("Snow"); break;
		case EmitterType::FIRE: config = config.child("Fire"); break;
		case EmitterType::SMOKE: config = config.child("Smoke"); break;
		case EmitterType::FIREWORKS: config = config.child("Fireworks"); break;
		}

		properties.min_lifespan = config.child("lifespan").attribute("min").as_float();
		properties.max_lifespan = config.child("lifespan").attribute("max").as_float();
		properties.min_vx = config.child("velocity").attribute("min_vx").as_float();
		properties.max_vx = config.child("velocity").attribute("max_vx").as_float();
		properties.min_vy = config.child("velocity").attribute("min_vy").as_float();
		properties.max_vy = config.child("velocity").attribute("max_vy").as_float();
		properties.center_x = config.child("gravity").attribute("x").as_float();
		properties.center_y = config.child("gravity").attribute("y").as_float();
		properties.gravity_ax = config.child("gravity").attribute("ax").as_float();
		properties.gravity_ay = config.child("gravity").attribute("ay").as_float();
		properties.min_x = config.child("draw").attribute("min_x").as_float();
		properties.max_x = config.child("draw").attribute("max_x").as_float();
		properties.min_y = config.child("draw").attribute("min_y").as_float();
		properties.max_y = config.child("draw").attribute("max_y").as_float();
		properties.w = config.child("draw").attribute("w").as_float();
		properties.h = config.child("draw").attribute("h").as_float();

		particles = new Particle[maxParticles];
		for (int i = 0; i < maxParticles; i++) particles[i] = StartParticle();
	}

	Particle StartParticle()
	{
		Particle p;

		p.lifetime = 0.0f;
		p.lifespan = properties.min_lifespan + rand() % (int)(properties.max_lifespan - properties.min_lifespan+1);
		p.x = center_x + properties.min_x + rand() % (int)(properties.max_x - properties.min_x+1);
		p.y = center_y + properties.min_y + rand() % (int)(properties.max_y - properties.min_y+1);
		p.vx = properties.min_vx + rand() % (int)(properties.max_vx - properties.min_vx+1);
		p.vy = properties.min_vy + rand() % (int)(properties.max_vy - properties.min_vy+1);

		return p;
	}

	void Update(float dt)
	{
		for (int i = 0; i < maxParticles; i++)
		{
			if (particles[i].lifetime == 0) particles[i] = StartParticle();

			particles[i].lifetime--;

			particles[i].x += particles[i].vx;
			particles[i].y += particles[i].vy;
			particles[i].vx += properties.gravity_ax;
			particles[i].vy += properties.gravity_ay;

		}
	}

	void Draw(SDL_Renderer* renderer, bool debugDraw)
	{
		for (int i = 0; i < maxParticles; i++)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderDrawPoint(renderer, particles[i].x, particles[i].y);
			SDL_Rect particleRect{ particles[i].x - 2, particles[i].y - 2,5,5 };
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
			SDL_RenderDrawLine(renderer, center_x - 10, center_y, center_x + 10, center_y);
			SDL_RenderDrawLine(renderer, center_x, center_y - 10, center_x, center_y + 10);
		}
	}

};

class ParticleSystem
{
public:

	List<Emitter*>* emitters = new List<Emitter*>;

	bool debugDraw = false;

	pugi::xml_document particles_config;
	pugi::xml_node type_config;

	ParticleSystem()
	{
		srand(time(0));
		pugi::xml_parse_result result = particles_config.load_file("particles_config.xml");
		if (!result) printf("ERROR while loading particles_config.xml file: %s", result.description());
		type_config = particles_config.child("ParticleProperties");
	}

	~ParticleSystem()
	{
		RELEASE(emitters);
	}

	void AddEmitter(EmitterType t, int x, int y, int m)
	{
		Emitter emitter;
		emitter.Init(t, x, y, m, type_config);
		emitters->Add(&emitter);
	}

	void Update(float dt, int* mouse, int* keyboard)
	{
		if (keyboard[SDL_SCANCODE_1] == 1) AddEmitter(EmitterType::SPARKLES, mouse[0], mouse[1], 10);

		debugDraw = bool(keyboard[SDL_SCANCODE_D] == 2);
		//if (keyboard[SDL_SCANCODE_D] == 1) debugDraw = !debugDraw;
		if (emitters->start) for (ListItem<Emitter*>* emitter = emitters->start; emitter; emitter = emitter->next) emitter->data->Update(dt);
	}

	void Draw(SDL_Renderer* renderer)
	{
		if (emitters->start)
			for (ListItem<Emitter*>* emitter = emitters->start; emitter; emitter = emitter->next) emitter->data->Draw(renderer, debugDraw);
	}

	int CountEmitters()
	{
		return (emitters->start) ? emitters->size : 0;
	}

	int CountParticles()
	{
		int ret = 0;
		if (emitters->start) for (ListItem<Emitter*>* emitter = emitters->start; emitter; emitter = emitter->next) ret += emitter->data->maxParticles;
		return ret;
	}

};

#endif
