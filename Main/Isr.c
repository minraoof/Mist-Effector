//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
#include "Isr.h"
#include "GenericTypeDefs.h"
#include "Key.h"
#include "ADC.h"
#include "RTC.h"
#include "UI.h"
#include "USB_Module.h"



//*****************************************************************************
// MACROS
//*****************************************************************************



//*****************************************************************************
// VARIABLES
//*****************************************************************************


//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************


#pragma code isrh = 0x08
void isrh(void)
{
	_asm goto ISRH _endasm
}

#pragma code isrl = 0x18
void isrl(void)
{
	_asm goto ISRL _endasm
}		

#pragma code

/********************************************************************
 * Function:		ISRH()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		High Interrupt Service
 *
 * Note:			None
 *******************************************************************/
#pragma interrupt ISRH
void ISRH(void)
{

	if(INTCONbits.TMR0IF)
	{
		INTCONbits.TMR0IF = FALSE;
		Key_DebounceInterruptHandler();		
	}
	
	if(INTCON3bits.INT1IF)
	{
		INTCON3bits.INT1F = FALSE;	
		Key_ParseKey();	
	}

	if(PIR1bits.ADIF)
	{		
		ADC_InterruptHandler();
	}

	if(PIR3 & 0x01)
	{
		PIR3 &= 0xFE;
		RTCif_SetHalfSecondTimeout();
		
		if(RTCif_GetWDTFlag() == TRUE)
		{
			while(1)
			;		
		}	
	}

	if(INTCONbits.INT0IF)
	{		
		USBif_InterruptHandler();
	}
	
	if(PIR1bits.TMR2IF)
	{
		PIR1bits.TMR2IF = 0;
	}
	
	if(PIR1bits.CCP1IF)
	{		
		PIR1bits.CCP1IF = FALSE;
		UIIf_HandleFastBlinkLEDTimeout();
	}

	if(PIR2bits.CCP2IF)
	{
		PIR2bits.CCP2IF = FALSE;
		UI_HandleCCP2Interrupt();
	}
	
}
/********************************************************************
 * Function:		ISRL()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Low Interrupt Service
 *
 * Note:			None
 *******************************************************************/
#pragma interruptlow ISRL
void ISRL(void)
{

}		
















