#ifndef UI_H
#define UI_H

//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
#include <P18Cxxx.h>
#include "GenericTypeDefs.h"
#include "Module.h"
#include "Key.h"

//*****************************************************************************
// MACROS
//*****************************************************************************
#ifdef UI_IMPORTED
#define UI_TYPE
#else
#define UI_TYPE extern
#endif


#define UI_LED_YELLOWGREEN_PORT_INPUT		TRISAbits.TRISA5 = INPUT
#define UI_LED_YELLOWGREEN_PORT_OUTPUT	    TRISAbits.TRISA5 = OUTPUT
#define UI_LED_YELLOWGREEN_ON				LATAbits.LATA5 = TRUE
#define UI_LED_YELLOWGREEN_OFF			    LATAbits.LATA5 = FALSE
#define UI_LED_YELLOWGREEN_SWITCH			LATAbits.LATA5

#define UI_LED_BLUE_PORT_INPUT				TRISBbits.TRISB2 = INPUT ;\
											TRISBbits.TRISB1 = INPUT

#define UI_LED_BLUE_PORT_OUTPUT				TRISBbits.TRISB2 = OUTPUT;\
											TRISBbits.TRISB1   = OUTPUT
									
#define UI_LED_BLUE_ON						LATBbits.LATB2 = FALSE;\
											LATBbits.LATB1 = FALSE
									
#define UI_LED_BLUE_OFF						LATBbits.LATB2 = TRUE;\
											LATBbits.LATB1 = TRUE
										
#define UI_LED_BLUE_SWITCH1					LATBbits.LATB1
#define UI_LED_BLUE_SWITCH2					LATBbits.LATB2
											


#define UI_LED_RED_PORT_INPUT				TRISBbits.TRISB5 = INPUT
#define UI_LED_RED_PORT_OUTPUT				TRISBbits.TRISB5 = OUTPUT
#define UI_LED_RED_ON						LATBbits.LATB5 = TRUE
#define UI_LED_RED_OFF						LATBbits.LATB5 = FALSE
#define UI_LED_RED_SWITCH					LATBbits.LATB5

#define UI_PWM_PIZEO1_TIMER_START			T4CONbits.TMR4ON = TRUE
#define UI_PWM_PIZEO1_TIMER_STOP			T4CONbits.TMR4ON = FALSE
#define UI_PWM_PIZEO2_TIMER_START			T2CONbits.TMR2ON = TRUE
#define UI_PWM_PIZEO2_TIMER_STOP			T2CONbits.TMR2ON = FALSE

#define UI_PWM_PIZEO1_TIMER_CLEAR	TMR4 = 0x00

#define UI_PWM_PIZEO2_TIMER_CLEAR	TMR2 = 0x00

#define UI_PIZEO_1_PORT			TRISAbits.TRISA1
#define UI_PIZEO_2_PORT			TRISAbits.TRISA0

#define UI_PIZEO_1_ON			PORTAbits.RA1 = TRUE
#define UI_PIZEO_1_OFF			PORTAbits.RA1 = FALSE

#define UI_PIZEO_2_ON			PORTAbits.RA0 = TRUE
#define UI_PIZEO_2_OFF			PORTAbits.RA0 = FALSE

#define UI_PIZEO_1 				PORTAbits.RA1
#define UI_PIZEO_2 				PORTAbits.RA0




#define UI_PWM_50_DUTYCYCLE		188    //10	// for 300k Actually for 6MHz
#define UI_PWM_300K_FREQUENCY	93  //4	// for CPU 6MHz //93 = 1kHz

#define UI_PWM_TMR_PRESC_1		0
#define UI_PWM_TMR_POSC_1		0
#define UI_PWM_TMR_REESC_4		1
#define UI_PWM_TMR_REESC_16    	2


#define	PWM_PAC_high_PBD_high		0x0C
#define	PWM_PAC_high_PBD_low		0x0D
#define	PWM_PAC_low_PBD_high		0x0E
#define	PWM_PAC_low_PBD_low		0x0F

#define	UI_CCP_SINGLE_OUT			0
#define	UI_CCP_FULLBRIDGEOUT		1
#define	UI_CCP_FULLBRIDGEOUT1		2
#define	UI_CCP_FULLBRIDGEOUT2		3

#define UI_LED_FAST_BLINKING_TIMER_CLEAR	TMR1H = 0;\
											 TMR1L = 0
											 
#define UI_LED_FAST_BLINKING_ENABLE		T1CONbits.TMR1ON
#define UI_LED_FAST_BLINKING_10MS		187//7500


#define UI_PIEZO_TIMER_CLEAR		TMR3H = 0;\
									TMR3L = 0

#define UI_PIEZO_TIMER_ENABLE		T3CONbits.TMR3ON
#define UI_PIEZO_1MS				750






//*****************************************************************************
// VARIABLES
//*****************************************************************************
typedef enum
{
	UI_STATE_IDLE = 0,
	
	UI_STATE_PWM_PIZEO,
	
	UI_STATE_FACTORY_TESTING,
	
	UI_STATE_STOP
}UI_STATE;


typedef enum
{
	UI_TEST_STATE_IDLE = 0		
	
}UI_TEST_STATE;


typedef enum
{
	UI_FACTORY_STATE_IDLE = 0,
	UI_FACTORY_STATE_LED,
	UI_FACTORY_STATE_STANDBY,
	UI_FACTORY_STATE_GREEN,
	UI_FACTORY_STATE_RED,
	UI_FACTORY_STATE_BLUE,
	UI_FACTORY_STATE_PIZEO1,
	UI_FACTORY_STATE_PIZEO2,
	UI_FACTORY_STATE_PIZEO1_2

}UI_FACTORY_TEST_STATE;


typedef enum
{
	UI_EVENT_POWER_KEY_PRESS = 0,
	UI_EVENT_POWER_KEY_LONG_PRESS,
	UI_EVENT_BATTERY_HANDLER,
	UI_EVENT_FACTORY_TESTING_MODE	
}UI_EVENT;



typedef enum
{
	UI_LED_GREEN,
	UI_LED_RED,
	UI_LED_BLUE
}UI_LED;


typedef enum
{
	UI_LED_STATE_OFF = 0,
	UI_LED_STATE_ON
}UI_LED_STATE;


typedef enum
{
	UI_LED_FAST_BLINK_NULL = 0,
	UI_LED_FAST_BLINK_YELLOW = 1,
	UI_LED_FAST_BLINK_RED	= 2,
	UI_LED_FAST_BLINK_BLUE	= 4

}UI_LED_FAST_BLINK_TYPE;

typedef enum
{
	UI_LED_FAST_BLINK_50MS	 = 5,
	UI_LED_FAST_BLINK_100MS	 =10,
	UI_LED_FAST_BLINK_150MS	 =15,
	UI_LED_FAST_BLINK_200MS	 =20
}UI_LED_FAST_BLINK_TIME;

typedef enum
{
	BATTERY_SHUTDOWN,
	BATTERY_LOW,
	BATTERY_NORMAL,
	BATTERY_FULL_CHARGED
} BATTERY_STATUS;

typedef enum
{
	UI_RED_LED_STOP,
	UI_RED_LED_BLINK
}UI_RED_LED_STATUS;


typedef struct
{
	BYTE yState;
	
	/* Notice that the system is in low battery state.
	 * The System should be blink LED
	 */
	BYTE yLowBattery;
	
	// The sub state in the UI module. It is used for the factory testing
	BYTE yFactoryState;

	//Used for the LED blinking process.
	BYTE  yLEDState[3];

	//Used for the Charging LED turn off when battery volt is above 3.6
	BYTE yPowerFullStatus;

	//Check for the Charging IC Status three Pin, to blink red and blue alternatively
	BYTE yBatteryTempFault;

	//Check For the Red Charging LED blinking Status
	BYTE yRedLEDStatus;
	
}UI_STATUS;

typedef struct
{
	BYTE yLED;
	BYTE yLoop;
	BYTE yTimeMS;
	BYTE yTimeCount;
}UI_LED_FastBlink;

UI_TYPE	UI_STATUS  g_UI_Status;
UI_TYPE UI_LED_FastBlink g_UI_LEDFastBlink;
UI_TYPE BYTE		g_UI_TestModeFlag;
UI_TYPE	BYTE		g_UI_AlternateBlinkFlag;
UI_TYPE BYTE		g_UI_LowBatFlashStatus;




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
void UI_InitializeModule(void);

/********************************************************************
 * Function:		UI_SignalHandler()
 *
 * PreCondition:	None
 *
 * Input:			Module_Signal* pSignal: The event signal
 *
 * Output:		BYTE: The signal handle result.
 *
 * Side Effects:	None
 *
 * Overview:		The UI module signal handler. It control the state machine
 *				state.
 *
 * Note:			None
 *******************************************************************/
BYTE UI_SignalHandler(Module_Signal* pSignal);

 /********************************************************************
 * Function:		UI_PowerKeyHandler(KEY_DATA *pData)
 *
 * PreCondition:	None
 *
 * Input:			KEY_DATA ->pData Key detail
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		Handle the Power Key Event
 *
 * Note:			None
 *************************************************************/
void UI_PowerKeyHandler(void);
 
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
void UI_PowerKeyLongPressHandler(void);

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
void UI_HandleFactoryTesting(void);

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
BYTE  UIif_GetModuleState(void);

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
void  UIif_SetModuleState(BYTE yVal);

/********************************************************************
 * Function:		UI_ConfigPWMTimer2()
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
 void UI_ConfigPWMTimer2(void);

 /********************************************************************
 * Function:		UI_ConfigPWMTimer4()
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
 void UI_ConfigPWMTimer4(void);
 
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
 void UIif_StartPWMPIZEO1(void);
 
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
 void UIif_StopPWMPIZEO1(void);
 
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
 void UIif_StartPWMPIZEO2(void);
 
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
 void UIif_StopPWMPIZEO2(void);

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
void UIif_SetBlinkLED(BYTE yLED);
 
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
void UI_StopBlinkLED(BYTE yLED);
 
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
void UI_CheckBatteryHandler(void);
 
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
BYTE UIif_GetLowBatteryStatus(void);
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
void UIif_SetLowBatteryStatus(BYTE yVal);

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
 * Overview:		get the battery status to close the Red LED
 *
 * Note:			None
 *******************************************************************/
BYTE UIif_GetFullBatteryStatus(void);

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
void UIif_BatteryTempFault(void);

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
BYTE UIif_GetBatteryTempFaultStatus(void);

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
void UI_ConfigPiezoSoftwarePWM(void);
void UI_UpdateTimer1CCP(WORD wVal);
void UI_UpdateTimer3CCP(WORD val);
void UI_HandleCCP2Interrupt(void);
void UI_TurnOffPWM(void);
void UI_PollingPWM(void);

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
 void UIIf_HandleFastBlinkLEDTimeout(void);

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
 *
 * Note:			None
*******************************************************************/
void UIif_StartFastBlinkLED(BYTE yLED,
							BYTE yLoop,
							BYTE yTime
							);
							
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
void UIif_SetTestModeFlag(BYTE yVal);

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
BYTE UIif_GetTestModeFlag(void);

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
void UIif_AlternateLEDBlink(void);
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
void UIif_SwitchtoIdle(void);
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
void UIif_SwitchtoPizeo(void);
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
BYTE UIif_GetRedLEDStatus(void);
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
void UIif_SetRedLEDStatus(BYTE yVal);
/********************************************************************
 * Function:		UIif_SetPowerFullStatus(void)
 *
 * PreCondition:	None
 *
 * Input:               noe
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		check the status whether shut Alert send or not
 *
 * Note:			None
 *******************************************************************/
void UIif_SetPowerFullStatus(BYTE yVal);
#endif



