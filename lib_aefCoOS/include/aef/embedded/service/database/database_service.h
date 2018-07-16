
/**
* logger_service.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions used with the system logger service.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_SERVICE_DATABASE_DATABASE_SERVICE_H_
#define INCLUDE_AEF_EMBEDDED_SERVICE_DATABASE_DATABASE_SERVICE_H_

#include <aef/embedded/service/srvddk/uefsrvddk.h>

/**
* The IOCTL function codes from 0x800 to 0xFFF are for customer use.
*
* The format of I/O control codes for the iocontrol call:
*    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method)
*
* Configuring system service I/O control codes in this manner produces
* unique system-wide I/O control codes.  It also provides a mechanism for
* isolating kernel mode memory spaces from user mode memory spaces and
* allows us to use the memory protection unit (MPU) of the processor.
*/

/*
* Database service base function codes
*/
#define DATABASE_INITIALIZE					0x800
#define DATABASE_FORMAT						0x801
#define DATABASE_ERASE						0x802
#define DATABASE_READ_RECORD				0x803
#define DATABASE_WRITE_RECORD				0x804
#define DATABASE_UPDATE_RECORD				0x805
#define	DATABASE_DELETE_RECORD				0x806
#define DATABASE_GARBAGE_COLLECTION			0x807

/*
* Database service I/O Control codes
*/
#define IOCTL_DATABASE_INITIALIZE			SRVIOCTLCODE(SERVICE_TYPE_DATABASE,DATABASE_INITIALIZE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_DATABASE_FORMAT				SRVIOCTLCODE(SERVICE_TYPE_DATABASE,DATABASE_FORMAT,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_DATABASE_ERASE				SRVIOCTLCODE(SERVICE_TYPE_DATABASE,DATABASE_ERASE,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_DATABASE_READ_RECORD			SRVIOCTLCODE(SERVICE_TYPE_DATABASE,DATABASE_READ_RECORD,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_DATABASE_WRITE_RECORD			SRVIOCTLCODE(SERVICE_TYPE_DATABASE,DATABASE_WRITE_RECORD,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_DATABASE_UPDATE_RECORD		SRVIOCTLCODE(SERVICE_TYPE_DATABASE,DATABASE_UPDATE_RECORD,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_DATABASE_DELETE_RECORD		SRVIOCTLCODE(SERVICE_TYPE_DATABASE,DATABASE_DELETE_RECORD,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)
#define IOCTL_DATABASE_GARBAGE_COLLECTION	SRVIOCTLCODE(SERVICE_TYPE_DATABASE,DATABASE_GARBAGE_COLLECTION,SERVICE_METHOD_DIRECT,SERVICE_ANY_ACCESS)

/**
* Record types.
*/
enum RecordType
{
    RecordTypeDatabaseHeader                    =    0x00,	// Database header
    RecordTypeSessionHeader                     =    0x01,	// Session header
    RecordTypeEKeyEntry                         =    0x02,	// eKey entry
    RecordTypeUserPreferences                   =    0x03,	// User preferences
    RecordTypeSuperBlock                        =    0x04,	// Super block
    RecordTypeInvalid                           =    0xFF	// Invalid record type
};

/**
* eKey Type.
*/
enum EKeyType
{
    eKeyTypeDevice                              =    0x00,	// Device keys
    eKeyTypeFob                                 =    0x01,	// Fob keys
    eKeyTypeServer                              =    0x10,	// Server key
    eKeyTypeDelegate                            =    0x20,	// Delegate (RPU/WPU) key
    eKeyTypeThirdPartyDelegate                  =    0x30,	// Third party delegate key
    eKeyTypeInvalid                             =    0xFF	// Invalid eKey type
};

/**
* Offset of user preferences bits in bit field
* 0-based (e.g. 0, 1, 2, 3, ...), and NOT power-of-two based (1, 2, 4, 8, ...)
*/
enum UserPreferencesBits
{
    //True if the RPU can be reset via the reset button
    ePreferenceRpuResetEnabled                  =       0,

    //Invalid bit value (sentry)
    ePreferenceInvalid                          =  0xFFFF
};

/**
 * BlockEntry is the base of data in a block entry.  It is re-used in both the
 * super block and in the header for an entry in each block. 24 bytes.
 */
struct _BlockEntry_def
{
    //Deletion indicator. Has this entry been deleted?
    uint8_t deleteInd;

    //Fill indicator.  Is this entry in use?
    uint8_t fillInd;

    //Eventually Consistent Transaction Forwarding Address
    uint8_t ectForward;

    //Eventually Consistent Transaction Indicator flag
    uint8_t ectInd;

    //Record type.  See RecordType.
    uint8_t recordType;

    //Two reserved bytes that must be 0xFF, 0xFF
    //Used to round this structure to 24 bytes.
    uint8_t reserved[2];

    //size of this UUID.  Typically 6 or 16.
    uint8_t uuidSize;

    //UUID for this entry.
    uint8_t UUID[16];
} __attribute__((__packed__));
typedef struct _BlockEntry_def BlockEntry_t;

/**
 * EntryHeader is the base of all entries in the database.  42 bytes.
 */
struct _EntryHeader_def
{
	BlockEntry_t block_entry;

    //The encryption IV for this field.  AES-128-CBC.
    uint8_t IV[16];

    //This is the first set of bytes encrypted in each record.  Needed for
    //CBC-MAC to be secure.
    uint16_t encBytes;
} __attribute__((__packed__));
typedef struct _EntryHeader_def EntryHeader_t;

/**
 * EntryFooter wraps the entry with a CRC.  32 bytes.
 */
struct _EntryFooter_def
{
    //The CBC-MAC of this entry.
    uint8_t mac[32];
} __attribute__((__packed__));
typedef struct _EntryFooter_def EntryFooter_t;

/**
 * SuperBlockHeader is the base of all blocks in the database.  It shares the
 * first three bytes of the EntryHeader. 32 bytes.
 */
struct _SuperBlockHeader_def
{
    //Deletion indicator. Has this entry been deleted?
    uint8_t deleteInd;

    //Fill indicator.  Is this entry in use?
    uint8_t fillInd;

    //Eventually Consistent Transaction Forwarding Address
    uint8_t ectForward;

    //Eventually Consistent Transaction Indicator flag
    uint8_t ectInd;

    //Record type.  See RecordType.  RecordTypeSuperBlock.
    uint8_t recordType;

    //Generation of this block.  Used for garbage collection.
    uint32_t generation;

    //Reserved for future use.  Round the structure out to 32 bytes.
    uint8_t reserved[23];
} __attribute__((__packed__));
typedef struct _SuperBlockHeader_def SuperBlockHeader_t;

/**
 * This structure represents eKey accounting information in the superblock.
 * By caching this information in the superblock, we can eliminate the need to
 * load records on bootup. 8 bytes.
 */
struct _SuperBlockEKeyData_def
{
    //Only valid if this is an eKey
    uint8_t eKeyType;

    //Permissions for this eKey.
    uint32_t permissions;

    //Reserved for future use.  Used to round this structure out to 8 bytes.
    uint8_t reserved[3];
} __attribute__((__packed__));
typedef struct _SuperBlockEKeyData_def SuperBlockEKeyData_t;

/**
 * When the superblock entry is not used, all cached data is reserved.
 * 8 bytes.
 */
struct _SuperBlockUnusedData_def
{
    //The BlockEntry structure contains all of the information we need.  Here,
    //we reserve bytes to round the structure out to 32 bytes.
    uint8_t reserved[8];
} __attribute__((__packed__));
typedef struct _SuperBlockUnusedData_def SuperBlockUnusedData_t;

/**
 * This union represents all of the cached data stored in the superblock for
 * entries. 8 bytes.
 */
union _SuperBlockCachedData_def
{
    SuperBlockEKeyData_t eKeyData;
    SuperBlockUnusedData_t unused;
} __attribute__((__packed__));
typedef union _SuperBlockCachedData_def SuperBlockCachedData_t;

//SuperBlockCachedData must be 8 bytes in size
//static_assert(sizeof(SuperBlockCachedData_t) == 8, "SuperBlockCachedData must be 8 bytes in length.");

/**
 * In a superblock, all data past the SuperBlockHeader is composed of entries
 * which describe the entire superblock.  The superblock is 32k in size, and is
 * divided into 32 1k regions.  The first region is reserved for the super
 * block table itself.  The other 31 regions are available for use in the
 * database.  In this first 1k region, there is enough space to hold a 32-byte
 * record describing each 1k region.  The first 32-byte record is the
 * SuperBlockHeader.  The remaining 31 entries are SuperBlockEntry records.
 * Since these entries are all contiguous, the DBU can perform 8*4 = 32
 * 256-byte reads from the dataflash chip to initialize the entire database.
 */
struct _SuperBlockEntry_def
{
	BlockEntry_t block_entry;
    SuperBlockCachedData_t data;
} __attribute__((__packed__));
typedef struct _SuperBlockEntry_def SuperBlockEntry_t;

//SuperBlockEntry must be 32 bytes in size
_Static_assert(sizeof(SuperBlockEntry_t) == 32, "SuperBlockEntry must be 32 bytes in length.");

/**
 * Database header.  This entry marks the version of the database.
 * 78 bytes + 42 bytes header + 32 bytes footer = 152 bytes
 */
struct _DatabaseHeader_def
{
    //The header for this entry.
    EntryHeader_t header;

    //The version of the database.  Encrypted.
    uint8_t version;

    //Reserved flags.  Set to 0xFF, 0xFF, 0xFF, 0xFF.  Encrypted.
    uint8_t flags[4];

    //Reserved fields for future use.  Set to 0xFF....  Encrypted.
    uint8_t reserved[73];

    //The footer for this entry.
    EntryFooter_t footer;
} __attribute__((__packed__));
typedef struct _DatabaseHeader_def DatabaseHeader_t;


//This structure should be a multiple of the AES block length
_Static_assert(((sizeof(DatabaseHeader_t) - sizeof(EntryHeader_t) + 2) % 16) == 0, "DatabaseHeader must be rounded to the AES block length");

/**
 * Session header.  This entry contains information about this session.
 * 110 bytes + 42 bytes header + 32 bytes footer = 186 bytes
 */
struct _SessionHeader_def
{
    //The header for this entry.
    EntryHeader_t header;

    //a UUID identifying the TTE session. Encrypted.
    uint8_t sessionUuid[16];

    //The rolling shared secret. Encrypted.
    uint8_t rollingSecret[64];

    //The server UUID for this session. Encrypted.
    uint8_t serverUUID[16];

    //The hardened secret between the DBU and server for this session. Encrypted.
    uint8_t hardenedSecret[32];

    //Reserved bytes.  Round this structure to 160 bytes.  Encrypted.
    uint8_t reserved[14];

    //The footer for this entry.
    EntryFooter_t footer;
} __attribute__((__packed__));
typedef struct _SessionHeader_def SessionHeader_t;

//This structure should be a multiple of the AES block length
_Static_assert(((sizeof(SessionHeader_t) - sizeof(EntryHeader_t) + 2) % 16) == 0, "SessionHeader must be rounded to the AES block length");

/**
 * eKey Entry.  This entry contains information about an eKey in our database.
 * 78 bytes + 42 bytes header + 32 bytes footer = 152 bytes
 */
struct _EKeyEntry_def
{
    //The header for this entry.
    EntryHeader_t header;

    //the version of this eKey entry
    uint8_t version;

    //eKey type.  See EKeyType.
    uint8_t eKeyType;

    //The established shared secret for this eKey. Encrypted.
    uint8_t sharedSecret[32];

    //The permissions field for this eKey. Encrypted.
    uint32_t permissions;

    //The serial number for this eKey.  Must increment with each change /
    //revocation.  Encrypted.
    uint32_t serialNumber;

	//Reserved for future use
    uint8_t reserved[36];

    //The footer for this entry.
    EntryFooter_t footer;
} __attribute__((__packed__));
typedef struct _EKeyEntry_def EKeyEntry_t;

//This structure should be a multiple of the AES block length
_Static_assert(((sizeof(EKeyEntry_t) - sizeof(EntryHeader_t) + 2) % 16) == 0, "EKeyEntry must be rounded to the AES block length");

/**
 * User Preferences.
 * 62 bytes + 42 bytes header + 32 bytes footer = 136
 */
struct _UserPreferences_def
{
    //The header for this entry.
    EntryHeader_t header;

    //Bit field of preferences (see UserPreferencesBits)
    //All are on by default (0xFFFFFFFFFFFFFFFF)
    uint8_t preferencesBits[8];

    //Reserved for future use
    uint8_t reserved[54];

    //The footer for this entry.
    EntryFooter_t footer;
} __attribute__((__packed__));
typedef struct _UserPreferences_def UserPreferences_t;

//This structure should be a multiple of the AES block length
_Static_assert(((sizeof(UserPreferences_t) - sizeof(EntryHeader_t) + 2) % 16) == 0, "UserPreferences must be rounded to the AES block length");

#define INVALID_BLOCKREF					0xFFFF

//The record management bytes include all bytes up to and including the IV.
//There are only two bytes after the IV -- the encrypted bytes -- which must be
//part of the encrypted payload and the AES-CBC-MAC payload as per proper
//cryptographic use of these algorithms.  Therefore, this size should be
//sizeof(EntryHeader) - sizeof(EntryHeader::encBytes)
#define RECORD_MANAGEMENT_BYTES_SIZE (sizeof(EntryHeader_t) - sizeof(EntryHeader_t::encBytes))

//The PROTECTED region covers all of the record EXCEPT the footer and the first two bytes
#define HEADER_PROTECTED_SIZE ((sizeof(DatabaseHeader_t) - RECORD_MANAGEMENT_BYTES_SIZE) - sizeof(EntryFooter_t))
#define HEADER_PROTECTED_OFFSET RECORD_MANAGEMENT_BYTES_SIZE

//The PROTECTED region covers all of the session EXCEPT the footer and the first two bytes
#define SESSION_PROTECTED_SIZE ((sizeof(SessionHeader_t) - RECORD_MANAGEMENT_BYTES_SIZE) - sizeof(EntryFooter_t))
#define SESSION_PROTECTED_OFFSET RECORD_MANAGEMENT_BYTES_SIZE

//The PROTECTED region covers all of the eKey EXCEPT the footer and the first two bytes
#define EKEY_PROTECTED_SIZE ((sizeof(EKeyEntry_t) - RECORD_MANAGEMENT_BYTES_SIZE) - sizeof(EntryFooter_t))
#define EKEY_PROTECTED_OFFSET RECORD_MANAGEMENT_BYTES_SIZE

#endif /* INCLUDE_AEF_EMBEDDED_SERVICE_DATABASE_DATABASE_SERVICE_H_ */
