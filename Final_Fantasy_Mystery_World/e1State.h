#ifndef __E1STATE_H__
#define __E1STATE_H__

#include "e1DynamicEntity.h"
#include <list>

enum class EventStates {
	SNOWSTORM,
	POISON,

	NONE
};

class e1State :public e1Entity {
public:
	e1State(int x, int y, const char* name);
	~e1State();

	bool PreUpdate();
	bool Update(float dt);

private:
	EventStates state = EventStates::NONE;
	e1Entity* target = nullptr;

	Animation* animation = nullptr;

	bool doing_animation = false;

	uint turn_count = 0u;
	uint turn_effect = 0u;
};

#endif // !__E1STATE_H__
