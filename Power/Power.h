#ifndef POWER_H
#define POWER_H

//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************

#include "GenericTypeDefs.h"
#include "Main.h"


//*****************************************************************************
// MACROS
//*****************************************************************************
#ifdef POWER_IMPORTED
#define POWER_TYPE
#else 
#define POWER_TYPE extern
#endif


#define POWER_CHARGING_INPUT_SELECT_PORT			TRISDbits.TRISD0
#define POWER_CHARGING_INPUT_SELECT					PORTDbits.RD0//LATDbits.LATD0		//	0 is from USB, 1 is from AC adapter

#define POWER_CHARGING_TIMER_ENABLE_PORT			TRISDbits.TRISD4
#define POWER_CHARGING_TIMER_ENABLE					PORTDbits.RD4//LATDbits.LATD4

#define POWER_CHARGING_POWER_GOOD_PORT			TRISDbits.TRISD2
#define POWER_CHARGING_POWER_GOOD				PORTDbits.RD2 //LATDbits.LATD2

#define POWER_CHARGING_ENABLE_PORT				TRISDbits.TRISD6
#define POWER_CHARGING_ENABLE					PORTDbits.RD6//LATDbits.LATD6

#define POWER_CHARGING_STAT1_PORT				TRISDbits.TRISD3
#define POWER_CHARGING_STAT1					PORTDbits.RD3 //LATDbits.LATD3

#define POWER_CHARGING_STAT2_PORT				TRISDbits.TRISD1
#define POWER_CHARGING_STAT2					PORTDbits.RD1  //LATDbits.LATD1

#define POWER_CHARGING_PROG2_PORT				TRISDbits.TRISD5
#define POWER_CHARGING_PROG2					PORTDbits.RD5 //LATDbits.LATD5

//#define POWER_EXTERNAL_POWER_PORT				_TRISB11
//#define POWER_EXTERNAL_POWER					_RB11

//#define POWER_MCU_OFF_PORT						//		_TRISG13
//#define POWER_MCU_OFF							//		_LATG13

#define POWER_BATTERY_GAUGE_1_4					561//567//569  //558		// Formaula (V*1023)/330, V=(v*100)/2 offset 11
#define POWER_BATTERY_GAUGE_2_4					580
#define POWER_BATTERY_GAUGE_3_4					589
#define POWER_BATTERY_GAUGE_FULL				605
#define POWER_BATTERY_DRAIN_THRESHOLD			4

#define POWER_BATTERY_4V1_ADC					639 //646 //652	//644
#define POWER_BATTERY_39V_ADC					604
#define POWER_BATTERY_38V_ADC					599
#define POWER_BATTERY_34V_ADC					527
#define POWER_BATTERY_3V75_ADC					580
#define POWER_BATTERY_3V93_ADC					614
#define POWER_BATTERY_3V95_ADC					622		//off set 10
#define POWER_BATTERY_3V96_ADC					624
#define POWER_IC_TEMPERATURE_FAULT				0
#define POWER_DEVICE_BANDGAP_ADC				372




//*****************************************************************************
// VARIABLES
//*****************************************************************************

POWER_TYPE BYTE	g_Power_BatterySample;
POWER_TYPE BYTE	g_Power_SampleCount;
POWER_TYPE BYTE	g_Power_ShutDown;
POWER_TYPE BYTE	g_Full_Charged;
POWER_TYPE BYTE	g_Power_LowBattery;
POWER_TYPE BYTE g_Power_ChargingSprayTurnonStatus;
POWER_TYPE BYTE g_Power_BandGapSample;
POWER_TYPE BYTE g_Power_FullChargeTimeout;

POWER_TYPE WORD	g_Power_CurrentVoltage;
POWER_TYPE WORD	g_Power_LowBatteryThreshold;
POWER_TYPE WORD g_Power_FullBatteryThreshold;
POWER_TYPE WORD	g_Power_TempBattery[3];





//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************

/********************************************************************
 * Function:		Power_Initialize()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Initialize the power module.
 *                  
 * Note:			None
 *******************************************************************/
void Power_Initialize(void);

/********************************************************************
 * Function:		Powerif_SetBatteryValue()
 *
 * PreCondition:	None
 *
 * Input:			WORD wValue: The ADC value sample by MCU.
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the ADC value. If this is called during initialization,
 *				it will not run the battery sample algorithm.
 *                  
 * Note:			None
 *******************************************************************/
void Powerif_SetBatteryValue(WORD wValue);
/********************************************************************
 * Function:		Power_SampleBatteryAlgorithm()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Calculate the voltage. With the three sample value to get
 *				the more accurate value.
 *                  
 * Note:			None
 *******************************************************************/
void Power_SampleBatteryAlgorithm(void);
/********************************************************************
 * Function:		Powerif_IsLowPowerState()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: The status for low battery.
 *
 * Side Effects:	None
 *
 * Overview:		Check the power module is in the low power state or not.
 *                  
 * Note:			None
 *******************************************************************/
BYTE Powerif_IsLowPowerState(void);

/********************************************************************
 * Function:		Powerif_IsBatteryFullCharged()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: The status for low battery.
 *
 * Side Effects:	None
 *
 * Overview:		Check the battery is full charged.
 *                  
 * Note:			None
 *******************************************************************/
BYTE Powerif_IsBatteryFullCharged(void);

/********************************************************************
 * Function:		Powerif_SetBatteryFullChargedStatus()
 *
 * PreCondition:	None
 *
 * Input:			:TRUE or FALSE
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		Clear the Status of Battery Full Charge
 *                  
 * Note:			None
 *******************************************************************/
void Powerif_SetBatteryFullChargedStatus(BYTE yVal);

/********************************************************************
 * Function:		Powerif_GetBatteryLevel()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: The battery level.
 *
 * Side Effects:	None
 *
 * Overview:		Check the voltage and return the battery level.
 *                  
 * Note:			None
 *******************************************************************/
BYTE Powerif_GetBatteryLevel(void);
/********************************************************************
 * Function:		Powerif_IsBatteryBatterySprayLevel()
 *
 * PreCondition:	None
 *
 * Input:			NONE
 *
 * Output:			If above 3.8V or equral return TRUE:
 *					else FALSE
 *
 * Side Effects:	None
 *
 * Overview:		When battey is 3.8V above return value is true
 *                  
 * Note:			None
 *******************************************************************/
BYTE Powerif_IsBatterySprayLevel(void);

/********************************************************************
 * Function:		Powerif_CheckICTemperature()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			BYTE - IC Status
 *
 * Side Effects:	None
 *
 * Overview:		Check the Charging IC Status Pin if temperature and timer fault
 					return TRUE
 *
 * Note:			None
 *******************************************************************/
BYTE Powerif_CheckBatteryICTemperature(void);
/********************************************************************
 * Function:		Powerif_GetBatterySampleState()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: The battery sample state
 *
 * Side Effects:	None
 *
 * Overview:		Get the state for the battery has sampled or not
 *                  
 * Note:			None
 *******************************************************************/
BYTE Powerif_GetBatterySampleState(void);
/********************************************************************
 * Function:		Powerif_SetSprayTurnonStatus()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: The battery sample state
 *
 * Side Effects:	None
 *
 * Overview:		Get the state for the battery has sampled or not
 *                  
 * Note:			None
 *******************************************************************/
void Powerif_SetChargingSprayTurnonStatus(BYTE yVal);
/********************************************************************
 * Function:		Powerif_GetSprayTurnonStatus()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			TRUE:FALSE
 *
 * Side Effects:	None
 *
 * Overview:		When Plug in adaptor,This Flag need set, clear when
 *					volt is above 3.8Volt
 *                  
 * Note:			None
 *******************************************************************/
BYTE Powerif_GetChargingSprayTurnonStatus(void);
/********************************************************************
 * Function:		Powerif_CheckToClearLowBatFlat()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: The Status to Clear the low Battery Flat
 *
 * Side Effects:	None
 *
 * Overview:		To Clear the Low battery Flat, resolution is 1V
 *                  
 * Note:			None
 *******************************************************************/
 BYTE Powerif_CheckToClearLowBatFlag(void);
/********************************************************************
 * Function:		Powerif_HandleBandGapRef()
 *
 * PreCondition:	None
 *
 * Input:			WORD: Band Gap Reference ADC value
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		To Sample the Band Gap references ADC to get accuracy
 *					in Low Battery Voltage.
 *				  
 * Note:			None
 *******************************************************************/
 void Powerif_HandleBandGapRef(WORD wValue);
/********************************************************************
 * Function:		Powerif_GetBandGapSampleState()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: Sample state of band gap references value
 *
 * Side Effects:	None
 *
 * Overview:		None
 *				  
 * Note:			None
*******************************************************************/
BYTE Powerif_GetBandGapSampleState(void);
/********************************************************************
 * Function:		Powerif_GetLowBatReadFlash()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			BYTE: Return the address of 0x7008 of MCU Flash
 *
 * Side Effects:	None
 *
 * Overview:		If LowBat happan once during power on return TRUE
 *					If no LowBat happan during power on return FALSE
 *				  
 * Note:			None
*******************************************************************/
BYTE Powerif_GetLowBatReadFlashData(void);
/********************************************************************
 * Function:		Powerif_SetLowBatFlashData()
 *
 * PreCondition:	None
 *
 * Input:			BYTE: Set the address of 0x7008 of MCU Flash
 *					if Low Bat occur before slide switch off set 0xC2
 *					else set 0xC3
 *
 * Output:			None
 *					
 *
 * Side Effects:	None
 *
 * Overview:		If LowBat happan once during power on return TRUE
 *					If no LowBat happan during power on return FALSE
 *				  
 * Note:			None
*******************************************************************/
void Powerif_SetLowBatFlashData(BYTE yVal);
/********************************************************************
 * Function:		Powerif_ClearFullChargeFlag()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Clear the full charged flag when losing the power source
 *
 * Note:			None
 *******************************************************************/
void Powerif_ClearFullChargeFlag(void);


/********************************************************************
 * Function:		Powerif_SetFullChargeTimeFlag()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		None
 *
 * Side Effects:	None
 *
 * Overview:		Set the full charged flag
 *
 * Note:			None
 *******************************************************************/
void Powerif_SetFullChargeTimeFlag(BYTE yValue);
/********************************************************************
 * Function:		Power_GetChipStatus()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:		BYTE: The charging chip status.
 *
 * Side Effects:	None
 *
 * Overview:		Get the chip status to check the AC adapter and other parts.
 *                  
 * Note:			None
 *******************************************************************/
BYTE Power_GetChipStatus(void);

#endif

