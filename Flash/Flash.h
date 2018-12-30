#ifndef FLASH_H
#define FLASH_H

//*****************************************************************************
// INCLUDE FILES
//*****************************************************************************

#include "GenericTypeDefs.h"
#include "Main.h"


//*****************************************************************************
// MACROS
//*****************************************************************************

#ifdef  FLASH_IMPORTED
#define FLASH_TYPE
#else
#define FLASH_TYPE extern
#endif

#define FLASH_ERASE_BLOCK 512
#define MCUFLASH_ADDRESS_START_ERASE 0x7000
#define MCUFLASH_ADDRESS_LOWBAT	0x7000

#define FLASH_LOWBAT_TYPE	  0xC2
#define FLASH_NOLOWBAT_TYPE 0xC3

//*****************************************************************************
// VARIABLES
//*****************************************************************************




//*****************************************************************************
// FUNCTION DEFINITION
//*****************************************************************************
 /*********************************************************************
 Function:        	void FLASH_Erase(unsigned long startaddr, unsigned long endaddr)

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
void Flash_Erase(DWORD startaddr, DWORD endaddr);

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
void Flash_Read(DWORD startaddr, DWORD num_bytes, PBYTE flash_array);

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
void Flash_WordWrite(unsigned long startaddr, unsigned int data);

#endif

