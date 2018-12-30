#ifndef KEY_H
#define KEY_H

//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************

#include "GenericTypeDefs.h"
#include "Main.h"

//*****************************************************************************
// MACROS
//*****************************************************************************
#ifdef KEY_IMPORTED
#define KEY_TYPE
#else
#define KEY_TYPE extern
#endif

#define KEY_POWER_PORT						TRISBbits.TRISB4//_TRISA4
#define KEY_POWER							PORTBbits.RB4//_RA4
#define KEY_POWER_INTERRUPT					INTCON3bits.INT1IE//CNENAbits.CNIEA4
#define KEY_POWER_PULL_UP					//CNPUAbits.CNPUA4

#define KEY_DEBOUNCE_TIMER_ENABLE			T0CONbits.TMR0ON//T1CONbits.TON
#define KEY_DEBOUNCE_TIMER					//TMR1

/*#define KEY_PRESS_TIMER_ENABLE				T2CONbits.TON
#define KEY_PRESS_TIMER						TMR2*/

#define KEY_PRESS_LONG_KEY_COUNT					3

#define KEY_DEBOUNCE_50MS			234//938//1172 //for 6MHz
#define KEY_EXTERNAL_INT1EDG		         INTCON2bits.INTEDG1
#define KEY_SENSE_RISING_EDGE		INTCON2bits.INTEDG1 = TRUE
#define KEY_SENSE_FALLING_EDGE		INTCON2bits.INTEDG1 = FALSE

//*****************************************************************************
// VARIABLES
//*****************************************************************************

typedef enum
{
	KEY_STATE_RELEASE = 0,
	KEY_STATE_PRESS,
	KEY_STATE_LONG_PRESS
}KEY_STATE;

typedef enum
{
	KEY_VALUE_NONE = 0,

	KEY_VALUE_POWER,
	
	KEY_VALUE_TOTAL
}KEY_VALUE;

typedef struct
{
	BYTE yState;
	BYTE yKey;
	BYTE yPreviousKey;
	BYTE yPressCount;
}KEY_STATE_STRUCT;

typedef struct
{
	BYTE yEvent;
	WORD wKeyValue;
}KEY_DATA;

KEY_TYPE KEY_STATE_STRUCT g_Key_State;
KEY_TYPE BYTE g_Key_NewEvent;


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
void Key_InitializeModule(void);

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
void KeyIf_NewEventHandler(void);

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
void Key_DebounceInterruptHandler(void);

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
void Key_ParseKey(void);

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
BYTE Key_GetKeyData(void);

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
WORD Key_FindKeyValue(BYTE yKey);

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
BYTE Keyif_GetKeyState(void);

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
void Key_PressTimeOut(void);

#endif

