#ifndef __STATICENTITY_H__
#define __STATICENTITY_H__

#include "p2Point.h"
#include <string>
#include "p2Animation.h"
#include "PugiXml\src\pugixml.hpp"
#include "p2PerfTimer.h"
#include "e1Entity.h"

class u1Image;

class e1StaticEntity : public e1Entity
{
public:

	enum class Type {
		TREE1,
		TREE2,
		TREE3,
		FLOWER,
		FOUNTAIN,
		BARREL,
		PLANT1,
		PLANT2,
		ROCK1,
		ROCK2,
		BREAKABLE_ROCK,
		STREETLIGHT,
		HOUSE1,
		HOUSE2,
		SHOP,
		HOME,
		FLOOR,
		CORNER_FLOOR,
		CORNER,
		RED_PLANT,
		HALF_TREE,
		QUEST_TREE1,
		QUEST_TREE2,
		QUEST_TREE3,
		QUEST_TREE4,
		FEATHER,
		CANDLE,
		PIECE_SHOP,
		SHOP_MAN,
		QUEST_FOUNTAIN,
		NPC1,
		NPC2,
		FURNITURE,
		DOORHOME,
		DOORSHOP,
		HELP1,
		HELP2,

		UNKNOWN
	};

	enum class InteractingStates {
		WAITING_INTERACTION,
		INTERACTING,
		NONE
	};
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt);

	virtual bool CleanUp() { return true; };

	virtual bool Load(pugi::xml_node&) { return true; };
	virtual bool Save(pugi::xml_node&) const { return true; };

	void ChangeAnimation(const iPoint &player_pos);

	InteractingStates GetState();
public:

	e1StaticEntity(int x, int y, const char* name);
	~e1StaticEntity();
	
private:

	void Draw(SDL_Texture* tex, float dt);
	void SetRect(int x, int y, int w, int h);

public:
	SDL_Rect frame;
	Animation* idle = nullptr;
	bool has_animation = false;
	Type static_type = Type::UNKNOWN;
private:
	
	
	bool has_dialog = false;
	
	InteractingStates interacting_state = InteractingStates::NONE;
	int max_distance_to_interact = 0; // distance in tiles

	u1Image* button_interact = nullptr;

};

#endif