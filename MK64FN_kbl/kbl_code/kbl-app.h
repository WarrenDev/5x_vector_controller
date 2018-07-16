
/**
* kbl-app.h
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief MK64FN kernel boot loader
*/

#ifndef KBL_CODE_KBL_APP_H_
#define KBL_CODE_KBL_APP_H_

#include <stdint.h>

#define FIRMWARE_CONTROL_LOCATION  (0x0067C000)
#define K64_IMAGE_LOCATION         (0x00700000)
#define K64_DEFAULT_APP_LOCATION   (0x00006000)

/**
* Boot loader control State
*/
typedef enum kbl_state_def
{
    FU_STATE_UNKNOWN        	= 0,
    FU_STATE_BOOT           	= 1,
    FU_STATE_FIRMWARE_UPGRADE	= 2,
    FU_STATE_FACTORY_RESET  	= 3,
    FU_STATE_FIRST_BOOT     	= 0xFF,
} kbl_state_t;

/**
* Boot loader control structure
*/
struct kbl_control_block_def
{
	uint8_t       		state;                  // State (1=boot, 2=upgrade K64, 3=factory reset)
    uint8_t             aesKey[32];             // AES key
    uint8_t             aesIV[16];              // AES IV
    uint32_t            imageSize;              // Image size
    uint32_t            imageVersion;           // Image version
    uint32_t            imageDestinationAddr;   // Image destination address in program flash
    uint32_t            imageSourceAddr;        // Image source address
    uint32_t            appAddr;                // Application jump address
    uint8_t             reserved[59];           // Reserved (pad to 128 bytes)
} __attribute__((__packed__));
typedef struct kbl_control_block_def kbl_control_block_t;

/**
* kbl_run
*
* Run the kernel boot loader
*
* @param none
*
* @return none
*/
void kbl_run ( void );

/**
* kbl_boot
*
* Jump to and start the application
*
* @param appAddr		Address location of the application
*
* @return none
*/
void kbl_boot ( uint32_t appAddr);

/**
* kbl_perform_firmware_upgrade
*
* Perform the firmware upgrade process
*
* @param cblk			Pointer to the boot loader control block
*
* @return none
*/
void kbl_perform_firmware_upgrade ( kbl_control_block_t* cblk );

/**
* kbl_perform_factory_reset
*
* Perform the factory reset process
*
* @param cblk			Pointer to the boot loader control block
*
* @return none
*/
void kbl_perform_factory_reset ( kbl_control_block_t* cblk );

/**
* kbl_read_control_block
*
* Load the control block from non-volatile memory
*
* @param cblk			Pointer to the boot loader control block
*
* @return none
*/
void kbl_read_control_block ( kbl_control_block_t* cblk );

/**
* kbl_write_control_block
*
* Store the control block in non-volatile memory
*
* @param cblk			Pointer to the boot loader control block
*
* @return none
*/
void kbl_write_control_block ( kbl_control_block_t* cblk );

#endif /* KBL_CODE_KBL_APP_H_ */
