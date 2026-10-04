#include "common.h"

#include <rand.h>

#include "tm1/car.h"
#include "tm1/interactives.h"
#include "tm1/shell.h"
#include "tm1/ua.h"

#include "tm1/ai_car_update.h"

extern s32 IceCreamSpecialChance;
extern s32 IceCreamForeChance;
extern s32 IceCreamAftChance;
extern s32 TaxiSpecialChance;
extern s32 TaxiForeChance;
extern s32 TaxiAftChance;
extern s32 PoliceSpecialChance;
extern s32 PoliceForeChance;
extern s32 PoliceAftChance;
extern s32 VetteSpecialChance;
extern s32 VetteForeChance;
extern s32 VetteAftChance;
extern s32 MonsterSpecialChance;
extern s32 MonsterForeChance;
extern s32 MonsterAftChance;
extern s32 SemiSpecialChance;
extern s32 SemiForeChance;
extern s32 SemiAftChance;
extern s32 DuneBuggySpecialChance;
extern s32 DuneBuggyForeChance;
extern s32 DuneBuggyAftChance;
extern s32 LamborghiniSpecialChance;
extern s32 LamborghiniForeChance;
extern s32 LamborghiniAftChance;
extern s32 HumveeSpecialChance;
extern s32 HumveeForeChance;
extern s32 HumveeAftChance;
extern s32 HarleySpecialChance;
extern s32 HarleyForeChance;
extern s32 HarleyAftChance;
extern s32 MadMaxSpecialChance;
extern s32 MadMaxForeChance;
extern s32 MadMaxAftChance;
extern s32 ImpalaSpecialChance;
extern s32 ImpalaForeChance;
extern s32 ImpalaAftChance;
extern s32 BossSpecialChance;
extern s32 BossForeChance;
extern s32 BossAftChance;

extern s32 IceCreamProfileFront[4];
extern s32 IceCreamProfileRear[6];
extern s32 TaxiProfileFront[4];
extern s32 TaxiProfileRear[6];
extern s32 SemiProfileFront[4];
extern s32 SemiProfileRear[6];
extern s32 ImpalaProfileFront[4];
extern s32 ImpalaProfileRear[6];
extern s32 MonsterProfileFront[4];
extern s32 MonsterProfileRear[6];
extern s32 DuneBuggyProfileFront[4];
extern s32 DuneBuggyProfileRear[6];
extern s32 PoliceProfileFront[4];
extern s32 PoliceProfileRear[6];
extern s32 MadMaxProfileFront[4];
extern s32 MadMaxProfileRear[6];
extern s32 HarleyProfileFront[4];
extern s32 HarleyProfileRear[6];
extern s32 HumveeProfileFront[4];
extern s32 HumveeProfileRear[6];
extern s32 VetteProfileFront[4];
extern s32 VetteProfileRear[6];
extern s32 LamborghiniProfileFront[4];
extern s32 LamborghiniProfileRear[6];
extern s32 BossProfileFront[4];
extern s32 BossProfileRear[6];
extern s32 Level1ProfileFront[4];
extern s32 Level1ProfileRear[6];

#ifdef NON_MATCHING
void AICarUpdateAttackProfile(CarAlt* car)
{
    if (car->unk40 != 0) {
        if ((s16)car->weap.fireDelay < 2) {
            AIPickAttackWeapon(car);
            if (car->weap.cur == 12) {
                car->weap.fireDelay = rand() % 20 + 10;
            }
        }
        UpdateWeapons((Car*)car, 0);
    } else {
        car->weap.cur = 12;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car_update", AICarUpdateAttackProfile);
#endif

#ifdef NON_MATCHING
void AIPickAttackWeapon(CarAlt* car)
{
    s16 lvl;

    lvl = car->weap.ammo[11];
    car->weap.cur = 12;
    if (lvl >= 6 && AIInSpecialParameters(car)) {
        if (rand() % 24 < (s16)car->weap.ammo[11]) {
            car->weap.cur = 11;
            return;
        }
    }
    switch (car->unk144) {
    case 0:
        if (rand() % 100 + 1 < (s16)car->weap.foreChance) {
            AICarChooseForeWeapon(car);
        } else {
            car->weap.cur = 12;
        }
        break;
    case 1:
        if (rand() % 100 + 1 < (s16)car->weap.aftChance) {
            AICarChooseAftWeapon(car);
        } else {
            car->weap.cur = 12;
        }
        break;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car_update", AIPickAttackWeapon);
#endif

#ifdef NON_MATCHING
u8 AIInSpecialParameters(CarAlt* car)
{
    switch (car->uaIndex) {
    case 10:
    case 20:
    case 30:
    case 60:
    case 80:
    case 90:
    case 110:
        return car->unk144 == 0;
    case 130:
        if (car->unk144 == 0) {
            return 1;
        }
        if (car->unk44 != 0) {
            return 1;
        }
        return car->unk144 == 1;
    default:
        if (car->unk144 != 0) {
            return 0;
        }
        return car->unk44 != 0;
    case 50:
        return car->unk44 != 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car_update", AIInSpecialParameters);
#endif

#ifdef NON_MATCHING
void AICarSetDefaultWeapons(CarAlt* car)
{
    CarWeap* w;
    s32* rear;
    s32 special;
    s32 fore;
    s32 aft;
    s32 diff;
    s32 kind;

    shellGetCurrentLevel();
    kind = car->uaIndex;
    w = &car->weap;
    switch (kind) {
    case 130:
        aft = BossAftChance;
        fore = BossForeChance;
        special = BossSpecialChance;
        rear = BossProfileFront;
        w->foreProfile = rear;
        rear = BossProfileRear;
        goto join;
    case 10:
        aft = IceCreamAftChance;
        fore = IceCreamForeChance;
        special = IceCreamSpecialChance;
        rear = IceCreamProfileFront;
        w->foreProfile = rear;
        rear = IceCreamProfileRear;
        goto join;
    case 20:
        aft = TaxiAftChance;
        fore = TaxiForeChance;
        special = TaxiSpecialChance;
        rear = TaxiProfileFront;
        w->foreProfile = rear;
        rear = TaxiProfileRear;
        goto join;
    case 30:
        aft = SemiAftChance;
        fore = SemiForeChance;
        special = SemiSpecialChance;
        rear = SemiProfileFront;
        w->foreProfile = rear;
        rear = SemiProfileRear;
        goto join;
    case 40:
        aft = MonsterAftChance;
        fore = MonsterForeChance;
        special = MonsterSpecialChance;
        rear = MonsterProfileFront;
        w->foreProfile = rear;
        rear = MonsterProfileRear;
        goto join;
    case 50:
        aft = PoliceAftChance;
        fore = PoliceForeChance;
        special = PoliceSpecialChance;
        rear = PoliceProfileFront;
        w->foreProfile = rear;
        rear = PoliceProfileRear;
        goto join;
    case 60:
        aft = LamborghiniAftChance;
        fore = LamborghiniForeChance;
        special = LamborghiniSpecialChance;
        rear = LamborghiniProfileFront;
        w->foreProfile = rear;
        rear = LamborghiniProfileRear;
        goto join;
    case 70:
        aft = HumveeAftChance;
        fore = HumveeForeChance;
        special = HumveeSpecialChance;
        rear = HumveeProfileFront;
        w->foreProfile = rear;
        rear = HumveeProfileRear;
        goto join;
    case 80:
        aft = HarleyAftChance;
        fore = HarleyForeChance;
        special = HarleySpecialChance;
        rear = HarleyProfileFront;
        w->foreProfile = rear;
        rear = HarleyProfileRear;
        goto join;
    case 90:
        aft = DuneBuggyAftChance;
        fore = DuneBuggyForeChance;
        special = DuneBuggySpecialChance;
        rear = DuneBuggyProfileFront;
        w->foreProfile = rear;
        rear = DuneBuggyProfileRear;
        goto join;
    case 100:
        aft = ImpalaAftChance;
        fore = ImpalaForeChance;
        special = ImpalaSpecialChance;
        rear = ImpalaProfileFront;
        w->foreProfile = rear;
        rear = ImpalaProfileRear;
        goto join;
    case 110:
        aft = VetteAftChance;
        fore = VetteForeChance;
        special = VetteSpecialChance;
        rear = VetteProfileFront;
        w->foreProfile = rear;
        rear = VetteProfileRear;
        goto join;
    case 120:
        aft = MadMaxAftChance;
        fore = MadMaxForeChance;
        special = MadMaxSpecialChance;
        rear = MadMaxProfileFront;
        w->foreProfile = rear;
        rear = MadMaxProfileRear;
    join:
        w->aftProfile = rear;
        w->specialChance = special;
        w->foreChance = fore;
        w->aftChance = aft;
        break;
    default:
        w->foreProfile = Level1ProfileFront;
        w->aftProfile = Level1ProfileRear;
        w->specialChance = 0;
        w->foreChance = 10;
        w->aftChance = 10;
        break;
    }
    diff = uaGetDifficulty();
    if (shellGetCurrentLevel == 0 && diff < 2) {
        w->foreChance = (s16)w->foreChance / 2;
        w->aftChance = (s16)w->aftChance / 2;
        if ((s16)w->foreChance < 10) {
            w->foreChance = 10;
        }
        if ((s16)w->aftChance < 10) {
            w->aftChance = 10;
        }
    }
    if (diff == 0) {
        w->specialChance = w->specialChance - 15;
        if ((s16)w->specialChance < 10) {
            w->specialChance = 10;
        }
        w->foreChance = w->foreChance - 10;
        if ((s16)w->foreChance < 10) {
            w->foreChance = 10;
        }
        w->aftChance = w->aftChance - 10;
        if ((s16)w->aftChance < 10) {
            w->aftChance = 10;
        }
    } else if (diff == 2) {
        w->specialChance = w->specialChance + 20;
        w->foreChance = w->foreChance + 10;
        if ((s16)w->foreChance >= 96) {
            w->foreChance = 95;
        }
        w->aftChance = w->aftChance + 10;
        if ((s16)w->aftChance >= 96) {
            w->aftChance = 95;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car_update", AICarSetDefaultWeapons);
#endif

#ifdef NON_MATCHING
void AICarChooseForeWeapon(CarAlt* car)
{
    s32 sum = 0;
    s16 roll = rand() % 100;
    s32* p = car->weap.foreProfile;
    s32 i = 0;

    do {
        sum += *p;
        if (roll < sum) {
            car->weap.cur = i;
            return;
        }
        i++;
        p++;
    } while (i < 4);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car_update", AICarChooseForeWeapon);
#endif

#ifdef NON_MATCHING
void AICarChooseAftWeapon(CarAlt* car)
{
    s32 sum = 0;
    s16 roll = rand() % 100;
    s32* probs = car->weap.aftProfile;
    s32 i = 5;

    do {
        sum += probs[i - 5];
        if (roll < sum) {
            car->weap.cur = i;
            return;
        }
        i++;
    } while (i < 11);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car_update", AICarChooseAftWeapon);
#endif
