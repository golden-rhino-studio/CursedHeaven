#ifndef __j1MINDFLYER_H__
#define __j1MINDFLYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1Pathfinding.h"
#include "j1Entity.h"

#include <vector>

#define DETECTION_RANGE 10
#define ATTACK_RANGE 4

struct SDL_Texture;
//struct Collider;

class j1MindFlyer : public j1Entity
{
public:
	j1MindFlyer(int x, int y, ENTITY_TYPES type);

	// Destructor
	virtual ~j1MindFlyer();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt, bool do_logic);

	bool DrawOrder(float dt);

	// Called before quitting
	bool CleanUp();

	bool PostUpdate();

	// Called to check collisions
	void OnCollision(Collider* col_1, Collider* col_2);

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void LoadProperties();
	void Move(const std::vector<iPoint>* path, float dt);

private:
	Animation idle_diagonal_up;
	Animation idle_diagonal_down;
	Animation idle_lateral;
	Animation idle_down;
	Animation idle_up;

	Animation diagonal_up;
	Animation diagonal_down;
	Animation lateral;
	Animation up;
	Animation down;

	Animation death;

public:
	fPoint initialPosition;

	iPoint margin;
	iPoint colliderSize;

	float speed;

	Movement direction;

	std::vector<iPoint>* path;

	bool target_found = false;
	bool dead = false;
	bool receivedBasicDamage = false;
	bool receivedAbilityDamage = false;

};
#endif // __j1MINDFLYER_H__