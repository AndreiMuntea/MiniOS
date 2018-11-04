#ifndef __ASM_DEFINITIONS_H__
#define __ASM_DEFINITIONS_H__

extern void 
DebugBreak(void);

extern void 
Halt(void);

extern void 
TestTrapFrame(void);

extern void
IntLoadIdt(
    void*   IdtDescriptor
);

extern void 
IntCommonISR(void);

extern void 
IntCriticalISR(void);

extern void 
IntKeyboardISR(void);

extern void 
IntTimerISR(void);

extern void 
TimerSleep(unsigned long long Milliseconds);

#endif //__ASM_DEFINITIONS_H__