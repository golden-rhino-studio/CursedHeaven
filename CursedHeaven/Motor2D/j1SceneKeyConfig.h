#ifndef __j1SCENEKEYCONFIG_H__
#define __j1SCENEKEYCONFIG_H__

#include "j1Module.h"
#include "j1Render.h"
#include "j1Timer.h"
#include "p2Point.h"
#include "j1Input.h"
#include <list>

struct SDL_Texture;
struct _TTF_Font;
class j1Button;
class j1Label;
class j1Box;

class j1SceneKeyConfig : public j1Module {

public:

	j1SceneKeyConfig();

	virtual ~j1SceneKeyConfig();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:

	std::list<j1Label*> key_labels;
	std::list<j1Box*> key_boxes;
	std::list<j1Button*> key_buttons;

private:

	bool continueGame = true;
	SDL_Texture* key_tex = nullptr;
};
#endif // !__j1SCENEKEYCONFIG_H__
