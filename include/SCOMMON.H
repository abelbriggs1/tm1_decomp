#ifndef __SCOMMON_H__
#define __SCOMMON_H__

/*
 * Small uninitialized globals that are visible to every TU.
 * Every TU must define them so that the first linked object owns all of them;
 * otherwise `ld` groups them by owner and can't match the original order.
 *
 * `ld` then orders them by a hash of their names. These suffixes were
 * brute-forced to reproduce the original order.
 * (psylink used a different hashing method, making the order impossible to match
 * otherwise.)
 */
#define soundVabIds soundVabIds_104
#define gInfiniteWeapons gInfiniteWeapons_38
#define worldEP worldEP_47
#define gGodMode gGodMode_1608

s16 soundVabIds[2];
s32 gInfiniteWeapons;
s32 worldEP;
s32 gGodMode;

#endif // __SCOMMON_H__
