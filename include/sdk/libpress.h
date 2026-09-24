/*
 * $PSLibId: Runtime Library Version 3.3$
 */
#ifndef _LIBPRESS_H_
#define _LIBPRESS_H_
/*
 *  (C) Copyright 1995 Sony Corporation,Tokyo,Japan. All Rights Reserved
 *
 * 		libpress.h: Prototypes for libpress
 *
 */
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif
extern void DecDCTReset(int mode);
extern int DecDCTBufSize(u_long *bs);
extern int DecDCTvlc(u_long *bs, u_long *buf);
extern void DecDCTin(u_long *buf, int mode);
extern void DecDCTout(u_long *buf, int size);
extern int DecDCTinSync( int mode) ;
extern int DecDCToutSync( int mode) ;
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif
#endif /* _LIBPRESS_H_ */
