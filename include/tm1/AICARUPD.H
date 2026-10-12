#ifndef __TM1_AICARUPD_H__
#define __TM1_AICARUPD_H__

#include "common.h"

#include "tm1/CAR.H"

void AICarUpdateAttackProfile(AICar* car);
void AIPickAttackWeapon(AICar* car);
u8 AIInSpecialParameters(AICar* car);
void AICarSetDefaultWeapons(AICar* car);
void AICarChooseForeWeapon(AICar* car);
void AICarChooseAftWeapon(AICar* car);

#endif /* __TM1_AICARUPD_H__ */
