#include <iostream> 
#include <sstream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1SceneMenu.h"
#include "j1SceneCredits.h"
#include "j1SceneSettings.h"
#include "j1ChooseCharacter.h"
#include "j1SceneLose.h"
#include "j1SceneVictory.h"
#include "j1Scene1.h"
#include "j1Scene2.h"
#include "j1Map.h"
#include "j1FadeToBlack.h"
#include "j1Collisions.h"
#include "j1Pathfinding.h"
#include "j1EntityManager.h"
#include "j1Fonts.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Particles.h"
#include "j1DialogSystem.h"
#include "j1Shop.h"
#include "j1Entity.h"
#include "j1Minimap.h"
#include "j1TransitionManager.h"
#include "j1SceneKeyConfig.h"
#include "j1Video.h"

#include "Brofiler/Brofiler.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;
	want_to_save = want_to_load = false;

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	menu = new j1SceneMenu();
	credits = new j1SceneCredits();
	settings = new j1SceneSettings();
	key_config = new j1SceneKeyConfig();
	choose_character = new j1ChooseCharacter();
	lose = new j1SceneLose();
	victory = new j1SceneVictory();
	scene1 = new j1Scene1();
	scene2 = new j1Scene2();
	map = new j1Map();
	entity = new j1EntityManager();
	path = new j1PathFinding();
	fade = new j1FadeToBlack();
	font = new j1Fonts();
	gui = new j1Gui();
	particles = new j1Particles();
	collisions = new j1Collisions();
	dialog = new j1DialogSystem();
	shop = new j1Shop;
	minimap = new j1Minimap();
	video = new j1Video();
	transitions = new j1TransitionManager();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(map);
	AddModule(path);
	AddModule(menu);
	AddModule(credits);
	AddModule(settings);
	AddModule(key_config);
	AddModule(choose_character);
	AddModule(lose);
	AddModule(scene1);
	AddModule(scene2);
	AddModule(shop);
	AddModule(entity);
	AddModule(particles);	
	AddModule(collisions);
	AddModule(font);
	AddModule(gui);
	AddModule(minimap);
	AddModule(dialog);
	AddModule(fade);
	AddModule(transitions);
	AddModule(victory);
	AddModule(video);

	// render last to swap buffer
	AddModule(render);
}

// Destructor
j1App::~j1App()
{
	std::list<j1Module*>::iterator item = modules.begin();
	
	for (; item != modules.end(); item = next(item))
	{
		RELEASE(*item);
	}

	modules.clear();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool j1App::Awake()
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;
		
	config = LoadConfig(config_file);

	if(config.empty() == false)
	{
		// Self-config
		ret = true;
		app_config = config.child("app");
		title.assign(app_config.child("title").child_value());
		organization.assign(app_config.child("organization").child_value());

		// We read from config file your framerate cap
		framerate_cap = config.child("app").attribute("framerate_cap").as_uint();
	}

	if(ret == true)
	{
		for (std::list<j1Module*>::iterator item = modules.begin(); item != modules.end(); ++item)
		{
			ret = (*item)->Awake(config.child((*item)->name.data()));
		}
	}

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	bool ret = true;

	for (std::list<j1Module*>::iterator item = modules.begin(); item != modules.end(); ++item)
	{
		ret = (*item)->Start();
	}

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	BROFILER_CATEGORY("AppUpdate", Profiler::Color::LightSeaGreen)

	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node j1App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if(result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	// We calculate the dt: differential time since last frame
	dt = frame_time.ReadSec();

	frame_time.Start();
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	if(want_to_save == true)
		SavegameNow();

	if(want_to_load == true)
		LoadGameNow();

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	// We can cap and uncap FPS by pressing F11
	if (App->input->GetKey(SDL_SCANCODE_F11) == j1KeyState::KEY_DOWN) {
		cappedFPS = !cappedFPS;
		frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	static char title[256];
	char* cap;
	char* vsync;

	if (cappedFPS)
		cap = "on";
	else
		cap = "off";
	
	if (App->render->usingVsync)
		vsync = "on";
	else
		vsync = "off";

	iPoint map_coords = { 0,0 };
	if (App->entity->currentPlayer != nullptr) {
		map_coords = App->map->WorldToMap((int)App->entity->currentPlayer->collider->rect.x , (int)App->entity->currentPlayer->collider->rect.y + (int)App->entity->currentPlayer->collider->rect.h);
	}

	sprintf_s(title, 256, "Cursed Heaven v0.5 ~ FPS: %d / Av.FPS: %.2f / Last Frame Ms: %02u / Cap %s / VSYNC %s / Tile: %d, %d",
		frames_on_last_update, avg_fps, last_frame_ms, cap, vsync, map_coords.x, map_coords.y);
	App->win->SetTitle(title);

	// We use SDL_Delay to make sure you get your capped framerate
	if ((last_frame_ms < (1000 / framerate_cap)) && cappedFPS) {
		SDL_Delay((1000 / framerate_cap) - last_frame_ms);
	}
}

float j1App::GetDt()
{
	return dt;
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	BROFILER_CATEGORY("AppPreUpdate", Profiler::Color::Orange)

	bool ret = true;
	std::list<j1Module*>::const_iterator item;
	item = modules.begin();
	j1Module* pModule = NULL;

	for (item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		pModule = *item;

		if (pModule->active == false)
			continue;

		ret = (*item)->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	std::list<j1Module*>::const_iterator item;
	item = modules.begin();

	j1Module* pModule = NULL;

	for (item = modules.begin(); item != modules.end() && ret; ++item)
	{
		pModule = *item;

		if (pModule->active == false) 
			continue;
		
		if (gamePaused) dt = 0.0f;
			ret = (*item)->Update(dt);
	}


	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	BROFILER_CATEGORY("AppPostUpdate", Profiler::Color::Yellow)

	bool ret = true;
	std::list<j1Module*>::const_iterator item;
	j1Module* pModule = NULL;

	for (item = modules.begin(); item != modules.end() && ret; ++item)
	{
		pModule = *item;

		if (!pModule->active) {
			continue;
		}

		ret = (*item)->PostUpdate();
	}
	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	bool ret = true;

	for (std::list<j1Module*>::iterator item = modules.begin(); item != modules.end(); ++item)
	{
		ret = (*item)->CleanUp();
	}

	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.data();
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.data();
}

// Load / Save
void j1App::LoadGame(const char* file)
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	want_to_load = true;
}

// ---------------------------------------
void j1App::SaveGame(const char* file) const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?

	want_to_save = true;
	save_game.assign(file);
}

bool j1App::LoadGameNow()
{
	bool ret = false;

	load_game.assign("save_game.xml");

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(load_game.data());

	if(result != NULL)
	{
		LOG("Loading new Game State from %s...", load_game.data());

		root = data.child("game_state");

		ret = true;

		std::list<j1Module*>::iterator item;

		for (item = modules.begin(); item != modules.end(); ++item)
		{
			ret = (*item)->Load(root.child((*item)->name.c_str()));
		}
		
		data.reset();
		if(ret == true)
			LOG("...finished loading");
		else
			LOG("...loading process interrupted with error on module %s", ((*item) != NULL) ? (*item)->name.data() : "unknown");
	}
	else
		LOG("Could not parse game state xml file %s. pugi error: %s", load_game.data(), result.description());

	want_to_load = false;

	return ret;
}

bool j1App::LoadSpecificModule(j1Module* module)
{
	bool ret = false;

	load_game.assign("save_game.xml");

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(load_game.data());

	if (result != NULL)
	{
		LOG("Loading new Game State from %s...", load_game.data());

		root = data.child("game_state");

		ret = true;
		ret = (module)->Load(root.child((module)->name.c_str()));

		data.reset();
		if (ret == true)
			LOG("...specific module finished loading");
		else
			LOG("...loading process interrupted with error on module %s", ((module) != NULL) ? (module)->name.data() : "unknown");
	}
	else
		LOG("Could not parse game state xml file %s. pugi error: %s", load_game.data(), result.description());

	return ret;
}

bool j1App::SavegameNow() const
{
	bool ret = true;

	save_game.assign("save_game.xml");

	LOG("Saving Game State to %s...", save_game.data());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;
	
	root = data.append_child("game_state");

	std::list<j1Module*>::const_iterator item;

	for (item = modules.begin(); item != modules.end(); ++item)
	{
		ret = (*item)->Save(root.append_child((*item)->name.c_str()));
	}

	if (ret == true)
	{
		data.save_file(save_game.data());
		LOG("... finished saving", save_game.data());
	}

	else
		LOG("Save process halted from an error in module %s", ((*item) != NULL) ? (*item)->name.data() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}