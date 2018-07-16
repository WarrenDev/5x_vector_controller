
/**
* kbl-app.cpp
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief MK64FN kernel boot loader
*/

#include "fsl_device_registers.h"
#include "bsp.h"
#include "kbl-app.h"

/**
* kbl_run
*
* Run the kernel boot loader
*
* @param none
*
* @return none
*/
void kbl_run ( void )
{
	kbl_control_block_t cblk;
	kbl_read_control_block ( &cblk );

    switch( cblk.state )
    {
        case FU_STATE_FIRMWARE_UPGRADE:
        	kbl_perform_firmware_upgrade ( &cblk );
        	break;
        case FU_STATE_FACTORY_RESET:
        	kbl_perform_factory_reset ( &cblk );
        	break;
        default:
        case FU_STATE_BOOT:
        	kbl_boot( cblk.appAddr );
        	break;
    }

	/**
	 * Should never get here
	 */
	while ( 1 ) ;
}

/**
* kbl_perform_firmware_upgrade
*
* Perform the firmware upgrade process
*
* @param cblk			Pointer to the boot loader control block
*
* @return none
*/
void kbl_perform_firmware_upgrade ( kbl_control_block_t* cblk )
{
}

/**
* kbl_perform_factory_reset
*
* Perform the factory reset process
*
* @param cblk			Pointer to the boot loader control block
*
* @return none
*/
void kbl_perform_factory_reset ( kbl_control_block_t* cblk )
{
}

/**
* kbl_boot
*
* Jump to and start the application
*
* @param appAddr		Address location of the application
*
* @return none
*/
void kbl_boot ( uint32_t appAddr )
{
    void (*appCode)(void);

    SCB->VTOR = ((appAddr) & 0x1FFFFF80);

    /**
    * Load the reset handler address in the application vector table
    * Address at this location should point to a thumb location, no need for a +1.
    */
    unsigned* p = (unsigned *)(appAddr + 4);

    appCode = (void (*)(void))*p;

    /**
    * Jump to user application
    */
    appCode();
}

/**
* kbl_read_control_block
*
* Load the control block from non-volatile memory
*
* @param cblk		Pointer to the control block
*
* @return none
*/
void kbl_read_control_block ( kbl_control_block_t* cblk )
{
	/**
	* Load values for boot loader testing (temporary)
	*/
	cblk->state = FU_STATE_BOOT;
	cblk->appAddr = K64_DEFAULT_APP_LOCATION;
}

/**
* kbl_write_control_block
*
* Store the control block in non-volatile memory
*
* @param cblk		Pointer to the control block
*
* @return none
*/
void kbl_write_control_block ( kbl_control_block_t* cblk )
{
}
