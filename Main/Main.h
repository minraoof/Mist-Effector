#ifndef MAIN_H
#define MAIN_H


//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
#include <P18Cxxx.h>
#include "GenericTypeDefs.h"

//*****************************************************************************
// MACROS
//*****************************************************************************
#define DIV_3		3

//*****************************************************************************
// STRUCTURE
//*****************************************************************************

//*****************************************************************************
// VARIABLES
//*****************************************************************************

//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************

/********************************************************************
 * Function:		Main_SystemInitialize()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the whole system.
 *
 * Note:			None
 *******************************************************************/
void Main_SystemInitialize(void);
 
/********************************************************************
 * Function:		Main_InitPortSetting()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the unused pin make output low
 					and make digital and analog port
 *
 * Note:			None
 *******************************************************************/
void Main_InitPortSetting(void);

/********************************************************************
 * Function:		Main_InitPinRemap()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the Pin Remap for module
 *
 * Note:			None
 *******************************************************************/
void Main_InitPinRemap(void);

/********************************************************************
 * Function:		Main_HandlePowerSave()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Check the AC/USB connect status first. If no external power
 *				than check the voltage. If lower than 3.4V than totally shut
 *				down the device. Else put the MCU to the sleep mode. The
 *				other part of the hardware should always be in standby
 *				mode to save the power even has the external power.
 *
 * Note:			None
 *******************************************************************/
void Main_HandlePowerSave(void);

/********************************************************************
 * Function:		Main_CheckSystemIdleState()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Check the system is in idle state then put the MCU to
 *				sleep mode.
 *
 * Note:			None
 *******************************************************************/
void Main_CheckSystemIdleState(void);

#endif







