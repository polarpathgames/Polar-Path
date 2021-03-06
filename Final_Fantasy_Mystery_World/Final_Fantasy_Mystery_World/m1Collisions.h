#ifndef __m1Collisions_H__
#define __m1Collisions_H__

#include "m1Module.h"
#include "SDL/include/SDL_rect.h"
#include <vector>
#include <list>

enum COLLIDER_TYPE
{
	COLLIDER_NONE,
	COLLIDER_PLAYER,
	COLLIDER_SHOP,
	COLLIDER_HOME,
	COLLIDER_NEXT_A,
	COLLIDER_NEXT_B,
	COLLIDER_LAST_A,
	COLLIDER_LAST_B,
	COLLIDER_MENU_QUEST,
	COLLIDER_QUEST_ICE,
	COLLIDER_CUTSCENE_BRIDGE,
	COLLIDER_BED,
	COLLIDER_QUEST_FIRE,

	COLLIDER_MAX
};

enum class ColliderInfo {
	ENTER,
	STAY,
	EXIT,
};

struct Collider
{
	SDL_Rect rect;

	bool to_delete = false;
	COLLIDER_TYPE type;
	ColliderInfo info = ColliderInfo::ENTER;
	m1Module* callback = nullptr;
	bool enable = true;
	std::list<Collider*> collisions;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, m1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	void SetShape(int w, int h)
	{
		rect.w = w;
		rect.h = h;
	}

	void SetType(COLLIDER_TYPE type)
	{
		this->type = type;
	}

	bool CheckCollision(const SDL_Rect& r) const;
	
};

class m1Collision :	public m1Module
{
public:
	m1Collision();
	~m1Collision();

	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, m1Module* callback = nullptr);
	bool DeleteCollider(Collider* col);

	std::vector<Collider*> GetColliders();
	/*std::vector<Collider*> Setcollider();*/
	void DebugDraw();

private:

	std::vector<Collider*> colliders;
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];

};

#endif