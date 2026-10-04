#ifndef __TM1_CAR_UPDATE_H__
#define __TM1_CAR_UPDATE_H__

#include "common.h"

#include "tm1/car.h"
#include "tm1/trigger_pts.h"

typedef struct {
    s16 w; /* 0x00 */
    s16 h; /* 0x02 */
    s16 t; /* 0x04 */
    s16 pad; /* 0x06 */
    s32 x; /* 0x08 */
    s32 z; /* 0x0C */
} Curb; /* 0x10 */

extern Curb curbs[];
extern s16 numCurbs;

typedef struct {
    u8 active; /* +0  */
    u8 pad1;
    s16 a; /* +2  */
    s16 b; /* +4  */
    s16 c; /* +6  */
    s32 x; /* +8  */
    s32 z; /* +12 */
} PotHole; /* 16  */

extern PotHole potHoles[];
extern s16 numPotHoles;
extern s16 numCheckPotHoles;

typedef struct SlickSpot {
    s16 w; /* +0 */
    s16 h; /* +2 */
    s32 x; /* +4 */
    s32 z; /* +8 */
} SlickSpot;

extern SlickSpot slickSpots[];
extern s16 numSlickSpots;

s16 GetClosestTriggerPt(void* car, u8 which);
void CalcTireCoordinates(Car* car, u8 which);
s16 GetGroupTestPointIsIn(s32 x, s32 z, s32 y);
void CheckBridges(Car* car, u8 which);
void CheckForBridge(CarTire* tire, u8 which, s32 height);
u8 GetBridgeGroundHeight(CarTire* tire, s16 idx, s32 height);
void GetBridgeTireHeight(CarTire* tire, s32 height);
void CheckPotHoles(Car* car, u8 which);
void CheckForPotHole(CarTire* tire, CarCollision* collision);
void CheckCurbs(Car* car, u8 which);
void CheckForCurb(CarTire* tire, CarCollision* collision);
void CheckSlickSpots(Car* car, u8 which);
void CheckHealthStands(Car* car, u8 which);
void UpdateTirePositions(Car* car, u8 which);
void PutAICarBackOnRoof(CarAlt* car);
void UpdateRollingCar(Car* car, u8 which);
void InitTireInfo(CarTire* tires);
void InitTireGroup(CarTire* tires, s16 group);
void UpdateCarOnSlickSpot(Car* car, u8 which);
s32 GetMinSpeedNeeded(s16 group, s16 idx);
s16 GetNumBridges(void);
s16 MakeFakePotHole(void);
void UpdateNumPotHoles(void);
TriggerPtStartPts* GetTriggerPtStartPts(void);
void CalcDistFromRoadCenter(CarAlt* car, s32* out, u8 which);
void CheckMonsterSmash(Car* car, u8 which);
void InitTriggerPoint(CarAlt* car);
void UpdateCurrentTriggerPt(CarAlt* car);
void CalcTurnStart(CarAlt* car, u8 which);
void GetNextTriggerPoint(CarAlt* car, s16 pt);
s16 GetClosestTriggerPointFromCurrPt(CarAlt* car, TriggerPt* tp, s16* dist);
s16 GetLookAheadPos(CarAlt* car, s32* out);

#endif // __TM1_CAR_UPDATE_H__
