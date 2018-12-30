#ifndef ADC_H
#define ADC_H


//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************
#include "GenericTypeDefs.h"
#include "Main.h"

//*****************************************************************************
// MACROS
//*****************************************************************************
#ifdef	ADC_IMPORTED
#define ADC_TYPE
#else
#define ADC_TYPE extern
#endif

#define ADC_BATTERY_PORT	 	TRISBbits.TRISB3
#define ADC_INTERRUPT_FLAG		 PIR1bits.ADIF
#define ADC_INTERRUPT_ENABLE 	PIE1bits.ADIE
#define ADC_INTERRUPT_PRIORITY	IPR1bits.ADIP
#define ADC_START_AUTO_SAMPLE	ADCON0bits.GO = TRUE
#define ADC_STOP_AUTO_SAMPLE	ADCON0bits.GO = FALSE
#define ADC_TURN_ON_MODULE		ADCON0bits.ADON = TRUE
#define ADC_TURN_OFF_MODULE	    ADCON0bits.ADON = FALSE

#define ADC_SAMPLE_TIME		3


#define ADC_CHANNEL_09			0x09


//*****************************************************************************
// VARIABLES
//*****************************************************************************
typedef enum
{
	ADC_TYPE_BATTERY = 0,
	ADC_TYPE_BANDGAP_REFERENCE
}ADC_TYPE;

typedef enum
{
	ADC_SAMPLE_OFF = 0,
	ADC_SAMPLE_ON
}ADC_SAMPLE_STATUS;

ADC_TYPE  BYTE	g_ADC_SampleFinish;
ADC_TYPE  BYTE	g_ADC_Status;
ADC_TYPE  BYTE	g_ADC_SampleTime;
ADC_TYPE  WORD	g_ADC_SampleType;
ADC_TYPE  WORD	g_ADC_SampleValue[3];
ADC_TYPE WORD	g_ADC_SampleFinishType;

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
void ADC_Initialize(void);

/********************************************************************
 * Function:		ADCif_CloseADC()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		This function first disables the ADC interrupt and then turns off
 *				the ADC module. The Interrupt Flag bit (ADIF) is also cleared.
 *				  
 * Note:			None
 *******************************************************************/
void ADCif_CloseADC(void);

/********************************************************************
 * Function:		ADCif_SampleADC()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		This function will enable the ADC convert process. Set the
 *				input sorce and active the auto sample and wait the interrupt.
 *				  
 * Note:			None
 *******************************************************************/
BYTE ADCif_SampleADC(WORD wType);

/********************************************************************
 * Function:		ADC_InterruptHandler()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Handle the ADC interrupt. It will pass the sample value to
 *				power or temperature module.
 *				  
 * Note:			None
 *******************************************************************/
void ADC_InterruptHandler(void);

/********************************************************************
 * Function:		ADCif_NewEventHandler()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Handle the new event for ADC sample.
 *				  
 * Note:			None
 *******************************************************************/
void ADCif_NewEventHandler(void);


#endif


