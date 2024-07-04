#ifndef __SDK_LIBGTE_H__
#define __SDK_LIBGTE_H__

typedef struct {
    short m[3][3]; // 3 x 3 matrix coefficient value
    long t[3]; // Parallel transfer volume
} MATRIX; // size = 0x1E

typedef struct {
    long vx, vy, vz; // Vector coordinates
    long pad; // System reserved
} VECTOR; // size = 0x10

typedef struct {
    short vx, vy, vz; // Vector coordinates
    short pad; // System reserved
} SVECTOR; // size = 0x8

typedef struct {
    unsigned char r, g, b; // Color palette
    unsigned char cd; // GPU code
} CVECTOR;

#endif // __SDK_LIBGTE_H__
