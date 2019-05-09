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
{
	bar_type = type;

	this->max_capacity = max_capacity;
	current_quantity = max_capacity;

	if (type == HPBAR)
	{
		current_width = max_width;
		empty_bar = App->gui->AddImage(x, y, { 1400, 3104, 185, 25 }, App->scene, parent, false, false, false, false);
		filled_bar = App->gui->AddImage(7, 5, { 1405, 3149, 172, 10 }, App->scene, empty_bar, false, false, false, false);
		PrintBarNumbers();
	}

	if (type == MANABAR)
	{
		current_width = max_width;
		empty_bar = App->gui->AddImage(x, y, { 1400, 3104, 185, 25 }, App->scene, parent, false, false, false, false);
		filled_bar = App->gui->AddImage(7, 5, { 1405, 3185, 172, 10 }, App->scene, empty_bar, false, false, false, false);
		PrintBarNumbers();
	}

	if (type == EXPBAR)
	{
		current_width = 0;
		current_quantity = 0;
		max_width = 369;
		empty_bar = App->gui->AddImage(x, y, { 1348, 3215, 372, 11 }, App->scene, parent, false, false, false, false); // this is empty
		filled_bar = App->gui->AddImage(2, 1, { 1353, 3233, 369, 8 }, App->scene, empty_bar, false, false, false, false); // this is filled
	}
	if (type == SKIPBAR)
	{
		current_quantity = 0;
		current_width = 0;
		max_width = 33;
		empty_bar = App->gui->AddImage(x, y, { 1486,2111,33,31 }, nullptr, parent, true, false, false, false);
		filled_bar = App->gui->AddImage(x, y, { 1418,2111,0,34 }, nullptr, empty_bar, true, false, false, false);
	}

	
}

u1Bar::~u1Bar() {
	bar_numbers_label = nullptr;
}

void u1Bar::UpdateBar(int quantity, UIType bar_type)
{
	if (empty_bar != nullptr)
	{
		if(bar_type == UIType::HPBAR || bar_type == UIType::MANABAR || bar_type == UIType::SKIPBAR)
			targe_width = CalculateBar(quantity);
		
		else
			targe_width = CalculateExpBar(quantity);

		if (targe_width != current_width) {
			has_change = true;
		}
		
	}
	if (bar_type != SKIPBAR && bar_type != UIType::EXPBAR)
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

	if (has_change) {
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

void u1Bar::PrintBarNumbers()
{
	if (bar_numbers_label != nullptr)
		App->gui->DeleteUIElement(bar_numbers_label);

	if (current_quantity < 0)
		current_quantity = 0;

	std::string bar_nums_str = std::to_string(current_quantity) + "/" + std::to_string(max_capacity);

	bar_numbers_label = App->gui->AddLabel(100, 5, bar_nums_str.c_str(), filled_bar, BLACK, FontType::FF32, App->scene, false);

}

