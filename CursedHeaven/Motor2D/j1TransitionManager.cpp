#include "j1App.h"
#include "j1TransitionManager.h"
#include "j1Transitions.h"
#include "j1Render.h"
#include "j1Window.h"
#include "FadeToColor.h"
#include "Wipe.h"
#include "Lines.h"
#include "Squares.h"
#include "j1Input.h"
#include "j1Particles.h"
#include "j1DialogSystem.h"

#include "j1Scene1.h"
#include "j1SceneCredits.h"
#include "j1SceneLose.h"
#include "j1SceneMenu.h"
#include "j1SceneSettings.h"
#include "j1SceneVictory.h"
#include "j1ChooseCharacter.h"

j1TransitionManager::j1TransitionManager()
{}

j1TransitionManager::~j1TransitionManager()
{}

bool j1TransitionManager::PostUpdate() {

	// Calls PostUpdate (switch of states) of every transition in the list
	for (std::list<j1Transitions*>::iterator item = transitions_list.begin(); item != transitions_list.end(); ++item) {
		(*item)->PostUpdate();
	}
	return true;
}

bool j1TransitionManager::CleanUp() {

	// Clears the list
	transitions_list.clear();

	return true;
}

void j1TransitionManager::CleanTransitions(j1Transitions* transition) {

	// Removes the list and deletes the transition
	transitions_list.remove(transition);
	delete transition;
}

void j1TransitionManager::SwitchScenes(SCENE scene1, SCENE scene2) {

	// From menu
	if (scene1 == SCENE::MENU) {

		App->menu->active = false;
		App->menu->CleanUp();

		// To Choose Character
		if (scene2 == SCENE::CHOOSE) {
			App->choose_character->active = true;
			App->choose_character->Start();
		}
		// To Credits Scene
		else if (scene2 == SCENE::CREDITS) {
			App->credits->active = true;
			App->credits->Start();
		}
		// To Settings Scene
		else if (scene2 == SCENE::SCENE_SETTINGS) {
			App->settings->active = true;
			App->settings->Start();
		}
	}

	// To Game
	if (scene2 == SCENE::SCENE1) {

		// From Victory
		if (scene1 == SCENE::VICTORY) {
			App->victory->active = false;
			App->victory->CleanUp();
		}
		// From Lose
		else if (scene1 == SCENE::LOSE) {
			App->lose->active = false;
			App->lose->CleanUp();
			App->lose->startGame = false;
		}
		// From Choose Character
		else if (scene1 == SCENE::CHOOSE) {
			App->choose_character->active = false;
			App->choose_character->CleanUp();
		}

		App->scene1->active = true;
		App->scene1->Start();

		App->particles->Start();

		App->dialog->active = true;
		App->dialog->Start();

		App->entity->active = true;
		App->entity->CreatePlayer();
		App->entity->Start();
	}

	// To menu
	else if (scene2 == SCENE::MENU) {

		// From Credits
		if (scene1 == SCENE::CREDITS) {
			App->credits->active = false;
			App->credits->CleanUp();
		}
		// From Settings
		else if (scene1 == SCENE::SCENE_SETTINGS) {
			App->settings->active = false;
			App->settings->CleanUp();
		}
		// From Victory
		else if (scene1 == SCENE::VICTORY) {
			App->victory->active = false;
			App->victory->CleanUp();
			App->victory->backToMenu = false;
			App->scene1->toVictoryScene = false;
		}
		// From Lose
		else if (scene1 == SCENE::LOSE) {
			App->lose->active = false;
			App->lose->CleanUp();
			App->lose->backToMenu = false;
			App->scene1->toLoseScene = false;
		}
		// From Game
		else if (scene1 == SCENE::SCENE1) {
			App->scene1->active = false;
			App->scene1->CleanUp();
			App->scene1->changingScene = false;

			App->entity->active = false;
			App->entity->CleanUp();
			App->entity->DestroyEntities();

		}

		App->menu->active = true;
		App->menu->Start();
		App->render->camera = { 0,0 };
	}

	// From Game
	else if (scene1 == SCENE::SCENE1) {

		App->particles->CleanUp();

		App->dialog->active = false;
		App->dialog->CleanUp();

		App->entity->active = false;
		App->entity->CleanUp();
		App->entity->DestroyEntities();

		App->scene1->active = false;
		App->scene1->CleanUp();

		// To Victory
		if (scene2 == SCENE::VICTORY) {
			App->victory->active = true;
			App->victory->Start();
			App->render->camera = { 0,0 };
		}
		// To Lose
		else if (scene2 == SCENE::LOSE) {
			App->lose->active = true;
			App->lose->Start();
			App->render->camera = { 0,0 };
		}
	}
}

void j1TransitionManager::FadingToColor(SCENE scene_to_remove, SCENE scene_to_appear, j1Color color, float time) {
	transitions_list.push_back(new FadeToColor(scene_to_remove, scene_to_appear, color, time));
}

void j1TransitionManager::Wiping(SCENE scene_to_remove, SCENE scene_to_appear, j1Color color, float time) {
	transitions_list.push_back(new Wipe(scene_to_remove, scene_to_appear, color, time));
}

void j1TransitionManager::LinesAppearing(SCENE scene_to_remove, SCENE scene_to_appear, j1Color color, float time) {
	transitions_list.push_back(new Lines(scene_to_remove, scene_to_appear, color, time));
}

void j1TransitionManager::SquaresAppearing(SCENE scene_to_remove, SCENE scene_to_appear, int transition, j1Color color, float time) {
	transitions_list.push_back(new Squares(scene_to_remove, scene_to_appear, transition, color, time));
}