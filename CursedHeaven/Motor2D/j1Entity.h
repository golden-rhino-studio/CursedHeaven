#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "j1EntityManager.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "p2Log.h"

#define DETECTION_RANGE 15

struct Collider;
class Animation;

struct SDL_Texture;

enum ENTITY_TYPES;
enum DIRECTION;

class j1Entity
{
public:
	j1Entity(int x, int y, ENTITY_TYPES type);

	~j1Entity() {}

	virtual bool Start() {
		return true;
	};
	virtual bool Update(float dt, bool do_logic) {
		return true;
	};
	virtual bool PostUpdate() {
		return true;
	};
	virtual bool PreUpdate() {
		return true;
	};
	virtual bool CleanUp() {
		return true;
	};
	virtual bool DrawOrder(float dt) {
		return true;
	}

public:

	virtual bool Load(pugi::xml_node&) { return true; };
	virtual bool Save(pugi::xml_node&) const { return true; };

	virtual void Draw(SDL_Rect* r, bool flip = false, int x = 0, int y = 0, float scale = 1.0f, int offset = 0, bool rage = false, bool behind = false);
	virtual void OnCollision(Collider* c1, Collider* c2) {};

	ENTITY_TYPES type;
	DIRECTION direction;

	bool movement = false;

	uint current_height = 0;

	fPoint position;
	fPoint initialPosition;
	float speed;

	Animation* animation = nullptr;
	Collider* collider = nullptr;
	SDL_Texture* sprites = nullptr;
	SDL_Texture* debug_tex = nullptr;
	SDL_Texture* hud_tex = nullptr;

	float height = 0;
	float order = 0;
	// Combat values
	int basicDamage = 0;
	int lifePoints = 0;
	int totalLifePoints = 0;
	int offset = 0;
};

#endif // __ENTITY_H__