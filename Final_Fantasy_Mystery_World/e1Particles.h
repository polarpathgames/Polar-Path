#ifndef _E1PARTICLES_H_
#define _E1PARTICLES_H_

#define MAX_ARROW 3

#include "e1DynamicEntity.h"

class e1Particles : public e1DynamicEntity {

public:

	enum class ParticleType {
		ARROW,

		NONE
	};

	e1Particles(const int &x, const int &y);
	virtual ~e1Particles();

	bool PreUpdate();
	bool Update(float dt);

	bool CleanUp();

	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) const { return true; };

	void SetParticle(const ParticleType & particle_type, const Direction &dir);

private:

	void MoveArrow(float dt);
	bool EnemyNextTile(const Direction &dir);

public:

	ParticleType particle_type = ParticleType::NONE;

	iPoint arrow_tile = { 0,0 };
	iPoint max_arrow_distance = { 0,0 };
};



#endif // !_E1PARTICLES_H_

