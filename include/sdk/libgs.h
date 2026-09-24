/*
 * $PSLibId: Runtime Library Version 3.3$
 */
#ifndef _LIBGS_H_
#define _LIBGS_H_
/*
 *	libgs.h: Graphic Library Header
 *
 *
 *		Version 1.**	Apr,  8, 1994
 *
 *		Copyright (C) 1993 by Sony Corporation
 *			All rights Reserved
 */

#ifndef NULL
#define NULL 0
#endif

/*** packet peripheral pointer ***/
typedef unsigned char   PACKET;

#define PSBANK 0x80000000
/*** --- Zsort resolution --- ***/
#define ZRESOLUTION     0x3fff

/*** --- coordinate keyword - ***/
#define WORLD NULL
#define SCREEN ((GsCOORDINATE *)0x0001)
#define SCREEN2 ((GsCOORDINATE2 *)0x0001)



typedef struct _GsCOORDINATE {
	long rx,ry,rz;
	short sx,sy,sz;
	long  tx,ty,tz;
	struct _GsCOORDINATE  *super;
	struct _GsCOORDINATE  *sub;
	} GsCOORDINATE;

typedef struct {
	VECTOR scale;
	SVECTOR rotate;
	VECTOR trans;
	} GsCOORD2PARAM;

typedef struct _GsCOORDINATE2 {
        unsigned long flg;
        MATRIX coord;
	MATRIX workm;
	GsCOORD2PARAM  *param;
	struct _GsCOORDINATE2  *super;
	struct _GsCOORDINATE2  *sub;
	} GsCOORDINATE2;

typedef struct {
        long rx,ry,rz;
	short sx,sy,sz;
	long tx,ty,tz;
	GsCOORDINATE *super;
        } GsVIEW;

typedef struct {
        MATRIX view;
	GsCOORDINATE2 *super;
        } GsVIEW2;

typedef struct {
        long vpx,vpy,vpz;
	long vrx,vry,vrz;
	long rz;
	GsCOORDINATE *super;
	} GsRVIEW;

typedef struct {
        long vpx,vpy,vpz;
	long vrx,vry,vrz;
	long rz;
	GsCOORDINATE2 *super;
	} GsRVIEW2;

typedef struct {
  int vx,vy,vz;
	unsigned char r,g,b;
	} GsF_LIGHT;


typedef struct {
        unsigned p : 24;
        unsigned char num : 8;
	} GsOT_TAG;


typedef struct {
	unsigned long length;
	GsOT_TAG *org;
	unsigned long offset;
	unsigned long point;
	GsOT_TAG *tag;
	} GsOT;

/*** --- object structure --- ***/
typedef struct  {
        unsigned long attribute;	/* pers,trans,rotate,disp */
        MATRIX  lwmatrix;		/* local-world  matrix */
	MATRIX  lightm;
	GsCOORDINATE    coord;		/* local dmatrix */
	unsigned long  *tmd;
	unsigned long  id;
        } GsDOBJ;


typedef struct  {
        unsigned long attribute;	/* pers,trans,rotate,disp */
	GsCOORDINATE2  *coord2;	/* local dmatrix */
	unsigned long  *tmd;
	unsigned long  id;
       } GsDOBJ2;

typedef struct  {
        unsigned long attribute;	/* pers,trans,rotate,disp */
	GsCOORDINATE2  *coord2;	/* local dmatrix */
	unsigned long  *pmd;	/* pmd top address */
	unsigned long  *base;	/* object base address */
	unsigned long  *sv;	/* shared vertex base */
	unsigned long  id;
       } GsDOBJ3;

typedef struct  {
  unsigned long attribute;
  GsCOORDINATE2 *coord2;
  unsigned long *tmd;
  unsigned long *packet;
  unsigned long id;
} GsDOBJ5;

typedef struct {
	unsigned long attribute;
	short x,y;
	unsigned short w,h;
	unsigned short tpage;
	unsigned char u,v;
	short cx,cy;
	unsigned char r,g,b;
	short mx,my;
	short scalex,scaley;
	long rotate;
} GsSPRITE;

typedef struct {
	unsigned long attribute;
	short x,y;
	DR_MODE mode[2];	/* Draw mode primitive */
	SPRT packet[2];		/* Sprite primitive */
} GsSPARRAY;

typedef struct {
	unsigned char u, v;
	unsigned short cba;
	unsigned short flag;
	unsigned short tpage;
	} GsCELL;

typedef struct {
	unsigned char cellw,cellh;
	unsigned short ncellw,ncellh;
	GsCELL *base;
	unsigned short *index;
	} GsMAP;

typedef struct {
	unsigned long attribute;
	short x,y;
	short w,h;
	short scrollx,scrolly;
	unsigned char r,g,b;
	GsMAP *map;
	short mx,my;
	short scalex,scaley;
	long rotate;
	} GsBG;

typedef struct {
	unsigned long attribute;
	short x0,y0;
	short x1,y1;
	unsigned char r,g,b;
        } GsLINE;

typedef struct {
	unsigned long attribute;
	short x0,y0;
	short x1,y1;
	unsigned char r0,g0,b0;
	unsigned char r1,g1,b1;
        } GsGLINE;

typedef struct {
	unsigned long attribute;
	short x,y;
	unsigned short w,h;
	unsigned char r,g,b;
        } GsBOXF;

typedef struct {
        short dqa;
        long  dqb;
        unsigned char rfc,gfc,bfc;
        } GsFOGPARAM;


typedef struct {
  unsigned long pmode;
  short px,py;
  unsigned short pw,ph;
  unsigned long *pixel;
  short cx,cy;
  unsigned short cw,ch;
  unsigned long *clut;
} GsIMAGE;

typedef struct {
  short offx,offy;
} _GsPOSITION;

typedef struct {
  unsigned long farz;
  unsigned long nearz;
} GsZCLIP;

/* Object Management Table */
typedef struct {
	GsDOBJ *top;
	int nobj;
	int maxobj;
} GsOBJTABLE;

typedef struct {
	GsDOBJ2 *top;
	int nobj;
	int maxobj;
} GsOBJTABLE2;

typedef struct 
{
  SVECTOR  **diff_sp_vertex;
  unsigned long   *diff_off_vertex;
  unsigned long   *diff_n_vertex;
  unsigned long   *diff_obj_vertex;
  long     *mimepr;
} GsMIMEV;

typedef struct
{
  SVECTOR  **diff_sp_norm;
  unsigned long   *diff_off_norm;
  unsigned long   *diff_n_norm;
  unsigned long   *diff_obj_norm;
  long     *mimepr;
} GsMIMEN;

typedef struct
{
  PACKET
    *(*f3[2][3])();
  PACKET
    *(*nf3[2])();
  PACKET
    *(*g3[2][3])();
  PACKET
    *(*ng3[2])();
  PACKET
    *(*tf3[2][3])();
  PACKET
    *(*ntf3[2])();
  PACKET
    *(*tg3[2][3])();
  PACKET
    *(*ntg3[2])();
  PACKET
    *(*f4[2][3])();
  PACKET
    *(*nf4[2])();
  PACKET
    *(*g4[2][3])();
  PACKET
    *(*ng4[2])();
  PACKET
    *(*tf4[2][3])();
  PACKET
    *(*ntf4[2])();
  PACKET
    *(*tg4[2][3])();
  PACKET
    *(*ntg4[2])();
} _GsFCALL;


#define GsDivMODE_NDIV 0
#define GsDivMODE_DIV  1
#define GsLMODE_NORMAL 0
#define GsLMODE_FOG    1
#define GsLMODE_LOFF   2


/*
 * libgs macro
 */
#define GsOFSGTE 0
#define GsOFSGPU 4
#define GsINTER  1
#define GsNONINTER 0

/*
 * object attribute set macro
 */
#define GsLDIM0 0
#define GsLDIM1 1
#define GsLDIM2 2
#define GsLDIM3 3
#define GsLDIM4 4
#define GsLDIM5 5
#define GsLDIM6 6
#define GsLDIM7 7
#define GsFOG   (1<<3)
#define GsMATE  (1<<4)
#define GsLLMOD (1<<5)
#define GsLOFF  (1<<6)
#define GsZIGNR (1<<7)
#define GsNBACKC (1<<8)
#define GsDIV1   (1<<9)
#define GsDIV2   (2<<9)
#define GsDIV3   (3<<9)
#define GsDIV4	 (4<<9)
#define GsDIV5	 (5<<9)
#define GsAZERO  (0<<28)
#define GsAONE   (1<<28)
#define GsATWO   (2<<28)
#define GsATHREE (3<<28)
#define GsALON   (1<<30)
#define GsDOFF   (1<<31)
/*
 * BG/sprite attribute set macro
 */
#define GsPERS   (1<<26)
#define GsROTOFF (1<<27)

/*
 * TOD animation
 */
#define GsTOD_ATTR (0)
#define GsTOD_COORD (1)
#define GsTOD_TMDID (2)
#define GsTOD_PARENT (3)
#define GsTOD_MATRIX (4)
#define GsTOD_TMDDATA (5)
#define GsTOD_LIGHT (6)
#define GsTOD_CAMERA (7)
#define GsTOD_OBJCTL (8)
#define GsTOD_USER0 (9)
#define GsTOD_USER1 (10)
#define GsTOD_USER2 (11)
#define GsTOD_USER3 (12)
#define GsTOD_USER4 (13)
#define GsTOD_SYSFUNC (15)

#define GsTOD_CREATE (0)
#define GsTOD_NOCREATE (1)
#define GsTOD_COORDONLY (2)

#define GsOBJ_UNDEF (0xffffffff)

#define GsIncFrame()  (PSDCNT=(PSDCNT++ == 0?1:PSDCNT));
#define GsSetAzwh(z,w,h)    GsADIVZ = (z),GsADIVW = (w),GsADIVH = (h);

/*
 * TMD structure
 */
/*** GTE PACKET to-GPU command '<packet-name>.code' ***/
#define GPU_COM_F3    0x20
#define GPU_COM_TF3   0x24
#define GPU_COM_G3    0x30
#define GPU_COM_TG3   0x34

#define GPU_COM_F4    0x28
#define GPU_COM_TF4   0x2c
#define GPU_COM_G4    0x38
#define GPU_COM_TG4   0x3c

#define GPU_COM_NF3   0x21
#define GPU_COM_NTF3  0x25
#define GPU_COM_NG3   0x31
#define GPU_COM_NTG3  0x35

#define GPU_COM_NF4   0x29
#define GPU_COM_NTF4  0x2d
#define GPU_COM_NG4   0x39
#define GPU_COM_NTG4  0x3d


/*** TMD structure ****/
typedef struct {
  u_char out,in,dummy,cd;
  u_char r0,g0,b0,code;
  u_short n0,v0;
  u_short v1,v2;
} TMD_P_F3;

typedef struct {
  u_char out,in,dummy,cd;
  u_char r0,g0,b0,code;
  u_short n0,v0;
  u_short n1,v1;
  u_short n2,v2;
} TMD_P_G3;

typedef struct {
  u_char out,in,dummy,cd;
  u_char r0,g0,b0,code;
  u_short v0,v1;
  u_short v2,p;
} TMD_P_NF3;

typedef struct {
  u_char out,in,dummy,cd;
  u_char r0,g0,b0,code;
  u_char r1,g1,b1,p1;
  u_char r2,g2,b2,p2;
  u_short v0,v1;
  u_short v2,p;
} TMD_P_NG3;

typedef struct {
  u_char out,in,dummy,cd;
  u_char r0,g0,b0,code;
  u_short n0,v0;
  u_short v1,v2;
  u_short v3,p;
} TMD_P_F4;

typedef struct {
  u_char out,in,dummy,cd;
  u_char r0,g0,b0,code;
  u_short n0,v0;
  u_short n1,v1;
  u_short n2,v2;
  u_short n3,v3;
} TMD_P_G4;

typedef struct {
  u_char out,in,dummy,cd;
  u_char r0,g0,b0,code;
  u_short v0,v1;
  u_short v2,v3;
} TMD_P_NF4;

typedef struct {
  u_char out,in,dummy,cd;
  u_char r0,g0,b0,code;
  u_char r1,g1,b1,p1;
  u_char r2,g2,b2,p2;
  u_char r3,g3,b3,p3;
  u_short v0,v1;
  u_short v2,v3;
} TMD_P_NG4;



typedef struct {
  u_char out,in,dummy,cd;
  u_char tu0, tv0;u_short	clut;
  u_char tu1, tv1;u_short	tpage;
  u_char tu2, tv2;u_short	p;
  u_short n0,v0;
  u_short v1,v2;
} TMD_P_TF3;

typedef struct {
  u_char out,in,dummy,cd;
  u_char tu0, tv0;u_short	clut;
  u_char tu1, tv1;u_short	tpage;
  u_char tu2, tv2;u_short	p;
  u_short n0,v0;
  u_short n1,v1;
  u_short n2,v2;
} TMD_P_TG3;

typedef struct {
  u_char out,in,dummy,cd;
  u_char tu0, tv0;u_short	clut;
  u_char tu1, tv1;u_short	tpage;
  u_char tu2, tv2;u_short	p0;
  u_char r0,g0,b0,p1;
  u_short v0,v1;
  u_short v2,p2;
} TMD_P_TNF3;

typedef struct {
  u_char out,in,dummy,cd;
  u_char tu0, tv0;u_short	clut;
  u_char tu1, tv1;u_short	tpage;
  u_char tu2, tv2;u_short	p0;
  u_char r0,g0,b0,p1;
  u_char r1,g1,b1,p2;
  u_char r2,g2,b2,p3;
  u_short v0,v1;
  u_short v2,p4;
} TMD_P_TNG3;

typedef struct {
  u_char out,in,dummy,cd;
  u_char tu0, tv0;u_short	clut;
  u_char tu1, tv1;u_short	tpage;
  u_char tu2, tv2;u_short	p0;
  u_char tu3, tv3;u_short	p1;
  u_short n0,v0;
  u_short v1,v2;
  u_short v3,p2;
} TMD_P_TF4;

typedef struct {
  u_char out,in,dummy,cd;
  u_char tu0, tv0;u_short	clut;
  u_char tu1, tv1;u_short	tpage;
  u_char tu2, tv2;u_short	p0;
  u_char tu3, tv3;u_short	p1;  
  u_short n0,v0;
  u_short n1,v1;
  u_short n2,v2;
  u_short n3,v3;
} TMD_P_TG4;

typedef struct {
  u_char out,in,dummy,cd;
  u_char tu0, tv0;u_short	clut;
  u_char tu1, tv1;u_short	tpage;
  u_char tu2, tv2;u_short	p0;
  u_char tu3, tv3;u_short	p1;  
  u_char r0,g0,b0,p2;
  u_short v0,v1;
  u_short v2,v3;
} TMD_P_TNF4;

typedef struct {
  u_char out,in,dummy,cd;
  u_char tu0, tv0;u_short	clut;
  u_char tu1, tv1;u_short	tpage;
  u_char tu2, tv2;u_short	p0;
  u_char tu3, tv3;u_short	p1;
  u_char r0,g0,b0,p2;
  u_char r1,g1,b1,p3;
  u_char r2,g2,b2,p4;
  u_char r3,g3,b3,p5;
  u_short v0,v1;
  u_short v2,v3;
} TMD_P_TNG4;

struct TMD_STRUCT
{
  u_long *vertop;		/* vertex top address of TMD format */
  u_long vern;			/* the number of vertex of TMD format */
  u_long *nortop;		/* normal top address of TMD format */
  u_long norn;			/* the number of normal of TMD format */
  u_long *primtop;		/* primitive top address of TMD format */
  u_long primn;			/* the number of primitives of TMD format */
  u_long scale;			/* the scale factor of TMD format */
};

/*
 * active sub divide structure
 *
 */

#define minmax4(x1,x2,x3,x4,x5,x6) x1>x2?(x6=x1,x5=x2):(x5=x1,x6=x2),\
                                   x3>x6?x6=x3:x3<x5?x5=x3:0,\
                                   x4>x6?x6=x4:x4<x5?x5=x4:0

#define minmax3(x1,x2,x3,x4,x5)    x1>x2?(x5=x1,x4=x2):(x4=x1,x5=x2),\
                                   x3>x5?x5=x3:x3<x4?x4=x3:0


typedef struct {
  short   vx,vy,vz;
  u_char  tu,tv;
} VERT;

typedef struct {
  short   vx,vy,vz;
  u_char  tu,tv;
  CVECTOR col;
} VERTC;


typedef struct
{
  u_long limit;			/* divide limit */
  long   hwd,vwd;		/* dummy */
  int    shift;			/* OT shift */
  u_long  *org;			/* OT org */
  u_long   *pk;			/* packet base */
  long     otz;			/* gte otz */
  long   adivz;		        /* active divide codition z */
  short  adivw,adivh;           /* active divide condition w,h */
  long    flg0;			/* gte flag */
  long     flg;			/* gte flag */
  short  minx,miny,maxx,maxy;	/* polygon min-max */
  short  hwd0,vwd0;		/* resolution of screen */
  u_long   *tag;		/* work temprly for addPrim */
  POLY_FT4  si;			/* work packet */
} GsADIV_FT4;

typedef struct {
  VERT     vt[4];
} GsADIV_P_FT4;



typedef struct
{
  u_long limit;			/* divide limit */
  long   hwd,vwd;		/* dummy */
  int    shift;			/* OT shift */
  u_long  *org;			/* OT org */
  u_long   *pk;			/* packet base */
  long     otz;			/* gte otz */
  long   adivz;			/* active divide codition z */
  short  adivw,adivh;           /* active divide condition w,h */  
  long    flg0;			/* gte flag */
  long     flg;			/* gte flag */
  short  minx,miny,maxx,maxy;	/* polygon min-max */
  short  hwd0,vwd0;		/* resolution of screen */
  u_long   *tag;		/* work temprly for addPrim */
  POLY_GT4  si;			/* work packet */
} GsADIV_GT4;

typedef struct {
  VERTC   vt[4];
} GsADIV_P_GT4;


typedef struct
{
  u_long limit;			/* divide limit */
  long   hwd,vwd;		/* dummy */  
  int    shift;			/* OT shift */
  u_long  *org;			/* OT org */
  u_long   *pk;			/* packet base */
  long     otz;			/* gte otz */
  long   adivz;			/* active divide codition z */
  short  adivw,adivh;           /* active divide condition w,h */  
  long    flg0;			/* gte flag */
  long     flg;			/* gte flag */
  short  minx,miny,maxx,maxy;	/* polygon min-max */
  short  hwd0,vwd0;		/* resolution of screen */
  u_long   *tag;		/* work temprly for addPrim */
  POLY_G4  si;			/* work packet */
} GsADIV_G4;

typedef struct {
  VERTC   vt[4];
} GsADIV_P_G4;

typedef struct
{
  u_long limit;			/* divide limit */
  long   hwd,vwd;		/* dummy */  
  int    shift;			/* OT shift */
  u_long  *org;			/* OT org */
  u_long   *pk;			/* packet base */
  long     otz;			/* gte otz */
  long   adivz;			/* active divide codition z */
  short  adivw,adivh;           /* active divide condition w,h */  
  long    flg0;			/* gte flag */
  long     flg;			/* gte flag */
  short  minx,miny,maxx,maxy;	/* polygon min-max */
  short  hwd0,vwd0;		/* resolution of screen */
  u_long   *tag;		/* work temprly for addPrim */
  POLY_F4  si;			/* work packet */
} GsADIV_F4;

typedef struct {
  VERT   vt[4];
} GsADIV_P_F4;


typedef struct
{
  u_long limit;			/* divide limit */
  long   hwd,vwd;		/* dummy */
  int    shift;			/* OT shift */
  u_long  *org;			/* OT org */
  u_long   *pk;			/* packet base */
  long     otz;			/* gte otz */
  long   adivz;			/* active divide codition z */
  short  adivw,adivh;           /* active divide condition w,h */
  long     flg;			/* gte flag */
  short  minx,miny,maxx,maxy;	/* polygon min-max */
  short  hwd0,vwd0;		/* resolution of screen */
  u_long   *tag;		/* work temprly for addPrim */
  POLY_FT3  si;			/* work packet */
} GsADIV_FT3;

typedef struct {
  VERT     vt[3];
} GsADIV_P_FT3;

typedef struct
{
  u_long limit;			/* divide limit */
  long   hwd,vwd;		/* dummy */
  int    shift;			/* OT shift */
  u_long  *org;			/* OT org */
  u_long   *pk;			/* packet base */
  long     otz;			/* gte otz */
  long   adivz;			/* active divide codition z */
  short  adivw,adivh;           /* active divide condition w,h */  
  long     flg;			/* gte flag */
  short  minx,miny,maxx,maxy;	/* polygon min-max */
  short  hwd0,vwd0;		/* resolution of screen */
  u_long   *tag;		/* work temprly for addPrim */
  POLY_GT3  si;			/* work packet */
} GsADIV_GT3;

typedef struct {
  VERTC   vt[3];
} GsADIV_P_GT3;

typedef struct
{
  u_long limit;			/* divide limit */
  long   hwd,vwd;		/* dummy */  
  int    shift;			/* OT shift */
  u_long  *org;			/* OT org */
  u_long   *pk;			/* packet base */
  long     otz;			/* gte otz */
  long   adivz;			/* active divide codition z */
  short  adivw,adivh;           /* active divide condition w,h */  
  long     flg;			/* gte flag */
  short  minx,miny,maxx,maxy;	/* polygon min-max */
  short  hwd0,vwd0;		/* resolution of screen */
  u_long   *tag;		/* work temprly for addPrim */
  POLY_G3  si;			/* work packet */
} GsADIV_G3;

typedef struct {
  VERTC  vt[3];
} GsADIV_P_G3;

typedef struct
{
  u_long limit;			/* divide limit */
  long   hwd,vwd;		/* dummy */  
  int    shift;			/* OT shift */
  u_long  *org;			/* OT org */
  u_long   *pk;			/* packet base */
  long     otz;			/* gte otz */
  long   adivz;			/* active divide codition z */
  short  adivw,adivh;           /* active divide condition w,h */  
  long     flg;			/* gte flag */
  short  minx,miny,maxx,maxy;	/* polygon min-max */
  short  hwd0,vwd0;		/* resolution of screen */
  u_long   *tag;		/* work temprly for addPrim */
  POLY_F3  si;			/* work packet */
} GsADIV_F3;

typedef struct {
  VERT  vt[3];
} GsADIV_P_F3;


/*
 * PROTOTYPE DIFINITIONS
 */
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

void GsInitGraph(unsigned short x,unsigned short y,unsigned short intmode,
	       unsigned short dith,unsigned short varmmode);
void GsInit_3D(void);
void GsMapModelingData(unsigned long *p);

void GsSetProjection(long h);
int  GsSetRefView(GsRVIEW *pv);
int  GsSetFlatLight(int id,GsF_LIGHT *lt);
void GsSetLightMode(int mode);
void GsSetFogParam(GsFOGPARAM *fogparm);
void GsSetAmbient(long r,long g,long b);
void GsInitCoordinate(GsCOORDINATE *super,GsCOORDINATE *base);
void GsGetObjLw(GsDOBJ *objp);
void GsGetObjLight(GsDOBJ *objp);
void GsGetObjMatrix(GsDOBJ *objp);
void GsDrawOt(GsOT *ot);
void GsSetWorkBase(PACKET *outpacketp);

void GsSortObject(GsDOBJ *objp,GsOT *ot,int shift);
void GsSortObject2(GsDOBJ2 *objp,GsOT *ot,int shift);
void GsSortObject3(GsDOBJ3 *objp,GsOT *ot,int shift);
void GsSortObject4(GsDOBJ2 *objp,GsOT *ot,int shift,u_long *scratch);
void GsSortObject5(GsDOBJ5 *objp,GsOT *ot,int shift,u_long *scratch);
void GsSortObject5J(GsDOBJ5 *objp,GsOT *ot,int shift,u_long *scratch);

void GsSortSprite(GsSPRITE *sp,GsOT *ot,unsigned short pri);
void GsSortSpriteA(GsSPRITE *sp,GsOT *ot,unsigned short pri);
void GsSortFastSprite(GsSPRITE *sp,GsOT *ot,unsigned short pri);
void GsSortFastSpriteA(GsSPRITE *sp,GsOT *ot,unsigned short pri);
void GsSortFlipSprite(GsSPRITE *sp,GsOT *ot,unsigned short pri);
void GsInitFastSpriteArray(GsSPARRAY *sp, short n);
void GsSetFastSpriteArray(GsSPARRAY *sp,unsigned short w, unsigned short h,
			unsigned short tpage, unsigned char u, unsigned char v,
			unsigned short cx, unsigned short cy,
			unsigned char r, unsigned char g, unsigned char b);
void GsSortFastSpriteArray(GsSPARRAY *sp,unsigned short n,
			GsOT *ot,unsigned short pri);
void GsSortBg(GsBG *bg,GsOT *ot,unsigned short pri);
void GsSortFastBg(GsBG *bg,GsOT *ot,unsigned short pri);
void GsInitFixBg16(GsBG *bg,u_long *work);
void GsSortFixBg16(GsBG *bg,u_long *work,GsOT *otp,unsigned short pri);
void GsInitFixBg32(GsBG *bg,u_long *work);
void GsSortFixBg32(GsBG *bg,u_long *work,GsOT *otp,unsigned short pri);
void GsSortLine(GsLINE *lp,GsOT *ot,unsigned short pri);
void GsSortGLine(GsGLINE *lp,GsOT *ot,unsigned short pri);
void GsSortBoxFill(GsBOXF *bp,GsOT *ot,unsigned short pri);
void GsSortPoly(void *pp,GsOT *ot,unsigned short pri);

void GsClearOt(unsigned short offset,unsigned short point,GsOT *otp);
GsOT *GsSortOt(GsOT *ot_src,GsOT *ot_dest);
GsOT *GsCutOt(GsOT *ot_src,GsOT *ot_dest);
void GsDefDispBuff(unsigned short x0,unsigned short y0,unsigned short x1,unsigned short y1);
void GsClearDispBuff(unsigned char ,unsigned char ,unsigned char);
void GsSortClear(unsigned char ,unsigned char ,unsigned char ,GsOT *);
void GsGetTimInfo(unsigned long *im,GsIMAGE *tim);
void GsSwapDispBuff(void);
int GsGetActiveBuff(void);
void GsSetDrawBuffClip(void);
void GsSetDrawBuffOffset(void);
void GsSetClip(RECT *clip);
DRAWENV *GsSetClip2(RECT *clip);
void GsSetOffset(long x,long y);
void GsSetOrign(long x,long y);
int GsInitObjTable(GsOBJTABLE *, GsDOBJ *, int);
GsDOBJ *GsSearchObjByID(GsOBJTABLE *, unsigned long);
GsDOBJ *GsCreateNewObj(GsOBJTABLE *, unsigned long);
GsDOBJ *GsRemoveObj(GsOBJTABLE *, unsigned long);
int GsInitObjTable2(GsOBJTABLE2 *, GsDOBJ2 *, GsCOORDINATE2 *, GsCOORD2PARAM *, int);
GsDOBJ2 *GsSearchObjByID2(GsOBJTABLE2 *, unsigned long);
GsDOBJ2 *GsCreateNewObj2(GsOBJTABLE2 *, unsigned long);
GsDOBJ2 *GsRemoveObj2(GsOBJTABLE2 *, unsigned long);
unsigned long *GsSearchTMDByID(unsigned long *, int *, int);

void gte_mul_matrix(MATRIX *m0,MATRIX *m1);
void gte_mul_matrix2(MATRIX *m0,MATRIX *m1);
void gte_apply_matrix(MATRIX *m,short x,short y,short z,short *xo,short *yo,short *zo);
void gte_apply_matrixl(MATRIX *m,long x,long y,long z,long *xo,long *yo,long *zo);
void gte_rotate_x_matrix(MATRIX *m,long rx);
void gte_rotate_y_matrix(MATRIX *m,long ry);
void gte_rotate_z_matrix(MATRIX *m,long rz);
void gte_trans_matrix(MATRIX *m,long tx,long ty,long tz);
int gte_scale_matrix(MATRIX *m,short sx,short sy,short sz);
void gte_rev_matrix(MATRIX *m0,MATRIX *m1);
void GsInitCoordinate2(GsCOORDINATE2 *super,GsCOORDINATE2 *base);
void GsInitCoord2param(GsCOORD2PARAM *);
void GsMulCoord0(MATRIX *m1,MATRIX *m2,MATRIX *m3);
void GsMulCoord2(MATRIX *m1,MATRIX *m2);
void GsMulCoord3(MATRIX *m1,MATRIX *m2);
void GsGetLw(GsCOORDINATE2 *m,MATRIX *out);
void GsGetLs(GsCOORDINATE2 *m,MATRIX *out);
void GsGetLws(GsCOORDINATE2 *m,MATRIX *outw,MATRIX *outs);

void GsLinkObject(unsigned long tmd,GsDOBJ *obj_base,int n);
void GsLinkObject2(unsigned long tmd_base,GsDOBJ2 *objp,int n);
u_long GsLinkObject3(unsigned long pmd_base,GsDOBJ3 *objp);
void GsLinkObject4(unsigned long tmd_base,GsDOBJ2 *objp,int n);
void GsLinkObject5(unsigned long tmd_base,GsDOBJ5 *objp,int n);

void GsSetLightMatrix(MATRIX *mp);
void GsSetLightMatrix2(MATRIX *mp);
int GsSetRefView2(GsRVIEW2 *pv);
int    GsSetView2(GsVIEW2 *pv);
void GsSetLwMatrix(MATRIX *mp);
void GsSetClip2D(RECT *rectp);
void GsInitVcount();
long GsGetVcount();
void GsClearVcount();
unsigned long *GsSetTodFrame();
unsigned long *GsSetTodPacket();
unsigned long *GsSetTodFrame2(int,unsigned long *,GsOBJTABLE2 *,int *,unsigned long *,int);
unsigned long *GsSetTodPacket2(unsigned long *,GsOBJTABLE2 *,int *,unsigned long *,int);
void GsClearDispArea(unsigned char r,unsigned char g,unsigned char b);

u_long *GsPresetObject(GsDOBJ5 *objp,u_long *base_addr);
void GsScaleScreen(SVECTOR *scale);

PACKET *GsA4divF3L(TMD_P_F3 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divF3LFG(TMD_P_F3 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divF3NL(TMD_P_F3 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divNF3(TMD_P_NF3 *op,VERT *vp,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divF4L(TMD_P_F4 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divF4LFG(TMD_P_F4 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divF4NL(TMD_P_F4 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divNF4(TMD_P_NF4 *op,VERT *vp,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divG3L(TMD_P_G3 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divG3LFG(TMD_P_G3 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divG3NL(TMD_P_G3 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divNG3(TMD_P_NG3 *op,VERT *vp,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divG4L(TMD_P_G4 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divG4LFG(TMD_P_G4 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divG4NL(TMD_P_G4 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divNG4(TMD_P_NG4 *op,VERT *vp,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divFT3L(TMD_P_TF3 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divFT3LFG(TMD_P_TF3 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divFT3NL(TMD_P_TF3 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divNTF3(TMD_P_TNF3 *op,VERT *vp,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divFT4L(TMD_P_TF4 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divFT4LFG(TMD_P_TF4 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divFT4NL(TMD_P_TF4 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divNTF4(TMD_P_TNF4 *op,VERT *vp,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divTG3L(TMD_P_TG3 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divTG3LGG(TMD_P_TG3 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divTG3NL(TMD_P_TG3 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divNTG3(TMD_P_TNG3 *op,VERT *vp,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divTG4L(TMD_P_TG4 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divTG4LFG(TMD_P_TG4 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divTG4NL(TMD_P_TG4 *op,VERT *vp,VERT *np,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
PACKET *GsA4divTNG4(TMD_P_TNG4 *op,VERT *vp,PACKET *pk,int n,
		   int shift,GsOT *ot,u_long *scratch);
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif



/* EXTERN */
extern RECT CLIP2;		/* clipping area */
extern short PSDBASEX[2],PSDBASEY[2];	/* double buffer base */
extern short PSDIDX;		/* double buffer index */
extern u_long PSDCNT;		/* frame counter for using matrix cache */
extern _GsPOSITION POSITION;	/* 2d offset */
extern DRAWENV GsDRAWENV;	/* DRAWENV of Gs */
extern DISPENV GsDISPENV;	/* DISPENV of Gs */
extern MATRIX  GsLSMATRIX;	/* Local-Screen Matrix of Gs */
extern MATRIX  GsWSMATRIX;	/* Current World-Screen Matrix of Gs */
extern MATRIX  GsWSMATRIX_ORG;  /* Original World-Screen Matrix of Gs */
extern long    HWD0,VWD0; 	/* rezolution of Holyzontal and Vertical */
extern MATRIX  GsLIGHTWSMATRIX;	/* World-Screen Light Matrix of Gs */
extern MATRIX  GsIDMATRIX;	/* Unit Matrix */
extern MATRIX  GsIDMATRIX2;	/* Unit Matrix including Aspect retio */
extern PACKET  *GsOUT_PACKET_P;	/* Work Base pointer */
extern long    GsADIVZ;         /* Active sub divide condition (z) */
extern short  GsADIVW,GsADIVH;  /* Active sub divide condition (w,h) */
extern int     GsLIGHT_MODE;	/* lighting mode global */
extern u_long  GsMATE_C,GsLMODE,GsLIGNR,GsLIOFF,GsZOVER,GsBACKC,GsNDIV;
extern u_long  GsTRATE,GsTON,GsDISPON;


#if 0
extern _GsFCALL GsFCALL5;	/* GsSortObject5J Func Table */
                                /* fook only useing functions */
jt_init5()			/* Gs SortObject5J Fook Func */
{
  PACKET *GsPrstF3NL(),*GsPrstF3LFG(),*GsPrstF3L(),*GsPrstNF3();
  PACKET *GsTMDdivF3NL(),*GsTMDdivF3LFG(),*GsTMDdivF3L(),*GsTMDdivNF3();
  PACKET *GsPrstG3NL(),*GsPrstG3LFG(),*GsPrstG3L(),*GsPrstNG3();
  PACKET *GsTMDdivG3NL(),*GsTMDdivG3LFG(),*GsTMDdivG3L(),*GsTMDdivNG3();
  PACKET *GsPrstTF3NL(),*GsPrstTF3LFG(),*GsPrstTF3L(),*GsPrstTNF3();
  PACKET *GsTMDdivTF3NL(),*GsTMDdivTF3LFG(),*GsTMDdivTF3L(),*GsTMDdivTNF3();
  PACKET *GsPrstTG3NL(),*GsPrstTG3LFG(),*GsPrstTG3L(),*GsPrstTNG3();
  PACKET *GsTMDdivTG3NL(),*GsTMDdivTG3LFG(),*GsTMDdivTG3L(),*GsTMDdivTNG3();
  PACKET *GsPrstF4NL(),*GsPrstF4LFG(),*GsPrstF4L(),*GsPrstNF4();
  PACKET *GsTMDdivF4NL(),*GsTMDdivF4LFG(),*GsTMDdivF4L(),*GsTMDdivNF4();
  PACKET *GsPrstG4NL(),*GsPrstG4LFG(),*GsPrstG4L(),*GsPrstNG4();
  PACKET *GsTMDdivG4NL(),*GsTMDdivG4LFG(),*GsTMDdivG4L(),*GsTMDdivNG4();
  PACKET *GsPrstTF4NL(),*GsPrstTF4LFG(),*GsPrstTF4L(),*GsPrstTNF4();
  PACKET *GsTMDdivTF4NL(),*GsTMDdivTF4LFG(),*GsTMDdivTF4L(),*GsTMDdivTNF4();
  PACKET *GsPrstTG4NL(),*GsPrstTG4LFG(),*GsPrstTG4L(),*GsPrstTNG4();
  PACKET *GsTMDdivTG4NL(),*GsTMDdivTG4LFG(),*GsTMDdivTG4L(),*GsTMDdivTNG4();

  /* flat triangle */
  GsFCALL5.f3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsPrstF3L;
  GsFCALL5.f3[GsDivMODE_NDIV][GsLMODE_FOG]    = GsPrstF3LFG;
  GsFCALL5.f3[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsPrstF3NL;
  GsFCALL5.f3[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsTMDdivF3L;
  GsFCALL5.f3[GsDivMODE_DIV][GsLMODE_FOG]     = GsTMDdivF3LFG;
  GsFCALL5.f3[GsDivMODE_DIV][GsLMODE_LOFF]    = GsTMDdivF3NL;
  GsFCALL5.nf3[GsDivMODE_NDIV]                = GsPrstNF3;
  GsFCALL5.nf3[GsDivMODE_DIV]                 = GsTMDdivNF3;
  /* gour triangle */
  GsFCALL5.g3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsPrstG3L;
  GsFCALL5.g3[GsDivMODE_NDIV][GsLMODE_FOG]    = GsPrstG3LFG;
  GsFCALL5.g3[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsPrstG3NL;
  GsFCALL5.g3[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsTMDdivG3L;
  GsFCALL5.g3[GsDivMODE_DIV][GsLMODE_FOG]     = GsTMDdivG3LFG;
  GsFCALL5.g3[GsDivMODE_DIV][GsLMODE_LOFF]    = GsTMDdivG3NL;
  GsFCALL5.ng3[GsDivMODE_NDIV]                = GsPrstNG3;
  GsFCALL5.ng3[GsDivMODE_DIV]                 = GsTMDdivNG3;
  /* texture flat triangle */
  GsFCALL5.tf3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsPrstTF3L;
  GsFCALL5.tf3[GsDivMODE_NDIV][GsLMODE_FOG]    = GsPrstTF3LFG;
  GsFCALL5.tf3[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsPrstTF3NL;
  GsFCALL5.tf3[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsTMDdivTF3L;
  GsFCALL5.tf3[GsDivMODE_DIV][GsLMODE_FOG]     = GsTMDdivTF3LFG;
  GsFCALL5.tf3[GsDivMODE_DIV][GsLMODE_LOFF]    = GsTMDdivTF3NL;
  GsFCALL5.ntf3[GsDivMODE_NDIV]                = GsPrstTNF3;
  GsFCALL5.ntf3[GsDivMODE_DIV]                 = GsTMDdivTNF3;
  /* texture gour triangle */
  GsFCALL5.tg3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsPrstTG3L;
  GsFCALL5.tg3[GsDivMODE_NDIV][GsLMODE_FOG]    = GsPrstTG3LFG;
  GsFCALL5.tg3[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsPrstTG3NL;
  GsFCALL5.tg3[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsTMDdivTG3L;
  GsFCALL5.tg3[GsDivMODE_DIV][GsLMODE_FOG]     = GsTMDdivTG3LFG;
  GsFCALL5.tg3[GsDivMODE_DIV][GsLMODE_LOFF]    = GsTMDdivTG3NL;
  GsFCALL5.ntg3[GsDivMODE_NDIV]                = GsPrstTNG3;
  GsFCALL5.ntg3[GsDivMODE_DIV]                 = GsTMDdivTNG3;
  /* flat quad */
  GsFCALL5.f4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsPrstF4L;
  GsFCALL5.f4[GsDivMODE_NDIV][GsLMODE_FOG]    = GsPrstF4LFG;
  GsFCALL5.f4[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsPrstF4NL;
  GsFCALL5.f4[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsTMDdivF4L;
  GsFCALL5.f4[GsDivMODE_DIV][GsLMODE_FOG]     = GsTMDdivF4LFG;
  GsFCALL5.f4[GsDivMODE_DIV][GsLMODE_LOFF]    = GsTMDdivF4NL;
  GsFCALL5.nf4[GsDivMODE_NDIV]                = GsPrstNF4;
  GsFCALL5.nf4[GsDivMODE_DIV]                 = GsTMDdivNF4;
  /* gour quad */
  GsFCALL5.g4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsPrstG4L;
  GsFCALL5.g4[GsDivMODE_NDIV][GsLMODE_FOG]    = GsPrstG4LFG;
  GsFCALL5.g4[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsPrstG4NL;
  GsFCALL5.g4[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsTMDdivG4L;
  GsFCALL5.g4[GsDivMODE_DIV][GsLMODE_FOG]     = GsTMDdivG4LFG;
  GsFCALL5.g4[GsDivMODE_DIV][GsLMODE_LOFF]    = GsTMDdivG4NL;
  GsFCALL5.ng4[GsDivMODE_NDIV]                = GsPrstNG4;
  GsFCALL5.ng4[GsDivMODE_DIV]                 = GsTMDdivNG4;
  /* texture flat quad */
  GsFCALL5.tf4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsPrstTF4L;
  GsFCALL5.tf4[GsDivMODE_NDIV][GsLMODE_FOG]    = GsPrstTF4LFG;
  GsFCALL5.tf4[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsPrstTF4NL;
  GsFCALL5.tf4[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsTMDdivTF4L;
  GsFCALL5.tf4[GsDivMODE_DIV][GsLMODE_FOG]     = GsTMDdivTF4LFG;
  GsFCALL5.tf4[GsDivMODE_DIV][GsLMODE_LOFF]    = GsTMDdivTF4NL;
  GsFCALL5.ntf4[GsDivMODE_NDIV]                = GsPrstTNF4;
  GsFCALL5.ntf4[GsDivMODE_DIV]                 = GsTMDdivTNF4;
  /* texture gour quad */
  GsFCALL5.tg4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsPrstTG4L;
  GsFCALL5.tg4[GsDivMODE_NDIV][GsLMODE_FOG]    = GsPrstTG4LFG;
  GsFCALL5.tg4[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsPrstTG4NL;
  GsFCALL5.tg4[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsTMDdivTG4L;
  GsFCALL5.tg4[GsDivMODE_DIV][GsLMODE_FOG]     = GsTMDdivTG4LFG;
  GsFCALL5.tg4[GsDivMODE_DIV][GsLMODE_LOFF]    = GsTMDdivTG4NL;
  GsFCALL5.ntg4[GsDivMODE_NDIV]                = GsPrstTNG4;
  GsFCALL5.ntg4[GsDivMODE_DIV]                 = GsTMDdivTNG4;
}
#endif

#if 0
extern _GsFCALL GsFCALL4;	/* GsSortObject4J Func Table */
				/* fook only useing functions */
jt_init4()			/* Gs SortObject5J Fook Func */
{
  PACKET *GsTMDfastF3NL(),*GsTMDfastF3LFG(),*GsTMDfastF3L(),*GsTMDfastNF3();
  PACKET *GsTMDdivF3NL(),*GsTMDdivF3LFG(),*GsTMDdivF3L(),*GsTMDdivNF3();
  PACKET *GsTMDfastG3NL(),*GsTMDfastG3LFG(),*GsTMDfastG3L(),*GsTMDfastNG3();
  PACKET *GsTMDdivG3NL(),*GsTMDdivG3LFG(),*GsTMDdivG3L(),*GsTMDdivNG3();
  PACKET *GsTMDfastTF3NL(),*GsTMDfastTF3LFG(),*GsTMDfastTF3L(),*GsTMDfastTNF3();
  PACKET *GsTMDdivTF3NL(),*GsTMDdivTF3LFG(),*GsTMDdivTF3L(),*GsTMDdivTNF3();
  PACKET *GsTMDfastTG3NL(),*GsTMDfastTG3LFG(),*GsTMDfastTG3L(),*GsTMDfastTNG3();
  PACKET *GsTMDdivTG3NL(),*GsTMDdivTG3LFG(),*GsTMDdivTG3L(),*GsTMDdivTNG3();
  PACKET *GsTMDfastF4NL(),*GsTMDfastF4LFG(),*GsTMDfastF4L(),*GsTMDfastNF4();
  PACKET *GsTMDdivF4NL(),*GsTMDdivF4LFG(),*GsTMDdivF4L(),*GsTMDdivNF4();
  PACKET *GsTMDfastG4NL(),*GsTMDfastG4LFG(),*GsTMDfastG4L(),*GsTMDfastNG4();
  PACKET *GsTMDdivG4NL(),*GsTMDdivG4LFG(),*GsTMDdivG4L(),*GsTMDdivNG4();
  PACKET *GsTMDfastTF4NL(),*GsTMDfastTF4LFG(),*GsTMDfastTF4L(),*GsTMDfastTNF4();
  PACKET *GsTMDdivTF4NL(),*GsTMDdivTF4LFG(),*GsTMDdivTF4L(),*GsTMDdivTNF4();
  PACKET *GsTMDfastTG4NL(),*GsTMDfastTG4LFG(),*GsTMDfastTG4L(),*GsTMDfastTNG4();
  PACKET *GsTMDdivTG4NL(),*GsTMDdivTG4LFG(),*GsTMDdivTG4L(),*GsTMDdivTNG4();
  
  /* flat triangle */
  GsFCALL4.f3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastF3L;
  GsFCALL4.f3[GsDivMODE_NDIV][GsLMODE_FOG]    = GsTMDfastF3LFG;
  GsFCALL4.f3[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsTMDfastF3NL;
  GsFCALL4.f3[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsTMDdivF3L;
  GsFCALL4.f3[GsDivMODE_DIV][GsLMODE_FOG]     = GsTMDdivF3LFG;
  GsFCALL4.f3[GsDivMODE_DIV][GsLMODE_LOFF]    = GsTMDdivF3NL;
  GsFCALL4.nf3[GsDivMODE_NDIV]                = GsTMDfastNF3;
  GsFCALL4.nf3[GsDivMODE_DIV]                 = GsTMDdivNF3;
  /* gour triangle */
  GsFCALL4.g3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastG3L;
  GsFCALL4.g3[GsDivMODE_NDIV][GsLMODE_FOG]    = GsTMDfastG3LFG;
  GsFCALL4.g3[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsTMDfastG3NL;
  GsFCALL4.g3[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsTMDdivG3L;
  GsFCALL4.g3[GsDivMODE_DIV][GsLMODE_FOG]     = GsTMDdivG3LFG;
  GsFCALL4.g3[GsDivMODE_DIV][GsLMODE_LOFF]    = GsTMDdivG3NL;
  GsFCALL4.ng3[GsDivMODE_NDIV]                = GsTMDfastNG3;
  GsFCALL4.ng3[GsDivMODE_DIV]                 = GsTMDdivNG3;
  /* texture flat triangle */
  GsFCALL4.tf3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTF3L;
  GsFCALL4.tf3[GsDivMODE_NDIV][GsLMODE_FOG]    = GsTMDfastTF3LFG;
  GsFCALL4.tf3[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsTMDfastTF3NL;
  GsFCALL4.tf3[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsTMDdivTF3L;
  GsFCALL4.tf3[GsDivMODE_DIV][GsLMODE_FOG]     = GsTMDdivTF3LFG;
  GsFCALL4.tf3[GsDivMODE_DIV][GsLMODE_LOFF]    = GsTMDdivTF3NL;
  GsFCALL4.ntf3[GsDivMODE_NDIV]                = GsTMDfastTNF3;
  GsFCALL4.ntf3[GsDivMODE_DIV]                 = GsTMDdivTNF3;
  /* texture gour triangle */
  GsFCALL4.tg3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTG3L;
  GsFCALL4.tg3[GsDivMODE_NDIV][GsLMODE_FOG]    = GsTMDfastTG3LFG;
  GsFCALL4.tg3[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsTMDfastTG3NL;
  GsFCALL4.tg3[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsTMDdivTG3L;
  GsFCALL4.tg3[GsDivMODE_DIV][GsLMODE_FOG]     = GsTMDdivTG3LFG;
  GsFCALL4.tg3[GsDivMODE_DIV][GsLMODE_LOFF]    = GsTMDdivTG3NL;
  GsFCALL4.ntg3[GsDivMODE_NDIV]                = GsTMDfastTNG3;
  GsFCALL4.ntg3[GsDivMODE_DIV]                 = GsTMDdivTNG3;
  /* flat quad */
  GsFCALL4.f4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastF4L;
  GsFCALL4.f4[GsDivMODE_NDIV][GsLMODE_FOG]    = GsTMDfastF4LFG;
  GsFCALL4.f4[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsTMDfastF4NL;
  GsFCALL4.f4[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsTMDdivF4L;
  GsFCALL4.f4[GsDivMODE_DIV][GsLMODE_FOG]     = GsTMDdivF4LFG;
  GsFCALL4.f4[GsDivMODE_DIV][GsLMODE_LOFF]    = GsTMDdivF4NL;
  GsFCALL4.nf4[GsDivMODE_NDIV]                = GsTMDfastNF4;
  GsFCALL4.nf4[GsDivMODE_DIV]                 = GsTMDdivNF4;
  /* gour quad */
  GsFCALL4.g4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastG4L;
  GsFCALL4.g4[GsDivMODE_NDIV][GsLMODE_FOG]    = GsTMDfastG4LFG;
  GsFCALL4.g4[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsTMDfastG4NL;
  GsFCALL4.g4[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsTMDdivG4L;
  GsFCALL4.g4[GsDivMODE_DIV][GsLMODE_FOG]     = GsTMDdivG4LFG;
  GsFCALL4.g4[GsDivMODE_DIV][GsLMODE_LOFF]    = GsTMDdivG4NL;
  GsFCALL4.ng4[GsDivMODE_NDIV]                = GsTMDfastNG4;
  GsFCALL4.ng4[GsDivMODE_DIV]                 = GsTMDdivNG4;
  /* texture flat quad */
  GsFCALL4.tf4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTF4L;
  GsFCALL4.tf4[GsDivMODE_NDIV][GsLMODE_FOG]    = GsTMDfastTF4LFG;
  GsFCALL4.tf4[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsTMDfastTF4NL;
  GsFCALL4.tf4[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsTMDdivTF4L;
  GsFCALL4.tf4[GsDivMODE_DIV][GsLMODE_FOG]     = GsTMDdivTF4LFG;
  GsFCALL4.tf4[GsDivMODE_DIV][GsLMODE_LOFF]    = GsTMDdivTF4NL;
  GsFCALL4.ntf4[GsDivMODE_NDIV]                = GsTMDfastTNF4;
  GsFCALL4.ntf4[GsDivMODE_DIV]                 = GsTMDdivTNF4;
  /* texture gour quad */
  GsFCALL4.tg4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTG4L;
  GsFCALL4.tg4[GsDivMODE_NDIV][GsLMODE_FOG]    = GsTMDfastTG4LFG;
  GsFCALL4.tg4[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsTMDfastTG4NL;
  GsFCALL4.tg4[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsTMDdivTG4L;
  GsFCALL4.tg4[GsDivMODE_DIV][GsLMODE_FOG]     = GsTMDdivTG4LFG;
  GsFCALL4.tg4[GsDivMODE_DIV][GsLMODE_LOFF]    = GsTMDdivTG4NL;
  GsFCALL4.ntg4[GsDivMODE_NDIV]                = GsTMDfastTNG4;
  GsFCALL4.ntg4[GsDivMODE_DIV]                 = GsTMDdivTNG4;
}
#endif

#if 0
extern _GsFCALL GsFCALL4;	/* GsSortObject4J Func Table */
jt_init4()			/* Gs SortObject4J Active sub divide Func */
{
  PACKET *GsTMDfastF3NL(),*GsTMDfastF3LFG(),*GsTMDfastF3L(),*GsTMDfastNF3();
  PACKET *GsA4divF3NL(),*GsA4divF3LFG(),*GsA4divF3L(),*GsA4divNF3();
  PACKET *GsTMDfastG3NL(),*GsTMDfastG3LFG(),*GsTMDfastG3L(),*GsTMDfastNG3();
  PACKET *GsA4divG3NL(),*GsA4divG3LFG(),*GsA4divG3L(),*GsA4divNG3();
  PACKET *GsTMDfastTF3NL(),*GsTMDfastTF3LFG(),*GsTMDfastTF3L(),*GsTMDfastTNF3();
  PACKET *GsA4divTF3NL(),*GsA4divTF3LFG(),*GsA4divTF3L(),*GsA4divTNF3();
  PACKET *GsTMDfastTG3NL(),*GsTMDfastTG3LFG(),*GsTMDfastTG3L(),*GsTMDfastTNG3();
  PACKET *GsA4divTG3NL(),*GsA4divTG3LFG(),*GsA4divTG3L(),*GsA4divTNG3();
  PACKET *GsTMDfastF4NL(),*GsTMDfastF4LFG(),*GsTMDfastF4L(),*GsTMDfastNF4();
  PACKET *GsA4divF4NL(),*GsA4divF4LFG(),*GsA4divF4L(),*GsA4divNF4();
  PACKET *GsTMDfastG4NL(),*GsTMDfastG4LFG(),*GsTMDfastG4L(),*GsTMDfastNG4();
  PACKET *GsA4divG4NL(),*GsA4divG4LFG(),*GsA4divG4L(),*GsA4divNG4();
  PACKET *GsTMDfastTF4NL(),*GsTMDfastTF4LFG(),*GsTMDfastTF4L(),*GsTMDfastTNF4();
  PACKET *GsA4divTF4NL(),*GsA4divTF4LFG(),*GsA4divTF4L(),*GsA4divTNF4();
  PACKET *GsTMDfastTG4NL(),*GsTMDfastTG4LFG(),*GsTMDfastTG4L(),*GsTMDfastTNG4();
  PACKET *GsA4divTG4NL(),*GsA4divTG4LFG(),*GsA4divTG4L(),*GsA4divTNG4();
  PACKET *GsA4divTF4L();
  
  /* flat triangle */
  GsFCALL4.f3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastF3L;
  GsFCALL4.f3[GsDivMODE_NDIV][GsLMODE_FOG]    = GsTMDfastF3LFG;
  GsFCALL4.f3[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsTMDfastF3NL;
  GsFCALL4.f3[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsA4divF3L;
  GsFCALL4.f3[GsDivMODE_DIV][GsLMODE_FOG]     = GsA4divF3LFG;
  GsFCALL4.f3[GsDivMODE_DIV][GsLMODE_LOFF]    = GsA4divF3NL;
  GsFCALL4.nf3[GsDivMODE_NDIV]                = GsTMDfastNF3;
  GsFCALL4.nf3[GsDivMODE_DIV]                 = GsA4divNF3;
  /* gour triangle */
  GsFCALL4.g3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastG3L;
  GsFCALL4.g3[GsDivMODE_NDIV][GsLMODE_FOG]    = GsTMDfastG3LFG;
  GsFCALL4.g3[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsTMDfastG3NL;
  GsFCALL4.g3[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsA4divG3L;
  GsFCALL4.g3[GsDivMODE_DIV][GsLMODE_FOG]     = GsA4divG3LFG;
  GsFCALL4.g3[GsDivMODE_DIV][GsLMODE_LOFF]    = GsA4divG3NL;
  GsFCALL4.ng3[GsDivMODE_NDIV]                = GsTMDfastNG3;
  GsFCALL4.ng3[GsDivMODE_DIV]                 = GsA4divNG3;
  /* texture flat triangle */
  GsFCALL4.tf3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTF3L;
  GsFCALL4.tf3[GsDivMODE_NDIV][GsLMODE_FOG]    = GsTMDfastTF3LFG;
  GsFCALL4.tf3[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsTMDfastTF3NL;
  GsFCALL4.tf3[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsA4divTF3L;
  GsFCALL4.tf3[GsDivMODE_DIV][GsLMODE_FOG]     = GsA4divTF3LFG;
  GsFCALL4.tf3[GsDivMODE_DIV][GsLMODE_LOFF]    = GsA4divTF3NL;
  GsFCALL4.ntf3[GsDivMODE_NDIV]                = GsTMDfastTNF3;
  GsFCALL4.ntf3[GsDivMODE_DIV]                 = GsA4divTNF3;
  /* texture gour triangle */
  GsFCALL4.tg3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTG3L;
  GsFCALL4.tg3[GsDivMODE_NDIV][GsLMODE_FOG]    = GsTMDfastTG3LFG;
  GsFCALL4.tg3[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsTMDfastTG3NL;
  GsFCALL4.tg3[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsA4divTG3L;
  GsFCALL4.tg3[GsDivMODE_DIV][GsLMODE_FOG]     = GsA4divTG3LFG;
  GsFCALL4.tg3[GsDivMODE_DIV][GsLMODE_LOFF]    = GsA4divTG3NL;
  GsFCALL4.ntg3[GsDivMODE_NDIV]                = GsTMDfastTNG3;
  GsFCALL4.ntg3[GsDivMODE_DIV]                 = GsA4divTNG3;
  /* flat quad */
  GsFCALL4.f4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastF4L;
  GsFCALL4.f4[GsDivMODE_NDIV][GsLMODE_FOG]    = GsTMDfastF4LFG;
  GsFCALL4.f4[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsTMDfastF4NL;
  GsFCALL4.f4[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsA4divF4L;
  GsFCALL4.f4[GsDivMODE_DIV][GsLMODE_FOG]     = GsA4divF4LFG;
  GsFCALL4.f4[GsDivMODE_DIV][GsLMODE_LOFF]    = GsA4divF4NL;
  GsFCALL4.nf4[GsDivMODE_NDIV]                = GsTMDfastNF4;
  GsFCALL4.nf4[GsDivMODE_DIV]                 = GsA4divNF4;
  /* gour quad */
  GsFCALL4.g4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastG4L;
  GsFCALL4.g4[GsDivMODE_NDIV][GsLMODE_FOG]    = GsTMDfastG4LFG;
  GsFCALL4.g4[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsTMDfastG4NL;
  GsFCALL4.g4[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsA4divG4L;
  GsFCALL4.g4[GsDivMODE_DIV][GsLMODE_FOG]     = GsA4divG4LFG;
  GsFCALL4.g4[GsDivMODE_DIV][GsLMODE_LOFF]    = GsA4divG4NL;
  GsFCALL4.ng4[GsDivMODE_NDIV]                = GsTMDfastNG4;
  GsFCALL4.ng4[GsDivMODE_DIV]                 = GsA4divNG4;
  /* texture flat quad */
  GsFCALL4.tf4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTF4L;
  GsFCALL4.tf4[GsDivMODE_NDIV][GsLMODE_FOG]    = GsTMDfastTF4LFG;
  GsFCALL4.tf4[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsTMDfastTF4NL;
  GsFCALL4.tf4[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsA4divTF4L;
  GsFCALL4.tf4[GsDivMODE_DIV][GsLMODE_FOG]     = GsA4divTF4LFG;
  GsFCALL4.tf4[GsDivMODE_DIV][GsLMODE_LOFF]    = GsA4divTF4NL;
  GsFCALL4.ntf4[GsDivMODE_NDIV]                = GsTMDfastTNF4;
  GsFCALL4.ntf4[GsDivMODE_DIV]                 = GsA4divTNF4;
  /* texture gour quad */
  GsFCALL4.tg4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTG4L;
  GsFCALL4.tg4[GsDivMODE_NDIV][GsLMODE_FOG]    = GsTMDfastTG4LFG;
  GsFCALL4.tg4[GsDivMODE_NDIV][GsLMODE_LOFF]   = GsTMDfastTG4NL;
  GsFCALL4.tg4[GsDivMODE_DIV][GsLMODE_NORMAL]  = GsA4divTG4L;
  GsFCALL4.tg4[GsDivMODE_DIV][GsLMODE_FOG]     = GsA4divTG4LFG;
  GsFCALL4.tg4[GsDivMODE_DIV][GsLMODE_LOFF]    = GsA4divTG4NL;
  GsFCALL4.ntg4[GsDivMODE_NDIV]                = GsTMDfastTNG4;
  GsFCALL4.ntg4[GsDivMODE_DIV]                 = GsA4divTNG4;
}
#endif

#endif /* _LIBGS_H_ */
