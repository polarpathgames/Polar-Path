#include "e1NPC.h"
#include "App.h"
#include "m1Render.h"
#include "m1Textures.h"
#include "m1Audio.h"
#include "m1EntityManager.h"
#include "Brofiler/Brofiler.h"
#include <string>

e1NPC::e1NPC(const int &x, const int &y) : e1DynamicEntity(x, y)
{
	type = e1Entity::EntityType::NPC;
	current_animation = &IdleDownLeft;
	SetPivot(10, 30);
	direction = Direction::DOWN_LEFT;
}

e1NPC::~e1NPC()
{
}

bool e1NPC::Load(pugi::xml_node &)
{
	return true;
}

bool e1NPC::Save(pugi::xml_node &) const
{
	return true;
}

bool e1NPC::CleanUp()
{
	return true;
}

