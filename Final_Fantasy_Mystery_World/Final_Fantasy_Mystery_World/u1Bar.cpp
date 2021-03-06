#include "App.h"
#include "u1Bar.h"
#include "u1Image.h"
#include "e1Player.h"
#include "u1Label.h"
#include "m1MenuManager.h"
#include "m1Scene.h"
#include "m1Render.h"
#include "e1Player.h"
#include "m1EntityManager.h"
#include "p2Log.h"

u1Bar::u1Bar(const int &x, const int &y, int max_capacity, UIType type, u1GUI* parent, m1Module* callback) :u1GUI(BAR, x, y, parent, {0, 0, 0, 0}, true, false, false, false)
 , bar_type(type), max_capacity(max_capacity), current_quantity(max_capacity)
{
	if (type == HPBAR)
	{
		current_width = max_width;
		empty_bar = App->gui->AddImage(-2, -2, { 2232, 197, 118, 20 }, nullptr, this, false, false, false, false);
		filled_bar = App->gui->AddImage(0, 0, { 2139, 331, 118, 19 }, nullptr, empty_bar, false, false, false, false);
		PrintBarNumbers();
	}

	else if (type == MANABAR)
	{
		current_width = max_width;
		empty_bar = App->gui->AddImage(-2, -2, { 2232, 197, 118, 20 }, nullptr, this, false, false, false, false);
		filled_bar = App->gui->AddImage(0, 0, { 2139, 361, 118, 19 }, nullptr, empty_bar, false, false, false, false);
		PrintBarNumbers();
	}

	else if (type == EXPBAR)
	{
		current_width = 0;
		current_quantity = 0;
		max_width = 372;
		empty_bar = App->gui->AddImage(0, 0, { 1374, 3237, 372, 11 }, nullptr, this, false, false, false, false); // this is empty
		filled_bar = App->gui->AddImage(1, 0, { 1371, 3217, 372, 11 }, nullptr, empty_bar, false, false, false, false); // this is filled
		empty_bar->drawable = true;
	}

	else if (type == ENEMYBAR) 
	{
		max_width = 812;
		current_width = max_width;
		empty_bar = App->gui->AddImage(0, 0, { 2119, 86, 820, 24 }, nullptr, this, false, false, false, false);
		filled_bar = App->gui->AddImage(4, 4, { 2123, 55, 812, 16 }, nullptr, empty_bar, false, false, false, false);
	}
	else if (type == SKIPBAR)
	{
		current_quantity = 0;
		current_width = 0;
		max_width = 33;
		empty_bar = App->gui->AddImage(0, 0, { 1486,2111,33,31 }, nullptr, this, true, false, false, false);
		filled_bar = App->gui->AddImage(0, 0, { 1418,2111,0,34 }, nullptr, empty_bar, true, false, false, false);
	}

	else if (type == SKILLBAR)
	{
		current_quantity = 0;
		current_width = 0;
		current_height = 0;
		max_width = 11;
		max_height = 56;
		/*empty_bar = App->gui->AddImage(0, 0, { 2725, 197, 13, 58 }, nullptr, this, true, false, false, false);*/
		empty_bar = App->gui->AddImage(0, 0, { 2707, 198, 11, 56 }, nullptr, this, false, false, false, false);  // In reality is FILLED
		filled_bar = App->gui->AddImage(0, 0, { 2726, 198, 11, 56 }, nullptr, empty_bar, false, false, false, false); // In reality is EMPTY
	}

}

u1Bar::~u1Bar() {
}

void u1Bar::UpdateBar(int quantity, UIType bar_type)
{
	if (empty_bar != nullptr)
	{
		if (bar_type == UIType::HPBAR || bar_type == UIType::MANABAR || bar_type == UIType::SKIPBAR || bar_type == UIType::ENEMYBAR || bar_type == UIType::DRAGONBAR /*|| bar_type == UIType::SKILLBAR*/)
			targe_width = CalculateBar(quantity);

		else if (bar_type == UIType::SKILLBAR)
		{
			empty_bar->drawable = false;
			targe_height = CalculateSkillBar(quantity);
		}
		
		else {
			empty_bar->drawable = false;
			got_xp = true;
			targe_width = CalculateExpBar(quantity);
		}
			

		if (targe_width != current_width) {
			has_change = true;
		}

		if (targe_height != current_height) {
			has_change = true;
		}
		
	}
	if (bar_type != SKIPBAR && bar_type != UIType::EXPBAR && bar_type != UIType::ENEMYBAR&& bar_type != UIType::DRAGONBAR && bar_type != UIType::SKILLBAR)
		PrintBarNumbers();
}

int u1Bar::CalculateBar(int quantity)
{
	int new_width = current_width;
	int new_quantity = (current_quantity + quantity);
	current_quantity += quantity;

	if (max_capacity != 0)
		new_width = (new_quantity * max_width) / max_capacity;

	if (current_quantity <= 0) {
		current_quantity = 0;
		return 0;
	}

	if (current_quantity >= max_capacity) {
		current_quantity = max_capacity;
		return max_width;
	}

	return new_width;

}

int u1Bar::CalculateSkillBar(int quantity)
{
	int new_height = current_height;
	int new_quantity = (current_quantity + quantity);
	current_quantity += quantity;

	LOG("CURRENT HEIGHT: %i", current_height);
	LOG("TARGE HEIGHT: %i", targe_height);

	if (max_capacity != 0)
		new_height = (new_quantity * max_height) / max_capacity;

	if (current_quantity <= 0) {
		current_quantity = 0;
		return 0;
	}

	if (current_quantity >= max_capacity) {
		current_quantity = max_capacity;
		return max_height;
	}

	return new_height;
}

int u1Bar::CalculateExpBar(int xp)
{
	int new_width = current_width;
	int new_quantity = (current_quantity + xp);
	current_quantity += xp;

	if (current_quantity >= max_capacity) {
		current_quantity = 0;
		new_width = 0;
	}

	if (current_quantity < max_capacity)
		new_width = (new_quantity * max_width) / max_capacity;

	if (current_quantity <= 0) {
		current_quantity = 0;
		return new_width = 0;
	}

	LOG("CURRENT QUANTITY: %i", current_quantity);

	return new_width;
}

void u1Bar::InnerDraw()
{

	if (!drawable)
		return;

	if (has_change && bar_type == UIType::EXPBAR) {
		if (current_width > targe_width) {

			current_width += 100 * App->GetDeltaTime();

			if (current_width >= max_width) {
				current_width = 0;
			}
		}

		else if (current_width < targe_width) {
			current_width += 100 * App->GetDeltaTime();
		}
		else {
			has_change = false;
		}
		filled_bar->section.w = current_width;
	}

	if (has_change && bar_type == UIType::SKILLBAR) {
		if (current_height > targe_height) {
			current_height -= 100 * App->GetDeltaTime();

		/*	if (current_height <=0) {
				current_height = max_height;
			}*/
		}
		else if (current_height < targe_height) {
			current_height += 100 * App->GetDeltaTime();
		}
		else {
			has_change = false;
		}
		filled_bar->section.h = current_height;
	}

	if (has_change && bar_type != UIType::EXPBAR && bar_type != UIType::SKILLBAR) {
		if (current_width > targe_width) {
			current_width -= 100 * App->GetDeltaTime();
		}
		else if (current_width < targe_width) {
			current_width += 100 * App->GetDeltaTime();
		}
		else {
			has_change = false;
		}
		filled_bar->section.w = current_width;
	}

	if (drawable) {
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), empty_bar->draw_offset.x, empty_bar->draw_offset.y, &empty_bar->section, false, SDL_FLIP_NONE, 0);
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), filled_bar->draw_offset.x, filled_bar->draw_offset.y, &filled_bar->section, false, SDL_FLIP_NONE, 0);
	}
}

void u1Bar::PrintBarNumbers(int addition)
{
	if (bar_numbers_label != nullptr)
		App->gui->DeleteUIElement(bar_numbers_label);

	if (current_quantity < 0)
		current_quantity = 0;

	
	aux_quantity = current_quantity + addition;

	std::string bar_nums_str = std::to_string(aux_quantity) + "/" + std::to_string(max_capacity);

	bar_numbers_label = App->gui->AddLabel(39, -10, bar_nums_str.c_str(), filled_bar, WHITE, FontType::FF32, App->scene, false);

}



