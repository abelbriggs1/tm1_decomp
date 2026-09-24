#ifndef __TM1_FILEIO_H__
#define __TM1_FILEIO_H__

#include "common.h"

s32 fileioOpenFile(char* name, s32* sizeOut);
s32 fileioOpenFileAsync(char* name, s32* sizeOut);
s32 fileioSeekToFileAsync(char* name);
s32 fileioReadFile(s32 fd, u_long* buf, s32 size);
void fileioReadFileAsync(s32 fd, u_long* buf, s32 size);
s32 fileioAsyncReadCompleted(void);
s32 fileioCloseFile(s32 fd);
s32 fileioLoadFileIntoRam(void* dst, char* name);
s32 fileioWriteFileFromRam(void* buf, char* name, s32 len);

#endif // __TM1_FILEIO_H__
