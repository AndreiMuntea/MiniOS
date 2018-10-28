#ifndef __ASM_DEFINITIONS_H__
#define __ASM_DEFINITIONS_H__

extern void 
DebugBreak(void);

extern void
IntLoadIdt(
    void*   IdtDescriptor
);

extern void 
IntCommonISR(void);

#endif //__ASM_DEFINITIONS_H__