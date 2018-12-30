#ifndef RTC_H
#define RTC_H


//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
#include "GenericTypeDefs.h"
#include "Main.h"

//*****************************************************************************
// MACROS
//*****************************************************************************

#ifdef	RTC_IMPORTED
#define RTC_TYPE
#else
#define RTC_TYPE extern
#endif

#define RTC_ALARM_INTERRUPT_FLAG 		PIR3bits.RTCCIF
//#define RTC_ALARM_INTERRUPT_ENABLE		PIE3bits.RTCCIP
//#define RTC_ALARM_INTERRUPT_PRIORITY	IPR3bits.RTCCIE	


//*****************************************************************************
// VARIABLES

typedef struct
{
	BYTE	yHalfSecond;
	
	BYTE	yRedLED;
	
	BYTE	yGreenLED;

	BYTE	yBlueLED;
	
	WORD	wBatterySample;

	BYTE	yChargingLEDOff;
	
	BYTE	yKeyLongPressTime;

	WORD    yBatteryTemperatureCheckTime;
	
	BYTE	yLowBatteryLEDFlashingTime;
	
	BYTE	yKeyPressTimeforFacotyTestMode;
	
	BYTE	yWDT;

 	BYTE	ySprayOffTime;

	WORD	wFullChargingTime;

	BYTE 	yPizeoPWMTurnoffTime;
	
}RTC_SYSTEM_TIMER;

typedef union
{
	struct
	{
		BYTE	yMinuateFlag:1;
		
		BYTE	yRedLEDFlag:1;
		
		BYTE	yGreenLEDFlag:1;

		BYTE	yBlueLEDFlag:1;
		
		BYTE	yBatterySampleFlag:1;

		BYTE	yChargingLEDOffFlag:1;
		
		BYTE	yKeyLongPressFlag:1;
	
		BYTE	yBatteryTemperatureCheckFlag:1;
		
		BYTE 	yLowBatteryLEDFlashingFlag:1;
		
		BYTE    yKeyPressTimeforFacotyTestModeFlag:1;
		
		BYTE	yWDTFlag:1;

		BYTE	ySprayOffTimeFlag:1;

		BYTE	yFullChargingFlag:1;

		BYTE 	yPizeoPWMTurnoffTimeFlag:1;
		
		//Reserved 3		
	};
	WORD wFlags;
	
}RTC_TIMER_FLAG;

RTC_TYPE RTC_SYSTEM_TIMER	g_RTC_SystemTimer;
RTC_TYPE RTC_TIMER_FLAG		g_RTC_TimerFlag;
RTC_TYPE DWORD				g_RTC_SystemUptime;
RTC_TYPE BYTE				g_RTC_HalfSecond;

//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************
/********************************************************************
 * Function:		ADC_Initialize()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the ADC module. It will setup the analog to digital
 *				convert module for battery and temperature sensor.
 *				  
 * Note:			None
 *******************************************************************/
void RTC_Initialize(void);

/********************************************************************
 * Function:		RTC_TimerHandler()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		The timer will time out every 1/2 second. Decrease the value
 *				in the RTC timer. When the value is to 0, it will set the flag
 *				and check the flag after exiting the interrupt.
 *
 * Note:			None
 *******************************************************************/
void RTC_TimerHandler(void);

/********************************************************************
 * Function:		RTC_TimeOutHandler()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Handle the time out flag.
 *
 * Note:			None
 *******************************************************************/
void RTC_TimeOutHandler(void);

/********************************************************************
 * Function:		RTCif_SetHalfSecondTimeout()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the flag to indicate that the half second timeout. It will
 *				check the timer in the normal process loop.
 *
 * Note:			None
 *******************************************************************/
void RTCif_SetHalfSecondTimeout(void);

/********************************************************************
 * Function:		RTCif_SetLEDTimer()
 *
 * PreCondition:	None
 *
 * Input:			WORD wLED: The LED
 *				BYTE wTime: The timer value
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the RTC timer for LED highlighting.
 *
 * Note:			None
 *******************************************************************/
void RTCif_SetLEDTimer(
	WORD wLED,
	BYTE yTime
);

/********************************************************************
 * Function:		RTCif_SetBatterySampleTime()
 *
 * PreCondition:	None
 *
 * Input:			WORD wTime: The sample waiting time
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the battery sample timer for battery.
 *
 * Note:			None
 *******************************************************************/
void RTCif_SetBatterySampleTime(WORD wTime);

/********************************************************************
 * Function:		RTCif_SetBatterySampleTime()
 *
 * PreCondition:	None
 *
 * Input:			BYTE wTime: The sample waiting time
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the battery sample timer for battery.
 *
 * Note:			None
 *******************************************************************/
void RTCif_SetChargingLEDOffTime(BYTE yTime);

/********************************************************************
 * Function:		RTCif_GetBatterySampleTime()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			Charging LED off time
 *
 * Side Effects:	None
 *
 * Overview:		return the Charging led off time
 *
 * Note:			None
 *******************************************************************/
BYTE RTCif_GetChargingLEDOffTime(void);

/********************************************************************
 * Function:		RTCif_SetKeyLongPressTime()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yTime: The long key press time
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the long key press time.
 *
 * Note:			None
 *******************************************************************/
 void RTCif_SetKeyLongPressTime(BYTE yTime);
 
 /********************************************************************
 * Function:		RTCif_SetBatteryTemperatureCheckTime()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yTime: The time for Charign IC Status Pin
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		If Charging Have Temperature falut or something 
 *					need to blink the Green LED
 *
 * Note:			None
 *******************************************************************/
 void RTCif_SetBatteryTemperatureCheckTime(WORD wTime);
 
  /********************************************************************
 * Function:		RTCif_SetyLowBatterLEDFlashingTime()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yTime: The time for Charign IC Status Pin
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		If Charging Have Temperature falut or something 
 *					need to blink the Green LED
 *
 * Note:			None
 *******************************************************************/
 void RTCif_SetLowBatteryLEDFlashingTime(BYTE yTime);
 
  /********************************************************************
 * Function:		RTCif_SetyLowBatterLEDFlashingTime()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yTime: The time for Charign IC Status Pin
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		If Charging Have Temperature falut or something 
 *					need to blink the Green LED
 *
 * Note:			None
 *******************************************************************/
void RTCif_SetKeyPressTimeforFacotyTestMode(BYTE yTime);
/********************************************************************
 * Function:		RTCif_GetWDTFlag()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			BYTE: The WDT time out flag
 *
 * Side Effects:	None
 *
 * Overview:		Get the WDT timeout flag. If true the device will reboot.
 *
 * Note:			None
 *******************************************************************/
BYTE RTCif_GetWDTFlag(void);
/********************************************************************
 * Function:		RTCif_ClearWDT()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Reset the software WDT timer. The timer here is 20 seconds
 *
 * Note:			None
 *******************************************************************/
void RTCif_ClearWDT(void);
/********************************************************************
 * Function:		RTCif_SetSprayOffTime()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			Time Value
 *
 * Side Effects:	None
 *
 * Overview:		Set the Value for the time to turn off the spray
 *
 * Note:			None
 *******************************************************************/
void RTCif_SetSprayOffTime(BYTE yTime);
/********************************************************************
 * Function:		RTCif_SetSprayOffTime()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			Time Value
 *
 * Side Effects:	None
 *
 * Overview:		Set the Value for the time to turn off the spray
 *
 * Note:			None
 *******************************************************************/
BYTE RTCif_GetSprayOffTime(void);
/********************************************************************
 * Function:		RTCif_SetFullChargingTime()
 *
 * PreCondition:	None
 *
 * Input:			WORD wValue: The time value
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the timer to check full charged when voltage is higher
 *				than 4.1V
 *
 * Note:			None
 *******************************************************************/
void RTCif_SetFullChargingTime(WORD wValue);


/********************************************************************
 * Function:		RTCif_GetFullChargingTime()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		WORD: The time value
 *
 * Side Effects:	None
 *
 * Overview:		Get the timer value for checking full charged when voltage is
 *				higher than 4.1V
 *
 * Note:			None
 *******************************************************************/
WORD RTCif_GetFullChargingTime(void);
BYTE RTCif_GetPizeoPWMTurnOffTime(void);
void RTCif_SetPizeoPWMTurnOffTime(BYTE yTime);
#endif





