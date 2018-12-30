//*****************************************************************************
// INCLUDES
//*****************************************************************************
#define ADC_IMPORTED
#include "ADC.h"
#include "Power.h"
#include "Delay.h"

//*****************************************************************************
// MACROS
//*****************************************************************************


//*****************************************************************************
// VARIABLES
//*****************************************************************************
//BYTE g_ADC_Status, g_ADC_SampleTime, g_ADC_SampleFinish = FALSE;
//WORD g_ADC_SampleType, g_ADC_SampleFinishType, g_ADC_SampleValue[3];


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
void ADC_Initialize(void)
{

	g_ADC_SampleFinish = FALSE;
	ADC_BATTERY_PORT	= INPUT;
	
//	ADC_BATTERY_PORT = INPUT;
	
	ADCif_CloseADC();
	
	ADC_INTERRUPT_PRIORITY = TRUE; //High Priority

	ADCON0bits.CHS = ADC_CHANNEL_09;	//AN9
	ADCON1bits.ADFM = 1;		//Right justified
	ADCON1bits.ADCS = 0x04;		//Fosc/4
	ADCON1bits.ACQT = 0x04;		//8TAD

}
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
void ADCif_CloseADC(void)
{
	ADC_STOP_AUTO_SAMPLE ;
	
	ADC_TURN_OFF_MODULE;
	
	g_ADC_Status = ADC_SAMPLE_OFF;
		
	g_ADC_SampleTime = 0;
	

	ADC_INTERRUPT_FLAG     = FALSE;
	ADC_INTERRUPT_ENABLE   = FALSE;	
		
}
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
BYTE ADCif_SampleADC(WORD wType)
{

	if(g_ADC_Status == ADC_SAMPLE_ON)
	{
			return FALSE;
	}

	ADCif_CloseADC();

	if(wType == ADC_TYPE_BATTERY)
	{
		ADCON0bits.CHS = 0x09; 	//AN9
	}
	else if(wType == ADC_TYPE_BANDGAP_REFERENCE)
	{
		ANCON1bits.VBGEN = TRUE; //Before measure the bandgap need to turn on Vbg ref
		DelayMs(20);	//Based on device datasheet need to wait at least 10ms
		ADCON0bits.CHS = 0x0F;	//bandgap references
	}

	g_ADC_SampleType = wType;
	g_ADC_SampleTime = 0;

	g_ADC_SampleValue[0] = 0;
	g_ADC_SampleValue[1] = 0;
	g_ADC_SampleValue[2] = 0;

	g_ADC_Status = ADC_SAMPLE_ON;

	ADC_INTERRUPT_FLAG      = FALSE;
	ADC_INTERRUPT_ENABLE = TRUE;

	ADC_TURN_ON_MODULE;
	ADC_START_AUTO_SAMPLE;

	return TRUE;
	
}
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
void ADC_InterruptHandler(void)
{

	if(g_ADC_SampleTime <  (ADC_SAMPLE_TIME-1))
	{				
				
		if(g_ADC_SampleType == ADC_TYPE_BATTERY ||
			g_ADC_SampleType == ADC_TYPE_BANDGAP_REFERENCE)
		{
			g_ADC_SampleValue[g_ADC_SampleTime] = ((WORD) ADRESH << 8) | ADRESL; 
		}
				
		g_ADC_SampleTime++;

		PIR1bits.ADIF = 0;
				
		ADC_START_AUTO_SAMPLE;
	}
	else
	{
		if(g_ADC_SampleType == ADC_TYPE_BATTERY ||
			g_ADC_SampleType == ADC_TYPE_BANDGAP_REFERENCE)
		{
			g_ADC_SampleValue[g_ADC_SampleTime] = ((WORD) ADRESH << 8) | ADRESL; 
		}

		g_ADC_SampleFinish = TRUE;

		g_ADC_SampleFinishType = g_ADC_SampleType;

		if(Powerif_GetBatterySampleState()  == FALSE ||
			Powerif_GetBandGapSampleState() == FALSE)
		{
			ADCif_NewEventHandler();
		}
		

		ADCif_CloseADC();		
	}
}
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
void ADCif_NewEventHandler(void)
{
	if(TRUE == g_ADC_SampleFinish)
	{
		WORD wValue;

		g_ADC_SampleFinish = FALSE;

		wValue = (g_ADC_SampleValue[0] + g_ADC_SampleValue[1]  + g_ADC_SampleValue[2])/3; 

		if(g_ADC_SampleFinishType ==  ADC_TYPE_BATTERY)
		{
			Powerif_SetBatteryValue(wValue);
		}
		else if(g_ADC_SampleFinishType == ADC_TYPE_BANDGAP_REFERENCE)
		{
			Powerif_HandleBandGapRef(wValue);
		}
			
	}	
}


