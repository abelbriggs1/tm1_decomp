#include "common.h"

#include <rand.h>

#include "tm1/car.h"
#include "tm1/interactives.h"
#include "tm1/shell.h"
#include "tm1/ua.h"

#include "tm1/ai_car_update.h"

// clang-format off
static CarFrontProfile IceCreamProfileFront = {
    0x21, 0x00, 0x41, 0x02
};
static CarRearProfile IceCreamProfileRear = {
    0x00, 0x5A, 0x0A, 0x00, 0x00, 0x00
};
static CarFrontProfile TaxiProfileFront = {
    0x46, 0x00, 0x1E, 0x00
};
static CarRearProfile TaxiProfileRear = {
    0x0A, 0x00, 0x00, 0x5A, 0x00, 0x00
};
static CarFrontProfile SemiProfileFront = {
    0x14, 0x50, 0x00, 0x00
};
static CarRearProfile SemiProfileRear = {
    0x14, 0x46, 0x0A, 0x00, 0x00, 0x00
};
static CarFrontProfile ImpalaProfileFront = {
    0x14, 0x14, 0x00, 0x00
};
static CarRearProfile ImpalaProfileRear = {
    0x23, 0x05, 0x3C, 0x00, 0x00, 0x00
};
static CarFrontProfile MonsterProfileFront = {
    0x0A, 0x5A, 0x00, 0x00
};
static CarRearProfile MonsterProfileRear = {
    0x00, 0x50, 0x00, 0x14, 0x00, 0x00
};
static CarFrontProfile DuneBuggyProfileFront = {
    0x5A, 0x05, 0x05, 0x00
};
static CarRearProfile DuneBuggyProfileRear = {
    0x32, 0x14, 0x00, 0x00, 0x00, 0x1E
};
static CarFrontProfile PoliceProfileFront = {
    0x0A, 0x5A, 0x00, 0x00
};
static CarRearProfile PoliceProfileRear = {
    0x00, 0x14, 0x00, 0x00, 0x50, 0x00
};
static CarFrontProfile MadMaxProfileFront = {
    0x64, 0x00, 0x00, 0x00
};
static CarRearProfile MadMaxProfileRear = {
    0x14, 0x00, 0x14, 0x00, 0x1E, 0x1E
};
static CarFrontProfile HarleyProfileFront = {
    0x50, 0x14, 0x00, 0x00
};
static CarRearProfile HarleyProfileRear = {
    0x00, 0x00, 0x0A, 0x00, 0x00, 0x5A
};
static CarFrontProfile HumveeProfileFront = {
    0x5A, 0x00, 0x05, 0x05
};
static CarRearProfile HumveeProfileRear = {
    0x00, 0x00, 0x64, 0x00, 0x00, 0x00
};
static CarFrontProfile VetteProfileFront = {
    0x64, 0x00, 0x00, 0x00
};
static CarRearProfile VetteProfileRear = {
    0x00, 0x00, 0x00, 0x64, 0x00, 0x00
};
static CarFrontProfile LamborghiniProfileFront = {
    0x5F, 0x00, 0x00, 0x05
};
static CarRearProfile LamborghiniProfileRear = {
    0x5A, 0x00, 0x0A, 0x00, 0x00, 0x00
};
static CarFrontProfile BossProfileFront = {
    0x0F, 0x0F, 0x23, 0x23
};
static CarRearProfile BossProfileRear = {
    0x14, 0x0A, 0x19, 0x14, 0x0A, 0x0F
};
static CarFrontProfile Level1ProfileFront = {
    0x3C, 0x14, 0x00, 0x00
};
static CarRearProfile Level1ProfileRear = {
    0x32, 0x00, 0x00, 0x1E, 0x00, 0x00
};

static s32 IceCreamSpecialChance = 35;
static s32 IceCreamForeChance = 60;
static s32 IceCreamAftChance = 90;

static s32 TaxiSpecialChance = 90;
static s32 TaxiForeChance = 10;
static s32 TaxiAftChance = 75;

static s32 PoliceSpecialChance = 35;
static s32 PoliceForeChance = 25;
static s32 PoliceAftChance = 50;

static s32 VetteSpecialChance = 90;
static s32 VetteForeChance = 30;
static s32 VetteAftChance = 10;

static s32 MonsterSpecialChance = 50;
static s32 MonsterForeChance = 20;
static s32 MonsterAftChance = 50;

static s32 SemiSpecialChance = 35;
static s32 SemiForeChance = 70;
static s32 SemiAftChance = 10;

static s32 DuneBuggySpecialChance = 50;
static s32 DuneBuggyForeChance = 50;
static s32 DuneBuggyAftChance = 90;

static s32 LamborghiniSpecialChance = 95;
static s32 LamborghiniForeChance = 30;
static s32 LamborghiniAftChance = 80;

static s32 HumveeSpecialChance = 60;
static s32 HumveeForeChance = 60;
static s32 HumveeAftChance = 60;

static s32 HarleySpecialChance = 35;
static s32 HarleyForeChance = 80;
static s32 HarleyAftChance = 60;

static s32 MadMaxSpecialChance = 80;
static s32 MadMaxForeChance = 50;
static s32 MadMaxAftChance = 20;

static s32 ImpalaSpecialChance = 75;
static s32 ImpalaForeChance = 80;
static s32 ImpalaAftChance = 80;

static s32 BossSpecialChance = 100;
static s32 BossForeChance = 75;
static s32 BossAftChance = 75;

#ifdef NON_MATCHING
void AICarUpdateAttackProfile(AICar* car)
{
    if (car->unk40 != 0) {
        if ((s16)car->weap.fireDelay < 2) {
            AIPickAttackWeapon(car);
            if (car->weap.cur == 12) {
                car->weap.fireDelay = rand() % 20 + 10;
            }
        }
        UpdateWeapons(car, 0);
    } else {
        car->weap.cur = 12;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car_update", AICarUpdateAttackProfile);
#endif

#ifdef NON_MATCHING
void AIPickAttackWeapon(AICar* car)
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
u8 AIInSpecialParameters(AICar* car)
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
void AICarSetDefaultWeapons(AICar* car)
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
void AICarChooseForeWeapon(AICar* car)
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
void AICarChooseAftWeapon(AICar* car)
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
