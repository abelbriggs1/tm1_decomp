#ifndef __TM1_LEVELS_H__
#define __TM1_LEVELS_H__

// Map of levels to their IDs/indices in the code.
#define LEVEL1_ARENA 0
#define LEVEL2_WAREHOUSE 1
#define LEVEL3_FREEWAY 2
#define LEVEL4_CITY_PARK 3
#define LEVEL5_CYBURBIA 4
#define LEVEL6_ROOFTOPS 5

// Bonus arena with 5 opponents at the end of Hard mode (Fight for Your Life)
// Uses the ARENA stage.
#define LEVEL7_ARENA_5 6

#define LEVEL_MAX (LEVEL7_ARENA_5 + 1)

// Converts a level index to its 1-based level number, as used by the `LevelX`
// function names.
#define LEVEL_NUM(level) ((level) + 1)

#endif // __TM1_LEVELS_H__
