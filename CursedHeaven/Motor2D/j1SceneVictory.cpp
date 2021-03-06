#include "j1SceneVictory.h"
#include "j1SceneLose.h"
#include "j1SceneMenu.h"
#include "j1SceneCredits.h"
#include "j1ChooseCharacter.h"
#include "j1App.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1FadeToBlack.h"
#include "j1Audio.h"
#include "j1Map.h"
#include "j1Gui.h"
#include "j1Button.h"
#include "j1Label.h"
#include "j1Box.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1DialogSystem.h"
#include "j1Particles.h"
#include "j1Video.h"
#include "Brofiler/Brofiler.h"

j1SceneVictory::j1SceneVictory()
{
	name.assign("victory");
}

j1SceneVictory::~j1SceneVictory()
{}

bool j1SceneVictory::Awake(pugi::xml_node &)
{
	LOG("Loading Credits");
	bool ret = true;

	if (App->video->active == true)
		active = false;

	if (App->menu->active == true)
		active = false;

	if (active == false)
		LOG("Credits not active.");

	if (App->menu->active == false)
	{
		LOG("Unable to load the credits.");
		ret = false;
	}

	return ret;
}

bool j1SceneVictory::Start()
{
	if (active)
	{
		App->map->draw_with_quadtrees = false;

		// The audio is played
		App->audio->PlayMusic("audio/music/song028.ogg", 1.0f);

		// Loading textures
		gui_tex2 = App->tex->Load("gui/uipack_rpg_sheet.png");

		SDL_Rect idle = { 631, 12, 151, 38 };
		SDL_Rect hovered = { 963, 12, 151, 38 };
		SDL_Rect clicked = { 797, 12, 151, 38 };
		App->gui->CreateButton(&victoryButtons, BUTTON, 130, 100 + 70, idle, hovered, clicked, gui_tex2, PLAY_GAME);
		App->gui->CreateButton(&victoryButtons, BUTTON, 130, 125 + 70, idle, hovered, clicked, gui_tex2, GO_TO_MENU);

		App->gui->CreateLabel(&victoryLabels, LABEL, 145, 170, App->gui->font1, "Play Again", App->gui->beige);
		App->gui->CreateLabel(&victoryLabels, LABEL, 143, 195, App->gui->font1, "Go to Menu", App->gui->beige);

		App->gui->CreateLabel(&victoryLabels, LABEL, 116, 30, App->gui->font3, "YOU WIN!", {255, 255, 102, 255} );
		App->gui->CreateLabel(&victoryLabels, LABEL, 86, 80, App->gui->font2, "HEAVEN IS SAFE NOW", App->gui->beige);

		startup_time.Start();
	}
	return true;
}

bool j1SceneVictory::PreUpdate()
{
	return true;
}

bool j1SceneVictory::Update(float dt)
{
	App->gui->UpdateButtonsState(&victoryButtons, App->gui->buttonsScale);
	// Button actions
	for (std::list<j1Button*>::iterator item = victoryButtons.begin(); item != victoryButtons.end(); ++item) {
		if ((*item)->visible) {
			switch ((*item)->state)
			{
			case IDLE:
				(*item)->situation = (*item)->idle;
				break;

			case HOVERED:
				(*item)->situation = (*item)->hovered;
				break;

			case RELEASED:
				(*item)->situation = (*item)->idle;
				if ((*item)->bfunction == PLAY_GAME) {
					App->fade->FadeToBlack();
					startGame = true;

				}
				else if ((*item)->bfunction == GO_TO_MENU) {
					App->fade->FadeToBlack();
					backToMenu = true;

				}
				break;

			case CLICKED:
				(*item)->situation = (*item)->clicked;
				break;
			}
		}
	}

	// Managing scene transitions
	if (App->fade->IsFading() == 0) {
		if (startGame) {
			ChangeScene(CHOOSE);
		}
		else if (backToMenu)
			ChangeScene(MENU);
	}

	// ---------------------------------------------------------------------------------------------------------------------
	// DRAWING EVERYTHING ON THE SCREEN
	// ---------------------------------------------------------------------------------------------------------------------	

	// Blitting the buttons and labels of the menu
	for (std::list<j1Button*>::iterator item = victoryButtons.begin(); item != victoryButtons.end(); ++item)
	{
		if ((*item)->parent != nullptr) continue;
		(*item)->Draw(App->gui->buttonsScale);
	}
	for (std::list<j1Label*>::iterator item = victoryLabels.begin(); item != victoryLabels.end(); ++item)
	{
		if ((*item)->parent != nullptr) continue;
		if ((*item)->visible) (*item)->Draw();
	}

	return true;
}

bool j1SceneVictory::PostUpdate()
{
	BROFILER_CATEGORY("CreditsPostUpdate", Profiler::Color::Yellow)

		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
			continueGame = false;

	return continueGame;
}

bool j1SceneVictory::CleanUp()
{
	LOG("Freeing all textures");
	App->tex->UnLoad(gui_tex2);

	for (std::list<j1Button*>::iterator item = victoryButtons.begin(); item != victoryButtons.end(); ++item) {
		(*item)->CleanUp();
		victoryButtons.remove(*item);
	}

	for (std::list<j1Label*>::iterator item = victoryLabels.begin(); item != victoryLabels.end(); ++item) {
		(*item)->CleanUp();
		victoryLabels.remove(*item);
	}

	return true;
}

void j1SceneVictory::ChangeScene(SCENE objectiveScene)
{
	this->active = false;
	backToMenu = false;
	startGame = false;
	App->entity->player_type = NO_PLAYER;
	CleanUp();

	if (objectiveScene == SCENE::MENU) {
		App->menu->active = true;
		App->menu->Start();
	}
	else if (objectiveScene == SCENE::CHOOSE) {
		App->choose_character->active = true;
		App->choose_character->Start();
	}
}