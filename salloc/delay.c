
#include "delay.h"

void Delay10Us(WORD us)
{
	Delay10TCYx(((GetSystemClock/4000000)*(us)));
}	
void DelayMs (WORD ms)
{
	BYTE _iTemp = ms;
	while(_iTemp--)
		Delay1KTCYx((GetInstructionClock+999999)/1000000);
}	

