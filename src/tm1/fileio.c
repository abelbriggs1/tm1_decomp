#include "common.h"

#include <libcd.h>
#include <libetc.h>
#include <stdio.h>
#include <sys/types.h>

// TODO: Strange behavior; constant string thrown into `.sdata` but not accessed via
// `$gp`. Both `-G0` and `-G8` produce the wrong result.
#ifdef NON_MATCHING
s32 fileioOpenFile(char* name, s32* sizeOut)
{
    CdlFILE file;
    char path[40];
    u8 failed = 1;
    u8 done = 0;
    s32 tries = 0;

    sprintf(path, "\\\\%s;1", name); /* retail8018C46C starts with two backslash bytes */
    do {
        if (CdSearchFile(&file, path) != 0) {
            done = 1;
            failed = 0;
        } else if (tries >= 300) {
            done = 1;
        } else {
            tries++;
            VSync(0);
        }
    } while (!done);

    if (failed) {
        return 0;
    }
    *sizeOut = file.size;
    CdControl(CdlSetloc, (u_char*)&file.pos, 0);
    return 1;
}
#else
INCLUDE_ASM("/mnt/brahms/projects/tm_decomp/asm/nonmatchings/tm1/fileio", fileioOpenFile);
#endif // NON_MATCHING

s32 fileioOpenFileAsync(char* name, s32* sizeOut)
{
    return fileioOpenFile(name, sizeOut);
}

s32 fileioSeekToFileAsync(char* name)
{
    CdlFILE file;

    /* Retail ignores a failed search and still consumes file.pos. This path
     * needs a successful descriptor; do not claim missing-file recovery. */
    CdSearchFile(&file, name);
    CdControl(CdlSetloc, (u_char*)&file.pos, 0);
    return 0;
}

s32 fileioReadFile(s32 fd, u_long* buf, s32 size)
{
    u_char result[8];

    /* Retail ADDIU wraps before signed division, including at INT_MAX. */
    CdRead((s32)((u32)size + 2047u) / 2048, buf, 0x81);
    CdReadSync(0, result);
    return size;
}

void fileioReadFileAsync(s32 fd, u_long* buf, s32 size)
{
    CdRead((s32)((u32)size + 2047u) / 2048, buf, 0x81);
}

s32 fileioAsyncReadCompleted(void)
{
    u_char result[8];

    return CdReadSync(1, result) == 0;
}

s32 fileioCloseFile(s32 fd)
{
    return 0;
}

s32 fileioLoadFileIntoRam(void* dst, char* name)
{
    s32 size;
    s32 fd;
    s32 n;

    fd = fileioOpenFile(name, &size);
    /* Bug: open returns 0 on failure, not -1, and leaves size unwritten. */
    if (fd == -1) {
        return 0;
    }
    n = fileioReadFile(fd, dst, size);
    fileioCloseFile(fd);
    return n;
}

s32 fileioWriteFileFromRam(void* buf, char* name, s32 len)
{
    s32 fd;
    s32 tries;
    s32 n;

    fd = PCopen(name, 1, 0);
    if (fd == -1) {
        tries = 0;
        while (1) {
            if (tries >= 180) {
                break;
            }
            VSync(0);
            fd = PCopen(name, 1, 0);
            tries++;
            if (fd != -1) {
                break;
            }
        }
        if (fd == -1) {
            goto fail;
        }
    }
    n = PCwrite(fd, buf, len);
    PCclose(fd);
    return n;
fail:
    return 0;
}

void fileioInit(void)
{
    CdInit();
    CdSetDebug(0);
}
