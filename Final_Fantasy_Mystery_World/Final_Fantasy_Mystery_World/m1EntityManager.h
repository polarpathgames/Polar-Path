#ifndef __J1ENTITYMANAGER_H__
#define __J1ENTITYMANAGER_H__

#include "m1Module.h"
#include "p2Animation.h"
#include "e1Entity.h"
#include <vector>

enum class Direction;

class m1EntityManager : public m1Module
{
public:

	m1EntityManager();
	~m1EntityManager();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	void SortEntities(std::vector<e1Entity *> &draw_entities);
	void SortAllEntities();

	void DrawEntities(std::vector<e1Entity *> &draw_entities, float dt);

	void UpdateEntities(float dt, std::vector<e1Entity *> &draw_entities);

	bool PostUpdate();

	bool Load(pugi::xml_node& node);

	bool Save(pugi::xml_node& node ) const;

	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);
	void OnCollisionEnter(Collider* c1, Collider* c2);
	void OnCollisionExit(Collider* c1, Collider* c2);

	e1Entity* CreateEntity(e1Entity::EntityType type, int PositionX, int PositionY, std::string name);
	void AssignEntityTurn(e1Entity* ent) { entity_turn = ent; }
	bool ThereAreEnemies();
	bool ThereIsEntity(e1Entity::EntityType type);
	bool IsInEntitiesVector(e1Entity * entity);
	bool ThereIsEntity(const char*);
	e1Entity* FindEntity(e1Entity::EntityType type);
	e1Entity* FindEntity(const char*);
	bool IsPlayerPoisonedOrBurned();
	void DeleteEntities();
	void DeleteEntitiesNoPlayer();
	void DeleteEntity(e1Entity* entity_to_delete);

	void SpawnRupees(const int & x, const int & y, const int& number, const int & range, const int & gold_per, const int & red_per, const int& blue_per, const int & green_per);

	// Finds a tile in all zone around origin tile
	bool FindFreeTileAround(const iPoint &tile, const uint & range, std::vector<iPoint> * list_to_fill);
	iPoint FindRandomFreeTileAround(const iPoint & tile, const uint & range);
	// Finds a tile in border range
	iPoint FindFirstFreeTileOnRange(const iPoint & tile, const uint & range);
	// Finds a random tile in border range
	iPoint FindRandomFreeTileOnRange(const iPoint & tile, const uint & range);
	bool IsWalkable(const iPoint & start_tile);

	bool CheckForBarrelsAndSnowMan(const iPoint &act_tile, const Direction &direction);

	const std::vector<e1Entity*> GetEntities();
	const std::vector<SDL_Texture*> GetTextures();

	static bool SortByYPos(const e1Entity * ent1, const e1Entity * ent2);

	uint entities_drawn = 0u;
	e1Entity* entity_turn = nullptr;
	bool turns_active = true;
private:

	

	std::vector<e1Entity*> entities;
	std::vector<e1Entity*> entities_to_create;
	std::vector<SDL_Texture*> texture;
};

#endif