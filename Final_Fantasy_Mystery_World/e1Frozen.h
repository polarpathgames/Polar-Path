#ifndef __E1FROZEN_H__
#define __E1FROZEN_H__

#include "e1Enemy.h"

class e1Frozen :public e1Enemy {

	enum class Phase {
		NORMAL, HARD
	};
public:
	e1Frozen(const int& x, const int& y);
	~e1Frozen();
public:

	void UpdateEnemy();

	void IdAnimToEnum();

	void PrepareDistanceAttack();
	bool IsSpecialAttack1Finished();
	void AfetSpecialAttack1();

private:
	Phase phase = Phase::NORMAL;
};

#endif // !__E1FROZEN_H__