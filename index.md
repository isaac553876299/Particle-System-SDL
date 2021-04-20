# Particle System SDL2

## What
What is a particle system?
In 1982, William T. Reeves, a researcher at Lucasfilm Ltd., was working on the film Star Trek II: The Wrath of Khan. Much of the movie revolves around the Genesis Device, a torpedo that when shot at a barren, lifeless planet has the ability to reorganize matter and create a habitable world for colonization. During the sequence, a wall of fire ripples over the planet while it is being “terraformed".

[video](https://user-images.githubusercontent.com/60979696/115384075-e1e6ba80-a1d6-11eb-96fd-0c8ea9353e33.mp4)

The term particle system, an incredibly common and useful technique in computer graphics, was coined in the creation of this particular effect.

>“A particle system is a collection of many many minute particles that together represent a fuzzy object. Over a period of time, particles are generated into a system, move and change from within the system, and die from the system.” —William Reeves, "Particle Systems—A Technique for Modeling a Class of Fuzzy Objects," ACM Transactions on Graphics 2:2 (April 1983), 92.
<

Since the early 1980s, particle systems have been used in countless video games, animations, digital art pieces, and installations to model various irregular types of natural phenomena, such as fire, smoke, waterfalls, fog, grass, bubbles, and so on. A particle system is made after the recreation of an environment filled with entities *whose behaviour can be imprinted* into rather small particles which are bound to the system properties. In the end, they serve as a method to **simulate** either natural or fictional **phenomena** which would otherwise be harder **to study, observe or interact with**.

## Why
Why use a particle system?
As stated above, the point of using a particle system rather than conventional animations or processing modules is to avoid complicated and complex development of the phenomena to recreate, which by using normal methods would take much more time to generate. Although almost anything could be done without them, particle systems provide a fit solution to these sistuations, adn if implemented properly, can be generally much more faster and provide a better performance and adaptability.
Lets get into some examples. Lately, Pixar has added the use of high level CGI animation into their films, and so, cases like Brave's protagonist, Merida, can be proud of her hair with over 1500 "particles", which are actually sculpted curves, that would otherwise be quite tedious to animate by hand (~~but not impossible~~).

<img src="https://user-images.githubusercontent.com/60979696/115386446-b0bbb980-a1d9-11eb-88e3-9063e75a8fb0.jpg" width="250" height="209" /><img src="https://user-images.githubusercontent.com/60979696/115386978-5a9b4600-a1da-11eb-8718-c01d9cf0c623.jpg" width="300" height="208" /><img src="https://user-images.githubusercontent.com/60979696/115387192-9cc48780-a1da-11eb-979a-96efecf26c13.gif" width="400" height="209" />

Another example simulating a collision of minecraft blocks if they were made of particles, this time using advanced physics equations to make it realistic.

[video](https://user-images.githubusercontent.com/60979696/115390034-1611a980-a1de-11eb-9bb1-15ff91afad3e.mp4)

More reasons to use particle system:

[video](https://user-images.githubusercontent.com/60979696/115391889-42c6c080-a1e0-11eb-8ab1-682e35c220f1.mp4)

This is why we use particle systems, draw this by hand...

<img src="https://user-images.githubusercontent.com/60979696/115391902-465a4780-a1e0-11eb-8ea0-a08597b7907e.gif" width="1000" height="418"/>

It must be stated that for our concerns, most non-realistic videogames, and so using little simple physics, do not require such complex particle systems. As an example, our own game does not have physics implemented at all, and so, we will take into account that we can and **should** make use of simpler equations when constructing a particle system. If the purpose of the system is to simulate a realistic environment, then go for it. But it must be decided whether we just need some fancy animations or effects in our videogame in order to choose the proper configuration for our system.

## How
How to make a particle system?

First things first: memory isn't infinite (RAM we talking about). Second things second: the more processing, the more time (CPU).
Those are our main concerns when its time to start programming. Each particle will take some space in memory, and since we have a limited amount of that, we must ensure that we allocate only the essencial amount, meaning we should optimize our system so that no repeated information is stored (more later). Next concern is processing in real time, we don't want our framerate to drop just because we added some particles, even if hundreds. Thus, the equations used and the logic behind the behavioure of the particles should be simple operations and as few memory access or copies as possible.

That said, we are ready to start.

`ParticleSystem` will be a class, generally only one instance of this class will be created every time we run the game, the thing is to program it so we don't need another instance. This class will be the general manager for all the system, nothing of it outside this class.

Second important class or struct is the `Particle` itself.
```
enum class ParticleType
{
    SPARKLE,
    RAIN,
    FIRE
}
```
After setting some types to use, we run into a decision: which properties should every particle have? Here are some examples, depending on what behaviour do we need to recreate:
```
template <class T>
struct Particle
{
    ParticleType type;
    T lifetime, lifespan;
    T position_x, position_y;
    T velocity_x, velocity_y;
    T acceleration_x, acceleration_y;
    T width, height;
    T mass, center_of_mass_x, center_of_mass_y;
    SDL_Texture* texture;
}
```
Note the use of template class for variables type, up to us to decide whether we prefer `int` or `float` to calculate our values. WELL, given the example above, it is clear that a lot of memory is required for just a single particle, we must cut it. First, we decided no realistic physics would be implemented so for now we delete mass properties. Second, we must realize that certain properties of the particle will be repeated in memory after we create some of them, see `texture`, `type` and even `width/height` or `lifespan`.
The problem is we still need those properties to make it work, so how do we manage to store them, but just once for all particles that share them? Solution is we use an intermetiate class called `Emitter`, which contains a set of particles that share some values, and updates them.
```
class Emitter
{
    public:
    ParticleType type;
    SDL_Texture* texture; //if needed for particles
    float position_x, position_y;
    Particle* particles;
}
```
Note that for now we will be using `float` values. By suing this class, we get rid of unnecessary repeated properties adn so use less memory. `particles` pointer is an array fo the particles bound to this type, spawnpoint (position of Emitter) and texture if it is being used.
Given this hierarchy, class `ParticleSystem` should have an array of Emitters, adn so the structure is closed: `ParticleSystem`->`Emitters`->`Particles`.

Next is the functions or methods which update our system and its components, manage the stored information and finally draw the result or provide some output.
Some functions are `Update` which steps all particles one frame, `Draw` whic outputs the resulting render and or the debug information, and few `Load` or `Add`functions which take `pugi::xml` arguments to parse required properties from the useful config file into our system components. Here is a simplified non-defined snipped of code which shows the whole structure:

```c++
// only store non-repeated properties
struct Particle
{
  // time spent alive, max time permitted alive
	int lifetime, lifespan;
  // position
	int x, y;
  // velocity
	int vx, vy;
};

// utilility struct to save xml file loaded content
struct ParticleProperties
{
  // margin values for random assignation when creating a Particle
	float min_lifespan, max_lifespan;
	float min_vx, max_vx;
	float min_vy, max_vy;
	float center_x, center_y, gravity_ax, gravity_ay;
	float min_x, max_x, min_y, max_y, w, h;
};

// types of Particles, and as it is a repeated property, types of Emitters
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
  // boolean to deactivate emitter without destroying it
	bool active;
  // position or center of emision for particles
	int center_x, center_y;
  // loaded from config file
	ParticleProperties properties;
  // amount of particles which this emitter stores
	int maxParticles;
  // type of particles emitted
	EmitterType type;
  // array of particles
	Particle* particles;

	Emitter();
	~Emitter();
  // after creation, initialize Emitter and load properties form xml file
	void Init(EmitterType _type, int _x, int _y, int _maxParticles, pugi::xml_node config);
  // single particle initalization/randomization
	Particle StartParticle();
  // update positions
	void Update(float dt);
  // draw result, and if debugDraw is true, draw debug information too
	void Draw(SDL_Renderer* renderer, bool debugDraw);
};

class ParticleSystem
{
public:
  // convenient list for emitters rather than array
	List<Emitter*>* emitters = new List<Emitter*>;
  // whether to draw debug info or not
	bool debugDraw = false;
  // loaded config file
	pugi::xml_document particles_config;
  // loaded config node to particle properties
	pugi::xml_node type_config;
  // utility debugging counters for emitters and particles
	unsigned int emitters_count = 0;
	unsigned int particles_count = 0;

	ParticleSystem();
	~ParticleSystem();
  // initialize new emitter after receiving type
	void AddEmitter(EmitterType type, int x, int y, int maxp);
  // update existing emitters, and if input certain keys, add new emitters
	void Update(float dt, int* mouse, int* keyboard);
  // draw existing emitters
	void Draw(SDL_Renderer* renderer);
};
```

Time to talk about random values. What defines a particle system from other "systems" is its rather somewhat random generation of particles or selection of values. Mostly useful because it makes for a more realistic output. To implement this randomness, particle properties in the config xml file include minimum(`min`) and maximum(`max`) values to be given to certain properties such as inital velocity or spawn position with respect to the emitter's center.
Since we are using float values adn to avoid complicated code, c++ library `time.h` comes in handy as for using its rand()% function, which given an int returns some pseudo-random value between 0 and that int, and so we can take advantage of this to construct some random generator given our parameters:
`p.vx = properties.min_vx + rand() % (int)(1 + properties.max_vx - properties.min_vx);`
*The `1 + ` is to avoid impossible operation in case max - min = 0*

One last word is to pay attention to drawing methods. Since each particle has a lifetime which is capped at its lifespan, we can use this values to generate the alpha mod for either textures or normal rectangles and so we can draw the result with the respective amount of transparency that its lifetime represents.
`unsigned int alpha = 255 * (1 - (particles[i].lifetime / particles[i].lifespan));`

Now go pupils and do your thing!
<img src="https://user-images.githubusercontent.com/60979696/115391907-478b7480-a1e0-11eb-83c6-0775d6cdc178.gif" width="920" height="516" />

For detailed information and advanced c++ learning visit:

[khanacademy.org](https://www.khanacademy.org/computing/computer-programming/programming-natural-simulations/programming-particle-systems/a/intro-to-particle-systems)

[web.cs.wpi.edu](https://web.cs.wpi.edu/~matt/courses/cs563/talks/psys.html)

[cesium.com](https://cesium.com/docs/tutorials/particle-systems/)

>this one is specially recommended!
>
[natureofcode.com](https://natureofcode.com/book/chapter-4-particle-systems/)
