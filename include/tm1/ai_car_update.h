#ifndef __TM1_AI_CAR_UPDATE_H__
#define __TM1_AI_CAR_UPDATE_H__

#include "common.h"

#include "tm1/car.h"

void AICarUpdateAttackProfile(AICar* car);
void AIPickAttackWeapon(AICar* car);
u8 AIInSpecialParameters(AICar* car);
void AICarSetDefaultWeapons(AICar* car);
void AICarChooseForeWeapon(AICar* car);
void AICarChooseAftWeapon(AICar* car);

#endif /* __TM1_AI_CAR_UPDATE_H__ */
