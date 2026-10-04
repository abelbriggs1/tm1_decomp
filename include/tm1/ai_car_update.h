#ifndef __TM1_AI_CAR_UPDATE_H__
#define __TM1_AI_CAR_UPDATE_H__

#include "common.h"

#include "tm1/car.h"

void AICarUpdateAttackProfile(CarAlt* car);
void AIPickAttackWeapon(CarAlt* car);
u8 AIInSpecialParameters(CarAlt* car);
void AICarSetDefaultWeapons(CarAlt* car);
void AICarChooseForeWeapon(CarAlt* car);
void AICarChooseAftWeapon(CarAlt* car);

#endif /* __TM1_AI_CAR_UPDATE_H__ */
