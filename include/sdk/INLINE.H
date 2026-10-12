/*
 * $PSLibId: Runtime Library Version 3.3$
 */
#define gte_ldv0(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000007f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000007e":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldv1(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000000bf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000000be":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldv2(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000000ff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000000fe":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldv3(r1,r2,r3) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $14,%0"::"r"(r3):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000013f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000013e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000013e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000013e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000013e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000013e":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldv3c(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000017f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000017e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000017e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000017e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000017e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000017e":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldv01(r1,r2) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000001bf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000001be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000001be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000001be":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldv01c(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000001ff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000001fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000001fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000001fe":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldrgb(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000023f":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldrgb3(r1,r2,r3) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $14,%0"::"r"(r3):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000027f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000027e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000027e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000027e":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldlv0(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000002bf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000002be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000002be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000002be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000002be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000002be":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldlvl(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000002ff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000002fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000002fe":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldsv(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000033f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000033e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000033e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000033e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000033e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000033e":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldbv(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000037f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000037e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000037e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000037e":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldcv(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000003bf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000003be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000003be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000003be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000003be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000003be":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldclmv(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000003ff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000003fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000003fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000003fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000003fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000003fe":::"$12","$13","$14","$15","memory"); \
}
#define gte_lddp(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000043f":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldsxy3(r1,r2,r3) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $14,%0"::"r"(r3):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000047f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000047e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000047e":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldsz3(r1,r2,r3) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $14,%0"::"r"(r3):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000004bf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000004be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000004be":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldsz4(r1,r2,r3,r4) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $14,%0"::"r"(r3):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $15,%0"::"r"(r4):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000004ff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000004fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000004fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000004fe":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldopv1(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000053f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000053e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000053e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000053e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000053e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000053e":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldopv2(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000057f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000057e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000057e":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldlzc(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000005bf":::"$12","$13","$14","$15","memory"); \
}
#define gte_SetRGBcd(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000005ff":::"$12","$13","$14","$15","memory"); \
}
#define gte_SetBackColor(r1,r2,r3) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $14,%0"::"r"(r3):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000063f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000063e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000063e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000063e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000063e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000063e":::"$12","$13","$14","$15","memory"); \
}
#define gte_SetFarColor(r1,r2,r3) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $14,%0"::"r"(r3):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000067f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000067e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000067e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000067e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000067e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000067e":::"$12","$13","$14","$15","memory"); \
}
#define gte_SetGeomOffset(r1,r2) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000006bf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000006be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000006be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000006be":::"$12","$13","$14","$15","memory"); \
}
#define gte_SetGeomScreen(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000006ff":::"$12","$13","$14","$15","memory"); \
}
#define gte_SetRotMatrix(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000073f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000073e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000073e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000073e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000073e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000073e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000073e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000073e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000073e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000073e":::"$12","$13","$14","$15","memory"); \
}
#define gte_SetLightMatrix(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000077f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000077e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000077e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000077e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000077e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000077e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000077e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000077e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000077e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000077e":::"$12","$13","$14","$15","memory"); \
}
#define gte_SetColorMatrix(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000007bf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000007be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000007be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000007be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000007be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000007be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000007be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000007be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000007be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000007be":::"$12","$13","$14","$15","memory"); \
}
#define gte_SetTransMatrix(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000007ff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000007fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000007fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000007fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000007fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000007fe":::"$12","$13","$14","$15","memory"); \
}
#define gte_SetTransVector(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000083f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000083e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000083e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000083e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000083e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000083e":::"$12","$13","$14","$15","memory"); \
}
#define gte_ld_intpol_uv0(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000087f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000087e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000087e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000087e":::"$12","$13","$14","$15","memory"); \
}
#define gte_ld_intpol_uv1(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000008bf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000008be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000008be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000008be":::"$12","$13","$14","$15","memory"); \
}
#define gte_ld_intpol_bv0(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000008ff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000008fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000008fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000008fe":::"$12","$13","$14","$15","memory"); \
}
#define gte_ld_intpol_bv1(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000093f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000093e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000093e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000093e":::"$12","$13","$14","$15","memory"); \
}
#define gte_ld_intpol_sv0(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000097f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000097e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000097e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000097e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000097e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000097e":::"$12","$13","$14","$15","memory"); \
}
#define gte_ld_intpol_sv1(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000009bf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000009be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000009be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000009be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000009be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000009be":::"$12","$13","$14","$15","memory"); \
}
#define gte_ldfc(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000009ff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000009fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000009fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000009fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000009fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000009fe":::"$12","$13","$14","$15","memory"); \
}
#define gte_rtps() {\
  __asm__ volatile (".word 0x00000a3f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000a3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000a3e":::"$12","$13","$14","$15","memory"); \
}
#define gte_rtpt() {\
  __asm__ volatile (".word 0x00000a7f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000a7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000a7e":::"$12","$13","$14","$15","memory"); \
}
#define gte_rt() {\
  __asm__ volatile (".word 0x00000abf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000abe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000abe":::"$12","$13","$14","$15","memory"); \
}
#define gte_rtv0() {\
  __asm__ volatile (".word 0x00000aff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000afe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000afe":::"$12","$13","$14","$15","memory"); \
}
#define gte_rtv1() {\
  __asm__ volatile (".word 0x00000b3f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000b3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000b3e":::"$12","$13","$14","$15","memory"); \
}
#define gte_rtv2() {\
  __asm__ volatile (".word 0x00000b7f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000b7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000b7e":::"$12","$13","$14","$15","memory"); \
}
#define gte_rtir() {\
  __asm__ volatile (".word 0x00000bbf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000bbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000bbe":::"$12","$13","$14","$15","memory"); \
}
#define gte_rtir_sf0() {\
  __asm__ volatile (".word 0x00000bff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000bfe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000bfe":::"$12","$13","$14","$15","memory"); \
}
#define gte_ll() {\
  __asm__ volatile (".word 0x00000c3f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000c3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000c3e":::"$12","$13","$14","$15","memory"); \
}
#define gte_lc() {\
  __asm__ volatile (".word 0x00000c7f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000c7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000c7e":::"$12","$13","$14","$15","memory"); \
}
#define gte_dpcl() {\
  __asm__ volatile (".word 0x00000cbf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000cbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000cbe":::"$12","$13","$14","$15","memory"); \
}
#define gte_dpcs() {\
  __asm__ volatile (".word 0x00000cff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000cfe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000cfe":::"$12","$13","$14","$15","memory"); \
}
#define gte_dpct() {\
  __asm__ volatile (".word 0x00000d3f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000d3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000d3e":::"$12","$13","$14","$15","memory"); \
}
#define gte_intpl() {\
  __asm__ volatile (".word 0x00000d7f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000d7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000d7e":::"$12","$13","$14","$15","memory"); \
}
#define gte_sqr12() {\
  __asm__ volatile (".word 0x00000dbf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000dbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000dbe":::"$12","$13","$14","$15","memory"); \
}
#define gte_sqr0() {\
  __asm__ volatile (".word 0x00000dff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000dfe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000dfe":::"$12","$13","$14","$15","memory"); \
}
#define gte_ncs() {\
  __asm__ volatile (".word 0x00000e3f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000e3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000e3e":::"$12","$13","$14","$15","memory"); \
}
#define gte_nct() {\
  __asm__ volatile (".word 0x00000e7f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000e7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000e7e":::"$12","$13","$14","$15","memory"); \
}
#define gte_ncds() {\
  __asm__ volatile (".word 0x00000ebf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000ebe":::"$12","$13","$14","$15","memory"); \
}
#define gte_ncdt() {\
  __asm__ volatile (".word 0x00000eff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000efe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000efe":::"$12","$13","$14","$15","memory"); \
}
#define gte_nccs() {\
  __asm__ volatile (".word 0x00000f3f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000f3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000f3e":::"$12","$13","$14","$15","memory"); \
}
#define gte_ncct() {\
  __asm__ volatile (".word 0x00000f7f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000f7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000f7e":::"$12","$13","$14","$15","memory"); \
}
#define gte_cdp() {\
  __asm__ volatile (".word 0x00000fbf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000fbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000fbe":::"$12","$13","$14","$15","memory"); \
}
#define gte_cc() {\
  __asm__ volatile (".word 0x00000fff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000ffe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00000ffe":::"$12","$13","$14","$15","memory"); \
}
#define gte_nclip() {\
  __asm__ volatile (".word 0x0000103f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000103e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000103e":::"$12","$13","$14","$15","memory"); \
}
#define gte_avsz3() {\
  __asm__ volatile (".word 0x0000107f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000107e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000107e":::"$12","$13","$14","$15","memory"); \
}
#define gte_avsz4() {\
  __asm__ volatile (".word 0x000010bf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000010be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000010be":::"$12","$13","$14","$15","memory"); \
}
#define gte_op12() {\
  __asm__ volatile (".word 0x000010ff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000010fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000010fe":::"$12","$13","$14","$15","memory"); \
}
#define gte_op0() {\
  __asm__ volatile (".word 0x0000113f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000113e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000113e":::"$12","$13","$14","$15","memory"); \
}
#define gte_gpf12() {\
  __asm__ volatile (".word 0x0000117f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000117e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000117e":::"$12","$13","$14","$15","memory"); \
}
#define gte_gpf0() {\
  __asm__ volatile (".word 0x000011bf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000011be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000011be":::"$12","$13","$14","$15","memory"); \
}
#define gte_gpl12() {\
  __asm__ volatile (".word 0x000011ff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000011fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000011fe":::"$12","$13","$14","$15","memory"); \
}
#define gte_gpl0() {\
  __asm__ volatile (".word 0x0000123f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000123e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000123e":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsxy(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000127f":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsxy3(r1,r2,r3) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $14,%0"::"r"(r3):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000012bf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000012be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000012be":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsxy3c(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000012ff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000012fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000012fe":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsxy2(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000133f":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsxy1(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000137f":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsxy0(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000013bf":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsxy01(r1,r2) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000013ff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000013fe":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsxy01c(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000143f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000143e":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsxy3_f3(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000147f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000147e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000147e":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsxy3_g3(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000014bf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000014be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000014be":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsxy3_ft3(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000014ff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000014fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000014fe":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsxy3_gt3(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000153f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000153e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000153e":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsxy3_f4(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000157f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000157e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000157e":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsxy3_g4(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000015bf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000015be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000015be":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsxy3_ft4(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000015ff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000015fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000015fe":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsxy3_gt4(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000163f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000163e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000163e":::"$12","$13","$14","$15","memory"); \
}
#define gte_stdp(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000167f":::"$12","$13","$14","$15","memory"); \
}
#define gte_stflg(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000016bf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000016be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000016be":::"$12","$13","$14","$15","memory"); \
}
#define gte_stflg_4(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000016ff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000016fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000016fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000016fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000016fe":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsz(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000173f":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsz3(r1,r2,r3) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $14,%0"::"r"(r3):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000177f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000177e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000177e":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsz4(r1,r2,r3,r4) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $14,%0"::"r"(r3):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $15,%0"::"r"(r4):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000017bf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000017be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000017be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000017be":::"$12","$13","$14","$15","memory"); \
}
#define gte_stszotz(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000017ff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000017fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000017fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000017fe":::"$12","$13","$14","$15","memory"); \
}
#define gte_stotz(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000183f":::"$12","$13","$14","$15","memory"); \
}
#define gte_stopz(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000187f":::"$12","$13","$14","$15","memory"); \
}
#define gte_stlvl(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000018bf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000018be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000018be":::"$12","$13","$14","$15","memory"); \
}
#define gte_stlvnl(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000018ff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000018fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000018fe":::"$12","$13","$14","$15","memory"); \
}
#define gte_stsv(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000193f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000193e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000193e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000193e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000193e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000193e":::"$12","$13","$14","$15","memory"); \
}
#define gte_stclmv(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000197f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000197e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000197e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000197e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000197e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x0000197e":::"$12","$13","$14","$15","memory"); \
}
#define gte_stbv(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000019bf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000019be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000019be":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000019be":::"$12","$13","$14","$15","memory"); \
}
#define gte_stcv(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000019ff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000019fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000019fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000019fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000019fe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x000019fe":::"$12","$13","$14","$15","memory"); \
}
#define gte_strgb(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001a3f":::"$12","$13","$14","$15","memory"); \
}
#define gte_strgb3(r1,r2,r3) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $14,%0"::"r"(r3):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001a7f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001a7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001a7e":::"$12","$13","$14","$15","memory"); \
}
#define gte_strgb3_g3(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001abf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001abe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001abe":::"$12","$13","$14","$15","memory"); \
}
#define gte_strgb3_gt3(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001aff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001afe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001afe":::"$12","$13","$14","$15","memory"); \
}
#define gte_strgb3_g4(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001b3f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001b3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001b3e":::"$12","$13","$14","$15","memory"); \
}
#define gte_strgb3_gt4(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001b7f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001b7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001b7e":::"$12","$13","$14","$15","memory"); \
}
#define gte_ReadGeomOffset(r1,r2) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001bbf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001bbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001bbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001bbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001bbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001bbe":::"$12","$13","$14","$15","memory"); \
}
#define gte_ReadGeomScreen(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001bff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001bfe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001bfe":::"$12","$13","$14","$15","memory"); \
}
#define gte_ReadRotMatrix(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c3f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c3e":::"$12","$13","$14","$15","memory"); \
}
#define gte_ReadLightMatrix(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c7f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001c7e":::"$12","$13","$14","$15","memory"); \
}
#define gte_ReadColorMatrix(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001cbf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001cbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001cbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001cbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001cbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001cbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001cbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001cbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001cbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001cbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001cbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001cbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001cbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001cbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001cbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001cbe":::"$12","$13","$14","$15","memory"); \
}
#define gte_stlzc(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001cff":::"$12","$13","$14","$15","memory"); \
}
#define gte_stfc(r1) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001d3f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001d3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001d3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001d3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001d3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001d3e":::"$12","$13","$14","$15","memory"); \
}
#define gte_nop() {\
  __asm__ volatile (".word 0x00001d7f":::"$12","$13","$14","$15","memory"); \
}
#define gte_subdvl(r1,r2,r3) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $14,%0"::"r"(r3):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dbf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dbe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dbe":::"$12","$13","$14","$15","memory"); \
}
#define gte_subdvd(r1,r2,r3) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $14,%0"::"r"(r3):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dfe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dfe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dfe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dfe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dfe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dfe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dfe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dfe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dfe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dfe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001dfe":::"$12","$13","$14","$15","memory"); \
}
#define gte_adddvl(r1,r2,r3) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $14,%0"::"r"(r3):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e3f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e3e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e3e":::"$12","$13","$14","$15","memory"); \
}
#define gte_adddvd(r1,r2,r3) {\
  __asm__ volatile ("move  $12,%0"::"r"(r1):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $13,%0"::"r"(r2):"$12","$13","$14","$15","memory"); \
  __asm__ volatile ("move  $14,%0"::"r"(r3):"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e7f":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e7e":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001e7e":::"$12","$13","$14","$15","memory"); \
}
#define gte_FlipRotMatrixX() {\
  __asm__ volatile (".word 0x00001ebf":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001ebe":::"$12","$13","$14","$15","memory"); \
}
#define gte_FlipTRX() {\
  __asm__ volatile (".word 0x00001eff":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001efe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001efe":::"$12","$13","$14","$15","memory"); \
  __asm__ volatile (".word 0x00001efe":::"$12","$13","$14","$15","memory"); \
}
