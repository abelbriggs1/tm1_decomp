/*
 * File:convert.h
 */
/*
 * $PSLibId: Runtime Library Version 3.3$
 */
#ifndef _CONVERT_H
#define _CONVERT_H

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif
extern unsigned int todigit(char);
extern double atof(char *);	/**/
extern int atoi(char *);
extern long atol(char *);
extern double strtod(char *, char **); /**/
extern long strtol(char *,char**, int);	/**/
extern unsigned long strtoul(char *, char **, int);
extern long labs(long);
extern char *itoa(int);

/*
extern int abs();
extern div_t div();
extern ldiv_t ldiv();
*/
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
