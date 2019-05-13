#include "e1Enemy.h"
#include "m1Input.h"
#include "App.h"
#include "m1Render.h"
#include "m1Textures.h"
#include "m1Audio.h"
#include "u1UI_Element.h"
#include "e1Drop.h"
#include "m1GUI.h"
#include "m1EntityManager.h"
#include "m1Fonts.h"
#include "p2Rooms.h"
#include "m1Map.h"
#include <vector>
#include <string>
#include "m1Pathfinding.h"
#include "e1Player.h"
#include "m1Scene.h"
#include "m1Window.h"
#include "Brofiler/Brofiler.h"
#include <map>
#include "p2Log.h"



e1Enemy::e1Enemy(const int &x, const int &y) : e1DynamicEntity(x,y)
{
	type = e1Entity::EntityType::ENEMY;
	ground = App->tex->Load("assets/sprites/enemy_pos.png");
	current_animation = &IdleDownLeft;

	direction = Direction::DOWN_LEFT;
	state = State::IDLE;
	//has_turn = false;
	velocity.x = 160;
	velocity.y = 80;
	
	original_position = position;
	target_position = position;
	initial_position = position;
	movement_count = { 0,0 };
	actual_tile = App->map->WorldToMap(position.x, position.y);

	// THIS ALWAYS LAST
	
	
}

e1Enemy::~e1Enemy()
{
}

void e1Enemy::InitStats()
{
	for (std::list<Property<int>*>::iterator item = general_properties.properties.begin(); item != general_properties.properties.end(); item++) {
		if (strcmp((*item)->GetName(), "experience") == 0) {
			stats.experience = (*item)->GetValue();
		}
		else if (strcmp((*item)->GetName(), "attack_power") == 0) {
			stats.basic_attack_damage = (*item)->GetValue();
		}
		else if (strcmp((*item)->GetName(), "live") == 0) {
			stats.max_live = stats.live = (*item)->GetValue();
		}
		else if (strcmp((*item)->GetName(), "ratio_blue_rupee") == 0) {
			ratio_blue_rupee = (*item)->GetValue();
		}
		else if (strcmp((*item)->GetName(), "ratio_green_rupee") == 0) {
			ratio_green_rupee = (*item)->GetValue();
		}
		else if (strcmp((*item)->GetName(), "ratio_red_rupee") == 0) {
			ratio_red_rupee = (*item)->GetValue();
		}
		else if (strcmp((*item)->GetName(), "ratio_gold_rupee") == 0) {
			ratio_gold_rupee = (*item)->GetValue();
		}
		else if (strcmp((*item)->GetName(), "ratio_rupee") == 0) {
			ratio_rupee = (*item)->GetValue();
		}
		else if (strcmp((*item)->GetName(), "ratio_poti_hp") == 0) {
			ratio_poti_hp = (*item)->GetValue();
		}
		else if (strcmp((*item)->GetName(), "ratio_poti_mana") == 0) {
			ratio_poti_mana = (*item)->GetValue();
		}
	}
}

bool e1Enemy::PreUpdate()
{
	switch (state)
	{
	case State::IDLE:
		if (CanAttack() || IsPlayerInRange(range_to_distance_attack)) {
			if (want_to_attack) {
				state = State::BEFORE_ATTACK;
				time_to_wait_before_attack.Start();
			}
			else {
				Escape();
			}
		}
		else if (IsPlayerInRange(range_to_walk)) {
			state = State::WALKING;
			MovementLogic();
		}
		else {
			turn_done = true;
		}
		break;
	case State::BEFORE_ATTACK:
		if (time_to_wait_before_attack.ReadSec() >= 0.25f) {
			time_to_wait_before_attack.Stop();
			state = State::ATTACKING;
			LookToPlayer();

			if (IsPlayerNextTile()) {
				type_attack = Attacks::BASIC;
				PrepareBasicAttack();
			}
			else {
				type_attack = Attacks::SPECIAL_1;
				PrepareDistanceAttack();
			}
			
			ChangeAnimation(direction, state, type_attack);
		}
		break;
	case State::SLEEPING:
		if (IsPlayerNextTile()) {
			state = State::IDLE;
		}
		turn_done = true;
		break;
	default:
		//turn_done = true;
		break;
	}

	return true;
}

bool e1Enemy::Update(float dt)
{
	switch (state)
	{
	case State::IDLE:
		position.x = initial_position.x + movement_count.x;
		position.y = initial_position.y + movement_count.y;
		target_position = position;
		break;
	case State::WALKING:
		PerformMovement(dt);
		break;
	case State::BEFORE_ATTACK:
		break;
	case State::ATTACKING: {
		bool attack = false;

		switch (type_attack)
		{
		case Attacks::BASIC:
			if (current_animation->Finished()) {
				App->audio->PlayFx(App->scene->fx_plant_attack);
				CheckBasicAttackEffects(e1Entity::EntityType::PLAYER, direction, stats.basic_attack_damage);
				FinishBasicAttack();
				attack = true;
			}
			break;
		case Attacks::SPECIAL_1:
			if (IsSpecialAttack1Finished()) {
				App->audio->PlayFx(App->scene->fx_dog_attack);
				App->scene->player->ReduceLives(stats.special_attack_damage);
				AfetSpecialAttack1();
				attack = true;
			}
			break;
		case Attacks::SPECIAL_2:
			break;
		}

		if (attack) {
			state = State::AFTER_ATTACK;
			ChangeAnimation(direction, state);
			time_attack = SDL_GetTicks();
		}

	}
		break;
	case State::AFTER_ATTACK:
		if (RestTimeAfterAttack(time_attack)) {
			AfterAttack();
		}
		break;
	case State::DEATH:
		if (current_animation->Finished()) {
			Drop();
			App->audio->PlayFx(App->scene->fx_kill_enemy);
			App->scene->player->UpdateExperience(stats.experience);
			App->map->quest_rooms->AddEntityToNotRepeat(original_position);
			to_delete = true;
			turn_done = true;
		}
		break;
	default:
		break;
	}

	if (App->debug)
		App->render->Blit(ground, App->map->MapToWorld(actual_tile.x, actual_tile.y).x + 1, App->map->MapToWorld(actual_tile.x, actual_tile.y).y - 8, NULL, true);

	UpdateEnemy();

	return true;
}

bool e1Enemy::Load(pugi::xml_node &)
{
	return true;
}

bool e1Enemy::Save(pugi::xml_node &) const
{
	return true;
}

bool e1Enemy::CleanUp()
{
	App->tex->UnLoad(ground);
	ground = nullptr;
	return true;
}

bool e1Enemy::IsAnotherEnemyNextTile()
{
	BROFILER_CATEGORY("IsAnotherEnemyNextTile", Profiler::Color::SeaGreen);
	bool ret = false;
	std::vector<e1Entity*> entities = App->entity_manager->GetEntities();
	std::vector<e1Entity*>::iterator item = entities.begin();

	for (; item != entities.end(); ++item) {
		if ((*item) != nullptr && (*item)->type == e1Entity::EntityType::ENEMY) {
			iPoint origin = actual_tile;
			iPoint destination = (*item)->actual_tile;

			if (origin.x + 1 == destination.x && origin.y == destination.y) {
				direction = Direction::DOWN_RIGHT;
				current_animation = &IdleDownRight;
				next_enemy_pos.push_back(destination);
				ret = true;
			}
			if (origin.x == destination.x && origin.y + 1 == destination.y) {
				direction = Direction::DOWN_LEFT;
				current_animation = &IdleDownLeft;
				next_enemy_pos.push_back(destination);
				ret = true;
			}
			if (origin.x == destination.x && origin.y - 1 == destination.y) {
				direction = Direction::UP_RIGHT;
				current_animation = &IdleUpRight;
				next_enemy_pos.push_back(destination);
				ret = true;
			}
			if (origin.x - 1 == destination.x && origin.y == destination.y) {
				direction = Direction::UP_LEFT;
				current_animation = &IdleUpLeft;
				next_enemy_pos.push_back(destination);
				ret = true;
			}
			if (origin.x + 1 == destination.x && origin.y + 1 == destination.y) {
				direction = Direction::DOWN;
				current_animation = &IdleDown;
				next_enemy_pos.push_back(destination);
				ret = true;
			}
			if (origin.x - 1 == destination.x && origin.y + 1 == destination.y) {
				direction = Direction::LEFT;
				current_animation = &IdleLeft;
				next_enemy_pos.push_back(destination);
				ret = true;
			}
			if (origin.x - 1 == destination.x && origin.y - 1 == destination.y) {
				direction = Direction::UP;
				current_animation = &IdleUp;
				next_enemy_pos.push_back(destination);
				ret = true;
			}
			if (origin.x + 1 == destination.x && origin.y - 1 == destination.y) {
				direction = Direction::RIGHT;
				current_animation = &IdleRight;
				next_enemy_pos.push_back(destination);
				ret = true;
			}

		}
	}

	return ret;
}

bool e1Enemy::IsPlayerNextTile()
{
	BROFILER_CATEGORY("IsAnotherPlayerNextTile", Profiler::Color::SeaGreen);
	bool ret = false;
	std::vector<e1Entity*> entities = App->entity_manager->GetEntities();
	std::vector<e1Entity*>::iterator item = entities.begin();

	for (; item != entities.end(); ++item) {
		if ((*item) != nullptr && (*item)->type == e1Entity::EntityType::PLAYER) {
			iPoint origin = actual_tile;
			iPoint destination = (*item)->actual_tile;

			if (origin.x + 1 == destination.x && origin.y == destination.y) {
				direction = Direction::DOWN_RIGHT;
				current_animation = &IdleDownRight;
				ret = true;
			}
			if (origin.x == destination.x && origin.y + 1 == destination.y) {
				direction = Direction::DOWN_LEFT;
				current_animation = &IdleDownLeft;
				ret = true;
			}
			if (origin.x == destination.x && origin.y - 1 == destination.y) {
				direction = Direction::UP_RIGHT;
				current_animation = &IdleUpRight;
				ret = true;
			}
			if (origin.x - 1 == destination.x && origin.y == destination.y) {
				direction = Direction::UP_LEFT;
				current_animation = &IdleUpLeft;
				ret = true;
			}
			if (origin.x + 1 == destination.x && origin.y + 1 == destination.y) {
				direction = Direction::DOWN;
				current_animation = &IdleDown;
				ret = true;
			}
			if (origin.x - 1 == destination.x && origin.y + 1 == destination.y) {
				direction = Direction::LEFT;
				current_animation = &IdleLeft;
				ret = true;
			}
			if (origin.x - 1 == destination.x && origin.y - 1 == destination.y) {
				direction = Direction::UP;
				current_animation = &IdleUp;
				ret = true;
			}
			if (origin.x + 1 == destination.x && origin.y - 1 == destination.y) {
				direction = Direction::RIGHT;
				current_animation = &IdleRight;
				ret = true;
			}
				
		}
	}

	return ret;
}

bool e1Enemy::IsPlayerInRange(const int& range) {
	iPoint distance = App->scene->player->actual_tile - actual_tile;

	return abs(distance.x) <= range && abs(distance.y) <= range;
}

void e1Enemy::LookToPlayer() {
	iPoint distance = App->scene->player->actual_tile - actual_tile;

	if (abs(distance.x) == abs(distance.y)) {
		if (distance.x > 0) {
			if (distance.y > 0) {
				direction = Direction::DOWN;
			}
			else if (distance.y < 0) {
				direction = Direction::RIGHT;
			}
		}
		if (distance.x < 0) {
			if (distance.y > 0) {
				direction = Direction::LEFT;
			}
			else if (distance.y < 0) {
				direction = Direction::UP;
			}
		}
	}
	
	else if (distance.x == 0) {
		if (distance.y < 0) {
			direction = Direction::UP_RIGHT;
		}
		else if (distance.y > 0) {
			direction = Direction::DOWN_LEFT;
		}
	}

	else if (distance.y == 0) {
		if (distance.x < 0) {
			direction = Direction::DOWN_RIGHT;
		}
		else if (distance.x > 0) {
			direction = Direction::UP_LEFT;
		}
	}

	else {
		if (distance.x > 0) {
			if (distance.y > 0) {
				if (distance.y > distance.x) {
					direction = Direction::DOWN_LEFT;
				}
				else {
					direction = Direction::DOWN_RIGHT;
				}
			}
			else {
				if (abs(distance.y) > distance.x) {
					direction = Direction::UP_LEFT;
				}
				else {
					direction = Direction::DOWN_RIGHT;
				}
			}
		}
		else if (distance.x < 0) {
			if (distance.y > 0) {
				if (distance.y > abs(distance.x)) {
					direction = Direction::DOWN_LEFT;
				}
				else {
					direction = Direction::UP_LEFT;
				}
			}
			else {
				if (distance.y > distance.x) {
					direction = Direction::UP_LEFT;
				}
				else {
					direction = Direction::UP_RIGHT;
				}
			}
		}
	}

	/*float degree = atan2f(distance.y, distance.x);

	if (degree == 90.0f) {
		direction = Direction::DOWN_LEFT;
	}
	else if (degree == -90.0f) {
		direction = Direction::UP_RIGHT;
	}
	else if (degree == 0.0f) {
		direction = Direction::DOWN_RIGHT;
	}
	else if (degree == -180.0f) {
		direction = Direction::UP_LEFT;
	}
	else if (IN_RANGE(degree, 90, 0)) {
		direction = Direction::DOWN;
	}
	else if (IN_RANGE(degree, 0, -90)) {
		direction = Direction::DOWN;
	}
	else if (IN_RANGE(degree, 90, 0)) {
		direction = Direction::DOWN;
	else if (IN_RANGE(degree, 90, 0)) {
		direction = Direction::DOWN;
	}
	}*/
	
}

void e1Enemy::MovementLogic()
{
	BROFILER_CATEGORY("MovementLogic Enemy", Profiler::Color::SeaGreen);
	iPoint origin = actual_tile;
	iPoint destination = App->scene->player->actual_tile;

	if (!IsAnotherEnemyNextTile())
		App->pathfinding->CreatePath(origin, destination);
	else {
		App->pathfinding->RecalculatePath(origin, destination, next_enemy_pos);
		next_enemy_pos.clear();
	}
	

	iPoint target = App->pathfinding->GetLastPath();
	if (App->pathfinding->last_path.empty()) {
		turn_done = true;
		state = State::IDLE;
		return;
	}

	if (target_position == position && !IsPlayerNextTile()) {
		if (target.x == origin.x && target.y > origin.y) {
			direction = Direction::DOWN_LEFT;
			target_position.create(position.x - (App->map->data.tile_width / 2), position.y + (App->map->data.tile_height / 2));
			movement_count.x -= (App->map->data.tile_width / 2);
			movement_count.y += (App->map->data.tile_height / 2);
			actual_tile += {0, 1};
		}
		else if (target.x > origin.x && target.y == origin.y) {
			direction = Direction::DOWN_RIGHT;
			target_position.create(position.x + (App->map->data.tile_width / 2), position.y + (App->map->data.tile_height / 2));
			movement_count.x += (App->map->data.tile_width / 2);
			movement_count.y += (App->map->data.tile_height / 2);
			actual_tile += {1, 0};
		}
		else if (target.x == origin.x && target.y < origin.y) {
			direction = Direction::UP_RIGHT;
			target_position.create(position.x + (App->map->data.tile_width / 2), position.y - (App->map->data.tile_height / 2));
			movement_count.x += (App->map->data.tile_width / 2);
			movement_count.y -= (App->map->data.tile_height / 2);
			actual_tile += {0, -1};
		}
		else if (target.x < origin.x && target.y == origin.y) {
			direction = Direction::UP_LEFT;
			target_position.create(position.x - (App->map->data.tile_width / 2), position.y - (App->map->data.tile_height / 2));
			movement_count.x -= (App->map->data.tile_width / 2);
			movement_count.y -= (App->map->data.tile_height / 2);
			actual_tile += {-1, 0};
		}
		else if (target.x < origin.x && target.y < origin.y) {
			direction = Direction::UP;
			target_position.create(position.x, position.y - App->map->data.tile_height);
			movement_count.y -= App->map->data.tile_height;
			actual_tile += {-1, -1};
		}
		else if (target.x > origin.x && target.y > origin.y) {
			direction = Direction::DOWN;
			target_position.create(position.x, position.y + App->map->data.tile_height);
			movement_count.y += App->map->data.tile_height;
			actual_tile += {1, 1};
		}
		else if (target.x < origin.x && target.y > origin.y) {
			direction = Direction::LEFT;
			target_position.create(position.x - App->map->data.tile_width, position.y);
			movement_count.x -= App->map->data.tile_width;
			actual_tile += {-1, 1};
		}
		else if (target.x > origin.x && target.y < origin.y) {
			direction = Direction::RIGHT;
			target_position.create(position.x + App->map->data.tile_width, position.y);
			movement_count.x += App->map->data.tile_width;
			actual_tile += {1, -1};
		}
		turn_done = true;
	}



}

void e1Enemy::PerformMovement(float dt)
{
	BROFILER_CATEGORY("PerformMovement Enemy", Profiler::Color::SeaGreen);
	switch (direction)
	{
	case Direction::DOWN_LEFT:
		if (position.x >= initial_position.x + movement_count.x && position.y <= initial_position.y + movement_count.y) {
			position.x -= floor(velocity.x * dt);
			position.y += floor(velocity.y * dt);
			current_animation = &GoDownLeft;
		}
		else {
			target_position = position;
			current_animation = &IdleDownLeft;
			IsPlayerNextTile();
			state = State::IDLE;
		}
		break;
	case Direction::UP_RIGHT:
		if (position.x <= initial_position.x + movement_count.x  && position.y >= initial_position.y + movement_count.y) {
			position.x += floor(velocity.x * dt);
			position.y -= floor(velocity.y * dt);
			current_animation = &GoUpRight;
		}
		else {
			target_position = position;
			current_animation = &IdleUpRight;
			IsPlayerNextTile();
			state = State::IDLE;
		}
		break;
	case Direction::UP_LEFT:
		if (position.x >= initial_position.x + movement_count.x  && position.y >= initial_position.y + movement_count.y) {
			position.x -= floor(velocity.x * dt);
			position.y -= floor(velocity.y * dt);
			current_animation = &GoUpLeft;
		}
		else {
			target_position = position;
			current_animation = &IdleUpLeft;
			IsPlayerNextTile();
			state = State::IDLE;
		}
		break;
	case Direction::DOWN_RIGHT:
		if (position.x <= initial_position.x + movement_count.x && position.y <= initial_position.y + movement_count.y) {
			position.x += floor(velocity.x * dt);
			position.y += floor(velocity.y * dt);
			current_animation = &GoDownRight;
		}
		else {
			target_position = position;
			current_animation = &IdleDownRight;
			IsPlayerNextTile();
			state = State::IDLE;
		}
		break;
	case Direction::LEFT:
		if (position.x >= initial_position.x + movement_count.x && position.y == initial_position.y + movement_count.y) {
			position.x -= floor(velocity.x * dt);
			current_animation = &GoLeft;
		}
		else {
			target_position = position;
			current_animation = &IdleLeft;
			IsPlayerNextTile();
			state = State::IDLE;
		}
		break;
	case Direction::RIGHT:
		if (position.x <= initial_position.x + movement_count.x && position.y == initial_position.y + movement_count.y) {
			position.x += floor(velocity.x * dt);
			current_animation = &GoRight;
		}
		else {
			target_position = position;
			current_animation = &IdleRight;
			IsPlayerNextTile();
			state = State::IDLE;
		}
		break;
	case Direction::UP:
		if (position.x == initial_position.x + movement_count.x && position.y >= initial_position.y + movement_count.y) {
			position.y -= floor(velocity.y * dt);
			current_animation = &GoUp;
		}
		else {
			target_position = position;
			current_animation = &IdleUp;
			IsPlayerNextTile();
			state = State::IDLE;
		}
		break;
	case Direction::DOWN:
		if (position.x == initial_position.x + movement_count.x && position.y <= initial_position.y + movement_count.y) {
			position.y += floor(velocity.y * dt);
			current_animation = &GoDown;
		}
		else {
			target_position = position;
			current_animation = &IdleDown;
			IsPlayerNextTile();
			state = State::IDLE;
		}
		break;
	default:
		break;
	}

}

void e1Enemy::GetHitted(const int & damage_taken)
{
	stats.live -= damage_taken;
	times_hitted++;
	//(int)(camera.x * speed) + x * scale;
	iPoint pos{ 0,0 };
	pos.x = (int)(App->render->camera.x) + (position.x + pivot.x - 5) * (int)App->win->GetScale();
	pos.y = (int)(App->render->camera.y) + position.y * (int)App->win->GetScale();
	App->gui->AddHitPointLabel(pos.x, pos.y, std::to_string(damage_taken).data(), App->gui->screen,RED, FontType::PMIX24);
	state = State::IDLE;
	if (stats.live <= 0 || App->scene->player->god_mode) {
		state = State::DEATH;
		ChangeAnimation(direction, state);
	}
}

void e1Enemy::Drop()
{

	int drop_item = App->random.Generate(1, 100);


	std::vector<int> drop_ratio;
	drop_ratio.push_back(ratio_poti_hp);
	drop_ratio.push_back(ratio_poti_mana);
	drop_ratio.push_back(ratio_rupee);
	std::sort(drop_ratio.begin(), drop_ratio.end(), std::less<int>());

	std::vector<int>::iterator item = drop_ratio.begin();
	int i_dont_really_know_how_to_name_this_variable_love_me_guys = 0; // :D
	for (; item != drop_ratio.end(); ++item) {
		if ((*item) + i_dont_really_know_how_to_name_this_variable_love_me_guys >= drop_item) {
			if ((*item) == ratio_rupee) {
				CalculateDrop();
			}
			else if ((*item) == ratio_poti_hp) {
				e1Drop* drop = (e1Drop*)App->entity_manager->CreateEntity(e1Entity::EntityType::DROP, actual_tile.x, actual_tile.y, "health_potion");
				App->map->quest_rooms->AddDrop(actual_tile, DropsType::HEALTH_POTION);
			}
			else if ((*item) == ratio_poti_mana) {
				e1Drop* drop = (e1Drop*)App->entity_manager->CreateEntity(e1Entity::EntityType::DROP, actual_tile.x, actual_tile.y, "mana_potion");
				App->map->quest_rooms->AddDrop(actual_tile, DropsType::MANA_POTION);
			}
			break;
		}
		else
			i_dont_really_know_how_to_name_this_variable_love_me_guys += (*item);
	}
}

int e1Enemy::CalculateDrop()
{
	int rupee_percentage = App->random.Generate(1, 100); //Calculate the percentage

	std::vector<int> drop_ratio;
	drop_ratio.push_back(ratio_green_rupee);
	drop_ratio.push_back(ratio_gold_rupee);
	drop_ratio.push_back(ratio_blue_rupee);
	drop_ratio.push_back(ratio_red_rupee);
	std::sort(drop_ratio.begin(), drop_ratio.end(), std::less<int>());

	e1Drop* drop = nullptr;
	std::vector<int>::iterator item = drop_ratio.begin();
	int i_dont_really_know_how_to_name_this_variable_love_me_guys = 0; // :D
	for (; item != drop_ratio.end(); ++item) {
		if ((*item) + i_dont_really_know_how_to_name_this_variable_love_me_guys >= rupee_percentage) {
			if ((*item) == ratio_green_rupee) {
				drop = (e1Drop*)App->entity_manager->CreateEntity(e1Entity::EntityType::DROP, actual_tile.x, actual_tile.y, "green_rupee");
				drop->SetGold(App->random.Generate(15, 25));
				App->map->quest_rooms->AddDrop(actual_tile, drop->drop_type);
			}
			else if ((*item) == ratio_blue_rupee) {
				drop = (e1Drop*)App->entity_manager->CreateEntity(e1Entity::EntityType::DROP, actual_tile.x, actual_tile.y, "blue_rupee");
				drop->SetGold(App->random.Generate(45, 65));
				App->map->quest_rooms->AddDrop(actual_tile, drop->drop_type);
			}
			else if ((*item) == ratio_red_rupee) {
				drop = (e1Drop*)App->entity_manager->CreateEntity(e1Entity::EntityType::DROP, actual_tile.x, actual_tile.y, "red_rupee");
				drop->SetGold(App->random.Generate(90, 110));
				App->map->quest_rooms->AddDrop(actual_tile, drop->drop_type);
			}
			else if ((*item) == ratio_gold_rupee) {
				drop = (e1Drop*)App->entity_manager->CreateEntity(e1Entity::EntityType::DROP, actual_tile.x, actual_tile.y, "gold_rupee");
				drop->SetGold(App->random.Generate(300, 400));
				App->map->quest_rooms->AddDrop(actual_tile, drop->drop_type);
			}
			break;
		}
		else
			i_dont_really_know_how_to_name_this_variable_love_me_guys += (*item);
	}
	return 0;
}
