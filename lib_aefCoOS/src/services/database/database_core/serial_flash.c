
/**
* serial_flash.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Implementation of serial flash memory device support.
*
*/

#include <CoOS.h>
#include "serial_flash.h"

#include <aef/embedded/osal/time.h>
#include <aef/embedded/osal/time_delay.h>

#define HIBYTE(w)					((uint8_t)(((uint16_t)(w) >> 8) & 0xFF))
#define LOBYTE(w)					((uint8_t)(w))
#define MAKEWORD(low,hi)			((uint16_t)(((uint8_t)(low))|(((uint16_t)((uint8_t)(hi)))<<8)))
#define HIWORD(w)					((uint16_t)(((uint32_t)(w) >> 16) & 0xFFFF))
#define LOWORD(w)					((uint16_t)(w))
#define MAKELONG(low,hi)			((uint32_t)(((uint16_t)(low))|(((uint32_t)((uint16_t)(hi)))<<16)))
#define LLSB(x)             		((x) & 0xFF)
#define LNLSB(x)            		(((x) >> 8) & 0xFF)
#define LNMSB(x)            		(((x) >> 16) & 0xFF)
#define LMSB(x)             		(((x) >> 24) & 0xFF)

static const stream_driver_vtable_t* serial_flash_spi_drv = NULL;
static stream_driver_ctx_t* serial_flash_spi_ctx = NULL;
static uint8_t	transfer_buffer[SPI_SERIAL_FLASH_BUFFER_SIZE];

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
void serial_flash_initialize ( const stream_driver_vtable_t* drv, stream_driver_ctx_t* ctx )
{
	serial_flash_spi_drv = drv;
	serial_flash_spi_ctx = ctx;
}

/**
* Read the status register of the memory device
*
* \param    None
*
* \returns  Status register contents
*/
uint8_t serial_flash_read_status ( void )
{
    uint32_t bytes_transferred = 0;
    uint8_t  command = SPI_SERIAL_FLASH_READ_STATUS;
    uint8_t  status = 0xFF;

    /*
     * Send read status command
     */
    serial_flash_spi_drv->write (serial_flash_spi_ctx, &command, 1, &bytes_transferred);
    if (bytes_transferred == 1)
    {
        /*
         * Read status register contents
         */
        serial_flash_spi_drv->read (serial_flash_spi_ctx, &status, 1, &bytes_transferred);
    }

    /*
     * Deactivate CS
     */
    serial_flash_spi_drv->iocontrol (serial_flash_spi_ctx, IOCTL_SPI_FLUSH, NULL, 0L, NULL, 0L, NULL);

    return status;
}

/**
* Set memory device write latch
*
* \param    enable		Enable or disable writes
*
* \returns  None
*/
void serial_flash_set_write_latch ( bool enable )
{

    uint32_t bytes_transferred = 0;
    uint8_t  command;

    command = (enable) ? SPI_SERIAL_FLASH_WRITE_LATCH_ENABLE : SPI_SERIAL_FLASH_WRITE_LATCH_DISABLE;

    /*
     * Send command
     */
    serial_flash_spi_drv->write (serial_flash_spi_ctx, &command, 1, &bytes_transferred);

    /*
     * Deactivate CS
     */
    serial_flash_spi_drv->iocontrol (serial_flash_spi_ctx, IOCTL_SPI_FLUSH, NULL, 0L, NULL, 0L, NULL);

//    WAITFOR_TIMEOUT ( (! (memory_read_status() & STATUS_BUSY)), 1000 );

}

/**
* Set memory device protection
*
* \param    protect		Enable or disable protection
*
* \returns  None
*/
void serial_flash_set_protection ( bool protect )
{
    uint32_t bytes_transferred = 0;
    uint32_t i;
    uint8_t  command[2];

    ////////////////////////////////////////////////////////
    // Twice to ensure right transitions in protection status register
    for (i = 0; i < 2; i++)
    {

    	////////////////////////////////////////////////////
    	// Enable write operation
        serial_flash_set_write_latch (true);

        command[0] = SPI_SERIAL_FLASH_WRITE_STATUS;
        command[1] = (protect) ? 0x5C : 0x00;

        /*
         * Send command
         */
        serial_flash_spi_drv->write (serial_flash_spi_ctx, command, sizeof(command), &bytes_transferred);

        /*
         * Deactivate CS
         */
        serial_flash_spi_drv->iocontrol (serial_flash_spi_ctx, IOCTL_SPI_FLUSH, NULL, 0L, NULL, 0L, NULL);

//        WAITFOR_TIMEOUT ( (! (memory_read_status() & STATUS_BUSY)), 1000 );

    } // for
}

/**
* Erase the entire memory device
*
* \param    None
*
* \returns  Non zero if successful, or 0 if failed
*/
uint32_t serial_flash_chip_erase ( void )
{
    uint32_t bytes_transferred = 0;
    uint8_t  command;
    uint32_t result;

    /*
     * Enable write
     */
    serial_flash_set_write_latch (true);

    command = SPI_SERIAL_FLASH_CHIP_ERASE;

    /*
     * Send command
     */
    serial_flash_spi_drv->write (serial_flash_spi_ctx, &command, 1, &bytes_transferred);

    /*
     * Deactivate CS
     */
    serial_flash_spi_drv->iocontrol (serial_flash_spi_ctx, IOCTL_SPI_FLUSH, NULL, 0L, NULL, 0L, NULL);

    time_delay (1);
    result = (uint32_t)serial_flash_read_status();

//    WAITFOR_TIMEOUT ( (! (memory_read_status() & STATUS_BUSY)), 60000 );

    /*
     * Disable write
     */
    serial_flash_set_write_latch (false);

    return ( (result == 0x00 ) ? false : true );
}

/**
* Erase memory device sub-sector
*
* \param    Addr		Address of the sub-sector to erase
*
* \returns  None
*/
void serial_flash_subsector_erase ( uint32_t addr )
{
    uint32_t bytes_transferred = 0;
    uint8_t  command[4];

    serial_flash_set_write_latch (true);

    command[0] = SPI_SERIAL_FLASH_SUBSECTOR_ERASE;
    command[1] = LNMSB(addr);
    command[2] = LNLSB(addr);
    command[3] = LLSB(addr);

    /*
     * Send command
     */
    serial_flash_spi_drv->write (serial_flash_spi_ctx, command, sizeof(command), &bytes_transferred);

    /*
     * Deactivate CS
     */
    serial_flash_spi_drv->iocontrol (serial_flash_spi_ctx, IOCTL_SPI_FLUSH, NULL, 0L, NULL, 0L, NULL);

    time_delay (1);
//    WAITFOR_TIMEOUT ( (! (memory_read_status() & STATUS_BUSY)), 15000 );

    ////////////////////////////////////////////////////////
    // disable write operation
    serial_flash_set_write_latch (false);
}

/**
* Erase memory device sector
*
* \param    Addr		Address of the sector to erase
*
* \returns  None
*/
void serial_flash_sector_erase ( uint32_t addr )
{
    uint32_t bytes_transferred = 0;
    uint8_t  command[4];

    serial_flash_set_write_latch (true);

    /*
     * Set up command buffer
     */
    command[0] = SPI_SERIAL_FLASH_SECTOR_ERASE;
    command[1] = LNMSB(addr);
    command[2] = LNLSB(addr);
    command[3] = LLSB(addr);

    /*
     * Send command
     */
    serial_flash_spi_drv->write (serial_flash_spi_ctx, command, sizeof(command), &bytes_transferred);

    /*
     * Deactivate CS
     */
    serial_flash_spi_drv->iocontrol (serial_flash_spi_ctx, IOCTL_SPI_FLUSH, NULL, 0L, NULL, 0L, NULL);

    time_delay (1);
//    WAITFOR_TIMEOUT ( (! (memory_read_status() & STATUS_BUSY)), 30000 );

    ////////////////////////////////////////////////////////
    // disable write operation
    serial_flash_set_write_latch (false);
}

/**
* Write a byte to the memory device
*
* \param    addr		Address to write the byte
* \param	data		Byte to write
*
* \returns  None
*/
void serial_flash_write_byte ( uint32_t addr, uint8_t data )
{
    uint32_t bytes_transferred = 0;
    uint8_t  command[5];

    serial_flash_set_write_latch (true);
    serial_flash_read_status ();

    /*
     * Set up command buffer
     */
    command[0] = SPI_SERIAL_FLASH_WRITE_DATA;
    command[1] = LNMSB(addr);
    command[2] = LNLSB(addr);
    command[3] = LLSB(addr);
    command[4] = data;

    /*
     * Send command
     */
    serial_flash_spi_drv->write (serial_flash_spi_ctx, command, sizeof(command), &bytes_transferred);

    /*
     * Deactivate CS
     */
    serial_flash_spi_drv->iocontrol (serial_flash_spi_ctx, IOCTL_SPI_FLUSH, NULL, 0L, NULL, 0L, NULL);

    /* There is 1 ms internal write cycle needed for memory */
    time_delay (1);
//    WAITFOR_TIMEOUT ( (! (memory_read_status() & STATUS_BUSY)), 1000 );

    ////////////////////////////////////////////////////////
    // Disable write operation must be enabled in memory
    serial_flash_set_write_latch (false);

}

/**
* Read a byte from the memory device
*
* \param    addr		Address of the byte
*
* \returns  Byte value read.
*/
uint8_t serial_flash_read_byte ( uint32_t addr )
{

    uint32_t bytes_transferred = 0;
    uint8_t  command[4];
    uint8_t  data = 0;

    /*
     * Set up command buffer
     */
    command[0] = SPI_SERIAL_FLASH_READ_DATA;
    command[1] = LNMSB(addr);
    command[2] = LNLSB(addr);
    command[3] = LLSB(addr);

    /*
     * Send command
     */
    serial_flash_spi_drv->write (serial_flash_spi_ctx, command, sizeof(command), &bytes_transferred);

    if ( bytes_transferred == sizeof(command) )
    {
    	serial_flash_spi_drv->read (serial_flash_spi_ctx, &data, 1, &bytes_transferred );
    }

    /*
     * Deactivate CS
     */
    serial_flash_spi_drv->iocontrol (serial_flash_spi_ctx, IOCTL_SPI_FLUSH, NULL, 0L, NULL, 0L, NULL);

    return data;
}

/**
* Write a byte stream to the memory device
*
* \param    addr		Address in the memory device
* \param	size		Number of bytes to write
* \param	data		Pointer to the data to write
*
* \returns  Byte value read.
*/
uint32_t serial_flash_write_data (uint32_t addr, uint32_t size, uint8_t* data)
{
    uint32_t bytes_transferred = 0;
	uint32_t bytes_to_write = size;
	uint32_t page_size;
	uint8_t  command[4];

	while (bytes_to_write > 0)
	{

	    /*
	     * Set page size to write
	     */
		page_size = bytes_to_write;
	    if (page_size > SPI_SERIAL_FLASH_PAGE_SIZE - (addr & (SPI_SERIAL_FLASH_PAGE_SIZE - 1)))
	    	page_size = SPI_SERIAL_FLASH_PAGE_SIZE - (addr & (SPI_SERIAL_FLASH_PAGE_SIZE - 1));
	    bytes_to_write -= page_size;

	    /*
	     * Enable writes
	     */
	    serial_flash_set_write_latch (true);
	    serial_flash_read_status ();

	    /*
	     * Set up command buffer
	     */
	    command[0] = SPI_SERIAL_FLASH_WRITE_DATA;
	    command[1] = LNMSB(addr);
	    command[2] = LNLSB(addr);
	    command[3] = LLSB(addr);

	    /*
	     * Send command
	     */
	    serial_flash_spi_drv->write (serial_flash_spi_ctx, command, sizeof(command), &bytes_transferred);

	    if ( bytes_transferred != sizeof(command) )
	    {
	    	size = 0;
	    	break;
	    }

	    /*
	     * Send data
	     */
	    serial_flash_spi_drv->write (serial_flash_spi_ctx, data, page_size, &bytes_transferred);

	    if ( bytes_transferred != page_size )
	    {
	    	size -= bytes_to_write;
	    	break;
	    }

	    /*
	     * Deactivate CS
	     */
	    serial_flash_spi_drv->iocontrol (serial_flash_spi_ctx, IOCTL_SPI_FLUSH, NULL, 0L, NULL, 0L, NULL);

	    /*
	     * Move to next block to write
	     */
	    addr += page_size;
	    data += page_size;

	    /*
	     * Allow for 5ms internal memory write cycle
	     */
	    time_delay (5);
//	    WAITFOR_TIMEOUT ( (! (memory_read_status() & STATUS_BUSY)), 5000 );

	} // while

    /*
     * Deactivate CS
     */
    serial_flash_spi_drv->iocontrol (serial_flash_spi_ctx, IOCTL_SPI_FLUSH, NULL, 0L, NULL, 0L, NULL);

    /*
     * Disable writes
     */
	serial_flash_set_write_latch (false);

	return size;
}

/**
* Read a byte stream from the memory device
*
* \param    addr		Address in the memory device
* \param	size		Number of bytes to read
* \param	data		Pointer to the buffer to store the bytes read.
*
* \returns  Byte value read.
*/
uint32_t serial_flash_read_data ( uint32_t addr, uint32_t size, uint8_t* data )
{
    uint32_t bytes_transferred = 0;
    uint8_t  command[4];

    ////////////////////////////////////////////////////////
    // Read instruction, address
    command[0] = SPI_SERIAL_FLASH_READ_DATA;
    command[1] = LNMSB(addr);
    command[2] = LNLSB(addr);
    command[3] = LLSB(addr);

    /*
     * Send command
     */
    serial_flash_spi_drv->write (serial_flash_spi_ctx, command, sizeof(command), &bytes_transferred);

    if ( bytes_transferred == sizeof(command) )
    {
    	serial_flash_spi_drv->read (serial_flash_spi_ctx, data, size, &bytes_transferred );
    }

    /*
     * Deactivate CS
     */
    serial_flash_spi_drv->iocontrol (serial_flash_spi_ctx, IOCTL_SPI_FLUSH, NULL, 0L, NULL, 0L, NULL);

    return ( (bytes_transferred != size) ? 0 : size );
} // CN25Q064ADrv::memory_read_data

