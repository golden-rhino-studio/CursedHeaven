#ifndef __j1SCENE1_H__
#define __j1SCENE1_H__

#include "j1Module.h"
#include "j1Button.h"
#include "j1Timer.h"
#include "PugiXml/src/pugixml.hpp"
#include <list>

struct SDL_Texture;
struct _TTF_Font;
class j1Box;

class j1Scene1 : public j1Module
{
public:

	j1Scene1();

	// Destructor
	virtual ~j1Scene1();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Called to change scene
	void ChangeSceneMenu();
	void ChangeSceneDeath();
	void ChangeScene2();

	// Load and Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void PlaceEntities(int room);

public:
	j1Box* settings_window = nullptr;
	bool player_created = false;
	bool backToMenu = false;
	bool toLoseScene = false;

	std::list<j1Button*> scene1Buttons;
	std::list<j1Label*> scene1Labels;
	std::list<j1Box*> scene1Boxes;

	j1Timer	startup_time;
	int time_scene1;
	bool changingScene = false;
	bool finishedDialog = false;
	bool profile_active = false;
	bool bossFightOn = false;
	bool ableSellerDialog = true;

	j1Timer windowTime;
	uint lastWindowTime = 0;
	j1Timer statsTime;
	uint lastStatsTime = 0;
	std::string current_points;
	int score_player = 0;

	uint potionCounter = 0;

private:
	SDL_Texture* debug_tex = nullptr;
	SDL_Texture* lvl1_tex = nullptr;
	SDL_Texture* gui_tex = nullptr;
	SDL_Texture* bg = nullptr;
	SDL_Rect rect = { 0, 0, 400, 200};

	bool closeSettings = false;
	bool continueGame = true;
	bool resettingLevel = false;	
	bool mustSave = false;
};

#endif // __j1SCENE1_H__