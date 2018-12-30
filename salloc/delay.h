#ifndef _DELAY_H
#define _DELAY_H

#include <delays.h>
#include "GenericTypeDefs.h"


#define GetSystemClock 6000000uL
#define GetInstructionClock (GetSystemClock/4)

void Delay10Us(WORD us);
void DelayMs (WORD ms);

#endif