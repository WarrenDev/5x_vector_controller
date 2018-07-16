
/**
* serial_flash.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used with serial flash support.
*/

#ifndef SRC_SERVICES_DATABASE_DATABASE_CORE_SERIAL_FLASH_H_
#define SRC_SERVICES_DATABASE_DATABASE_CORE_SERIAL_FLASH_H_

#include <aef/embedded/driver/spi/spi_driver.h>
#include <aef/embedded/driver/stream_driver.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

# ifdef   __cplusplus
extern "C" {
# endif

#define SPI_SERIAL_FLASH_PAGE_SIZE				0x00000100		// Page size 256 bytes
#define SPI_SERIAL_FLASH_SUBSECTOR_SIZE			0x00001000		// Sub-sector size 4096 bytes
#define	SPI_SERIAL_FLASH_SECTOR_SIZE			0x00010000		// Sector size 64KB
#define SPI_SERIAL_FLASH_SERIAL_FLASH_SIZE		0x00800000		// Memory size
#define SPI_SERIAL_FLASH_PAGES					37768			// Number of pages
#define SPI_SERIAL_FLASH_SUBSECTORS				2048			// Number of sub-sectors
#define SPI_SERIAL_FLASH_SECTORS				128				// Number of sectors
#define	SPI_SERIAL_FLASH_BUFFER_SIZE			SPI_SERIAL_FLASH_PAGE_SIZE
#define SPI_SERIAL_FLASH_WAIT_TIME				500				// Command wait interval in ms

////////////////////////////////////////////////////////////
// The SPI serial memory instructions
#define SPI_SERIAL_FLASH_WRITE_STATUS        	0x01
#define SPI_SERIAL_FLASH_WRITE_DATA          	0x02
#define SPI_SERIAL_FLASH_READ_DATA           	0x03
#define SPI_SERIAL_FLASH_WRITE_LATCH_DISABLE 	0x04
#define SPI_SERIAL_FLASH_READ_STATUS         	0x05
#define SPI_SERIAL_FLASH_WRITE_LATCH_ENABLE  	0x06
#define SPI_SERIAL_FLASH_CHIP_ERASE          	0xC7
#define SPI_SERIAL_FLASH_SUBSECTOR_ERASE		0x20
#define SPI_SERIAL_FLASH_SECTOR_ERASE			0xD8

////////////////////////////////////////////////////////////
// Status register bit identifier definitions
#define	STATUS_BUSY								0x01			// Busy (program, erase)
#define STATUS_WRITE_ENABLE_LATCH				0x02			// Write enable latch
#define STATUS_BLOCK_PROTECT_1					0x04
#define	STATUS_BLOCK_PROTECT_2					0x08
#define STATUS_BLOCK_PROTECT_3					0x10
#define	STATUS_BLOCK_TOPBOTTOM					0x20
#define STATUS_BLOCK_PROTECT_4					0x40
#define STATUS_REG_WRITE_ENABLE					0x80

/**
* Initialize serial flash SPI support
*
* \param    drv			Stream device driver vtable
* \param	ctx			Stream device driver context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
void serial_flash_initialize ( const stream_driver_vtable_t* drv, stream_driver_ctx_t* ctx );

/**
* Erase the entire memory device
*
* \param    None
*
* \returns  Non zero if successful, or 0 if failed
*/
uint32_t serial_flash_chip_erase ( void );

/**
* Erase memory device sub-sector
*
* \param    Addr		Address of the sub-sector to erase
*
* \returns  None
*/
void serial_flash_subsector_erase ( uint32_t addr );

/**
* Erase memory device sector
*
* \param    Addr		Address of the sector to erase
*
* \returns  None
*/
void serial_flash_sector_erase ( uint32_t addr );

/**
* Set memory device write latch
*
* \param    enable		Enable or disable writes
*
* \returns  None
*/
void serial_flash_set_write_latch ( bool enable );

/**
* Set memory device protection
*
* \param    protect		Enable or disable protection
*
* \returns  None
*/
void serial_flash_set_protection ( bool protect );

/**
* Read the status register of the memory device
*
* \param    None
*
* \returns  Status register contents
*/
uint8_t serial_flash_read_status ( void );

/**
* Write a byte to the memory device
*
* \param    addr		Address to write the byte
* \param	data		Byte to write
*
* \returns  None
*/
void serial_flash_write_byte ( uint32_t addr, uint8_t data );

/**
* Read a byte from the memory device
*
* \param    addr		Address of the byte
*
* \returns  Byte value read.
*/
uint8_t  serial_flash_read_byte ( uint32_t addr );

/**
* Write a byte stream to the memory device
*
* \param    addr		Address in the memory device
* \param	size		Number of bytes to write
* \param	data		Pointer to the data to write
*
* \returns  Byte value read.
*/
uint32_t serial_flash_write_data ( uint32_t addr, uint32_t size, uint8_t* data );

/**
* Read a byte stream from the memory device
*
* \param    addr		Address in the memory device
* \param	size		Number of bytes to read
* \param	data		Pointer to the buffer to store the bytes read.
*
* \returns  Byte value read.
*/
uint32_t serial_flash_read_data ( uint32_t addr, uint32_t size, uint8_t* data );

/**
* Perform a self test of the memory device
*
* \param    None
*
* \returns  Status of the self test
*/
bool serial_flash_self_test ( void );

/**
* Verify data stored in the memory device
*
* \param    data		Pointer to data to verify
* \param	size		Size of data to verify
* \param	addr		Address in memory device to verify
*
* \returns  Status of the verify
*/
bool serial_flash_verify_data	( void* data, uint32_t size, uint32_t addr );

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif /* SRC_SERVICES_DATABASE_DATABASE_CORE_SERIAL_FLASH_H_ */
