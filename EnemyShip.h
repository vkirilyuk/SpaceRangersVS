#ifndef enemyshipH
#define enemyshipH

#include "types.h"
class EnemyShip: public MovableAnimatedObject {
public:
    double HP;
    int weaponPower;
    int experience;
    RECT ShipRect;
    RECT ShipRectDef;
	double scaling;
};

#endif