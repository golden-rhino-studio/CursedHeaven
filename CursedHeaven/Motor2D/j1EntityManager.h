#ifndef __J1ENTITYMANAGER_H__
#define __J1ENTITYMANAGER_H__

#include "j1Module.h"
#include "j1UserInterfaceElement.h"
#include <list>

#define MAX_ENTITIES 1000

class j1Entity;
class j1DragoonKnight;
class j1BlackMage;
class j1Rogue;
class j1Tank;
class j1Player;

struct SDL_Texture;

enum DIRECTION {
	UP_,
	DOWN_,
	RIGHT_,
	LEFT_,
	UP_RIGHT_,
	UP_LEFT_,
	DOWN_RIGHT_,
	DOWN_LEFT_,
	NONE_
};

enum ENTITY_TYPES
{
	PLAYER,
	COIN,
	UNKNOWN
};

enum PLAYER_TYPES
{
	KNIGHT,
	MAGE,
	TANK,
	ROGUE
};

struct EntityInfo
{
	ENTITY_TYPES type = ENTITY_TYPES::UNKNOWN;
	iPoint position;
};

class j1EntityManager : public j1Module
{
public:
	// Constructor
	j1EntityManager();

	// Destructor
	~j1EntityManager();

	bool Awake(pugi::xml_node&);
	bool Start();

	// Called every frame
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	j1Entity* CreateEntity(ENTITY_TYPES type, int x = 0, int y = 0);

	void OnCollision(Collider* c1, Collider* c2);
	void CreatePlayer();
	void AddEnemy(int x, int y, ENTITY_TYPES type);
	void DestroyEntities();

private:

	void SpawnEnemy(const EntityInfo& info);

public:
	
	std::list<j1Entity*>	entities;

	// Pointers to diferent playable classes
	j1DragoonKnight*	knight = nullptr;
	j1BlackMage*		mage = nullptr;
	j1Tank*				tank = nullptr;
	j1Rogue*			rogue = nullptr;

	PLAYER_TYPES player_type = KNIGHT;

private:

	EntityInfo			queue[MAX_ENTITIES];
	bool				do_logic = false;
	float				accumulatedTime = 0.0f;
	float				updateMsCycle = 0.0f;

};

#endif // __J1ENTITYMANAGER_H__