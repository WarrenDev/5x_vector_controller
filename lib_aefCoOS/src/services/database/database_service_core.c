
/**
* database_service_core.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Database service core implementation.
*
*/

#include "database_service_core.h"

#include <aef/embedded/driver/device_manager.h>
#include <aef/embedded/driver/spi/spi_driver.h>
#include <aef/embedded/driver/stream_driver.h>
#include <aef/embedded/system/system_core.h>

#include "database_core/serial_flash.h"
#include "database_core/block_allocator.h"

/**
* Internal routines.
*/
static service_status_t database_core_start (service_ctx_t* ctx);
static service_status_t database_core_restart (service_ctx_t* ctx);
static service_status_t database_core_stop (service_ctx_t* ctx);
static service_status_t database_core_status (service_ctx_t* ctx);
static service_status_t database_core_pause (service_ctx_t* ctx);
static service_status_t database_core_continue (service_ctx_t* ctx);

static service_status_t database_core_initialize (service_ctx_t* ctx, void* config);
static service_status_t database_core_format (service_ctx_t* ctx);
static service_status_t database_core_erase (service_ctx_t* ctx);
static service_status_t database_core_readrecord (service_ctx_t* ctx, void* record_key, uint32_t size, void* record, uint32_t record_size, uint32_t* bytes_transferred);
static service_status_t database_core_writerecord (service_ctx_t* ctx, void* record, uint32_t size);
static service_status_t database_core_updaterecord (service_ctx_t* ctx, void* record, uint32_t size);
static service_status_t database_core_deleterecord (service_ctx_t* ctx, void* record_key, uint32_t size);
static service_status_t database_core_garbagecollection (service_ctx_t* ctx);

static uint32_t initialize_database ( void );
static uint32_t write_database_header ( void );
static uint32_t write_session_header ( uint8_t* sessionUUID, uint8_t* serverUUID, uint8_t* rSecret, uint8_t* sSecret, uint8_t* certificate, uint16_t certificateLength );
static uint32_t write_user_preferences_record ( uint8_t* uuid, uint8_t* preferences );
static uint32_t write_ekey_record ( uint8_t* uuid, uint8_t ekeyType, uint8_t* sharedSecret, uint32_t serialNumber, uint8_t permissions, uint8_t* certificate, uint16_t certificateLength );
static uint32_t update_ekey_record ( uint8_t* uuid, uint8_t ekeyType, uint8_t* sharedSecret, uint32_t serialNumber, uint8_t permissions, uint8_t* certificate, uint16_t certificateLength );
static DBCURSOR find_block ( uint8_t* uuid, void* pRecord, uint32_t maxReadSize );
static uint32_t delete_block ( uint8_t* uuid );
static DBCURSOR find_empty_block ( void );
static uint32_t read_block ( uint16_t block_ref, uint8_t* data, uint32_t length );
static uint32_t write_block ( uint16_t block_ref, uint8_t* data, uint32_t length );
static uint32_t bulk_erase (void);

static const stream_driver_vtable_t* database_spi_drv = NULL;
static stream_driver_ctx_t* spi_ctx = NULL;
static uint32_t chip_select = 4;

#define DB_NUMBER_OF_SECTORS	4L
#define DB_SECTOR_SIZE			0x00010000
#define RECORD_VERSION			1
/**
* Initialize the database service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t database_core_init (service_ctx_t* ctx)
{
	device_manager_vtable_t* device_manager = system_get_device_manager();
	database_spi_drv = device_manager->getdevice(DRV_SPI_B);

	if ( database_spi_drv != NULL )
	{
		ctx->state = SERVICE_START_PENDING;
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_INITIALIZATION;
}

/**
* De-initialize the database service.
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t database_core_deinit (service_ctx_t* ctx)
{
	return SERVICE_STATUS_SUCCESS;
}

/**
* Send a command to a system service instance.
*
* \param    ctx				Pointer to the service context
* \param    code			I/O control code to perform
* \param    input_buffer	Pointer to the input buffer
* \param    input_size		Input buffer size
* \param    output_buffer	Pointer to the output buffer
* \param    output_size		Output buffer size
* \param    bytes_read		Pointer to the actual bytes returned
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable perform the command.
*           SERVICE_FAILURE_INVALID_PARAMETER if the context or data buffer is invalid.
*/
service_status_t
database_core_ioctl (service_ctx_t* ctx, uint32_t code, void* input_buffer, uint32_t input_size, void* output_buffer, uint32_t output_size, uint32_t* bytes_transferred)
{
	service_status_t status = SERVICE_FAILURE_GENERAL;

	if ( ctx == NULL )
		return SERVICE_FAILURE_INVALID_PARAMETER;

	if ( ctx->state == SERVICE_UNINITIALIZED )
		return SERVICE_FAILURE_OFFLINE;

	if ( bytes_transferred != NULL )
		*bytes_transferred = 0;

	switch ( code )
	{
		case IOCTL_SERVICE_START:
			status = database_core_start(ctx);
			break;
    	case IOCTL_SERVICE_RESTART:
    		status = database_core_restart(ctx);
    		break;
		case IOCTL_SERVICE_STOP:
			status = database_core_stop(ctx);
			break;
		case IOCTL_SERVICE_STATUS:
			status = database_core_status(ctx);
			break;
		case IOCTL_SERVICE_PAUSE:
			status = database_core_pause(ctx);
			break;
		case IOCTL_SERVICE_CONTINUE:
			status = database_core_continue(ctx);
			break;
		case IOCTL_DATABASE_INITIALIZE:
			status = database_core_initialize(ctx, input_buffer);
			break;
		case IOCTL_DATABASE_FORMAT:
			status = database_core_format(ctx);
			break;
		case IOCTL_DATABASE_ERASE:
			status = database_core_erase(ctx);
			break;
		case IOCTL_DATABASE_READ_RECORD:
			status = database_core_readrecord(ctx, input_buffer, input_size, output_buffer, output_size, bytes_transferred);
			break;
		case IOCTL_DATABASE_WRITE_RECORD:
			status = database_core_writerecord(ctx, input_buffer, input_size);
			break;
		case IOCTL_DATABASE_UPDATE_RECORD:
			status = database_core_updaterecord(ctx, input_buffer, input_size);
			break;
		case IOCTL_DATABASE_DELETE_RECORD:
			status = database_core_deleterecord(ctx, input_buffer, input_size);
			break;
		case IOCTL_DATABASE_GARBAGE_COLLECTION:
			status = database_core_garbagecollection(ctx);
			break;
		default:
			break;
	}

	return status;
}

/**
* Start service control function.
* Starts the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t database_core_start (service_ctx_t* ctx)
{
//	char* pname = NULL;
//	uint32_t param;

	ctx->state = SERVICE_START_PENDING;

//	pname = database_spi_drv->getname();
//	spi_ctx = database_spi_drv->open(pname, 0, chip_select );
//	if (spi_ctx)
//	{
//		serial_flash_initialize ( database_spi_drv, spi_ctx );

//		param = SPI_DEVICE_MASTER_MODE;
//		database_spi_drv->iocontrol(spi_ctx, IO_IOCTL_SPI_SET_TRANSFER_MODE, &param, 4, NULL, 0, NULL );

//		param = 48000000;
//		database_spi_drv->iocontrol(spi_ctx, IO_IOCTL_SPI_SET_BAUD, &param, 4, NULL, 0, NULL );

//		param = SPI_CLK_POL_PHA_MODE3;
//		database_spi_drv->iocontrol(spi_ctx, IO_IOCTL_SPI_SET_MODE, &param, 4, NULL, 0, NULL );

//		param = SPI_DEVICE_BIG_ENDIAN;
//		database_spi_drv->iocontrol(spi_ctx, IO_IOCTL_SPI_SET_ENDIAN, &param, 4, NULL, 0, NULL );

//		ctx->state = SERVICE_RUNNING;
//		return SERVICE_STATUS_SUCCESS;
//	}

	ctx->state = SERVICE_RUNNING;

	return SERVICE_STATUS_SUCCESS;
}

/**
* Restart service control function.
* Restarts the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t database_core_restart (service_ctx_t* ctx)
{
	return SERVICE_FAILURE_GENERAL;
}

/**
* Stop service control function.
* Stops the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t database_core_stop (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		database_spi_drv->close (spi_ctx);
		ctx->state = SERVICE_STOPPED;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Status service control function.
* Retrieves status of the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t database_core_status (service_ctx_t* ctx)
{
	return SERVICE_FAILURE_GENERAL;
}

/**
* Pause service control function.
* Pauses the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t database_core_pause (service_ctx_t* ctx)
{
	ctx->state = SERVICE_PAUSED;
	return SERVICE_FAILURE_GENERAL;
}

/**
* Continue service control function.
* Continues the service
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t database_core_continue (service_ctx_t* ctx)
{
	ctx->state = SERVICE_RUNNING;
	return SERVICE_FAILURE_GENERAL;
}

/**
* Initialize the hardware database.
*
* \param    ctx				Pointer to the service context
* \param    config			Pointer to configuration information
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
database_core_initialize (service_ctx_t* ctx, void* config)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
//	    GenerateDatabaseEncryptionKeys();

		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Format the database
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if character is available.
*           SERVICE_FAILURE_GENERAL if no characters are present
*/
service_status_t
database_core_format (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		bulk_erase ();
		initialize_database ();
		write_database_header ();

		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Erase the database
* *
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
database_core_erase (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		bulk_erase ();
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Read a record from the database
*
* \param    ctx				Pointer to the service context
* \param	record_key		Pointer to the record key
* \param	size			Size of the record key
* \paran	record			Pointer to the record
* \param	record_size		Size of the record memory area
* \param	bytes_transferred	Pointer to the number of bytes transferred
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
database_core_readrecord (service_ctx_t* ctx, void* record_key, uint32_t size, void* record, uint32_t record_size, uint32_t* bytes_transferred)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		if ( record_key != NULL && size == DATABASE_UUID_SIZE && record != NULL )
		{
			if ( find_block( record_key, record, record_size ) != INVALID_BLOCKREF )
			{
				if ( bytes_transferred )
					*bytes_transferred = record_size;
				return SERVICE_STATUS_SUCCESS;
			}
		}
		else
		{
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Write a record to the database
*
* \param    ctx				Pointer to the service context
* \param	record			Pointer to memory for the database record
* \param	record_size		Size of the record
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
database_core_writerecord (service_ctx_t* ctx, void* record, uint32_t record_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Update a record in the database
*
* \param    ctx				Pointer to the service context
* \param	record			Pointer to memory for the database record
* \param	record_size		Size of the record
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
database_core_updaterecord (service_ctx_t* ctx, void* record, uint32_t record_size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Delete a record from the database
*
* \param    ctx				Pointer to the service context
* \param	record_key		Pointer to memory for the database record
* \param	size			Size of the record
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
database_core_deleterecord (service_ctx_t* ctx, void* record_key, uint32_t size)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		if ( record_key != NULL && size == DATABASE_UUID_SIZE )
		{
			if ( delete_block ( record_key ) == DATABASE_API_SUCCESS )
			{
				return SERVICE_STATUS_SUCCESS;
			}
		}
		else
		{
			return SERVICE_FAILURE_INVALID_PARAMETER;
		}
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Perform garbage collection on the database
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
service_status_t
database_core_garbagecollection (service_ctx_t* ctx)
{
	if ( ctx != NULL && ctx->state == SERVICE_RUNNING )
	{
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_GENERAL;
}

/**
* Create the initial database structure
*
* \param	None
*
* \returns  DATABASE_API_SUCCESS if successful.
*           DATABASE_API_ERROR_NOT_FOUND if unable to perform the command.
*/
uint32_t initialize_database ( void )
{
uint16_t BlockRef;
uint32_t MaxSuperBlocks = SUPER_BLOCKS;
SuperBlockHeader_t superBlockHeader;

	for ( uint8_t superBlock = 0; superBlock < MaxSuperBlocks; superBlock++ )
	{

		////////////////////////////////////////////////////
		// Initialize the superblock header
		memset ( &superBlockHeader, 0xFF, sizeof(SuperBlockHeader_t) );
		superBlockHeader.fillInd    = 0x00;
		superBlockHeader.recordType = RecordTypeSuperBlock;

		//The generation for the first superblock is 0.  The generation for each
	    //subsequent superblock is 0xFFFFFFFF, indicating that these are free blocks.
		if( superBlock == 0 )
		    superBlockHeader.generation = 0x0;
        else
            superBlockHeader.generation = 0xFFFFFFFF;

		////////////////////////////////////////////////////
		// Write the superblock header
		BlockRef = blockRefsToShortRef( superBlock, 0 );
		write_block ( BlockRef, (uint8_t*)&superBlockHeader, BLOCK_SIZE );

	} // for

	return DATABASE_API_SUCCESS;
}

/**
* Write the header record to the database
*
* \param	None
*
* \returns  DATABASE_API_SUCCESS if successful.
*           DATABASE_API_ERROR_NOT_FOUND if unable to perform the command.
*/
uint32_t write_database_header ( void )
{
    uint16_t BlockRef;
    DatabaseHeader_t DbaseHeader;

	////////////////////////////////////////////////////////
	// Initialize the database header entry
	memset ( &DbaseHeader, 0xFF, sizeof(DatabaseHeader_t) );
	memset ( DbaseHeader.header.block_entry.UUID, 0x00, DATABASE_UUID_SIZE );
	DbaseHeader.header.block_entry.fillInd    = 0x00;
	DbaseHeader.header.block_entry.recordType = RecordTypeDatabaseHeader;
	DbaseHeader.header.block_entry.uuidSize   = 0;
	DbaseHeader.header.encBytes   			  = sizeof(DatabaseHeader_t);
	DbaseHeader.version          		 	  = DATABASE_VERSION;

    //Encrypt and MAC the record
//    encryptAndMac(((uint8_t*)&DbaseHeader) + HEADER_PROTECTED_OFFSET, HEADER_PROTECTED_SIZE,
//            DbaseHeader.header.IV, &DbaseHeader.footer);

	////////////////////////////////////////////////////////
	// Write the database header
	BlockRef = blockRefsToShortRef( 0, 1 );
	write_block ( BlockRef, (uint8_t*)&DbaseHeader, sizeof(DatabaseHeader_t) );

	return DATABASE_API_SUCCESS;
}

/**
* Write a session header record to the database
*
* \param	sessionUUID			UUID of the session
* \param	serverUUID			UUID of the server
* \param	rSecret				Rolling secret
* \param	sSecret				Hardened secret
* \param	certificate			Pointer to certificate data
* \param	sertificateLength	Length of the certificate
* \param	record				Pointer of the data buffer
* \param	maxReadSize			Maximum record size to read
*
* \returns  DATABASE_API_SUCCESS if successful.
*           DATABASE_API_ERROR_NOT_FOUND if unable to perform the command.
*/
uint32_t write_session_header ( uint8_t* sessionUUID, uint8_t* serverUUID, uint8_t* rSecret, uint8_t* sSecret,
							    uint8_t* certificate, uint16_t certificateLength )
{
uint16_t 		 BlockRef;
uint8_t	         BlockData[sizeof(SessionHeader_t) + certificateLength];
SessionHeader_t* pSessionRecord = (SessionHeader_t*)&BlockData[0];

	////////////////////////////////////////////////////////
	// Initialize the header entry
	memset ( pSessionRecord, 0xFF, sizeof(SessionHeader_t) );
	memcpy ( pSessionRecord->header.block_entry.UUID, sessionUUID, DATABASE_UUID_SIZE );
	pSessionRecord->header.block_entry.fillInd    = 0x00;
	pSessionRecord->header.block_entry.recordType = RecordTypeSessionHeader;
	pSessionRecord->header.block_entry.uuidSize   = DATABASE_UUID_SIZE;
	pSessionRecord->header.encBytes               = sizeof(SessionHeader_t) + certificateLength;

	////////////////////////////////////////////////////////
	// Load session information
	memcpy ( pSessionRecord->sessionUuid,    sessionUUID, sizeof(pSessionRecord->sessionUuid) );
	memcpy ( pSessionRecord->serverUUID,     serverUUID,  sizeof(pSessionRecord->serverUUID) );
	memcpy ( pSessionRecord->rollingSecret,  rSecret,     sizeof(pSessionRecord->rollingSecret) );
	memcpy ( pSessionRecord->hardenedSecret, sSecret,     sizeof(pSessionRecord->hardenedSecret) );

    //Encrypt and MAC the record
//    encryptAndMac(((uint8_t*)pSessionRecord) + SESSION_PROTECTED_OFFSET, SESSION_PROTECTED_SIZE,
//            pSessionRecord->header.IV, &pSessionRecord->footer);

	////////////////////////////////////////////////////////
	// Load certificate or other extra data
	if ( (certificate != NULL) && (certificateLength != 0) )
		memcpy ( &BlockData[sizeof(SessionHeader_t)], certificate, certificateLength );

	////////////////////////////////////////////////////////
	// Write the session record to the database
	BlockRef = find_empty_block ();
	if ( BlockRef != INVALID_BLOCKREF )
	{
		write_block ( BlockRef, BlockData, sizeof(BlockData) );
		return DATABASE_API_SUCCESS;
	}

	return DATABASE_API_ERROR_FULL;
}

////////////////////////////////////////////////////////////
// CDatabase::WriteUserPreferencesRecord
//   Routine to create and write the user preferences record
/**
* Write a user preferences record to the database
*
* \param	uuid				UUID of this record
* \param	preferences			Use preferences
*
* \returns  DATABASE_API_SUCCESS if successful.
*           DATABASE_API_ERROR_NOT_FOUND if unable to perform the command.
*/
uint32_t write_user_preferences_record ( uint8_t* uuid, uint8_t* preferences )
{
DBCURSOR BlockRef;
UserPreferences_t PreferencesRecord;

	////////////////////////////////////////////////////////
	// Initialize the header entry
	memset ( &PreferencesRecord, 0xFF, sizeof(UserPreferences_t) );
	memcpy ( PreferencesRecord.header.block_entry.UUID, uuid, DATABASE_UUID_SIZE );
	PreferencesRecord.header.block_entry.fillInd    = 0x00;
	PreferencesRecord.header.block_entry.recordType = RecordTypeUserPreferences;
	PreferencesRecord.header.block_entry.uuidSize   = DATABASE_UUID_SIZE;
	PreferencesRecord.header.encBytes               = sizeof(UserPreferences_t);

	////////////////////////////////////////////////////////
	// Load preferences record information
	memcpy ( PreferencesRecord.preferencesBits, preferences, sizeof(PreferencesRecord.preferencesBits) );

	////////////////////////////////////////////////////////
	// Write the user preferences record
	BlockRef = find_empty_block ();
	if ( BlockRef != INVALID_BLOCKREF )
	{
		write_block ( BlockRef, (uint8_t*)&PreferencesRecord, sizeof(UserPreferences_t) );
		return DATABASE_API_SUCCESS;
	}

	return DATABASE_API_ERROR_FULL;
}

/**
* Write an eKey record to the database
*
* \param	uuid				UUID of the ekey
* \param	ekeyType			Type of the ekey
* \param	sharedSecret		Shared secret
* \param	serialNumber		Serial number
* \param	permissions			Permission information
* \param	certificate			Pointer to certificate data
* \param	sertificateLength	Length of the certificate
*
* \returns  DATABASE_API_SUCCESS if successful.
*           DATABASE_API_ERROR_NOT_FOUND if unable to perform the command.
*/
uint32_t write_ekey_record ( uint8_t* uuid, uint8_t ekeyType, uint8_t* sharedSecret, uint32_t serialNumber, uint8_t permissions,
						     uint8_t* certificate, uint16_t certificateLength )
{
	DBCURSOR     BlockRef;
	uint8_t	     BlockData[sizeof(EKeyEntry_t) + certificateLength];
	EKeyEntry_t* pEkeyRecord = (EKeyEntry_t*)&BlockData[0];

	/*
	 * Initialize entry header
	 */
	memset ( pEkeyRecord, 0xFF, sizeof(EKeyEntry_t) );
	memcpy ( pEkeyRecord->header.block_entry.UUID, uuid, DATABASE_UUID_SIZE );
	pEkeyRecord->header.block_entry.fillInd    = 0x00;
	pEkeyRecord->header.block_entry.recordType = RecordTypeEKeyEntry;
	pEkeyRecord->header.block_entry.uuidSize   = DATABASE_UUID_SIZE;
	pEkeyRecord->header.encBytes               = sizeof(EKeyEntry_t) + certificateLength;

	/*
	 * Load ekey information
	 */
	pEkeyRecord->eKeyType		= ekeyType;
	pEkeyRecord->permissions    = permissions;
	pEkeyRecord->serialNumber   = serialNumber;
	pEkeyRecord->version		= RECORD_VERSION;
	memcpy ( pEkeyRecord->sharedSecret, sharedSecret, sizeof(pEkeyRecord->sharedSecret) );
	if ( (certificate != NULL) && (certificateLength != 0) )
	{
		memcpy ( &BlockData[sizeof(EKeyEntry_t)], certificate, certificateLength );
	}

    //Encrypt and MAC the record
//    encryptAndMac(((uint8_t*)pEkeyRecord) + EKEY_PROTECTED_OFFSET, EKEY_PROTECTED_SIZE, pEkeyRecord->header.IV, &pEkeyRecord->footer);
    /*****************************************************************************
    DO NOT USE anything in the rec pointer after this point (except rec->header), it has been encrpyted
    and thus the values are incorrect.
    ******************************************************************************/

	/*
	 * Write the record
	 */
	BlockRef = find_empty_block ();
	if ( BlockRef != INVALID_BLOCKREF )
	{
		write_block ( BlockRef, BlockData, sizeof(BlockData) );
		return DATABASE_API_SUCCESS;
	}

	return DATABASE_API_ERROR_FULL;
}

/**
* Modify an existing record in the database
*
* \param	uuid				UUID of the ekey
* \param	ekeyType			Type of the ekey
* \param	sharedSecret		Shared secret
* \param	serialNumber		Serial number
* \param	permissions			Permission information
* \param	certificate			Pointer to certificate data
* \param	sertificateLength	Length of the certificate
*
* \returns  DATABASE_API_SUCCESS if successful.
*           DATABASE_API_ERROR_NOT_FOUND if unable to perform the command.
*/
uint32_t update_ekey_record ( uint8_t* uuid, uint8_t ekeyType, uint8_t* sharedSecret, uint32_t serialNumber, uint8_t permissions,
							  uint8_t* certificate, uint16_t certificateLength )
{

	delete_block ( uuid );
	return write_ekey_record ( uuid, ekeyType, sharedSecret, serialNumber, permissions, certificate, certificateLength );

}

/**
* Find a block in the database
*
* \param	uuid			UUID of the block to find
* \param	record			Pointer of the data buffer
* \param	maxReadSize		Maximum record size to read
*
* \returns  DATABASE_API_SUCCESS if successful.
*           DATABASE_API_ERROR_NOT_FOUND if unable to perform the command.
*/
DBCURSOR find_block( uint8_t* uuid, void* pRecord, uint32_t maxReadSize )
{
    DBCURSOR BlockRef;
    EKeyEntry_t blockHeader;
    uint32_t MaxSuperBlocks = SUPER_BLOCKS;
    uint32_t MaxEntries     = ENTRIES_PER_TABLE;

    /*
     * Search for block
     */
	for ( uint8_t superBlock = 0; superBlock < MaxSuperBlocks; superBlock++ )
	{
		for ( uint8_t Block = 1; Block < MaxEntries; Block++ )
		{
			BlockRef = blockRefsToShortRef( superBlock, Block );
			read_block ( BlockRef, (uint8_t*)&blockHeader, sizeof(EKeyEntry_t) );

			/*
			 * Verify the block is in use
			 */
            if ((blockHeader.header.block_entry.deleteInd == 0xFF) &&
                (blockHeader.header.block_entry.fillInd == 0x00) &&
                (blockHeader.header.block_entry.uuidSize == DATABASE_UUID_SIZE) &&
				memcmp ( blockHeader.header.block_entry.UUID, uuid, blockHeader.header.block_entry.uuidSize ) == 0)
            {
                //only read entire record if storage set
                if (pRecord)
                {
                    read_block ( BlockRef, (uint8_t*)pRecord, maxReadSize );
                    ////////////////////////////////////////////////
                    // Decrypt and Verify that the block is marked as in use, has not been deleted, and then compare the key
//                    if ( validateEKeyEntry( (EKeyEntry_t*)pRecord ))
//                    {
                        return BlockRef;
//                    }
                }
                else //No data requested, just looking for matching key.
                {
                    return BlockRef;
                }
            } // if
		} // for
	} // for

	/*
	 * Not found
	 */
	return INVALID_BLOCKREF;
}

/**
* Delete a block in the database
*
* \param	uuid	UUID of the block to delete
*
* \returns  DATABASE_API_SUCCESS if successful.
*           DATABASE_API_ERROR_NOT_FOUND if unable to perform the command.
*/
uint32_t delete_block ( uint8_t* uuid )
{
    DBCURSOR BlockRef;
    EntryHeader_t blockHeader;

    BlockRef = find_block ( uuid, &blockHeader, sizeof(EntryHeader_t) );
    if ( BlockRef != INVALID_BLOCKREF )
    {
		blockHeader.block_entry.deleteInd = 0x00;
		write_block ( BlockRef, (uint8_t*)&blockHeader, sizeof(EntryHeader_t) );
		return DATABASE_API_SUCCESS;
    }

	return DATABASE_API_ERROR_NOT_FOUND;
}

/**
* Locate an empty block in the database
*
* \param	None
*
* \returns  Block reference if successful.
*           INVALID_BLOCKREF if unable to perform the command.
*/
DBCURSOR find_empty_block ( void )
{
    DBCURSOR BlockRef;
    EntryHeader_t blockHeader;
    uint32_t MaxSuperBlocks = SUPER_BLOCKS;
    uint32_t MaxEntries     = ENTRIES_PER_TABLE;

    /*
     * Find an empty block
     */
	for ( uint8_t superBlock = 0; superBlock < MaxSuperBlocks; superBlock++ )
	{
		for ( uint8_t Block = 1; Block < MaxEntries; Block++ )
		{
			BlockRef = blockRefsToShortRef( superBlock, Block );
			read_block ( BlockRef, (uint8_t*)&blockHeader, sizeof(EntryHeader_t) );
			if ( blockHeader.block_entry.fillInd == 0xFF )
				return BlockRef;
		}
	}

	/*
	 * Ghostrider the pattern is full
	 */
	return INVALID_BLOCKREF;
}

/**
* Read a data block from the database
*
* \param	block_ref		Block reference
* \param	data			Pointer to the data
* \param    length			Size of the data to write
*
* \returns  DATABASE_API_SUCCESS if successful.
*           DATABASE_API_ERROR if unable to perform the command.
*/
uint32_t read_block ( uint16_t block_ref, uint8_t* data, uint32_t length )
{
uint8_t SuperBlock;
uint8_t Block;

	/*
	 * Determine the block address
	 */
	computeBlockRefsFromShortRef( block_ref, &SuperBlock, &Block );

	uint32_t Address = getSuperBlockOffset( SuperBlock ) + getBlockOffset ( Block );

	/*
	 * Read the blcok
	 */
	return serial_flash_read_data ( Address, length, data );
}

/**
* Write a data block to the database
*
* \param	block_ref		Block reference
* \param	data			Pointer to the data
* \param    length			Size of the data to write
*
* \returns  DATABASE_API_SUCCESS if successful.
*           DATABASE_API_ERROR if unable to perform the command.
*/
uint32_t write_block ( uint16_t block_ref, uint8_t* data, uint32_t length )
{
uint8_t SuperBlock;
uint8_t Block;

	/*
	 * Determine the block address
	 */
	computeBlockRefsFromShortRef( block_ref, &SuperBlock, &Block );

	uint32_t Address = getSuperBlockOffset( SuperBlock ) + getBlockOffset ( Block );

	/*
	 * Write the block
	 */
	serial_flash_write_data ( Address, length, data );

	return DATABASE_API_SUCCESS;
}

/**
* Erase the entire database
*
* \param    ctx				Pointer to the service context
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unable to perform the command.
*/
uint32_t bulk_erase (void)
{
	for ( uint32_t sector = 0; sector < DB_NUMBER_OF_SECTORS; sector++ )
		serial_flash_sector_erase ( (uint32_t)(sector * DB_SECTOR_SIZE) );

	return DATABASE_API_SUCCESS;
}

