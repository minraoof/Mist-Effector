//*****************************************************************************
// INCLUDES
//*****************************************************************************
#define FLASH_IMPORTED
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
 /*********************************************************************
 Function:        	void Flash_Erase(unsigned long startaddr, unsigned long endaddr)

 PreCondition:    	None
                  
 Input:           	startaddr - Strating address from which flash has to be erased
			endaddr - End address till which flash has to be erased
 
 Output:          	None
 
 Side Effects:    	Flash will be erased in blocks of number of bytes specified in device data sheet
 
 Overview:        	The function erases flash from starting address in terms of number of bytes specified in device data sheet
			till end address or nearest multiple of number of bytes specified in device data sheet
                   
 Note:            	1. If number of bytes between strating and end address in not in multiples 
			    of number of bytes specified in device data sheet, then excessive memory is erased upto nearest next 
			    multiple of number of bytes specified in device data sheet
			2. The starting and end address has to be in blocks of number of bytes specified in device data sheet
			     else function will allign the address to nearest previous and next 
			     number of bytes specified in device data sheet alligned address respectively
 ********************************************************************/
void Flash_Erase(DWORD startaddr, DWORD endaddr)
{
	CHAR yFlag = FALSE;
	UINT32_VAL flash_addr;

	flash_addr.Val = startaddr;

	while(flash_addr.Val < endaddr)
	{
		TBLPTRU = flash_addr.byte.UB;	//Load the address to Address pointer registers
		TBLPTRH = flash_addr.byte.HB;
		TBLPTRL = flash_addr.byte.LB;
		//*********Flash Erase sequence*****************
		EECON1bits.WREN = TRUE;
		EECON1bits.FREE = TRUE;
		if(INTCONbits.GIE)
		{
			INTCONbits.GIE = FALSE;
			yFlag = TRUE;
		}
		EECON2 = 0x55;
		EECON2 = 0xAA;
		EECON1bits.WR = TRUE;
		//EECON1 |= 0x02;
		
		if(yFlag)
		{
			INTCONbits.GIE = TRUE;
		}

		while(EECON1bits.WR == TRUE)
				;

		flash_addr.Val = flash_addr.Val + FLASH_ERASE_BLOCK;
	}

}
/*********************************************************************
 Function:        	void Flash_Read(unsigned long startaddr, unsigned int num_bytes, unsigned char *flash_array)

 PreCondition:    	None
                  
 Input:           	startaddr - Strating address from which flash has to be read
			num_bytes - Number of bytes of flash to be read
			*flash_array - Pointer to array to which the flash has be read
 
 Output:          	Reads the flash content to array passed as pointer
 
 Side Effects:    	None
 
 Overview:        	The function reads flash for number of bytes passed as parameter from starting address 
                   
 Note:            	Non zero number of bytes has to be passed as parameter for num_bytes
 ********************************************************************/
void Flash_Read(DWORD startaddr, DWORD num_bytes, PBYTE flash_array)
{
	UINT32_VAL	flash_addr;

	flash_addr.Val = startaddr;

		TBLPTRU = flash_addr.byte.UB;
		TBLPTRH	= flash_addr.byte.HB;
		TBLPTRL	= flash_addr.byte.LB;

		while(num_bytes--)
		{
			//***********Table read sequence*********
			_asm TBLRDPOSTINC _endasm
			*flash_array++ = TABLAT;
		}
}

/*********************************************************************
 Function:        	void Flash_WordWrite(unsigned long startaddr, unsigned int data)

 PreCondition:    	None
                  
 Input:           	startaddr - Strating address from which flash has to be written
			data - Data to be written into flash
						
 Output:          	None
 
 Side Effects:    	None
 
 Overview:        	The function writes word to flash 
                   
 Note:            	1. Necessary to erase flash block (number of bytes specified in device data sheet) exclusively in application before writing 
			   if application had written data into to this block of flash(after erasing followed by programming).
			2. Starting address has to be an even address else boundary mismatch will occur 
			3. Writing can be done directly without erase if these bytes of flash is being written for the first time after programming
 ********************************************************************/
void Flash_WordWrite(unsigned long startaddr, unsigned int data)
{
	CHAR	yFlag = 0;
	UINT32_VAL flash_addr;
	UINT16_VAL flash_data;

		flash_addr.Val = startaddr;
		flash_data.Val = data;

		TBLPTRU = flash_addr.byte.UB;		//Load the address to Address pointer registers
		TBLPTRH = flash_addr.byte.HB;
		TBLPTRL = flash_addr.byte.LB;

		TABLAT = flash_data.byte.LB;
		_asm TBLWTPOSTINC _endasm

		TABLAT = flash_data.byte.HB;
		_asm TBLWT _endasm

		TBLPTRU = flash_addr.byte.UB;		//Load the address to Address pointer registers
		TBLPTRH = flash_addr.byte.HB;
		TBLPTRL = flash_addr.byte.LB;

		//************* Flash write sequence ***************
		EECON1bits.WPROG = 1;
		EECON1bits.WREN = 1;

		if(INTCONbits.GIE)
		{
			INTCONbits.GIE = FALSE;
			yFlag = TRUE;
		}
		
		EECON2 = 0x55;
		EECON2 = 0xAA;
		EECON1bits.WR = 1;
		//EECON1 |= 0x02;
		if(yFlag)
		{
			INTCONbits.GIE = TRUE;
		}

		while(EECON1bits.WR == TRUE)
				;
}

