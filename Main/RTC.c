//*****************************************************************************
// INCLUDES
//*****************************************************************************
#define RTC_IMPORTED
#include "RTC.h"
#include "UI.h"
#include "ADC.h"
#include "Key.h"
#include "Power.h"
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
/********************************************************************
 * Function:		RTC_Initialize()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the RTC. Now the 32k OSC haven't put in so use timer
 *				to implement.
 *
 * Note:			None
 *******************************************************************/
void RTC_Initialize(void)
{	

	
	INTCONbits.GIE = 0;
	T1CONbits.T1OSCEN = TRUE;	
	
	EECON2 = 0x55;
	EECON2 = 0xAA;
	RTCCFGbits.RTCWREN = TRUE;	//Write Enable	

	//Off the RTC
	RTCCFGbits.RTCEN  = FALSE;
	while(RTCCFGbits.RTCSYNC)
			;	

	//Enable the alarm, time out in every half second
	ALRMCFGbits.CHIME = TRUE;
	ALRMCFGbits.ALRMEN = TRUE;	
	
//Set the interrupt
	PIE3 |= 0x01;		//RTC Interrupt Enable
	PIR3 |= 0x01;		//RTC Hight Priority
	RTC_ALARM_INTERRUPT_FLAG 		= FALSE;
	
	RTCCFGbits.RTCEN = TRUE;
//	while(RTCCFGbits.RTCSYNC != TRUE)
//			;


	EECON2 = 0x55;
	EECON2 = 0xAA;
	RTCCFGbits.RTCWREN = FALSE;

	
	g_RTC_SystemTimer.yHalfSecond      			   = 0;
	g_RTC_SystemTimer.yGreenLED       			   = 0;
	g_RTC_SystemTimer.yRedLED     				   = 0;
	g_RTC_SystemTimer.yBlueLED      			   = 0;
//	g_RTC_SystemTimer.yBatterySample    		   = 0;
	g_RTC_SystemTimer.yChargingLEDOff   		   = 0;
	g_RTC_SystemTimer.yKeyLongPressTime 		   = 0;
//	g_RTC_SystemTimer.yBatteryTemperatureCheckTime = 0;
	g_RTC_SystemTimer.yLowBatteryLEDFlashingTime   = 0;
//	g_RTC_SystemTimer.yKeyPressTimeforFacotyTestMode = 0;
	g_RTC_SystemTimer.ySprayOffTime = 0;
	g_RTC_SystemTimer.wFullChargingTime = 0;
	g_RTC_SystemTimer.yPizeoPWMTurnoffTime = 0;

	g_RTC_TimerFlag.wFlags = 0;
	g_RTC_SystemUptime = 0;
	 
	INTCONbits.GIE = 1;
}	

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
void RTC_TimerHandler(void)
{
	if(RTC_ALARM_INTERRUPT_FLAG == TRUE)
	{
		RTC_ALARM_INTERRUPT_FLAG = FALSE;
		g_RTC_SystemTimer.yHalfSecond++;
	}
		
	if(g_RTC_SystemTimer.yHalfSecond == 0)
	{
		return;
	}

	g_RTC_SystemTimer.yHalfSecond--;

	g_RTC_HalfSecond++;

	if(g_RTC_HalfSecond % 2 == 0)
	{
		g_RTC_SystemUptime++;
		g_RTC_HalfSecond = 0;
	}		

	if(g_RTC_SystemTimer.yGreenLED > 0)
	{
		g_RTC_SystemTimer.yGreenLED--;

		if(g_RTC_SystemTimer.yGreenLED ==0)
		{
			g_RTC_TimerFlag.yGreenLEDFlag = TRUE;
		}
	}
	
	if(g_RTC_SystemTimer.yRedLED > 0)
	{
		g_RTC_SystemTimer.yRedLED--;

		if(g_RTC_SystemTimer.yRedLED == 0)
		{
			g_RTC_TimerFlag.yRedLEDFlag = TRUE;
		}			
	}

	if(g_RTC_SystemTimer.yBlueLED > 0)
	{
		g_RTC_SystemTimer.yBlueLED--;

		if(g_RTC_SystemTimer.yBlueLED == 0)
		{
			g_RTC_TimerFlag.yBlueLEDFlag = TRUE;
		}
	}

	if(g_RTC_SystemTimer.wBatterySample > 0)
	{
		g_RTC_SystemTimer.wBatterySample--;

		if(g_RTC_SystemTimer.wBatterySample == 0)
		{
			g_RTC_TimerFlag.yBatterySampleFlag = TRUE;
		}
	}

	if(g_RTC_SystemTimer.yChargingLEDOff > 0)
	{
		g_RTC_SystemTimer.yChargingLEDOff--;

		if(g_RTC_SystemTimer.yChargingLEDOff == 0)
		{
			g_RTC_TimerFlag.yChargingLEDOffFlag = TRUE;
		}

	}
		
	if(g_RTC_SystemTimer.yKeyLongPressTime > 0)
	{
		g_RTC_SystemTimer.yKeyLongPressTime--;
			
		if(g_RTC_SystemTimer.yKeyLongPressTime == 0)
		{
			g_RTC_TimerFlag.yKeyLongPressFlag = TRUE;				
		}		
		
	}
		
	if(g_RTC_SystemTimer.yBatteryTemperatureCheckTime >0)
	{
		g_RTC_SystemTimer.yBatteryTemperatureCheckTime--;
			
		if(g_RTC_SystemTimer.yBatteryTemperatureCheckTime == 0)
		{
			g_RTC_TimerFlag.yBatteryTemperatureCheckFlag = TRUE;	
		}	
			
	}
		
	if(g_RTC_SystemTimer.yLowBatteryLEDFlashingTime > 0)
	{
		g_RTC_SystemTimer.yLowBatteryLEDFlashingTime--;
		
		if(g_RTC_SystemTimer.yLowBatteryLEDFlashingTime == 0)
		{
			g_RTC_TimerFlag.yLowBatteryLEDFlashingFlag = TRUE;	
		}	
	}
	
	if(g_RTC_SystemTimer.yKeyPressTimeforFacotyTestMode > 0)
	{
		g_RTC_SystemTimer.yKeyPressTimeforFacotyTestMode--;
			
		if(g_RTC_SystemTimer.yKeyPressTimeforFacotyTestMode == 0)
		{
			g_RTC_TimerFlag.yKeyPressTimeforFacotyTestModeFlag = TRUE;
		}		
	}
		
	if(g_RTC_SystemTimer.yWDT > 0)
	{
		g_RTC_SystemTimer.yWDT--;
		
		if(g_RTC_SystemTimer.yWDT == 0)
		{
			g_RTC_TimerFlag.yWDTFlag = TRUE;
		}	
	}
	
	if(g_RTC_SystemTimer.ySprayOffTime > 0)
	{
		g_RTC_SystemTimer.ySprayOffTime--;

		if(g_RTC_SystemTimer.ySprayOffTime == 0)
		{
			g_RTC_TimerFlag.ySprayOffTimeFlag = TRUE;
		}
	}

	if (g_RTC_SystemTimer.wFullChargingTime > 0)
	{
		g_RTC_SystemTimer.wFullChargingTime--;

		if (g_RTC_SystemTimer.wFullChargingTime == 0)
		{
			g_RTC_TimerFlag.yFullChargingFlag = TRUE;
		}
	}

	if(g_RTC_SystemTimer.yPizeoPWMTurnoffTime > 0)
	{
		g_RTC_SystemTimer.yPizeoPWMTurnoffTime--;

		if(g_RTC_SystemTimer.yPizeoPWMTurnoffTime == 0)
		{
			g_RTC_TimerFlag.yPizeoPWMTurnoffTimeFlag = TRUE;
		}
	}
}
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
void RTC_TimeOutHandler(void)
{
	if(g_RTC_TimerFlag.wFlags == 0)
	{
		return;
	}

	if(g_RTC_TimerFlag.yGreenLEDFlag)
	{
		g_RTC_TimerFlag.yGreenLEDFlag = FALSE;

		UIif_SetBlinkLED(UI_LED_GREEN);
	}

	if(g_RTC_TimerFlag.yRedLEDFlag)
	{
		g_RTC_TimerFlag.yRedLEDFlag = FALSE;

		UIif_SetBlinkLED(UI_LED_RED);
	}

	if(g_RTC_TimerFlag.yBlueLEDFlag)
	{
		g_RTC_TimerFlag.yBlueLEDFlag = FALSE;

		UIif_SetBlinkLED(UI_LED_BLUE);
	}
		
	if(g_RTC_TimerFlag.yBatterySampleFlag)
	{
		BYTE yResult;

		g_RTC_TimerFlag.yBatterySampleFlag = FALSE;

		yResult = ADCif_SampleADC(ADC_TYPE_BATTERY);

		if(yResult == FALSE)
		{
			RTCif_SetBatterySampleTime(5);
		}
		else
		{
			RTCif_SetBatterySampleTime(60);
		}
	}

	if(g_RTC_TimerFlag.yChargingLEDOffFlag)
	{
		g_RTC_TimerFlag.yChargingLEDOffFlag = FALSE;

		if(UIif_GetBatteryTempFaultStatus()  == FALSE)
		{
			UI_StopBlinkLED(UI_LED_RED);
		}
	}
		
	if(g_RTC_TimerFlag.yKeyLongPressFlag)
	{
		g_RTC_TimerFlag.yKeyLongPressFlag = FALSE;
			
		Key_PressTimeOut();
	}
		
	if(g_RTC_TimerFlag.yBatteryTemperatureCheckFlag)
	{
		g_RTC_TimerFlag.yBatteryTemperatureCheckFlag = FALSE;	

		UIif_BatteryTempFault();

	}		
		
	if(g_RTC_TimerFlag.yLowBatteryLEDFlashingFlag)
	{
		g_RTC_TimerFlag.yLowBatteryLEDFlashingFlag = FALSE;
	}
		
	if(g_RTC_TimerFlag.yKeyPressTimeforFacotyTestModeFlag)
	{
		g_RTC_TimerFlag.yKeyPressTimeforFacotyTestModeFlag = FALSE;
			
		if(UIif_GetTestModeFlag() == FALSE)
		{
			UIif_SetTestModeFlag(TRUE);
				
			if(KEY_POWER == FALSE)
			{					
				Module_AddSignal(
					MODULE_UI,
					UI_EVENT_FACTORY_TESTING_MODE,
					NULL
					);	
			}	
		}	
	}

	if(g_RTC_TimerFlag.ySprayOffTimeFlag)
	{
		g_RTC_TimerFlag.ySprayOffTimeFlag = FALSE;

		if(USBif_GetUSBState() == TRUE ||
				g_UI_Status.yBatteryTempFault == TRUE)
		{
			Nop();
		}
		else
		{
			UI_StopBlinkLED(UI_LED_RED);
			
			if(UIif_GetModuleState() == UI_STATE_PWM_PIZEO)
			{					
				UIif_SwitchtoIdle();
			}
		}
	}
	
	if (g_RTC_TimerFlag.yFullChargingFlag)
	{
		g_RTC_TimerFlag.yFullChargingFlag = FALSE;

		Powerif_SetFullChargeTimeFlag(TRUE);
	}

	if(g_RTC_TimerFlag.yPizeoPWMTurnoffTimeFlag)
	{
		g_RTC_TimerFlag.yPizeoPWMTurnoffTimeFlag = FALSE;
		UI_TurnOffPWM();
	}
	

}
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
void RTCif_SetHalfSecondTimeout(void)
{
	g_RTC_SystemTimer.yHalfSecond++;
}
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
)
{
	if(wLED == UI_LED_GREEN)
	{
		g_RTC_SystemTimer.yGreenLED  = yTime;

		if(yTime == 0)
		{
			g_RTC_TimerFlag.yGreenLEDFlag = FALSE;
		}
	}
	else if(wLED == UI_LED_RED)
	{
		g_RTC_SystemTimer.yRedLED = yTime;

		if(yTime == 0)
		{
			g_RTC_TimerFlag.yRedLEDFlag = FALSE;
		}

	}
	else if(wLED  == UI_LED_BLUE)
	{
		g_RTC_SystemTimer.yBlueLED = yTime;

		if(yTime ==0 )
		{
			g_RTC_TimerFlag.yBlueLEDFlag = FALSE;
		}
	}
}
/********************************************************************
 * Function:		RTCif_SetBatterySampleTime()
 *
 * PreCondition:	None
 *
 * Input:			WORD wTime: The sample waiting time
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		Set the battery sample timer for battery.
 *
 * Note:			None
 *******************************************************************/
void RTCif_SetBatterySampleTime(WORD wTime)
{
	g_RTC_SystemTimer.wBatterySample = (wTime *2);
}
/********************************************************************
 * Function:		RTCif_SetChargingLEDOffTime()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yTime: The time for turn off Charing LED
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		Set the Charging LED off time when battery full
 *
 * Note:			None
 *******************************************************************/
void RTCif_SetChargingLEDOffTime(BYTE yTime)
{
	g_RTC_SystemTimer.yChargingLEDOff = (yTime *2);

}
/********************************************************************
 * Function:		RTCif_GetChargingLEDOffTime()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			Charging LED off time
 *
 * Side Effects:	None
 *
 * Overview:		return the charging led off time to turn off LED
 *
 * Note:			None
 *******************************************************************/
BYTE RTCif_GetChargingLEDOffTime(void)
{
	return g_RTC_SystemTimer.yChargingLEDOff;
}	
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
 void RTCif_SetKeyLongPressTime(BYTE yTime)
{
	 g_RTC_SystemTimer.yKeyLongPressTime = (yTime *2); 
}
/********************************************************************
 * Function:		RTCif_SetBatteryTemperatureCheckTime()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yTime: Charing IC Checking Time
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the Time for Checking Chargin IC status Pin
 *
 * Note:			None
 *******************************************************************/
 void RTCif_SetBatteryTemperatureCheckTime(WORD wTime)
{	 
	g_RTC_SystemTimer.yBatteryTemperatureCheckTime = (wTime *2);	 
} 
 /********************************************************************
 * Function:		RTCif_SetyLowBatterLEDFlashingTime()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yTime: Set LED Blink Time
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		When Battery Low, Low bat LED blinking
 *
 * Note:			None
 *******************************************************************/
 void RTCif_SetLowBatteryLEDFlashingTime(BYTE yTime)
{
	g_RTC_SystemTimer.yLowBatteryLEDFlashingTime = (yTime * 2);
} 
 /********************************************************************
 * Function:		RTCif_SetKeyPressTimeforFacotyTestMode()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yTime: Key Press time for Factory test
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		Set the Value of Key Press time when power on
 *
 * Note:			None
 *******************************************************************/
void RTCif_SetKeyPressTimeforFacotyTestMode(BYTE yTime)
{	
	g_RTC_SystemTimer.yKeyPressTimeforFacotyTestMode = (yTime * 2);
}	
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
BYTE RTCif_GetWDTFlag(void)
{
	return g_RTC_TimerFlag.yWDTFlag;
}	
/********************************************************************
 * Function:		RTCif_ClearWDT()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		Reset the software WDT timer. The timer here is 20 seconds
 *
 * Note:			None
 *******************************************************************/
void RTCif_ClearWDT(void)
{
	g_RTC_SystemTimer.yWDT = 40;
}
/********************************************************************
 * Function:		RTCif_SetSprayOffTime()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yTime: Set Time for Spray LED off
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		Set the Value for the time to turn off the spray
 *
 * Note:			None
 *******************************************************************/
void RTCif_SetSprayOffTime(BYTE yTime)
{
	g_RTC_SystemTimer.ySprayOffTime = (yTime * 2);
}
/********************************************************************
 * Function:		RTCif_GetSprayOffTime()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			BYTE: The Spray LED off time
 *
 * Side Effects:	None
 *
 * Overview:		Set the Value for the time to turn off the spray
 *
 * Note:			None
 *******************************************************************/
BYTE RTCif_GetSprayOffTime(void)
{
	return g_RTC_SystemTimer.ySprayOffTime;
}
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
void RTCif_SetFullChargingTime(WORD wValue)
{
	g_RTC_SystemTimer.wFullChargingTime = (wValue *2);
}
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
WORD RTCif_GetFullChargingTime(void)
{
	return g_RTC_SystemTimer.wFullChargingTime;
}
BYTE RTCif_GetPizeoPWMTurnOffTime(void)
{
	return g_RTC_SystemTimer.yPizeoPWMTurnoffTime;
}
void RTCif_SetPizeoPWMTurnOffTime(BYTE yTime)
{
	g_RTC_SystemTimer.yPizeoPWMTurnoffTime = (yTime);
}