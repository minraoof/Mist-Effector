//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
#define UI_IMPORTED
#include "UI.h"
#include "Key.h"
#include "salloc.h"
#include "RTC.h"
#include "Power.h"
#include "USB_Module.h"
#include "delay.h"
#include "ADC.h"
#include "Flash.h"


//*****************************************************************************
// MACROS
//*****************************************************************************



//*****************************************************************************
// VARIABLES
//*****************************************************************************
#if 0
BYTE g_UI_PiezoPowerDutyCycleOn[] =  {8,16,23,31,38,45,53,61,68,0};
BYTE g_UI_PiezoPowerDutyCycleOff[] = {68,61,53,45,38,31,23,16,8,0};

#endif

BYTE g_UI_PiezoPowerDutyCycleOn[] = {4,6,8,10,12,14,16,18,
									20,22,24,26,28,30,32,34,
									36,38,40,42,44,46,48,50,
									52,54,56,58,60,62,64,66,
									68,0};

BYTE g_UI_PiezoPowerDutyCycleOff[] = {68,66,64,62,60,58,56,54,
									52,50,48,46,44,42,40,38,
									36,34,32,30,28,26,24,22,
									20,18,16,14,12,10, 8, 6,
									4,0};
WORD g_UI_DutyIndex = 0;
BYTE g_UI_PWMStatus = FALSE;

//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************
/********************************************************************
 * Function:		UI_InitializeModule()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the UI module
 *
 * Note:			None
 *******************************************************************/
void UI_InitializeModule(void)
{
	
	g_UI_Status.yState = UI_STATE_IDLE;
	g_UI_Status.yLowBattery = FALSE;
	g_UI_Status.yFactoryState = UI_FACTORY_STATE_IDLE;

	g_UI_Status.yLEDState[UI_LED_GREEN] = UI_LED_STATE_OFF;
	g_UI_Status.yLEDState[UI_LED_RED] = UI_LED_STATE_OFF;
	g_UI_Status.yLEDState[UI_LED_BLUE] = UI_LED_STATE_OFF;
	
	g_UI_Status.yPowerFullStatus = FALSE;
	g_UI_Status.yBatteryTempFault = FALSE;	
	g_UI_Status.yRedLEDStatus = UI_RED_LED_STOP;	
	
	g_UI_TestModeFlag = FALSE;
	g_UI_AlternateBlinkFlag = FALSE;
	

	UI_LED_RED_OFF;
	UI_LED_YELLOWGREEN_OFF;
	UI_LED_BLUE_OFF;

	UI_LED_YELLOWGREEN_PORT_OUTPUT;
	UI_LED_RED_PORT_OUTPUT;
	UI_LED_BLUE_PORT_OUTPUT;	
	
	//DelayMs(200);
	UI_ConfigPiezoSoftwarePWM();

	UI_ConfigPWMTimer2();		//PIEZO2

	UI_ConfigPWMTimer4();		//PIEZO1

	g_UI_LowBatFlashStatus = Powerif_GetLowBatReadFlashData();

	if(Powerif_IsLowPowerState() == TRUE ||
		g_UI_LowBatFlashStatus == TRUE)
	{
		g_UI_Status.yLowBattery = TRUE;

		RTCif_SetSprayOffTime(90);
		UIif_SetBlinkLED(UI_LED_RED);
	}	

}
/********************************************************************
 * Function:		UI_SignalHandler()
 *
 * PreCondition:	None
 *
 * Input:			Module_Signal *pSignal - the event signal
 *
 * Output:			The Signal Handle result
 *
 * Side Effects:	None
 *
 * Overview:	The UI module signal handler. It control the state machine
 *				state.
 *
 * Note:			None
 *******************************************************************/
BYTE UI_SignalHandler(Module_Signal* pSignal)
{    

	if (NULL == pSignal)
	{
		return MODULE_RESULT_EMPTY;
	}
	
	switch (pSignal->wEvent)
	{
		case UI_EVENT_POWER_KEY_PRESS:
		{
			UI_PowerKeyHandler();			
		}
			break;
			
		case UI_EVENT_POWER_KEY_LONG_PRESS:
		{
			UI_PowerKeyLongPressHandler();
		}
			break;
			
		case UI_EVENT_BATTERY_HANDLER:
		{
			UI_CheckBatteryHandler();
		}	
		
			break;
			
		case UI_EVENT_FACTORY_TESTING_MODE:
		{
			g_UI_Status.yState = UI_STATE_FACTORY_TESTING;
			
			g_UI_Status.yFactoryState = UI_FACTORY_STATE_IDLE;
			
			UI_LED_RED_ON;
			
			UI_LED_YELLOWGREEN_ON;
			
			UI_LED_BLUE_ON;			
		}
			break;	
			
		default:
			break;
	}
	
	return MODULE_RESULT_OK;
}

/********************************************************************
 * Function:		UI_PowerKeyHandler(void)
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		Handle the Power Key Event
 *
 * Note:			None
 *************************************************************/
void UI_PowerKeyHandler(void)
{

	switch(g_UI_Status.yState)
	{
		case UI_STATE_IDLE:
		{					
			UIif_SwitchtoPizeo();
		}
			break;
			
		case UI_STATE_PWM_PIZEO:
		{				
			UIif_SwitchtoIdle();
		}				
			break;
			
		case UI_STATE_FACTORY_TESTING:
		{
			UI_HandleFactoryTesting();					
		}
			break;
				
		default:
			break;					
	}			
		
}
/********************************************************************
 * Function:		UI_PowerKeyLongPressHandler(void)
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		Handle the Power Key Event
 *
 * Note:			None
 *************************************************************/
void UI_PowerKeyLongPressHandler(void)
{	
	
}	
/********************************************************************
 * Function:		UI_HandleFactoryTesting()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		Handle the Power Key Event
 *
 * Note:			None
 *************************************************************/
void UI_HandleFactoryTesting(void)
{
	
	switch(g_UI_Status.yFactoryState)
	 {
		case UI_FACTORY_STATE_IDLE:
		{
			UI_StopBlinkLED(UI_LED_GREEN);
			UI_StopBlinkLED(UI_LED_RED);
			UI_StopBlinkLED(UI_LED_BLUE);
			
			UI_LED_YELLOWGREEN_OFF;
			UI_LED_RED_OFF;
			UI_LED_BLUE_OFF;
			DelayMs(200);

			UI_LED_YELLOWGREEN_ON;
			UI_LED_RED_ON;
			UI_LED_BLUE_ON;
			
			g_UI_Status.yFactoryState = UI_FACTORY_STATE_LED;
				 
		}
			break;

		case UI_FACTORY_STATE_LED:
		{
			DelayMs(100);
				
			UI_LED_YELLOWGREEN_OFF;
			UI_LED_RED_OFF;
			UI_LED_BLUE_OFF;

			g_UI_Status.yFactoryState = UI_FACTORY_STATE_STANDBY;
		}
			break;

		case UI_FACTORY_STATE_STANDBY:
		{
			UI_LED_YELLOWGREEN_ON;
			g_UI_Status.yFactoryState = UI_FACTORY_STATE_GREEN;
		}
			break;
			 
		case UI_FACTORY_STATE_GREEN:
		{
		 	UI_LED_YELLOWGREEN_OFF;
			UI_LED_RED_ON;
	
			g_UI_Status.yFactoryState = UI_FACTORY_STATE_RED;
		}
			break;

		case UI_FACTORY_STATE_RED:
		{
			UI_LED_RED_OFF;
			UI_LED_BLUE_ON;
			
			g_UI_Status.yFactoryState = UI_FACTORY_STATE_BLUE;					
		}
			break;
			 
		case UI_FACTORY_STATE_BLUE:
		{
			UI_LED_BLUE_OFF;
			UI_LED_RED_ON;
			UI_LED_YELLOWGREEN_ON;
					
			UIif_StartPWMPIZEO1();

			g_UI_Status.yFactoryState = UI_FACTORY_STATE_PIZEO1;
		 }
			 break;

		case UI_FACTORY_STATE_PIZEO1:
		{
			UI_LED_YELLOWGREEN_OFF;
			UI_LED_BLUE_ON;
				
	 		UIif_StopPWMPIZEO1();
			UIif_StartPWMPIZEO2();

			g_UI_Status.yFactoryState = UI_FACTORY_STATE_PIZEO2;
		}
			break;

		case UI_FACTORY_STATE_PIZEO2:
		{
			UI_LED_RED_OFF;
			UI_LED_YELLOWGREEN_ON;
			UI_LED_BLUE_ON;				
					  
			UIif_StartPWMPIZEO1();

			g_UI_Status.yFactoryState = UI_FACTORY_STATE_PIZEO1_2;
		}
			break;

		case UI_FACTORY_STATE_PIZEO1_2:
		{
			UIif_StopPWMPIZEO1();
			UIif_StopPWMPIZEO2();

			UI_LED_YELLOWGREEN_ON ;
			UI_LED_RED_ON;
			UI_LED_BLUE_ON;					
			DelayMs(100);

			g_UI_Status.yFactoryState = UI_FACTORY_STATE_IDLE;

		}
			break;
			 
		default:
			g_UI_Status.yFactoryState = UI_FACTORY_STATE_LED;
			break;			 
		 } 	
}

/********************************************************************
 * Function:		UIif_GetModuleState()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		return the UI Module State
 *
 * Side Effects:	None
 *
 * Overview:		Get the UI Module State
 *
 * Note:			None
 *************************************************************/
BYTE  UIif_GetModuleState(void)
{
	return g_UI_Status.yState;
}
/********************************************************************
 * Function:		UIif_SetModuleState()
 *
 * PreCondition:	None
 *
 * Input:			yVal: UI Module State
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		Set the UI Module State
 *
 * Note:			None
 *************************************************************/
void  UIif_SetModuleState(BYTE yVal)
{
	g_UI_Status.yState = yVal;
}	
/********************************************************************
 * Function:		UI_ConfigPWMTimer2()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the PWM for PIEZO2
 *
 * Note:			None
 *************************************************************/
 void UI_ConfigPWMTimer2(void)
{
#if 0	 
	UI_PWM_PIZEO2_TIMER_STOP;
		
	PR2 = UI_PWM_300K_FREQUENCY; //Freq 300k
		
	CCP1CONbits.CCP1M = PWM_PAC_high_PBD_high; // PWM mode

	CCP1CONbits.P1M = UI_CCP_SINGLE_OUT;//Single Output

	CCP1CONbits.DC1B = (UI_PWM_50_DUTYCYCLE & 0x03) ;  //1// 50 duty cycle

	CCPR1L = (UI_PWM_50_DUTYCYCLE >> 2);
	
	TRISAbits.TRISA0 = OUTPUT;		

	T2CONbits.T2OUTPS = UI_PWM_TMR_POSC_1; //Posc 1

	T2CONbits.T2CKPS = UI_PWM_TMR_REESC_16;//Presc 1
#endif	

	UI_PIZEO_2_OFF;
		
	UI_PIZEO_2_PORT = OUTPUT;

}
 /********************************************************************
 * Function:		UI_ConfigPWMTimer4()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the PWM Timer For PIZEO1
 *
 * Note:			None
 *************************************************************/
 void UI_ConfigPWMTimer4(void)
 {
 #if 0
	UI_PWM_PIZEO1_TIMER_STOP;
		
	PR4 = UI_PWM_300K_FREQUENCY; //Freq 300k		
		
	CCP2CONbits.CCP2M = PWM_PAC_high_PBD_high; // PWM mode

	CCP2CONbits.P2M = UI_CCP_SINGLE_OUT;//Single Output

	CCP2CONbits.DC2B = (UI_PWM_50_DUTYCYCLE & 0x03) ;  //1// 50 duty cycle

	CCPR2L = (UI_PWM_50_DUTYCYCLE >> 2);
		
	TRISAbits.TRISA1 = OUTPUT;		

	T4CONbits.T4OUTPS = UI_PWM_TMR_POSC_1; //Posc 1

	T4CONbits.T4CKPS = UI_PWM_TMR_REESC_16;//Presc 1

 #endif		
	UI_PIZEO_1_OFF;
		
	UI_PIZEO_1_PORT = OUTPUT;


 }
/********************************************************************
 * Function:		UIif_StartPWMPIZEO1()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Start the PIZEO1 Output Signal 
 *
 * Note:			None
 *************************************************************/
void UIif_StartPWMPIZEO1(void)
{
#if 0
	TMR4 = 0x00;
				
	PSTR2CONbits.STRA = TRUE;
			
	UI_PWM_PIZEO1_TIMER_START;	
#endif

	/*Testing**/
	g_UI_DutyIndex = 0;

	UI_UpdateTimer1CCP(g_UI_PiezoPowerDutyCycleOn[0]);
	UI_UpdateTimer3CCP(g_UI_PiezoPowerDutyCycleOff[0]);

	UI_LED_FAST_BLINKING_TIMER_CLEAR;
	UI_PIEZO_TIMER_CLEAR;
	UI_LED_FAST_BLINKING_ENABLE = TRUE;
	UI_PIEZO_TIMER_ENABLE = TRUE;

	RTCif_SetPizeoPWMTurnOffTime(3);

	g_UI_PWMStatus = TRUE;
	/*********/
}
/********************************************************************
 * Function:		UIif_StopPWMPIZEO1()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Stop the PIZEO1 Output Signal 
 *
 * Note:			None
 *************************************************************/
 void UIif_StopPWMPIZEO1(void)
{
#if 0
	UI_PWM_PIZEO1_TIMER_STOP;
		
	PSTR2CONbits.STRA = FALSE;
#endif
	/*****Testing*******/
	RTCif_SetPizeoPWMTurnOffTime(0);

	UI_LED_FAST_BLINKING_TIMER_CLEAR;
	UI_PIEZO_TIMER_CLEAR;

	UI_LED_FAST_BLINKING_ENABLE = FALSE;
	UI_PIEZO_TIMER_ENABLE = FALSE;

	g_UI_PWMStatus = FALSE;
	/*********/
	UI_PIZEO_1_OFF;
}

/********************************************************************
 * Function:		UIif_StartPWMPIZEO2()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Start the PIZEO2 Output Signal 1kHz 50% Duty Cycle
 *
 * Note:			None
 *************************************************************/
 void UIif_StartPWMPIZEO2(void)
{
#if 0
		TMR2 = 0x00;
				
		PSTR1CONbits.STRA = TRUE;
			
		UI_PWM_PIZEO2_TIMER_START;
#endif
	/*Testing**/
//	UI_LED_FAST_BLINKING_TIMER_CLEAR;
//	UI_LED_FAST_BLINKING_ENABLE = TRUE;
//	UI_PIEZO_TIMER_ENABLE = TRUE;
	/*********/
	Nop();
#if 0
		UI_PIZEO_2_ON;
#endif
}
/********************************************************************
 * Function:		UIif_StopPWMPIZEO2()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Stop the PIZEO2 Output Signal 
 *
 * Note:			None
 *************************************************************/
void UIif_StopPWMPIZEO2(void)
{	
#if 0
	UI_PWM_PIZEO2_TIMER_STOP;
		
	PSTR1CONbits.STRA = FALSE;
#endif
	/*Testing**/
	UI_LED_FAST_BLINKING_TIMER_CLEAR;
	UI_PIEZO_TIMER_CLEAR;
	UI_LED_FAST_BLINKING_ENABLE = FALSE;
	UI_PIEZO_TIMER_ENABLE = FALSE;
	/*********/
	
	UI_PIZEO_2_OFF;

}
/********************************************************************
 * Function:		UIif_SetBlinkLED()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yLED: The LED type.
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the blinking LED by the input type and state.
 *
 * Note:			None
 *******************************************************************/
void UIif_SetBlinkLED(BYTE yLED)
{
	if( g_UI_Status.yLEDState[yLED] == UI_LED_STATE_ON)
	{
		 g_UI_Status.yLEDState[yLED] = UI_LED_STATE_OFF;

		 if(yLED == UI_LED_RED)
		 {
			UI_LED_RED_ON;  //For Alternate led blinking Function
		 }
		 else if(yLED == UI_LED_GREEN)
		 {
			UI_LED_YELLOWGREEN_OFF;
		 }
		 else if(yLED == UI_LED_BLUE)
		 {
			UI_LED_BLUE_OFF;
		 }
		 
		RTCif_SetLEDTimer(yLED,
						1);

	}
	else if(g_UI_Status.yLEDState[yLED] == UI_LED_STATE_OFF)
	{
			
		g_UI_Status.yLEDState[yLED] = UI_LED_STATE_ON;

		if( yLED == UI_LED_RED)
		{
			UI_LED_RED_OFF; //Alternate led blinking function
		}
		else if(yLED == UI_LED_GREEN)
		{
			UI_LED_YELLOWGREEN_ON;
		}
		else if(yLED == UI_LED_BLUE)
		{
			UI_LED_BLUE_ON;
		}

		RTCif_SetLEDTimer(yLED,
						1);
		
	}

	if(yLED == UI_LED_RED)
	{
		if(UIif_GetRedLEDStatus() != UI_RED_LED_BLINK)
		{
			UIif_SetRedLEDStatus(UI_RED_LED_BLINK);
		}
	}
	

}
/********************************************************************
 * Function:		UI_StopBlinkLED()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yLED: The LED type.
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Stop the blink timer for LED. Set to dim mode and reset the
 *				LED state.
 *
 * Note:			None
 *******************************************************************/
void UI_StopBlinkLED(BYTE yLED)
{
	if(yLED == UI_LED_GREEN)
	{
		UI_LED_YELLOWGREEN_OFF ;			
	}
	else if(yLED == UI_LED_RED)
	{
		UI_LED_RED_OFF;					
	}
	else if(yLED == UI_LED_BLUE)
	{
		UI_LED_BLUE_OFF;
	}

	g_UI_Status.yLEDState[yLED] = UI_LED_STATE_OFF;	

	RTCif_SetLEDTimer(
					yLED,
					0
					);

	if(yLED == UI_LED_RED)
	{
		if(UIif_GetRedLEDStatus() != UI_RED_LED_STOP)
		{
			UIif_SetRedLEDStatus(UI_RED_LED_STOP);
		}
	}

}
/*******************************************************************************
 * Function:		UI_CheckBatteryHandler()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Check the battery is lowe than the low batter threshold
 *				if USB plug out LED need to flash
 *
 * Note:			None
 ******************************************************************************/
void UI_CheckBatteryHandler(void)
{
	
	if(g_UI_Status.yState == UI_STATE_FACTORY_TESTING ||
		g_UI_Status.yBatteryTempFault != FALSE)
	{	
			return;
	}

	if(Powerif_IsBatteryFullCharged())
	{
		RTCif_SetChargingLEDOffTime(20);
		if(Powerif_CheckToClearLowBatFlag() == TRUE &&
			USB_SENSE == TRUE)
		{
			g_UI_Status.yLowBattery = FALSE;

			if(g_UI_LowBatFlashStatus == TRUE)
			{
				g_UI_LowBatFlashStatus = FALSE;

				Powerif_SetLowBatFlashData(FLASH_NOLOWBAT_TYPE);
			}
		}		
	}
	else if(Powerif_IsLowPowerState())
	{	

		if(UIif_GetModuleState() == UI_STATE_PWM_PIZEO)
		{
			if(RTCif_GetSprayOffTime() == 0)	//Battery Checking time is every 60s 
			{									//Spray Off is 90s So need to check codition
				RTCif_SetSprayOffTime(90);		
				UIif_SetBlinkLED(UI_LED_RED);
			}			
		}
		
		if(g_UI_LowBatFlashStatus == FALSE)
		{
			g_UI_LowBatFlashStatus = TRUE;
			Powerif_SetLowBatFlashData(FLASH_LOWBAT_TYPE);
		}

		g_UI_Status.yLowBattery = TRUE;

		if(g_UI_Status.yPowerFullStatus == TRUE)
		{
			g_UI_Status.yPowerFullStatus = FALSE;
		}
	}
	else
	{
		if(Powerif_CheckToClearLowBatFlag() == TRUE &&
			USB_SENSE == TRUE)
		{
			g_UI_Status.yLowBattery = FALSE;

			if(g_UI_LowBatFlashStatus == TRUE)
			{
				g_UI_LowBatFlashStatus = FALSE;

				Powerif_SetLowBatFlashData(FLASH_NOLOWBAT_TYPE);
			}
		}

		if(g_UI_Status.yPowerFullStatus == TRUE)
		{
			g_UI_Status.yPowerFullStatus = FALSE;
		}
	}
}
/********************************************************************
 * Function:		UIif_GetLowBatteryStatus()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			BYTE: return true if battery is low
 *
 * Side Effects:	None
 *
 * Overview:		get the battery status to blink Green LED
 *
 * Note:			None
 *******************************************************************/
BYTE UIif_GetLowBatteryStatus(void)
{
	return g_UI_Status.yLowBattery;
}
/********************************************************************
 * Function:		UIif_SetLowBatteryStatus()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			BYTE: return true if battery is low
 *
 * Side Effects:	None
 *
 * Overview:		get the battery status to blink Green LED
 *
 * Note:			None
 *******************************************************************/
void UIif_SetLowBatteryStatus(BYTE yVal)
{
	g_UI_Status.yLowBattery = yVal;
}
/********************************************************************
 * Function:		UIif_GetFullBatteryStatus()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			BYTE: return true if battery is full
 *
 * Side Effects:	None
 *
 * Overview:		get the battery status to close the red led
 *
 * Note:			None
 *******************************************************************/
BYTE UIif_GetFullBatteryStatus(void)
{
	return g_UI_Status.yPowerFullStatus;
}
/********************************************************************
 * Function:		UIif_BatteryTempFault()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		if Battery Fault Two LED should be blink
 *
 * Note:			None
 *******************************************************************/
void UIif_BatteryTempFault(void)
{
	BYTE yResult  = FALSE;

	if(g_UI_Status.yState == UI_STATE_FACTORY_TESTING)
	{
		return;
	}

	yResult = Powerif_CheckBatteryICTemperature();
		
	if(yResult == TRUE)
	{
		g_UI_Status.yBatteryTempFault  = TRUE;
		RTCif_SetBatteryTemperatureCheckTime(5);

		if(g_UI_AlternateBlinkFlag == FALSE)
		{
			g_UI_AlternateBlinkFlag = TRUE;
			UIif_AlternateLEDBlink();	
		}
		
		UI_LED_BLUE_OFF;
			
		UIif_StopPWMPIZEO1();
		UIif_StopPWMPIZEO2();
		UIif_SetModuleState(UI_STATE_IDLE);
	}
	else
	{
		RTCif_SetBatteryTemperatureCheckTime(60);

		g_UI_AlternateBlinkFlag = FALSE;

		if(g_UI_Status.yBatteryTempFault == TRUE)
		{
			g_UI_Status.yBatteryTempFault = FALSE;
			UI_StopBlinkLED(UI_LED_RED);
			UI_StopBlinkLED(UI_LED_GREEN);

			if(USBif_GetUSBState() == TRUE)
			{				
				UI_LED_RED_ON;
				//ADCif_SampleADC(ADC_TYPE_BATTERY);
			}
					
		}		
	}
}
/********************************************************************
 * Function:		UIif_GetBatteryTempFaultStatus()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			STATUS of BATTERY Pin
 *
 * Side Effects:	None
 *
 * Overview:		Get the Status of the temperature fault
 *					return TRUE temp fault. return FALSE is OK.
 *
 * Note:			None
 *******************************************************************/
BYTE UIif_GetBatteryTempFaultStatus(void)
{
	return g_UI_Status.yBatteryTempFault ;
}
/********************************************************************
 * Function:		UI_ConfigPiezoSoftwarePWM()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		Blink LED for 100ms on and Off
 *
 * Note:			None
 *******************************************************************/
void UI_ConfigPiezoSoftwarePWM(void)
{	
	UI_LED_FAST_BLINKING_ENABLE = FALSE;
	UI_LED_FAST_BLINKING_TIMER_CLEAR;	
	
	T1CONbits.TMR1CS = 0; //Select Fosc/4
	T1CONbits.RD16   = 1; //Timer1 16bit operation
	T1CONbits.T1CKPS = 0x03;	
#if 0	
	CCPR1H	= (UI_LED_FAST_BLINKING_10MS >> 8);
	CCPR1L  = UI_LED_FAST_BLINKING_10MS;
#endif

	//UI_UpdateTimer1CCP(8);	 //ON time
	g_UI_DutyIndex = 0;
	UI_UpdateTimer1CCP(g_UI_PiezoPowerDutyCycleOn[0]);
	
	CCP1CONbits.CCP1M = 0x0A;	//Software Interrupt MODE
	
	PIR1bits.CCP1IF = FALSE;
	PIE1bits.CCP1IE = TRUE;
	IPR1bits.CCP1IP	= TRUE;

	/****Timer 3 Initialization***/
	UI_PIEZO_TIMER_ENABLE = FALSE;
	UI_PIEZO_TIMER_CLEAR;

//	T2CONbits.T2OUTPS = 0x0F; // 16 Posc
//	T2CONbits.T2CKPS  = 0x02; // 16 Posc

	T3CONbits.TMR3CS = 0;
	T3CONbits.T3CKPS = 0x03;
	T3CONbits.RD16 = 1;

	CCP2CONbits.CCP2M = 0x0A;

	PIR2bits.CCP2IF = FALSE;
	PIE2bits.CCP2IE = TRUE;
	IPR2bits.CCP2IP = TRUE;

	UI_UpdateTimer3CCP(g_UI_PiezoPowerDutyCycleOff[0]); //Off time
	/******************************/

	//PIR1bits.TMR1IF = FALSE;
	//PIE1bits.TMR1IE = TRUE;
	TCLKCON |= 0x01;			
}

void UI_UpdateTimer1CCP(WORD wVal)
{
	if(wVal == 0)
		return;

	CCPR1H	= (wVal >> 8);
	CCPR1L  = wVal;
}
void UI_UpdateTimer3CCP(WORD wVal)
{
	if(wVal == 0)
		return;
	CCPR2H = (wVal >> 8);
	CCPR2L = wVal;
}

void UI_HandleCCP2Interrupt(void)
{
//	UI_PIEZO_TIMER_CLEAR;
	if(g_UI_PWMStatus == TRUE)
	{
		UI_PIEZO_TIMER_ENABLE = FALSE;
		UI_PIZEO_1_ON;
		UI_PIZEO_2_ON;
		UI_LED_FAST_BLINKING_TIMER_CLEAR;
		UI_LED_FAST_BLINKING_ENABLE = TRUE;
	}
}

void UI_TurnOffPWM(void)
{
	if(UIif_GetModuleState() == UI_STATE_PWM_PIZEO)
	{
		if((g_UI_PiezoPowerDutyCycleOn[g_UI_DutyIndex+1] != 0) &&
			(g_UI_PiezoPowerDutyCycleOff[g_UI_DutyIndex+1] != 0))
		{
			g_UI_DutyIndex++;
			UI_UpdateTimer1CCP(g_UI_PiezoPowerDutyCycleOn[g_UI_DutyIndex]);
			UI_UpdateTimer3CCP(g_UI_PiezoPowerDutyCycleOff[g_UI_DutyIndex]);
			RTCif_SetPizeoPWMTurnOffTime(1);
		}
		else
		{
			UI_LED_FAST_BLINKING_TIMER_CLEAR;
			UI_LED_FAST_BLINKING_ENABLE = FALSE;

			UI_PIEZO_TIMER_CLEAR;
			UI_PIEZO_TIMER_ENABLE = FALSE;

			UI_PIZEO_1_ON;
			UI_PIZEO_2_ON;

			g_UI_PWMStatus = FALSE;
		}
	}	
}
void UI_PollingPWM(void)
{
	if(g_UI_PWMStatus == TRUE)
	{
		if(PIR2bits.CCP2IF)
		{
			PIR2bits.CCP2IF = FALSE;
			UI_PIEZO_TIMER_ENABLE = FALSE;
			UI_PIZEO_1_ON;
			UI_PIZEO_2_ON;
			UI_LED_FAST_BLINKING_TIMER_CLEAR;
			UI_LED_FAST_BLINKING_ENABLE = TRUE;
		}
		else if(PIR1bits.CCP1IF)
		{
			PIR1bits.CCP1IF = FALSE;
			UI_LED_FAST_BLINKING_ENABLE = FALSE;

			if(g_UI_PiezoPowerDutyCycleOn[g_UI_DutyIndex + 1] != 0 &&
				g_UI_PiezoPowerDutyCycleOff[g_UI_DutyIndex + 1] != 0)
			{
				UI_PIZEO_1_OFF;
				UI_PIZEO_2_OFF;
			}
			UI_PIEZO_TIMER_CLEAR;
			UI_PIEZO_TIMER_ENABLE = TRUE;
		}
	}	
}
/********************************************************************
 * Function:		UIif_StartFastBlinkLED()
 *
 * PreCondition:	None
 *
 * Input:			yLED : which LED
 *					yLoop: how many times
 *					yTime: blink on/off time length
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		Blink LED for 100ms for on/off which LED and 
 *					how many times
 *				e.g UIif_StartFastBlinkLED(UI_LED_FAST_BLINK_YELLOW,
 									    10,
 									    UI_LED_FAST_BLINK_100MS);
 									    
 *
 * Note:			None
*******************************************************************/
void UIif_StartFastBlinkLED(BYTE yLED,
							BYTE yLoop,
							BYTE yTime
							)
{
	g_UI_LEDFastBlink.yLED = yLED;
	g_UI_LEDFastBlink.yLoop = yLoop;
	g_UI_LEDFastBlink.yTimeMS = yTime;
	
	g_UI_LEDFastBlink.yTimeCount = yTime;
	
	UI_LED_FAST_BLINKING_TIMER_CLEAR;
	
	
	if(yLoop != 0)
	{
		UI_LED_FAST_BLINKING_ENABLE = TRUE;
	}
	else
	{
		UI_LED_FAST_BLINKING_ENABLE = FALSE;
	}		
	
}	
/********************************************************************
 * Function:		UIIf_HandleFastBlinkLEDTimeout()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		Blink LED for 10ms Time out Handler
 *
 * Note:			None
*******************************************************************/
void UIIf_HandleFastBlinkLEDTimeout(void)	
{
	//UI_LED_FAST_BLINKING_TIMER_CLEAR;
	if(g_UI_PWMStatus == TRUE)
	{
		UI_LED_FAST_BLINKING_ENABLE = FALSE;
	
		UI_PIZEO_1_OFF;
		UI_PIZEO_2_OFF;	
		
		UI_PIEZO_TIMER_CLEAR;
		UI_PIEZO_TIMER_ENABLE = TRUE;	 
	}
}
/********************************************************************
 * Function:		UIIf_SetTestModeFlag()
 *
 * PreCondition:	None
 *
 * Input:			Val: TRUE or FALSE
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		To Enter the Test Mode
 *
 * Note:			None
*******************************************************************/
void UIif_SetTestModeFlag(BYTE yVal) 
{
	g_UI_TestModeFlag = yVal;
}
/********************************************************************
 * Function:		UIIf_GetTestModeFlag()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		return the Status of Test Mode
 *
 * Note:			None
*******************************************************************/
BYTE UIif_GetTestModeFlag(void)
{
	return g_UI_TestModeFlag;
}

/********************************************************************
 * Function:		UIif_AlternateLEDBlink()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		change Red and Green LED State For Alternate blink
 *
 * Note:			None
*******************************************************************/
void UIif_AlternateLEDBlink(void)
{
	UI_LED_YELLOWGREEN_OFF;			
	g_UI_Status.yLEDState[UI_LED_GREEN] = UI_LED_STATE_OFF;

	UI_LED_RED_OFF;
	g_UI_Status.yLEDState[UI_LED_RED] = UI_LED_STATE_OFF;			

	UIif_SetBlinkLED(UI_LED_GREEN);
	UIif_SetBlinkLED(UI_LED_RED);		
}
/********************************************************************
 * Function:		UIif_SwitchtoIdle()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Turn off the pizeo and led off
 *
 * Note:			None
*******************************************************************/
void UIif_SwitchtoIdle(void)
{
	if(USBif_GetUSBState() == TRUE ||
		g_UI_Status.yBatteryTempFault == TRUE)
	{
		return;	
	}
		
	UIif_StopPWMPIZEO1();
	UIif_StopPWMPIZEO2();

	UI_LED_YELLOWGREEN_OFF;
	UI_LED_BLUE_OFF;
	
	g_UI_Status.yState = UI_STATE_IDLE;
	
}
/********************************************************************
 * Function:		UIif_SwitchtoPizeo()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Turn on the Pizeo and green LED
 *
 * Note:			None
*******************************************************************/
void UIif_SwitchtoPizeo(void)
{
	if(USBif_GetUSBState() == TRUE ||
		g_UI_Status.yBatteryTempFault == TRUE)
	{
		return;	
	}
	
	if(UIif_GetLowBatteryStatus() == TRUE)
	{
		return;
	}

	UI_LED_YELLOWGREEN_ON;
	UI_LED_BLUE_ON;
	
	g_UI_Status.yState = UI_STATE_PWM_PIZEO;	
	
	UIif_StartPWMPIZEO1();
	UIif_StartPWMPIZEO2();
}
/********************************************************************
 * Function:		UIif_GetRedLEDStatus()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			BYTE: Charging led (Red) status
 *
 * Side Effects:	None
 *
 * Overview:		return the status of red LED
 *
 * Note:			None
*******************************************************************/
BYTE UIif_GetRedLEDStatus(void)
{
	return	g_UI_Status.yRedLEDStatus;
}
/********************************************************************
 * Function:		UIif_GetRedLEDStatus()
 *
 * PreCondition:	None
 *
 * Input:			BYTE: Red LED status
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the value of the red led status
 *
 * Note:			None
*******************************************************************/
void UIif_SetRedLEDStatus(BYTE yVal)
{
	g_UI_Status.yRedLEDStatus = yVal;
}
/********************************************************************
 * Function:		UIif_SetPowerFullStatus(void)
 *
 * PreCondition:	None
 *
 * Input:,          noe
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		check the status whether shut Alert send or not
 *
 * Note:			None
 *******************************************************************/
void UIif_SetPowerFullStatus(BYTE yVal)
{
	g_UI_Status.yPowerFullStatus = yVal;
}