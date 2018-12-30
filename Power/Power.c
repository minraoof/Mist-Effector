//*****************************************************************************
// INCLUDES
//*****************************************************************************
#define POWER_IMPORTED
#include "Power.h"
#include "UI.h"
#include "USB_Module.h"
#include "RTC.h"
#include "ADC.h"
#include "delay.h"
#include "Flash.h"

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
void Power_Initialize(void)
{
	CHAR Temp = 0;
	
	POWER_CHARGING_INPUT_SELECT_PORT = OUTPUT;
	POWER_CHARGING_INPUT_SELECT = TRUE;	//SEL = TRUE (Charging from Adaptor Charging Current 1A)
										//SEL = FALSE(Charging from PC USB port)
	POWER_CHARGING_TIMER_ENABLE_PORT = OUTPUT;
	POWER_CHARGING_TIMER_ENABLE = TRUE;

	POWER_CHARGING_POWER_GOOD_PORT = INPUT;

	POWER_CHARGING_STAT1_PORT = INPUT;
	POWER_CHARGING_STAT2_PORT = INPUT;

	POWER_CHARGING_PROG2_PORT = OUTPUT;
	POWER_CHARGING_PROG2 = TRUE;

	POWER_CHARGING_ENABLE_PORT = TRUE;

	g_Power_LowBatteryThreshold = POWER_BATTERY_GAUGE_1_4;
	g_Power_FullBatteryThreshold = POWER_BATTERY_4V1_ADC;

	UI_LED_RED_ON;
	UI_LED_BLUE_ON;
	UI_LED_YELLOWGREEN_ON;
	
	DelayMs(100);

	g_Power_BandGapSample = FALSE;

	ADCif_SampleADC(ADC_TYPE_BANDGAP_REFERENCE);

	INTCONbits.PEIE = TRUE;
	INTCONbits.GIE  = TRUE;	

	while(g_Power_BandGapSample == FALSE)
			;
	
	
	ADCif_SampleADC(ADC_TYPE_BATTERY);	// For Power on Battery Measurement

//	INTCONbits.PEIE = TRUE;
//	INTCONbits.GIE  = TRUE;	
	
	g_Power_BatterySample = FALSE;
	
	while(g_Power_BatterySample == FALSE)
			;

	UI_LED_RED_OFF;
	UI_LED_BLUE_OFF;
	UI_LED_YELLOWGREEN_OFF;
	
	//Need to ADD ad measure
	
	g_Power_SampleCount = 0;
	g_Power_ShutDown = FALSE;
	g_Full_Charged  = FALSE;
	g_Power_FullChargeTimeout = FALSE;
	g_Power_ChargingSprayTurnonStatus = FALSE;

	RTCif_SetBatterySampleTime(60);
	
	RTCif_SetBatteryTemperatureCheckTime(60); //Battery Temperature Checking time every one minutes
}
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
void Powerif_SetBatteryValue(WORD wValue)
{

	if(g_Power_BatterySample)
	{
		g_Power_TempBattery[g_Power_SampleCount] = wValue;
	
		g_Power_SampleCount++;

		if(g_Power_SampleCount >= 3)
		{
			Power_SampleBatteryAlgorithm();
			
			g_Power_SampleCount = 0;

			Module_AddSignal(
				MODULE_UI,
				UI_EVENT_BATTERY_HANDLER,
				NULL
				);

		}
		else
		{
			ADCif_SampleADC(ADC_TYPE_BATTERY);
		}	
	}
	else
	{
		g_Power_CurrentVoltage = wValue;
			
		g_Power_BatterySample = TRUE;		
	}
}

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
void Power_SampleBatteryAlgorithm(void)
{
	WORD wTemp;

	if (g_Power_TempBattery[0] < POWER_BATTERY_GAUGE_1_4 ||
		g_Power_TempBattery[1] < POWER_BATTERY_GAUGE_1_4 ||
		g_Power_TempBattery[2] < POWER_BATTERY_GAUGE_1_4)
	{
		wTemp = (g_Power_TempBattery[0] + g_Power_TempBattery[1] + g_Power_TempBattery[2]) / 3;
	}
	else if (g_Power_TempBattery[1] <= g_Power_TempBattery[0])
	{
		if (g_Power_TempBattery[0] - g_Power_TempBattery[1] <= POWER_BATTERY_DRAIN_THRESHOLD)
		{
			if (g_Power_TempBattery[2] <= g_Power_TempBattery[1] )
			{
				if (g_Power_TempBattery[1] - g_Power_TempBattery[2] <= POWER_BATTERY_DRAIN_THRESHOLD)
				{
					wTemp = (g_Power_TempBattery[0] + g_Power_TempBattery[1] + g_Power_TempBattery[2]) / 3;
				}
				else
				{
					wTemp = (g_Power_TempBattery[0] + g_Power_TempBattery[1]) / 2;
				}
			}
			else
			{
				if (g_Power_TempBattery[2] <= g_Power_TempBattery[0])
				{
					wTemp = (g_Power_TempBattery[0] + g_Power_TempBattery[1] + g_Power_TempBattery[2]) / 3;
				}
				else
				{
					if (g_Power_TempBattery[2] - g_Power_TempBattery[0] > POWER_BATTERY_DRAIN_THRESHOLD)
					{
						wTemp = g_Power_TempBattery[2];
					}
					else
					{
						wTemp = (g_Power_TempBattery[0] + g_Power_TempBattery[1] + g_Power_TempBattery[2]) / 3;
					}
				}
			}
		}
		else
		{
			if (g_Power_TempBattery[2] <= g_Power_TempBattery[1] )
			{
				wTemp = g_Power_TempBattery[0];
			}
			else
			{
				if (g_Power_TempBattery[2] <= g_Power_TempBattery[0])
				{
					if (g_Power_TempBattery[0] - g_Power_TempBattery[2] > POWER_BATTERY_DRAIN_THRESHOLD)
					{
						wTemp = g_Power_TempBattery[0];
					}
					else
					{
						wTemp = (g_Power_TempBattery[0] + g_Power_TempBattery[2]) / 2;
					}
				}
				else
				{
					if (g_Power_TempBattery[2] - g_Power_TempBattery[0] <= POWER_BATTERY_DRAIN_THRESHOLD)
					{
						wTemp = (g_Power_TempBattery[0] + g_Power_TempBattery[2]) / 2;
					}
					else
					{
						wTemp = g_Power_TempBattery[2];
					}
				}
			}
		}
	}
	else
	{
		if (g_Power_TempBattery[1] - g_Power_TempBattery[0] <= POWER_BATTERY_DRAIN_THRESHOLD)
		{
			if (g_Power_TempBattery[2] > g_Power_TempBattery[1])
			{
				if (g_Power_TempBattery[2] - g_Power_TempBattery[1] <= POWER_BATTERY_DRAIN_THRESHOLD)
				{
					wTemp = (g_Power_TempBattery[0] + g_Power_TempBattery[1] + g_Power_TempBattery[2]) / 3;
				}
				else
				{
					wTemp = g_Power_TempBattery[2];
				}
			}
			else
			{
				if (g_Power_TempBattery[0] > g_Power_TempBattery[2])
				{
					if (g_Power_TempBattery[0] - g_Power_TempBattery[2] <= POWER_BATTERY_DRAIN_THRESHOLD)
					{
						wTemp = (g_Power_TempBattery[0] + g_Power_TempBattery[1] + g_Power_TempBattery[2]) / 3;
					}
					else
					{
						wTemp = (g_Power_TempBattery[0] + g_Power_TempBattery[1]) / 2;
					}
				}
				else
				{
					wTemp = (g_Power_TempBattery[0] + g_Power_TempBattery[1] + g_Power_TempBattery[2]) / 3;
				}
			}
		}
		else
		{
			if (g_Power_TempBattery[2] > g_Power_TempBattery[1])
			{
				if (g_Power_TempBattery[2] - g_Power_TempBattery[1] <= POWER_BATTERY_DRAIN_THRESHOLD)
				{
					wTemp = (g_Power_TempBattery[1] + g_Power_TempBattery[2]) / 2;
				}
				else
				{
					wTemp = g_Power_TempBattery[2];
				}
			}
			else
			{
				if (g_Power_TempBattery[0] > g_Power_TempBattery[2])
				{
					wTemp = g_Power_TempBattery[1];
				}
				else
				{
					if (g_Power_TempBattery[1] - g_Power_TempBattery[2] <= POWER_BATTERY_DRAIN_THRESHOLD)
					{
						wTemp = (g_Power_TempBattery[1] + g_Power_TempBattery[2]) / 2;
					}
					else
					{
						wTemp = g_Power_TempBattery[1];
					}
				}
			}
		}
	}

	g_Power_CurrentVoltage = wTemp;

	// Start the full charge timer
	if (g_Power_CurrentVoltage >= g_Power_FullBatteryThreshold &&
		RTCif_GetFullChargingTime() == 0 &&
		FALSE == g_Power_FullChargeTimeout)
	{
		RTCif_SetFullChargingTime(3600);
	}
	else if (g_Power_CurrentVoltage < g_Power_FullBatteryThreshold &&
		RTCif_GetFullChargingTime() != 0)
	{
		Powerif_ClearFullChargeFlag();
	}
	
	if (g_Power_CurrentVoltage > g_Power_FullBatteryThreshold &&
		TRUE == USB_SENSE)
	{
		g_Full_Charged = TRUE;
	}
	else
	{
		g_Full_Charged = FALSE;
	}
	
	if(g_Power_FullChargeTimeout == TRUE ||
		Power_GetChipStatus() == 2)
	{
		UIif_SetPowerFullStatus(TRUE);
	}
}
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
BYTE Powerif_IsLowPowerState(void)
{
	if(g_Power_LowBatteryThreshold == 0)
	{
		return FALSE;
	}

	if(g_Power_CurrentVoltage <= g_Power_LowBatteryThreshold)
	{
		return TRUE;
	}

	return FALSE;
}

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
BYTE Powerif_IsBatteryFullCharged(void)
{
	return g_Full_Charged;
}
/********************************************************************
 * Function:		Powerif_SetBatteryFullChargedStatus()
 *
 * PreCondition:	None
 *
 * Input:			BYTE: Set the Full charged Status
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		Clear the Status of Battery Full Charge
 *                  
 * Note:			None
 *******************************************************************/
void Powerif_SetBatteryFullChargedStatus(BYTE yVal)
{
	g_Full_Charged = yVal;
}	
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
BYTE Powerif_GetBatteryLevel(void)
{
	BYTE yLevel = 4;

	if (g_Power_CurrentVoltage < POWER_BATTERY_GAUGE_1_4)
	{
		yLevel = 0;
	}
	else if (g_Power_CurrentVoltage < POWER_BATTERY_GAUGE_2_4)
	{
		yLevel = 1;
	}
	else if (g_Power_CurrentVoltage < POWER_BATTERY_GAUGE_3_4)
	{
		yLevel = 2;
	}	
	else if (g_Power_CurrentVoltage < POWER_BATTERY_GAUGE_FULL)
	{
		yLevel = 3;
	}
	
	return yLevel;
}
/********************************************************************
 * Function:		Powerif_IsBatteryBatterySprayLevel()
 *
 * PreCondition:	None
 *
 * Input:			NONE
 *
 * Output:			BYTE: Battery level status
 *
 * Side Effects:	None
 *
 * Overview:		When battey is 3.8V above return value is true
 *                  
 * Note:			None
 *******************************************************************/
BYTE Powerif_IsBatterySprayLevel(void)
{

	if(g_Power_CurrentVoltage >= POWER_BATTERY_3V95_ADC)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
}

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
BYTE Powerif_CheckBatteryICTemperature(void)
{
	BYTE yResult=0;
		
	yResult |= POWER_CHARGING_STAT1;
	yResult <<= 1;
	yResult |= POWER_CHARGING_STAT2;
	yResult <<=1;
	yResult |= POWER_CHARGING_POWER_GOOD;
		
		
	if(yResult ==  POWER_IC_TEMPERATURE_FAULT)
	{
		return TRUE;
	}

	return FALSE;
}	
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
BYTE Powerif_GetBatterySampleState(void)
{
	return g_Power_BatterySample ;
}
/********************************************************************
 * Function:		Powerif_GetBatterySampleState()
 *
 * PreCondition:	None
 *
 * Input:				BYTE : Sample state True or False
 *
 * Output:		BYTE: The battery sample state
 *
 * Side Effects:	None
 *
 * Overview:		when plug out the USB battery need to sample
 *                  
 * Note:			None
 *******************************************************************/
 void Powerif_SetBatterySampleState(BYTE yVal)
{
	g_Power_BatterySample = yVal;
}
/********************************************************************
 * Function:		Powerif_SetSprayTurnonStatus()
 *
 * PreCondition:	None
 *
 * Input:			BYTE: Set the Spray Turn on Status
 *
 * Output:			None
 *
 * Side Effects:	None
 *
 * Overview:		When Adpator plugin need to set this flag,and
 *					Clear when battery volt is above 3.8 volt.
 *                  
 * Note:			None
 *******************************************************************/
void Powerif_SetChargingSprayTurnonStatus(BYTE yVal)
{
	g_Power_ChargingSprayTurnonStatus = yVal;
}
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
BYTE Powerif_GetChargingSprayTurnonStatus(void)
{
	return g_Power_ChargingSprayTurnonStatus;
}
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
BYTE Powerif_CheckToClearLowBatFlag(void)
{

	if(g_Power_LowBatteryThreshold == 0)
	{
		return FALSE;
	}

	if(g_Power_CurrentVoltage >= (g_Power_LowBatteryThreshold+15)) 
	{
		return TRUE;
	}

	return FALSE;
}
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
 * Overview:		To Sample the Band Gap voltage references ADC value to get accuracy
 *					in measurement of Low Battery Voltage.
 *				  
 * Note:			None
*******************************************************************/
void Powerif_HandleBandGapRef(WORD wValue)
{
	WORD wVolt = wValue;
	
	g_Power_LowBatteryThreshold = wVolt + 186; // BandGap Volt 1.2 + 0.6 = 1.8(Low Battery Volt)
	
	g_Power_FullBatteryThreshold = wVolt + 265; //BandGap Volt 1.2 + 0.855 = 2.055(Full Battery Volt)	

	if(g_Power_BandGapSample == FALSE)
	{
		g_Power_BandGapSample = TRUE;
	}
	
	ANCON1bits.VBGEN = FALSE; // Turn off Band Gap volt
}
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
BYTE Powerif_GetBandGapSampleState(void)
{
	return g_Power_BandGapSample;
}
/********************************************************************
 * Function:		Powerif_GetLowBatReadFlashData()
 *
 * PreCondition:	None
 *
 * Input:			None
 *
 * Output:			BYTE: Return the address of 0x7008 of MCU Flash
 *					if Low Bat occur before slide switch off return TRUE
					else return FALSE
 *
 * Side Effects:	None
 *
 * Overview:		If LowBat happan once during power on return TRUE
 *					If no LowBat happan during power on return FALSE
 *				  
 * Note:			None
*******************************************************************/
BYTE Powerif_GetLowBatReadFlashData(void)
{
	BYTE yResult = FALSE;
	BYTE yVal;

	Flash_Read(MCUFLASH_ADDRESS_LOWBAT,1,&yVal);

	if(yVal == FLASH_LOWBAT_TYPE)
	{
		yResult = TRUE;
	}
	else if(yVal == FLASH_NOLOWBAT_TYPE)
	{
		yResult = FALSE;
	}
	else
	{
		Powerif_SetLowBatFlashData(FLASH_NOLOWBAT_TYPE);
		yResult = FALSE;
	}
	
	return yResult;
}
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
void Powerif_SetLowBatFlashData(BYTE yVal)
{
	BYTE yTemp;

	Flash_Read(MCUFLASH_ADDRESS_LOWBAT,1,&yTemp);

	if(yVal != yTemp)
	{
		Flash_Erase(MCUFLASH_ADDRESS_START_ERASE,MCUFLASH_ADDRESS_START_ERASE + FLASH_ERASE_BLOCK);

		Flash_WordWrite(MCUFLASH_ADDRESS_LOWBAT,yVal);
	}
}
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
void Powerif_ClearFullChargeFlag(void)
{
	g_Power_FullChargeTimeout = FALSE;
	RTCif_SetFullChargingTime(0);
}
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
void Powerif_SetFullChargeTimeFlag(BYTE yValue)
{
	g_Power_FullChargeTimeout = yValue;
}
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
BYTE Power_GetChipStatus(void)
{
	BYTE yResult = 0;

	yResult |= POWER_CHARGING_POWER_GOOD;
	yResult <<= 1;

	yResult |= POWER_CHARGING_STAT1;
	yResult <<= 1;

	yResult |= POWER_CHARGING_STAT2;
	//yResult <<= 1;

	return yResult;
}


