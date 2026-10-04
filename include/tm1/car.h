#ifndef __TM1_CAR_H__
#define __TM1_CAR_H__

#include "common.h"
#include "tm1/cs.h"
#include "tm1/hd.h"
#include "tm1/math.h"
#include <libgte.h>

// Helper macro to access a generic car pointer as a `PlayerCar`.
#define PLAYER_CAR(c) ((PlayerCar*)(c))

// Helper macro to access a generic car pointer as an `AICar`.
#define AI_CAR(c) ((AICar*)(c))

typedef struct CarMotion {
    /*0x00*/ u16 unk00;
    /*0x02*/ u16 unk02;
    /*0x04*/ s16 unk04;
    /*0x06*/ u8 pad06[0x2];
    /*0x08*/ VECTOR3 pos;
    /*0x14*/ VECTOR3 vel;
    /*0x20*/ VECTOR3 rot;
    /*0x2C*/ VECTOR3 rotDelta;
    /*0x38*/ VECTOR3 rot2;
    /*0x44*/ VECTOR3 rot2Delta;
    /*0x50*/ MATRIX mat2;
    /*0x70*/ MATRIX mat;
    /*0x90*/ VECTOR3 lastPos;
    /*0x9C*/ VECTOR3 lastRot;
    /*0xA8*/ s32 unkA8;
    /*0xAC*/ s32 unkAC;
    /*0xB0*/ s32 unkB0;
    /*0xB4*/ MATRIX lastMat;
    /*0xD4*/ s32 unkD4;
} CarMotion; /* 0xD8 */

typedef struct CarCollision {
    /*0x00*/ u8 unk0;
    /*0x01*/ u8 unk1;
    /*0x02*/ u8 unk2;
    /*0x03*/ u8 unk3;
    /*0x04*/ u16 unk4;
    /*0x06*/ u16 count;
    /*0x08*/ s16 unk8;
    /*0x0A*/ s16 unkA;
    /*0x0C*/ s16 unkC;
    /*0x0E*/ s16 unkE;
    /*0x10*/ u16 unk10;
    /*0x12*/ s16 unk12;
    /*0x14*/ u16 unk14;
    /*0x16*/ u16 unk16;
} CarCollision; /* 0x18 */

typedef struct CarTire {
    /*0x000*/ u8 unk0;
    /*0x001*/ u8 unk1;
    /*0x002*/ u8 unk2;
    /*0x003*/ u8 unk3;
    /*0x004*/ u8 catapulted;
    /*0x005*/ u8 bombDamaged;
    /*0x006*/ u8 unk6;
    /*0x007*/ u8 pad07;
    /*0x008*/ s16 unk08;
    /*0x00A*/ u16 unk0A;
    /*0x00C*/ s16 unk0C;
    /*0x00E*/ s16 unk0E;
    /*0x010*/ s16 unk10;
    /*0x012*/ u16 unk12;
    /*0x014*/ u16 unk14;
    /*0x016*/ s16 unk16;
    /*0x018*/ s16 unk18;
    /*0x01A*/ s16 unk1A;
    /*0x01C*/ u16 unk1C;
    /*0x01E*/ u16 unk1E;
    /*0x020*/ s16 unk20;
    /*0x022*/ u16 unk22;
    /*0x024*/ u16 unk24;
    /*0x026*/ s16 unk26;
    /*0x028*/ s32 unk28;
    /*0x02C*/ VECTOR3 unk2C;
    /*0x038*/ s32 unk38;
    /*0x03C*/ s32 unk3C;
    /*0x040*/ s32 unk40;
} CarTire; /* 0x44 */

typedef struct CarStats {
    /*0x000*/ u8 unk00;
    /*0x001*/ u8 unk01;
    /*0x002*/ u8 monster;
    /*0x003*/ u8 unk03;
    /*0x004*/ u8 unk04;
    /*0x005*/ s8 unk05;
    /*0x006*/ s8 damageLevel;
    /*0x007*/ s8 damageThreshold;
    /*0x008*/ u8 cheatA;
    /*0x009*/ u8 cheatAArmed;
    /*0x00A*/ u8 cheatB;
    /*0x00B*/ u8 cheatBArmed;
    /*0x00C*/ u16 cheatTimer;
    /*0x00E*/ u16 colorId;
    /*0x010*/ u16 updateRate;
    /*0x012*/ u16 blasts;
    /*0x014*/ u16 unk14;
    /*0x016*/ u16 unk16;
    /*0x018*/ s16 unk18;
    /*0x01A*/ u16 spikeTimer;
    /*0x01C*/ s16 unk1C;
    /*0x01E*/ u16 unk1E;
    /*0x020*/ u16 deathTimer;
    /*0x022*/ u16 unk22;
    /*0x024*/ s16 unk24;
    /*0x026*/ u8 pad01a[0x4];
    /*0x02A*/ s16 unk2A;
    /*0x02C*/ s16 unk2C;
    /*0x02E*/ u8 pad01b[0x2];
    /*0x030*/ s32 unk30;
    /*0x034*/ s32 unk34;
    /*0x038*/ s32 unk38;
    /*0x03C*/ s32 unk3C;
    /*0x040*/ s32 unk40;
    /*0x044*/ s32 unk44;
    /*0x048*/ u16 lostTimer;
    /*0x04A*/ u16 unk4A;
    /*0x04C*/ s32 unk4C;
    /*0x050*/ s32 unk50;
    /*0x054*/ s32 healthRate;
    /*0x058*/ s32 healthCap;
    /*0x05C*/ s32 unk5C;
    /*0x060*/ u8 pad60[0x4];
    /*0x064*/ s32 triggerPt;
    /*0x068*/ s32 unk68;
    /*0x06C*/ s32 unk6C;
    /*0x070*/ s32 unk70;
    /*0x074*/ s32 unk74;
    /*0x078*/ s32 unk78;
    /*0x07C*/ s32 unk7C;
    /*0x080*/ s32 unk80;
    /*0x084*/ s32 unk84;
    /*0x088*/ s32 unk88;
    /*0x08C*/ s32 unk8C;
    /*0x090*/ s32 unk90;
    /*0x094*/ s32 unk94;
    /*0x098*/ s32 unk98;
    /*0x09C*/ s32 unk9C;
    /*0x0A0*/ s32 unkA0;
    /*0x0A4*/ s32 unkA4;
    /*0x0A8*/ s32 unkA8;
    /*0x0AC*/ s32 unkAC;
    /*0x0B0*/ s32 unkB0;
    /*0x0B4*/ s32 unkB4;
    /*0x0B8*/ s32 unkB8;
    /*0x0BC*/ s32 unkBC;
    /*0x0C0*/ s32 unkC0;
    /*0x0C4*/ s32 unkC4;
    /*0x0C8*/ s32 unkC8;
    /*0x0CC*/ s32 unkCC;
    /*0x0D0*/ s32 unkD0;
    /*0x0D4*/ s32 unkD4;
    /*0x0D8*/ s32 unkD8;
    /*0x0DC*/ s32 unkDC;
    /*0x0E0*/ s32 unkE0;
    /*0x0E4*/ s32 unkE4;
    /*0x0E8*/ s32 unkE8;
    /*0x0EC*/ s32 unkEC;
    /*0x0F0*/ s32 unkF0;
    /*0x0F4*/ s32 unkF4;
    /*0x0F8*/ s32 unkF8;
    /*0x0FC*/ s32 unkFC;
    /*0x100*/ s32 unk100;
    /*0x104*/ s32 unk104;
    /*0x108*/ s32 unk108;
    /*0x10C*/ s32 unk10C;
    /*0x110*/ u16 unk110;
    /*0x112*/ u16 unk112;
    /*0x114*/ u16 unk114;
    /*0x116*/ u8 pad116[0x2];
    /*0x118*/ u16 dropPower;
    /*0x11A*/ s16 unk11A;
    /*0x11C*/ s16 unk11C;
    /*0x11E*/ u8 pad11E[0x2];
} CarStats; /* 0x120 */

typedef struct CarBounce {
    /*0x00*/ s32 unk00;
    /*0x04*/ s32 unk04;
    /*0x08*/ s32 unk08;
    /*0x0C*/ s16 unk0C;
    /*0x0E*/ s16 unk0E;
    /*0x10*/ s16 unk10;
    /*0x12*/ s16 unk12;
    /*0x14*/ s16 unk14;
    /*0x16*/ s16 unk16;
    /*0x18*/ s32 unk18;
    /*0x1C*/ s32 unk1C;
    /*0x20*/ s32 unk20;
    /*0x24*/ s32 unk24;
    /*0x28*/ s32 unk28;
    /*0x2C*/ s32 unk2C;
    /*0x30*/ s32 unk30;
    /*0x34*/ s32 unk34;
    /*0x38*/ s32 unk38;
} CarBounce; /* 0x3C */

typedef struct CarWeap {
    /*0x00*/ u16 gunDelay;
    /*0x02*/ u16 fireDelay;
    /*0x04*/ u8 unk04;
    /*0x05*/ u8 pad05[0x3];
    /*0x08*/ u32 cur;
    /*0x0C*/ u16 ammo[14];
    /*0x28*/ u16 maxAmmo;
    /*0x2A*/ u8 pad2A[0x2];
    /*0x2C*/ s32* foreProfile;
    /*0x30*/ s32* aftProfile;
    /*0x34*/ u16 totalAmmo;
    /*0x36*/ u16 gunHeat;
    /*0x38*/ u8 gunOverheat;
    /*0x39*/ u8 pad39;
    /*0x3A*/ s16 timer;
    /*0x3C*/ u16 specialChance;
    /*0x3E*/ u16 foreChance;
    /*0x40*/ u16 aftChance;
    /*0x42*/ u8 unk42;
    /*0x43*/ u8 pad43;
    /*0x44*/ u16 unk44;
    /*0x46*/ u16 unk46;
    /*0x48*/ u8 unk48;
    /*0x49*/ u8 reset;
    /*0x4A*/ u8 pad4A[0x2];
} CarWeap; /* 0x4C */

typedef struct CarRoute {
    /*0x00*/ s32 prio;
    /*0x04*/ s32 unk04;
    /*0x08*/ s32 unk08;
    /*0x0C*/ s32 val;
    /*0x10*/ s32 unk10;
    /*0x14*/ s32 unk14;
} CarRoute; /* 0x18 */

typedef struct CarAttack {
    /*0x00*/ s16 unk00;
    /*0x02*/ s16 unk02;
    /*0x04*/ s16 unk04;
    /*0x06*/ s16 unk06;
} CarAttack; /* 0x8 */

// Player car struct.
typedef struct PlayerCar {
    /*0x000*/ u8 pad00;
    /*0x001*/ s8 standId;
    /*0x002*/ u16 playerIdx;
    /*0x004*/ SVECTOR dRot;
    /*0x00C*/ VECTOR3 dTrans;
    /*0x018*/ SVECTOR vrRot[6];
    /*0x048*/ VECTOR3 vrPos[6];
    /*0x090*/ s32 speedDelta;
    /*0x094*/ s32 uaIndex;
    /*0x098*/ u8 unk98;
    /*0x099*/ u8 unk99;
    /*0x09A*/ u8 unk9A;
    /*0x09B*/ u8 unk9B;
    /*0x09C*/ u8 pad03b[0x4];
    /*0x0A0*/ u8 flags[32];
    /*0x0C0*/ s16 unkC0;
    /*0x0C2*/ u8 padC2[0x2];
    /*0x0C4*/ s32 bearing;
    /*0x0C8*/ u8 skid[44];
    /*0x0F4*/ s32 unkF4;
    /*0x0F8*/ s32 unkF8;
    /*0x0FC*/ CarBounce bounce;
    /*0x138*/ CarStats stats;
    /*0x258*/ CarCollision collision;
    /*0x270*/ CarTire tires[4];
    /*0x380*/ CarMotion motion;
    /*0x458*/ CarWeap weap;
} PlayerCar; /* 0x4A4 */

// AI car struct.
typedef struct AICar {
    /*0x000*/ u8 driving;
    /*0x001*/ s8 unk01;
    /*0x002*/ u8 unk02;
    /*0x003*/ u8 pad03;
    /*0x004*/ s8 unk04;
    /*0x005*/ u8 pad05;
    /*0x006*/ u16 playerIdx;
    /*0x008*/ s16 routeVal;
    /*0x00A*/ s16 unk0A;
    /*0x00C*/ s16 unk0C;
    /*0x00E*/ u16 unk0E;
    /*0x010*/ s16 unk10;
    /*0x012*/ u8 pad12[0x6];
    /*0x018*/ s32 unk18;
    /*0x01C*/ s32 unk1C;
    /*0x020*/ s32 unk20;
    /*0x024*/ s32 unk24;
    /*0x028*/ s32 unk28;
    /*0x02C*/ s32 unk2C;
    /*0x030*/ s32 unk30;
    /*0x034*/ s32 unk34;
    /*0x038*/ s32 unk38;
    /*0x03C*/ s32 uaIndex;
    /*0x040*/ u8 unk40;
    /*0x041*/ u8 unk41;
    /*0x042*/ u8 unk42;
    /*0x043*/ u8 unk43;
    /*0x044*/ u8 unk44;
    /*0x045*/ u8 unk45;
    /*0x046*/ u8 unk46;
    /*0x047*/ u8 heliFlag;
    /*0x048*/ u8 unk48;
    /*0x049*/ u8 unk49;
    /*0x04A*/ u8 chosen;
    /*0x04B*/ u8 unk4B;
    /*0x04C*/ u8 unk4C;
    /*0x04D*/ u8 unk4D;
    /*0x04E*/ u8 skid[24];
    /*0x066*/ u8 flags[30];
    /*0x084*/ CarAttack attack[4];
    /*0x0A4*/ CarRoute route[4];
    /*0x104*/ s8 unk104;
    /*0x105*/ s8 unk105;
    /*0x106*/ s8 unk106;
    /*0x107*/ u8 unk107;
    /*0x108*/ CarBounce bounce;
    /*0x144*/ s32 unk144;
    /*0x148*/ CarCollision collision;
    /*0x160*/ u8 unk160;
    /*0x161*/ u8 unk161;
    /*0x162*/ u8 unk162;
    /*0x163*/ u8 unk163;
    /*0x164*/ u8 unk164;
    /*0x165*/ u8 unk165;
    /*0x166*/ u8 unk166;
    /*0x167*/ u8 unk167;
    /*0x168*/ u8 unk168;
    /*0x169*/ u8 unk169;
    /*0x16A*/ u8 unk16A;
    /*0x16B*/ u8 unk16B;
    /*0x16C*/ u8 unk16C;
    /*0x16D*/ u8 unk16D;
    /*0x16E*/ u8 unk16E;
    /*0x16F*/ u8 unk16F;
    /*0x170*/ s16 unk170;
    /*0x172*/ s16 unk172;
    /*0x174*/ s16 unk174;
    /*0x176*/ s16 unk176;
    /*0x178*/ s16 unk178;
    /*0x17A*/ s16 unk17A;
    /*0x17C*/ s16 unk17C;
    /*0x17E*/ s16 unk17E;
    /*0x180*/ s16 unk180;
    /*0x182*/ s16 unk182;
    /*0x184*/ s16 unk184;
    /*0x186*/ s16 unk186;
    /*0x188*/ s16 unk188;
    /*0x18A*/ s16 unk18A;
    /*0x18C*/ s16 unk18C;
    /*0x18E*/ s16 unk18E;
    /*0x190*/ s16 unk190;
    /*0x192*/ s16 unk192;
    /*0x194*/ s16 unk194;
    /*0x196*/ u8 pad196[0x2];
    /*0x198*/ s32 unk198;
    /*0x19C*/ s32 unk19C;
    /*0x1A0*/ s32 unk1A0;
    /*0x1A4*/ s32 unk1A4;
    /*0x1A8*/ s32 unk1A8;
    /*0x1AC*/ s32 unk1AC;
    /*0x1B0*/ s32 unk1B0;
    /*0x1B4*/ s32 unk1B4;
    /*0x1B8*/ s32 unk1B8;
    /*0x1BC*/ s32 unk1BC;
    /*0x1C0*/ s32 unk1C0;
    /*0x1C4*/ s32 unk1C4;
    /*0x1C8*/ s32 unk1C8;
    /*0x1CC*/ s32 unk1CC;
    /*0x1D0*/ s32 unk1D0;
    /*0x1D4*/ s32 unk1D4;
    /*0x1D8*/ s32 unk1D8;
    /*0x1DC*/ s32 unk1DC;
    /*0x1E0*/ s32 unk1E0;
    /*0x1E4*/ s32 unk1E4;
    /*0x1E8*/ s32 unk1E8;
    /*0x1EC*/ s32 unk1EC;
    /*0x1F0*/ u8 pad1F0[0x8];
    /*0x1F8*/ s32 unk1F8;
    /*0x1FC*/ s32 unk1FC;
    /*0x200*/ s32 unk200;
    /*0x204*/ s32 unk204;
    /*0x208*/ s32 bearing;
    /*0x20C*/ CarStats stats;
    /*0x32C*/ CarTire tires[4];
    /*0x43C*/ CarMotion motion;
    /*0x514*/ CarWeap weap;
} AICar; /* 0x560 */

typedef void Car;

void CarUpdate(PlayerCar* car);
void CarUpdateDeltas(Car* car, u8 isPlayer);
void CarUpdateControlPad(PlayerCar* car);
void CarRotUpdate(Car* car, u8 isPlayer);
void CarTransUpdate(Car* car, u8 isPlayer);
void CarCheckDynamics(PlayerCar* car);
void CarInitMotion(Car* car, u8 isPlayer);
void UpdateNonDriftingCar(PlayerCar* car);
void SetNoCarDrift(CarMotion* m, u8* f);
s32 CalcMaxRotBeforeDrift(Car* car, u8 isPlayer);
void SlowDownNonDriftingCar(Car* car, u8 isPlayer);
void UpdateDriftingCar(PlayerCar* car);
void SetFullCarDrift(CarMotion* m);
void BringBackDriftingCar(Car* car, u8 isPlayer);
void SlowDownDriftingCar(Car* car, u8 isPlayer);
void UpdateBearing(Car* car, u8 isPlayer);
void CheckHitDetection(Car* car, u8 isPlayer);
void CalcHitDynamics(Car* car, HdCsHit* hit, u8 isPlayer);
void CheckIfLostAICar(Car* car);
u8 CheckCSHit(Car* car, u8 isPlayer, Cs* obj, s32 a, s32 b);
void DoCsHitCalculations(Car* car, u8 isPlayer, Car* other, u8 otherIsPlayer, s32 oang, u16 mode);
void RotateCarsAwayFromCollision(CarMotion* a, CarMotion* b);
void RicochetOffObject(Car* car, u8 isPlayer, u8 doHit, s32 delta, u16 mode);
void SetCollisionBounce(Car* car, u8 isPlayer, s32 mag);
void InitNoCollision(CarCollision* c);
void SetCollisCount(CarCollision* c);
void CarViewUpdate(PlayerCar* car);
void SetBounce(Car* car, s32 level, s32 a, s32 b, u8 isPlayer);
void CarInitBounceDeltas(Car* car, u8 isPlayer);
void UpdateBounce(Car* car, u8 isPlayer);
void carTakeHit(s32 id, s32 amount, s32 a, s32 flag);
void InitCatapult(Car* car, u8 isPlayer);
void CheckCatapults(Car* car, u8 isPlayer);
void UpdateCatapultedTire(CarTire* t);
void InitBombDamage(Car* car, u8 isPlayer, s8 full, u8 noMotion);
void CheckBombDamage(Car* car, u8 isPlayer);
void UpdateBombDamagedTire(CarTire* t, CarMotion* m);
void CheckSpikeDamage(Car* car, u8 isPlayer);
void InitMonsterSmash(Car* carA, u8 isPlayerA, Car* carB, u8 isPlayerB, s16 unused_mode);
void SetCarVRMode(PlayerCar* car, u32 mode);
void UpdateCarDeath(Car* car, u8 isPlayer);
void TermCar(void);

#endif // __TM1_CAR_H__
