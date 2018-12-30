//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
#define KEY_IMPORTED
#include "Key.h"
#include "Module.h"
#include "UI.h"
#include "RTC.h"

//*****************************************************************************
// MACROS
//*****************************************************************************


//*****************************************************************************
// VARIABLES
//*****************************************************************************

//KEY_STATE_STRUCT g_Key_State;
//BYTE g_Key_NewEvent;

//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************

/********************************************************************
 * Function:		Key_InitializeModule()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Init the key module
 *
 * Note:			None
 *******************************************************************/
void Key_InitializeModule(void)
{	
		
	g_Key_State.yState 			= KEY_STATE_RELEASE;
	g_Key_State.yKey   			= KEY_VALUE_NONE;
	g_Key_State.yPreviousKey    = KEY_VALUE_NONE;
		
		
	g_Key_NewEvent = FALSE;
		
	KEY_POWER_PORT = INPUT;
	
#if 0	
		if(KEY_POWER == FALSE)
		{
			Module_AddSignal(MODULE_UI,
						     UI_EVENT_FACTORY_TESTING_MODE,
						     NULL);	
		}	
#endif

	RTCif_SetKeyPressTimeforFacotyTestMode(3);

	//Timer 0 Init		
	KEY_DEBOUNCE_TIMER_ENABLE= 0;
	
	INTCONbits.TMR0IE = FALSE;
	INTCONbits.TMR0IF = FALSE;

	INTCON2bits.TMR0IP = TRUE; //High Priority
	
	
	T0CONbits.T08BIT = FALSE; //use the 16 bit Timer
	T0CONbits.T0CS	 = FALSE;  //use Internal Clock
	T0CONbits.PSA	 = FALSE;  //Use Prescaler Output
	T0CONbits.T0PS	 = 0x07;   // 1:256 prescalar

	
	INTCONbits.TMR0IE = TRUE;
	
	//External Interrupt Init
	
	KEY_SENSE_FALLING_EDGE;
	
	INTCON3bits.INT1IP  = TRUE ; //High Priority
	INTCON3bits.INT1IF	= FALSE;
	INTCON3bits.INT1IE	= TRUE;	

	//Global Interrupt On
	INTCONbits.PEIE = TRUE;
	INTCONbits.GIE  = TRUE;
}
/********************************************************************
 * Function:		KeyIf_NewEventHandler()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Init the key module
 *
 * Note:			None
 *******************************************************************/
void KeyIf_NewEventHandler(void)
{

	if(g_Key_NewEvent == TRUE)
	{		
		BYTE yKeyValue = Key_FindKeyValue(g_Key_State.yKey);
			
		g_Key_NewEvent = FALSE;

		if(yKeyValue == KEY_VALUE_POWER)
		{
			if(g_Key_State.yState == KEY_STATE_PRESS)
			{
				Module_AddSignal(
					MODULE_UI,
					UI_EVENT_POWER_KEY_PRESS,
					NULL
					);			
			}
				
			else if(g_Key_State.yState == KEY_STATE_LONG_PRESS)
			{
					
				Module_AddSignal(
					MODULE_UI,
					UI_EVENT_POWER_KEY_LONG_PRESS,
					NULL
					);			
				}

			}		
	}	
}	

/********************************************************************
 * Function:		Key_DebounceInterruptHandler()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		The SW debounce handler. If the key value still no change
 *				it will send a press event to UI module
 *
 * Note:			None
 *******************************************************************/
void Key_DebounceInterruptHandler(void)
{

	BYTE yKey = Key_GetKeyData();

	KEY_DEBOUNCE_TIMER_ENABLE = FALSE;

	if(g_Key_State.yKey == yKey)
	{
		g_Key_State.yState = KEY_STATE_PRESS;
			
		KEY_SENSE_RISING_EDGE;

		g_Key_State.yKey = yKey;

		g_Key_State.yPressCount = 0;
			
		RTCif_SetKeyLongPressTime(KEY_PRESS_LONG_KEY_COUNT);

		g_Key_NewEvent = TRUE;
	}		
}

/********************************************************************
 * Function:		Key_ParseKey()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Handle the input key and change the state machine states.
 *
 * Note:			None
 *******************************************************************/
void Key_ParseKey(void)
{

	BYTE yKey = Key_GetKeyData() , yResult = FALSE;	

	if(g_Key_State.yState == KEY_STATE_RELEASE)
	{
		if(yKey  != 0x01 &&
			g_Key_State.yKey != yKey)
		{
			TMR0L = 0x00;
			TMR0H = 0x00;
		}
		if(yKey == 0x01)
		{
			KEY_DEBOUNCE_TIMER_ENABLE = FALSE;
		}
		else
		{
			g_Key_State.yKey = yKey;
			//KEY_SENSE_RISING_EDGE;				
			TMR0H	= (65536-KEY_DEBOUNCE_50MS) >> 8;
			TMR0L	= (65536-KEY_DEBOUNCE_50MS);	
			KEY_DEBOUNCE_TIMER_ENABLE = TRUE;
				
		}
	}
	else if(g_Key_State.yState == KEY_STATE_PRESS ||
			g_Key_State.yState == KEY_STATE_LONG_PRESS)
	{
		if(yKey == 0x01)
		{
			g_Key_State.yState = KEY_STATE_RELEASE;
			g_Key_State.yPreviousKey = g_Key_State.yKey;
			g_Key_State.yPressCount = 0;
			RTCif_SetKeyLongPressTime(0);
			yResult = TRUE;
		}
		else if(g_Key_State.yKey != yKey)
		{
			RTCif_SetKeyLongPressTime(KEY_PRESS_LONG_KEY_COUNT);
			g_Key_State.yPreviousKey = g_Key_State.yKey;
			g_Key_State.yKey = yKey;
			g_Key_State.yPressCount = 0;
		}
		else
		{
			g_Key_State.yState = KEY_STATE_RELEASE;
			g_Key_State.yPreviousKey = g_Key_State.yKey;
			g_Key_State.yPressCount = 0;
			RTCif_SetKeyLongPressTime(0);
			yResult = TRUE;	
		}
			KEY_SENSE_FALLING_EDGE;				
	}

	if(yResult == TRUE)
	{
		g_Key_NewEvent = TRUE;
	}	
	UIif_SetTestModeFlag(TRUE);
}
/********************************************************************
 * Function:		Key_GetKeyData()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: The key data
 *
 * Side Effects:	None
 *
 * Overview:		Get the current key data.
 *
 * Note:			None
 *******************************************************************/
BYTE Key_GetKeyData(void)
{
	BYTE yKey = 0x00;
		
	yKey |= KEY_POWER;

	return yKey;
}
/********************************************************************
 * Function:		Key_FindKeyValue()
 *
 * PreCondition:	None
 *
 * Input:			BYTE yKey: The input key value
 *
 * Output:		WORD: The type value of the key in WORD.
 *
 * Side Effects:	None
 *
 * Overview:		Transform the key value to a WORD type value.
 *
 * Note:			None
 *******************************************************************/
WORD Key_FindKeyValue(BYTE yKey)
{
	BYTE yIndex;
#if 0
	if(yKey == 0)
	{
		return KEY_VALUE_NONE ;
	}
#endif
	for(yIndex = 1; yIndex <KEY_VALUE_TOTAL;yIndex++)
	{
		if((yKey & 0x01) == 0)
		{
			return yIndex;
		}	
		yKey >>=1;
	}	

	return KEY_VALUE_NONE;	
}

/********************************************************************
 * Function:		Keyif_GetKeyState()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: The key's current status
 *
 * Side Effects:	None
 *
 * Overview:		Return the key's current status
 *
 * Note:			None
 *******************************************************************/
BYTE Keyif_GetKeyState(void)
{
	return g_Key_State.yState;
}

/********************************************************************
 * Function:		Key_PressTimeOut()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		The long key press timer time out event. If time out happened,
 *				it active the long key press event.
 *
 * Note:			None
 *******************************************************************/
void Key_PressTimeOut(void)
{
	g_Key_State.yState = KEY_STATE_LONG_PRESS;
		
	g_Key_NewEvent = TRUE;
	
}	
