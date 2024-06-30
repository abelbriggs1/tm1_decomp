
extern int SetRCnt(unsigned int, unsigned short, int);
extern int GetRCnt(unsigned int);
extern int StartRCnt(unsigned int);
extern int StopRCnt(unsigned int);

extern int OpenEvent(unsigned int class, int spec, int mode, void (*func)());
extern int EnableEvent(unsigned int event);
extern int DisableEvent(unsigned int event);
extern int CloseEvent(unsigned int event);
