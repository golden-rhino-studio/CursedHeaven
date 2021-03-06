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
#include "Exodus.h"
#include "j1Entity.h"
#include "j1Audio.h"

#include "Brofiler/Brofiler.h"

j1Particles::j1Particles()
{
	name.assign("particles");

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;

	// Mage basic attack
	mageShot.anim.LoadAnimation("shot", "mage");
	mageShot.life = 350;
	mageShot.type = BASIC_SHOOT;

	// Mage Q
	explosion.anim.LoadAnimation("explosion", "mage");
	explosion.life = 570;
	explosion.type = EXPLOSION;

	// Turret attack
	turretAttack.anim.LoadAnimation("shot", "turret", false);
	turretAttack.life = 1000;
	turretAttack.type = TURRET_SHOOT;

	// Mindflyer attack
	mindflyerAttack.anim.LoadAnimation("shot", "mindflyer", false);
	mindflyerAttack.life = 1200;
	mindflyerAttack.type = MINDFLYER_SHOOT;

	// Exodus' swords
	sword1.anim.LoadAnimation("sword_attack1", "exodus", false);
	sword1.life = 1000;
	sword2.anim.LoadAnimation("sword_attack2", "exodus", false);
	sword2.life = 1200;
	sword3.anim.LoadAnimation("sword_attack3", "exodus", false);
	sword3.life = 1000;
	sword1.type = sword2.type = sword3.type = SWORD_SHOOT;
	sword1.margin = sword2.margin = sword3.margin = { 13,0 };

	// EXODUS Vortex
	vortex1.anim.LoadAnimation("vortex1", "exodus",false);
	vortex1.life = 2000;
	vortex1.type = VORTEX;

}

j1Particles::~j1Particles()
{}

// Load assets
bool j1Particles::Start()
{
	LOG("Loading particles");
	part_tex = App->tex->Load("textures/character/particles.png");
	sword_tex = App->tex->Load("textures/Effects/Particle effects/wills_magic_pixel_particle_effects/sword_burst/spritesheet.png");
	vortex_tex = App->tex->Load("textures/Effects/Particle effects/wills_magic_pixel_particle_effects/vortex/spritesheet.png");

	mageShot.tex = part_tex;
	mindflyerAttack.tex = part_tex;
	turretAttack.tex = part_tex;
	explosion.tex = part_tex;
	sword1.tex = sword_tex;
	sword2.tex = sword_tex;
	sword3.tex = sword_tex;
	vortex1.tex = vortex_tex;

	return true;
}

bool j1Particles::CleanUp()
{
	LOG("Unloading particles");
	App->tex->UnLoad(sword_tex);
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
	BROFILER_CATEGORY("ParticlesUpdate", Profiler::Color::LightSeaGreen)

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
			if (p->fx_played == false)
			{
				p->fx_played = true;
			}

			if (p->type == SWORD_SHOOT) {
				if (p->anim.isLastFrame() && p->state == 0) {
					Particle* aux = new Particle(sword2);
					aux->anim.Reset();
					aux->born = SDL_GetTicks();
					aux->position.x = p->position.x;
					aux->position.y = p->position.y;
					aux->speed = p->speed;
					aux->rotation = p->rotation;
					if (aux->rotation == 0 || aux->rotation == 180) {
						aux->collider = App->collisions->AddCollider({ 0,0,24,50 }, COLLIDER_ENEMY_SHOT, this);
					}
					else {
						aux->collider = App->collisions->AddCollider({ 0,0,50,24 }, COLLIDER_ENEMY_SHOT, this);
					}
					aux->state = 1;
					active[i] = aux;
					delete p;
				}
				else if (p->anim.isLastFrame() && p->state == 2) {
					Particle* aux = new Particle(sword3);
					aux->anim.Reset();
					aux->born = SDL_GetTicks();
					aux->position.x = p->position.x;
					aux->position.y = p->position.y;
					aux->speed = p->speed;
					aux->rotation = p->rotation;
					if (aux->rotation == 0 || aux->rotation == 180) {
						aux->collider = App->collisions->AddCollider({ 0,0,24,50 }, COLLIDER_ENEMY_SHOT, this);
					}
					else {
						aux->collider = App->collisions->AddCollider({ 0,0,50,24 }, COLLIDER_ENEMY_SHOT, this);
					}
					aux->state = 3;
					active[i] = aux;
					delete p;
				}
				else if (p->anim.isLastFrame() && p->state == 3) {
					delete p;
					active[i] = nullptr;
				}
			}
			if (p->type == VORTEX) {
				/*if (p->anim.isLastFrame() && p->state == 0) {
					Particle* aux = new Particle(vortex1);
					aux->anim.Reset();
					aux->born = SDL_GetTicks();
					aux->position.x = p->position.x;
					aux->position.y = p->position.y;
					aux->speed = p->speed;
					aux->rotation = p->rotation;
					aux->collider = App->collisions->AddCollider({ 0,0,50,50 }, COLLIDER_ENEMY_SHOT, this);
					aux->state = 1;
					active[i] = aux;
					delete p;
				}
				else if (p->anim.isLastFrame() && p->state == 2) {
					Particle* aux = new Particle(vortex1);
					aux->anim.Reset();
					aux->born = SDL_GetTicks();
					aux->position.x = p->position.x;
					aux->position.y = p->position.y;
					aux->speed = p->speed;
					aux->rotation = p->rotation;
					aux->collider = App->collisions->AddCollider({ 0,0,50,50 }, COLLIDER_NONE, this);
					aux->state = 3;
					active[i] = aux;
					delete p;
				}
				else if (p->anim.isLastFrame() && p->state == 3) {
					delete p;
					active[i] = nullptr;
				}*/
			}
			if(p->type == TURRET_SHOOT) 
				App->render->Blit(p->tex, p->position.x, p->position.y, &(p->anim.GetCurrentFrame(dt)), SDL_FLIP_NONE, true, 0.7f, App->win->GetScale(), p->rotation);
			else App->render->Blit(p->tex, p->position.x, p->position.y, &(p->anim.GetCurrentFrame(dt)), SDL_FLIP_NONE, true, 1.0f, App->win->GetScale(),p->rotation);

		}
	}

	return true;
}

void j1Particles::AddParticle(const Particle& particle, int x, int y, float dt, COLLIDER_TYPE collider_type, Uint32 delay, int rotation)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] == nullptr)
		{
			Particle* p = new Particle(particle);
			p->born = SDL_GetTicks() + delay;
			p->position.x = x;
			p->position.y = y;
			p->rotation = rotation;
			p->state = 0;
			p->anim.Reset();
			if (collider_type != COLLIDER_NONE) {
				if (particle.type == TURRET_SHOOT) p->collider = App->collisions->AddCollider({ p->anim.GetCurrentFrame(dt).x, p->anim.GetCurrentFrame(dt).y, 22, 20}, collider_type, this);
				else p->collider = App->collisions->AddCollider(p->anim.GetCurrentFrame(dt), collider_type, this);
			}
			Collider* test = p->collider;
			active[i] = p;
			break;
		}
	}
}

void j1Particles::AddParticleSpeed(const Particle& particle, int x, int y, float dt, COLLIDER_TYPE collider_type, Uint32 delay, int rotation, fPoint speed)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] == nullptr)
		{
			Particle* p = new Particle(particle);
			p->born = SDL_GetTicks() + delay;
			p->position.x = x;
			p->position.y = y;
			p->rotation = rotation;
			p->speed = speed;
			p->state = 0;
			p->anim.Reset();
			if (collider_type != COLLIDER_NONE) {
				if (p->type == SWORD_SHOOT) {
					SDL_Rect* rect = &p->anim.GetCurrentFrame(dt); 
					if (rotation == 0 || rotation == 180) {
						p->collider = App->collisions->AddCollider({ 0,0,24,50 }, collider_type, this);
					}
					else {
						p->collider = App->collisions->AddCollider({ 0,0,50,24 }, collider_type, this);
					}
				}
				else
					p->collider = App->collisions->AddCollider(p->anim.GetCurrentFrame(dt), collider_type, this);
			}
			active[i] = p;
			break;
		}
	}
}

void j1Particles::OnCollision(Collider* c1, Collider* c2)
{
	bool destroy = false;
	if (c2->type == COLLIDER_PLAYER)
	{
		int ret = true;
		for (uint i = 0; i < MAX_ACTIVE_PARTICLES && ret; ++i)
		{
			if (active[i] != nullptr) {
				if (active[i]->collider == c1) {
					switch (active[i]->type) {
					case SWORD_SHOOT:
						if (App->entity->player_type != KNIGHT || !App->entity->currentPlayer->active_Q)
							App->entity->currentPlayer->lifePoints -= App->entity->exo->damage;
						destroy = true;
						break;
					case MINDFLYER_SHOOT:
						if (App->entity->player_type != KNIGHT || !App->entity->currentPlayer->active_Q)
							App->entity->currentPlayer->lifePoints -= App->entity->mindflyer_Damage;
						destroy = true;
						break;
					case TURRET_SHOOT:
						if (App->entity->player_type != KNIGHT || !App->entity->currentPlayer->active_Q)
							App->entity->currentPlayer->lifePoints -= App->entity->turret_Damage;
						destroy = true;
						break;
					case VORTEX:
						if ((App->entity->player_type != KNIGHT || !App->entity->currentPlayer->active_Q) && SDL_GetTicks() >= active[i]->born + 500) {
							App->entity->currentPlayer->lifePoints -= 1;
							if (App->entity->player_type == MAGE)
								App->audio->PlayFx(App->audio->damage_bm);
							else
								App->audio->PlayFx(App->audio->damage_dk);
							App->entity->currentPlayer->receivedDamage = true;
						}
						break;
					}
					ret = false;
				}
			}
		}
		if (destroy) {
			if (App->entity->player_type == MAGE)
				App->audio->PlayFx(App->audio->damage_bm);
			else
				App->audio->PlayFx(App->audio->damage_dk);

			App->entity->currentPlayer->receivedDamage = true;
		}
		
		if (App->entity->currentPlayer->lifePoints <= 0) App->entity->currentPlayer->dead = true;
	}

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (active[i] != nullptr && active[i]->collider == c1 && destroy)
		{
			//AddParticle(...) ---> If we want to print an explosion when hitting an enemy
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
	anim(p.anim), type(p.type), position(p.position), speed(p.speed),
	fx(p.fx), born(p.born), life(p.life), tex(p.tex), margin(p.margin)
{}

Particle::~Particle()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

bool Particle::Update(float dt)
{
	bool ret = true;
	if (life > 0)
	{
		if (type == SWORD_SHOOT && (SDL_GetTicks() - born) > life) {
			born = SDL_GetTicks();
			state = 2;
		}
		else {
			uint32 time = SDL_GetTicks();
			if (time >= born) {
				if (((int)time - (int)born) > life)
					ret = false;
			}
		}
	}

	if (type != SWORD_SHOOT || state == 1) {
		position.x += speed.x * dt;
		position.y += speed.y * dt;
	}


	if (collider != nullptr) {
		if (type == SWORD_SHOOT) {
			if (rotation == 0 || rotation == 180) {
				collider->SetPos(position.x + margin.x, position.y + margin.y);
			}
			else {
				collider->SetPos(position.x + margin.y, position.y + margin.x);
			}
		}
		else
			collider->SetPos(position.x, position.y);
	}

	return ret;
}