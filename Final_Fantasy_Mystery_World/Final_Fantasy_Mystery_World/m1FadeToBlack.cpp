#include <math.h>
#include "App.h"
#include "m1FadeToBlack.h"
#include "m1Render.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "m1Window.h"
#include "m1Scene.h"
#include "e1Player.h"
#include "m1Input.h"
#include "p2Log.h"
#include "m1EntityManager.h"
#include "e1Enemy.h"
#include "e1AmazingDragon.h"
#include "m1Cutscene.h"
#include "m1DialogSystem.h"
#include "App.h"
#include "u1Bar.h"
#include "u1Label.h"
#include "m1MenuManager.h"
#include "m1Map.h"

#include "Brofiler/Brofiler.h"

m1FadeToBlack::m1FadeToBlack()
{
	name.assign("fadetoblack");
}

m1FadeToBlack::~m1FadeToBlack()
{
}


// Load assets
bool m1FadeToBlack::Start()
{
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);

	screen = { 0,0,(int)App->win->width,  (int)App->win->height };

	return true;
}

// Update: draw background
bool m1FadeToBlack::PostUpdate()
{
	BROFILER_CATEGORY("UpdateFadeToBlack", Profiler::Color::DarkOrchid);

	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (vibration) {
			App->input->ControllerVibration(0.7F, 150);
			App->render->CameraTremble(1.0F, 9000);
		}
		if (now >= total_time)
		{	
			if (to_disable != nullptr)
				to_disable->Disable();
			if (to_enable != nullptr)
				to_enable->Enable();
			if (want_to_change_map)
				App->map->ChangeMap(map_to_change);
			if (is_quest && App->map->quest_rooms != nullptr) {
				App->map->quest_rooms->LoadRoom(App->map->quest_rooms->actual_room->id);
			}
			//if (map_to_change == Maps::TUTORIAL && !App->cutscene_manager->is_executing) //Maybe this will break something, if it does, i'm sorry Att: Ivan
				//App->menu_manager->ShowHUD(true);
			if (vibration) {
				std::vector<e1Entity*> entities = App->entity_manager->GetEntities();
				std::vector<e1Entity*>::iterator item = entities.begin();
				for (; item != entities.end(); ++item) {
					if ((*item) != nullptr && (*item)->type == e1Entity::EntityType::ENEMY && static_cast<e1Enemy*>(*item)->enemy_type == e1Enemy::EnemyType::AMAZING_DRAGON) {
						(*item)->drawable = true;
					
						(*item)->allow_turn = true;
						static_cast<e1AmazingDragon*>(*item)->active = true;
						break;
					}
				}
			}
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time) {
			current_step = fade_step::none;
			App->dialog->end_dial = true;
			to_disable = nullptr;
			to_enable = nullptr;
			vibration = false;
			if (App->scene->player != nullptr && App->cutscene_manager->is_executing == false)
				App->scene->player->BlockControls(false);
			if (map_to_change == Maps::LOBBY && App->globals.ability3_gained && !App->globals.Ability3PanelShowed) {
				App->globals.Ability3PanelShowed = true;
				App->menu_manager->CreateHelpAbilityMenu(AbilityType::ABILITY3);
				App->scene->player->BlockControls(true);
			}
		}
			
	} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool m1FadeToBlack::FadeToBlack(m1Module* module_off, m1Module* module_on, float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		to_enable = module_on;
		to_disable = module_off;
		is_quest = false;
		want_to_change_map = false;
		ret = true;
	}

	return ret;
}
bool m1FadeToBlack::FadeToBlack(m1Module * module_off, float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		to_disable = module_off;
		want_to_change_map = false;
		is_quest = false;
		ret = true;
	}

	return ret;
}
bool m1FadeToBlack::FadeToBlack(float time, m1Module * module_on)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		to_enable = module_on;
		want_to_change_map = false;
		is_quest = false;
		ret = true;
	}

	return ret;
}
bool m1FadeToBlack::FadeToBlack(float time, bool vibration)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		this->vibration = vibration;
		App->scene->player->BlockControls(true);
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		is_quest = false;
		want_to_change_map = false;
		ret = true;
	}

	return ret;
}
bool m1FadeToBlack::FadeToBlack(Maps type, float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		want_to_change_map = true;
		map_to_change = type;
		is_quest = false;
		if (map_to_change == Maps::HOME && App->map->actual_map != Maps::LOBBY)
		{
				App->scene->player->AugmentLives(App->scene->player->stats.max_lives);
				App->scene->player->AugmentMana(App->scene->player->stats.max_mana);
		}
		ret = true;
	}

	return ret;
}
bool m1FadeToBlack::FadeToBlack(m1Module * module_off, Maps type, float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		want_to_change_map = true;
		to_disable = module_off;
		map_to_change = type;
		is_quest = false;
		ret = true;
	}

	return ret;
}
bool m1FadeToBlack::FadeToBlack(bool IsQuest, float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		want_to_change_map = false;
		is_quest = IsQuest;
		ret = true;
	}

	return ret;
}
bool m1FadeToBlack::IsFading() const
{
	return current_step != fade_step::none;
}