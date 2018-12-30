//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
#define USB_IMPORTED
#include "USB_Module.h"
#include "UI.h"
#include "RTC.h"
#include "Power.h"
#include "ADC.h"
#include "Delay.h"


//*****************************************************************************
// MACROS
//*****************************************************************************


//*****************************************************************************
// VARIABLES
//*****************************************************************************



//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************
/********************************************************************
 * Function:		USB_InitializeModule()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the USB module
 *                  
 * Note:			None
 *******************************************************************/
void USB_InitializeModule(void)
{
	USB_SENSE_PORT = INPUT;
	USB_SENSE_INTERRUPT_FLAT = FALSE;
	USB_SENSE_INTERRUPT_ENABLE;		
		
	g_USB_State.yUSB = USB_SENSE;


	if(g_USB_State.yUSB == TRUE)
	{

		RTCif_SetSprayOffTime(0);
		UI_StopBlinkLED(UI_LED_RED);
		UI_LED_RED_ON;
		USB_SENSE_FALLING_EDGE;
		
		//Powerif_SetChargingSprayTurnonStatus(TRUE);
	}
		
	RCONbits.IPEN = TRUE;
}
/********************************************************************
 * Function:		USB_EventHandler()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the USB module
 *                  
 * Note:			None
 *******************************************************************/
 void USB_EventHandler(void)
{
	USBif_SetUSBState();

	if(g_USB_State.yUSB == FALSE)
			
		return;	

}
/********************************************************************
 * Function:		USBif_SetUSBState()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the USB module
 *                  
 * Note:			None
*********************************************************************/
 void USBif_SetUSBState(void)
{
	if(g_USB_State.yUSB  != USB_SENSE)
	{

	}
		g_USB_State.yUSB = USB_SENSE;		
}
/********************************************************************
 * Function:		USBif_GetUSBState()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		Return the USB State
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the USB module
 *                  
 * Note:			None
 *******************************************************************/
BYTE USBif_GetUSBState(void)
{
	return g_USB_State.yUSB;
}

/********************************************************************
 * Function:		USBif_InterruptHandler()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		Return the USB State
 *
 * Side Effects:	None
 *
 * Overview:		when USB interrupt come, call this function
 *                  
 * Note:			None
 *******************************************************************/
 void USBif_InterruptHandler(void)
{
	USB_SENSE_INTERRUPT_FLAT = FALSE;
	INTCON2bits.INTEDG0 = 1 - INTCON2bits.INTEDG0;

	if(UIif_GetModuleState()  == UI_STATE_FACTORY_TESTING  ||
	    g_UI_Status.yBatteryTempFault == TRUE)
	{
		return;
	}

	if(USB_SENSE == TRUE)
	{
		if(RTCif_GetChargingLEDOffTime() != 0 ||
		Powerif_IsBatteryFullCharged() != FALSE)
		{
			RTCif_SetChargingLEDOffTime(0);

			Powerif_SetBatteryFullChargedStatus(FALSE);						
		}

		if(RTCif_GetSprayOffTime() != 0)
		{
			RTCif_SetSprayOffTime(0);
			UI_StopBlinkLED(UI_LED_RED);
		}

		
		UIif_StopPWMPIZEO1();
		UIif_StopPWMPIZEO2();
		
		UI_LED_RED_ON;			
		UI_LED_BLUE_OFF;						
							
		UI_StopBlinkLED(UI_LED_GREEN);
		UIif_SetModuleState(UI_STATE_IDLE);

	}
	else
	{
		
		UI_StopBlinkLED(UI_LED_RED);

		Powerif_ClearFullChargeFlag();

		UIif_SetPowerFullStatus(FALSE);
	}
}

