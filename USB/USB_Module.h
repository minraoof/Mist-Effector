#ifndef USB_MODULE_H
#define USB_MODULE_H


//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
#include "GenericTypeDefs.h"
#include "Main.h"


//*****************************************************************************
// MACROS
//*****************************************************************************
#ifdef USB_IMPORTED
#define USB_TYPE
#else
#define USB_TYPE extern
#endif

#ifdef _BETA_REV
#define USB_SENSE_PORT					TRISBbits.TRISB0
#define USB_SENSE						PORTBbits.RB0
#define USB_SENSE_INTERRUPT_FLAT		INTCONbits.INT0IF
#define USB_SENSE_INTERRUPT_ENABLE		INTCONbits.INT0IE = TRUE
#define USB_SENSE_INTERRUPT_DISABLE		INTCONbits.INT0IE = FALSE
#define USB_SENSE_RISING_EDGE			INTCON2bits.INTEDG0 = TRUE
#define USB_SENSE_FALLING_EDGE			INTCON2bits.INTEDG0 = FALSE


#endif

typedef struct
{
	BYTE yUSB;
	BYTE  wEvent;
	
}USB_STATUS;

USB_TYPE USB_STATUS g_USB_State;



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
void USB_InitializeModule(void);

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
 void USB_EventHandler(void);

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
 *******************************************************************/
 void USBif_SetUSBState(void);

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
BYTE USBif_GetUSBState(void);

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
 void USBif_InterruptHandler(void);

#endif

