#ifndef __OS_ARCH_H__
#define __OS_ARCH_H__

void ArchPrint(void);
void ArchInit(void);

extern void EnableInterrupts(void);
extern void DisableInterrupts(void);

#endif
