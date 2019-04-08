#include "j1App.h"
#include "j1Particles.h"
#include "j1Map.h"
#include "j1Scene1.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Player.h"
#include "p2Animation.h"
#include "j1Scene1.h"

j1Particles::j1Particles()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;

	SDL_Rect r = { 232, 103, 16, 12 };
	shot_right.anim.PushBack(r);
	shot_right.anim.PushBack({ 250, 103, 16, 12 });
	shot_right.anim.loop = true;
	shot_right.life = 2500;
	/*shot_right.speed.x = 100;
	shot_right.speed.y = 100;*/
	shot_right.speed = particle_speed;
}

j1Particles::~j1Particles()
{}

// Load assets
bool j1Particles::Start()
{
	LOG("Loading particles");
	part_tex = App->tex->Load("textures/character/particles.png");
	return true;
}

bool j1Particles::CleanUp()
{
	LOG("Unloading particles");
	App->tex->UnLoad(part_tex);
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] != nullptr)
		{
			delete active[i];
			active[i] = nullptr;
		}
	}
	return true;
}

bool j1Particles::Update(float dt)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* p = active[i];
		if (p == nullptr)
			continue;
		if (p->Update(dt) == false)
		{
			delete p;
			active[i] = nullptr;
		}
		else if (SDL_GetTicks() >= p->born)
		{
			SDL_Rect r_anim = { 238, 109, 6, 6 };
			App->render->Blit(part_tex, p->position.x, p->position.y, &r_anim, SDL_FLIP_NONE);

			/*switch (p->Type) {
			case p->SHOOT:
				App->render->Blit(part_tex, p->position.x, p->position.y, &(p->anim.GetCurrentFrame(dt)));
				break;
			}*/
			if (p->fx_played == false)
			{
				p->fx_played = true;
			}
		}
	}
	return true;
}

void j1Particles::AddParticle(const Particle& particle, int x, int y, fPoint speed, COLLIDER_TYPE collider_type, Uint32 delay)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] == nullptr)
		{
			Particle* p = new Particle(particle);
			p->born = SDL_GetTicks() + delay;
			p->position.x = x;
			p->position.y = y;
			p->speed = speed;
			//p->Type = particle.Type;
			float dt = App->GetDt();
			if (collider_type != COLLIDER_NONE) {
				p->collider = App->collisions->AddCollider({ (int)p->position.x, (int)p->position.y, 6, 6 }, collider_type, this);
				//p->collider->type = COLLIDER_SHOT;
			}
			active[i] = p;
			break;
		}
	}
}

// every time a particle hits a wall it triggers an explosion particle
void j1Particles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (active[i] != nullptr && active[i]->collider == c1)
		{
			delete active[i];
			active[i] = nullptr;
			break;
		}
	}
}

// -------------------------------------------------------------
// -------------------------------------------------------------
Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) :
	anim(p.anim), position(p.position), speed(p.speed),
	fx(p.fx), born(p.born), life(p.life)
{}

Particle::~Particle()
{
	if (collider != nullptr) {
		collider->to_delete = true;
	}
}

bool Particle::Update(float dt)
{
	bool ret = true;
	if (life > 0)
	{
		if ((SDL_GetTicks() - born) > life)
			ret = false;
	}

	position.x += speed.x * dt;
	position.y += speed.y * dt;


	if (collider != nullptr) {
		collider->SetPos(position.x, position.y);
		if (collider->type == COLLIDER_SHOT)
			collider->SetPos(position.x, position.y);
	}
	return ret;
}