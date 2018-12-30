//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************

#include "Isr.h"
#include "GenericTypeDefs.h"
#include "salloc.h"
#include "Module.h"
#include "UI.h"
#include "Main.h"
#include "Key.h"
#include "Power.h"
#include "USB_Module.h"
#include "ADC.h"
#include "RTC.h"
#include "Flash.h"


//CONFIG1L
#pragma config WDTEN  = OFF			//Watchdog Timer Off
#pragma config PLLDIV = 3 			//PLLDIV -- PLL Prescaler Selection bits 3
#pragma config XINST = OFF 			// Extended Instruction Set 


//CONFIG1H
#pragma config CPUDIV = OSC2_PLL2 	// CPU System Clock Postscaler
#pragma config CP0    = OFF 		// Code Protect

//CONFIG2L
#pragma config OSC = HSPLL 			//Oscillator
#pragma config T1DIG = OFF 			//T1OSCEN Enforcement
//#pragma config LPT1OSC = OFF 		//Low-Power Timer1 Oscillator
#pragma config FCMEN = OFF 			//Fail-Safe Clock Monitor
#pragma config IESO	= OFF 			//Internal External Oscillator Switch Over Mode

//CONFIG2H
#pragma config WDTPS = 8192 		//Watchdog Postscaler

//CONFIG3L
//#pragma config DSWDTOSC = T1OSCREF //DSWDT Clock Select 
#pragma config RTCOSC   = T1OSCREF 	// RTCC Clock Select
#pragma config DSBOREN  = OFF 		// Deep Sleep BOR

//CONFIG3H
#pragma config IOL1WAY = OFF


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
* Function:		main()
*
* PreCondition:	None
*
* Input:			None
*
* Output:			None
*
* Side Effects:	None
*
* Overview:		The start of the firmware
*
* Note:			None
*******************************************************************/
 void main()
{
	Main_SystemInitialize();

	while(1)
	{			 
		Module_HandleSignal();

		Main_HandlePowerSave();

		RTC_TimerHandler();
		RTC_TimeOutHandler();

		KeyIf_NewEventHandler();

		ADCif_NewEventHandler();

		USB_EventHandler();		
		
		ClrWdt();

		UI_PollingPWM(); 
		
		RTCif_ClearWDT();
	}	
}

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
void Main_SystemInitialize(void)
{

	Main_InitPinRemap();
	 
	Main_InitPortSetting();	
		
	SRAMInitHeap();
	 
	Module_InitSignalQueue();	 
	
	ADC_Initialize();
	
	Power_Initialize();
	 
	RTC_Initialize();
	 
	Key_InitializeModule();	
	 
	UI_InitializeModule();	  
	 
	USB_InitializeModule();
		
}

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
void Main_InitPortSetting(void)
{

	if(RCONbits.POR)
	{
		RCON = 0;	
	}	
		
	ANCON0	= 0xFD;
	ANCON1  = 0xFF;  //AN9 Make Analog Input,ANCON0 and ANCON1 register wrong
	
			
	TRISEbits.TRISE2 = OUTPUT; //RE2
	PORTEbits.RE2  	 = FALSE;
		
	TRISEbits.TRISE1 = OUTPUT; //RE1
	PORTEbits.RE1    = FALSE;
		
	TRISAbits.TRISA2 = OUTPUT; //RA2
	PORTAbits.RA2    = FALSE;
		
	TRISAbits.TRISA3 = OUTPUT; //RA3
	PORTAbits.RA3    = FALSE;

	TRISDbits.TRISD7 = OUTPUT; //RD7
	PORTDbits.RD7    = FALSE;
		
	TRISCbits.TRISC7 = OUTPUT; //RC7
	PORTCbits.RC7	 = FALSE;
		
	TRISCbits.TRISC6 = OUTPUT; //RC6
	PORTCbits.RC6	 = FALSE;
		
	TRISCbits.TRISC2 = OUTPUT; //RC2
	PORTCbits.RC2    = FALSE;
	
	WDTCONbits.SWDTEN = TRUE ; //Watch dog timmer on
		
}

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
 * Overview:		Initialize the Pin Remap
 *
 * Note:			None
 *******************************************************************/
void Main_InitPinRemap(void)
{		
	INTCONbits.GIE = 0 ;	//Disable General I/O Interrupt

	EECON2 = 0x55;		//Unlock Register
	EECON2 = 0xAA;
	//_asm bsf PPSCON,0,BANKED _endasm
	PPSCONbits.IOLOCK = FALSE;
	RPINR1 = 0x07;  //RP7 (or)RB4 External Interrupt 1

	/*USB_SENSE*/
	//RPINR2 = 0x03;  //RP3 (or) RB0 External Interrupt 2	

	//RPOR0  = 0x0E;  //RP0 (or) RA0 PWM1

			
	//RPOR1 = 0x12;   //RP1 (or) RA1 PWM2	
		
	EECON2 = 0x55;		//Lock Register
	EECON2 = 0xAA;		

	//_asm bsf PPSCON,0,BANKED _endasm
	PPSCONbits.IOLOCK = TRUE;
}

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
void Main_HandlePowerSave(void)
{
	if(USBif_GetUSBState() == TRUE)
	{

	}
	else
	{
		Main_CheckSystemIdleState();
	}
}
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
void Main_CheckSystemIdleState(void)
{

	do{
		if(Module_CheckQueueEmpty()  == FALSE)
		{
			break;
		}
						
		if(UIif_GetModuleState()  != UI_STATE_IDLE &&
		   UIif_GetModuleState()  != UI_STATE_FACTORY_TESTING)
		{
			break;
		}			

		if(T2CONbits.TMR2ON == TRUE ||
		   T4CONbits.TMR4ON == TRUE ||
		   T1CONbits.TMR1ON == TRUE)
		{
			break;
		}
			
		if(UI_LED_FAST_BLINKING_ENABLE == TRUE)
		{
			break;	
		}	

		if(KEY_DEBOUNCE_TIMER_ENABLE == TRUE ||
		    Keyif_GetKeyState() != KEY_STATE_RELEASE)
		{
			break;
		}			  
		
		Sleep();
	}while(0);
}



