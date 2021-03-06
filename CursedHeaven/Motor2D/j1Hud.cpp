#include "j1Hud.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1UserInterfaceElement.h"
#include "j1Label.h"
#include "j1Scene1.h"
#include "j1Scene2.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1Shop.h"
#include "j1Window.h"
#include "j1Shop.h"

j1Hud::j1Hud() 
{
	
}

j1Hud::~j1Hud() {}

bool j1Hud::Start()
{
	hud_text = App->tex->Load("gui/hud.png");
	profile_text = App->tex->Load("gui/player_profile.png");
	dialog_tex = App->tex->Load("textures/dialog_cursedheaven.png");
	potion_tex = App->tex->Load("gui/potion.png");
	frames_tex = App->tex->Load("gui/UIFrames.png");
	life_points = 82;
	life_points_max = App->entity->currentPlayer->lifePoints;
	multiplier = 82 / life_points_max;
	
	return true;
}

bool j1Hud::Update(float dt)
{
	life_points = App->entity->currentPlayer->lifePoints * multiplier;

	// Profiles
	SDL_Rect dk_profile = { 0,0,42,42 };
	SDL_Rect bm_profile = { 42,0,43,42 };

	SDL_Rect upgrade = { 561,587,28,28 };

	SDL_Rect profile_frame = { 0, 0, 403, 190 };
	SDL_Rect potion_frame = { 0, 200, 104, 186 };
	SDL_Rect frame = { 228, 196, 399, 200 };

	// Tab clicked
	SDL_Rect blackMage = { 0,568,190,366 };
	SDL_Rect dragoonKnight = { 190,568,273,361 };
	SDL_Rect window_profile_1 = { 0,0,785,568 };
	SDL_Rect window_profile_2 = { 785,0,785,568 };
	SDL_Rect ability = { 561,587,28,28 };
	SDL_Rect potion1 = { 0, 0, 78, 78 };
	SDL_Rect potion_none = { 78,0,78,78 };

	// Coins
	SDL_Rect coins_r = { 0,42,35,13 };
	
	// DK Q
	SDL_Rect dk_available_q = {0,61,41,41};
	SDL_Rect dk_notavailable_q = {0,102,41,41};
	// DK E
	SDL_Rect dk_available_e = { 0,143,41,41 };
	SDL_Rect dk_notavailable_e = {0,184,41,41};

	// BM Q
	SDL_Rect bm_available_q = { 41,61,41,41 };
	SDL_Rect bm_notavailable_q = { 41,102,41,41 };
	// BM E
	SDL_Rect bm_available_e = { 41,143,41,41 };
	SDL_Rect bm_notavailable_e = { 41,184,41,41 };

	// Lifebar
	SDL_Rect lifebar = { 0,225,82,8 };
	SDL_Rect lifebar_r = { 0,233,life_points,6 };

	//dialog
	SDL_Rect chart = { 0, 0, 284, 71 };

	if (App->entity->player_type == MAGE)
		black_mage = true;
	else if (App->entity->player_type == KNIGHT)
		dragoon_knight = true;

	if (frames_tex != nullptr) {
		App->render->Blit(frames_tex, -10, -13, &profile_frame, SDL_FLIP_NONE, false, 0.35f);
		App->render->Blit(frames_tex, -10, 190, &potion_frame, SDL_FLIP_NONE, false, 0.35f);
		App->render->Blit(frames_tex, 620, 565, &frame, SDL_FLIP_NONE, false, 0.35f);
	}

	if (hud_text != nullptr) {
		if (black_mage) {
			// Icon profile
			App->render->Blit(hud_text, 11, 6, &bm_profile, SDL_FLIP_NONE, false, 1.0f);

			// Abilities
			// Q
			App->render->Blit(hud_text, 15, 400, &bm_available_q, SDL_FLIP_NONE, false, 0.5f);
			if (!App->entity->currentPlayer->available_Q) {
				App->render->Blit(hud_text, 15, 400, &bm_notavailable_q, SDL_FLIP_NONE, false, 0.5f);
			}
			// E
			App->render->Blit(hud_text, 15, 465, &bm_available_e, SDL_FLIP_NONE, false, 0.5f);
			if (!App->entity->currentPlayer->available_E) {
				App->render->Blit(hud_text, 15, 465, &bm_notavailable_e, SDL_FLIP_NONE, false, 0.5f);
			}
		}
		else if (dragoon_knight) {
			// Icon profile
			App->render->Blit(hud_text, 11, 6, &dk_profile, SDL_FLIP_NONE, false);

			// Abilities
			// Q
			App->render->Blit(hud_text, 15, 400, &dk_available_q, SDL_FLIP_NONE, false, 0.5f);
			if (!App->entity->currentPlayer->available_Q) {
				App->render->Blit(hud_text, 15, 400, &dk_notavailable_q, SDL_FLIP_NONE, false, 0.5f);
			}
			// E
			App->render->Blit(hud_text, 15, 465, &dk_available_e, SDL_FLIP_NONE, false, 0.5f);
			if (!App->entity->currentPlayer->available_E) {
				App->render->Blit(hud_text, 15, 465, &dk_notavailable_e, SDL_FLIP_NONE, false, 0.5f);
			}
		}
	}
	
	// Current points of the player (char*)
	current_points = App->entity->currentPlayer->current_points.c_str();

	//	App->scene1->current_points.erase();
	SDL_Rect temp;
	temp.x = temp.y = 0;
	temp.w = temp.h = 10;

	if (potion_tex != nullptr) {
		if (App->shop->potions == 0) {
			App->render->BlitHUD(potion_tex, 15, 200, &potion_none, SDL_FLIP_NONE, 1.0f, 0.2f, 0.0, pivot, pivot, false);
			App->render->BlitHUD(potion_tex, 15, 260, &potion_none, SDL_FLIP_NONE, 1.0f, 0.2f, 0.0, pivot, pivot, false);
			App->render->BlitHUD(potion_tex, 15, 320, &potion_none, SDL_FLIP_NONE, 1.0f, 0.2f, 0.0, pivot, pivot, false);
		}

		if (App->shop->potions == 1) {
			App->render->BlitHUD(potion_tex, 15, 200, &potion1, SDL_FLIP_NONE, 1.0f, 0.2f, 0.0, pivot, pivot, false);
			App->render->BlitHUD(potion_tex, 15, 260, &potion_none, SDL_FLIP_NONE, 1.0f, 0.2f, 0.0, pivot, pivot, false);
			App->render->BlitHUD(potion_tex, 15, 320, &potion_none, SDL_FLIP_NONE, 1.0f, 0.2f, 0.0, pivot, pivot, false);
		}

		if (App->shop->potions == 2) {
			App->render->BlitHUD(potion_tex, 15, 200, &potion1, SDL_FLIP_NONE, 1.0f, 0.2f, 0.0, pivot, pivot, false);
			App->render->BlitHUD(potion_tex, 15, 260, &potion1, SDL_FLIP_NONE, 1.0f, 0.2f, 0.0, pivot, pivot, false);
			App->render->BlitHUD(potion_tex, 15, 320, &potion_none, SDL_FLIP_NONE, 1.0f, 0.2f, 0.0, pivot, pivot, false);
		}

		if (App->shop->potions >= 3) {
			App->render->BlitHUD(potion_tex, 15, 200, &potion1, SDL_FLIP_NONE, 1.0f, 0.2f, 0.0, pivot, pivot, false);
			App->render->BlitHUD(potion_tex, 15, 260, &potion1, SDL_FLIP_NONE, 1.0f, 0.2f, 0.0, pivot, pivot, false);
			App->render->BlitHUD(potion_tex, 15, 320, &potion1, SDL_FLIP_NONE, 1.0f, 0.2f, 0.0, pivot, pivot, false);
		}
	}

	App->render->Blit(hud_text, 16, 140, &coins_r, SDL_FLIP_NONE, false);

	App->tex->UnLoad(score);
	score = App->font->Print(current_points, temp.w, temp.h, 0, App->gui->brown, App->gui->font1);

	App->render->BlitHUD(score, 69, 135, &temp, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0, pivot, pivot, false);

	App->render->Blit(hud_text, 140, 10, &lifebar, SDL_FLIP_NONE, false);
	App->render->Blit(hud_text, 143, 13, &lifebar_r, SDL_FLIP_NONE, false);

	if (App->scene1->profile_active) {
		
		if (black_mage) {
			App->render->Blit(profile_text, 150, 100, &window_profile_1, SDL_FLIP_NONE, false, 0.3f);
			App->render->Blit(profile_text, 185, 220, &blackMage, SDL_FLIP_NONE, false, 0.3f);
			
			//HEART
			if (App->shop->heartLevel == 1 || App->shop->heartLevel >= 2)
				App->render->Blit(profile_text, 520, 248, &upgrade, SDL_FLIP_NONE, false, 0.3f);
				
			else if (App->shop->heartLevel == 1)
				App->render->Blit(profile_text, 553, 248, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			//SWORD
			if (App->shop->swordLevel == 1 || App->shop->swordLevel >= 2)
				App->render->Blit(profile_text, 520, 368, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			else if (App->shop->swordLevel == 1)
				App->render->Blit(profile_text, 553, 368, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			//SHIELD
			if (App->shop->bookLevel == 1 || App->shop->bookLevel >= 2)
				App->render->Blit(profile_text, 520, 494, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			else if (App->shop->bookLevel == 1)
				App->render->Blit(profile_text, 553, 494, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			//SPEED
			if (App->shop->bootsLevel == 1 || App->shop->bootsLevel >= 2)
				App->render->Blit(profile_text, 731, 248, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			else if (App->shop->bootsLevel == 1)
				App->render->Blit(profile_text, 764, 248, &upgrade, SDL_FLIP_NONE, false, 0.3f);
			
			//ABILITY1
			if (App->shop->hourglassLevel == 1 || App->shop->hourglassLevel >= 2)
				App->render->Blit(profile_text, 731, 368, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			else if (App->shop->hourglassLevel == 1)
				App->render->Blit(profile_text, 764, 368, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			//ABILITY2
			if (App->shop->hourglassLevel == 1 || App->shop->hourglassLevel >= 2)
				App->render->Blit(profile_text, 731, 495, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			else if (App->shop->hourglassLevel == 1)
				App->render->Blit(profile_text, 764, 495, &upgrade, SDL_FLIP_NONE, false, 0.3f);
		}
		else if (dragoon_knight) {
			App->render->Blit(profile_text, 150, 100, &window_profile_2, SDL_FLIP_NONE, false, 0.3f);
			App->render->Blit(profile_text, 158, 220, &dragoonKnight, SDL_FLIP_NONE, false, 0.3f);

			//HEART
			if (App->shop->heartLevel == 1 || App->shop->heartLevel >= 2)
				App->render->Blit(profile_text, 520, 248, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			else if (App->shop->heartLevel == 1)
				App->render->Blit(profile_text, 553, 248, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			//SWORD
			if (App->shop->swordLevel == 1 || App->shop->swordLevel >= 2)
				App->render->Blit(profile_text, 520, 368, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			else if (App->shop->swordLevel == 1)
				App->render->Blit(profile_text, 553, 368, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			//SHIELD
			if (App->shop->bookLevel == 1 || App->shop->bookLevel >= 2)
				App->render->Blit(profile_text, 520, 494, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			else if (App->shop->bookLevel == 1)
				App->render->Blit(profile_text, 553, 494, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			//SPEED
			if (App->shop->bootsLevel == 1 || App->shop->bootsLevel >= 2)
				App->render->Blit(profile_text, 731, 248, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			else if (App->shop->bootsLevel == 1)
				App->render->Blit(profile_text, 764, 248, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			//ABILITY1
			if (App->shop->hourglassLevel == 1 || App->shop->hourglassLevel >= 2)
				App->render->Blit(profile_text, 731, 368, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			else if (App->shop->hourglassLevel == 1)
				App->render->Blit(profile_text, 764, 368, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			//ABILITY2
			if (App->shop->hourglassLevel == 1 || App->shop->hourglassLevel >= 2)
				App->render->Blit(profile_text, 731, 495, &upgrade, SDL_FLIP_NONE, false, 0.3f);

			else if (App->shop->hourglassLevel == 1)
				App->render->Blit(profile_text, 764, 495, &upgrade, SDL_FLIP_NONE, false, 0.3f);
		}

	}

	return true;
}

bool j1Hud::CleanUp()
{
	App->tex->UnLoad(frames_tex);
	App->tex->UnLoad(potion_tex);
	App->tex->UnLoad(dialog_tex);
	App->tex->UnLoad(profile_text);
	App->tex->UnLoad(hud_text);
	App->tex->UnLoad(score);

	//for (std::list<j1Button*>::iterator item = hud_buttons.begin(); item != hud_buttons.end(); ++item) {
	//	(*item)->CleanUp();
	//	hud_buttons.remove(*item);
	//}
	/*for (std::list<j1Label*>::iterator item = labels_list.begin(); item != labels_list.end(); ++item) {
		(*item)->CleanUp();
		labels_list.remove(*item);
	}*/

	return true;
}