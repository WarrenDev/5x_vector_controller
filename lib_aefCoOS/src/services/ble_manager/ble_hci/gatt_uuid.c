
/**
* gatt_uuid.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  GATT UUID types
*
*/

#include <stdint.h>
#include "gatt.h"
#include "gatt_uuid.h"

#define MSB(word) 	(uint8_t)(((uint16_t)(word) >> 8) & 0xFF)
#define LSB(word) 	(uint8_t)((uint16_t)(word) & 0xFF)

/**
 * GATT Services
 */
// Generic Access Profile Service UUID
const uint8_t gapServiceUUID[ATT_BT_UUID_SIZE] =
{
  LSB( GAP_SERVICE_UUID ), MSB( GAP_SERVICE_UUID )
};

// Generic Attribute Profile Service UUID
const uint8_t gattServiceUUID[ATT_BT_UUID_SIZE] =
{
  LSB( GATT_SERVICE_UUID ), MSB( GATT_SERVICE_UUID )
};

/**
 * GATT Attribute Types
 */
// Primary Service UUID
const uint8_t primaryServiceUUID[ATT_BT_UUID_SIZE] =
{
  LSB( GATT_PRIMARY_SERVICE_UUID ), MSB( GATT_PRIMARY_SERVICE_UUID )
};

// Secondary Service UUID
const uint8_t secondaryServiceUUID[ATT_BT_UUID_SIZE] =
{
  LSB( GATT_SECONDARY_SERVICE_UUID ), MSB( GATT_SECONDARY_SERVICE_UUID )
};

// Include UUID
const uint8_t includeUUID[ATT_BT_UUID_SIZE] =
{
  LSB( GATT_INCLUDE_UUID ), MSB( GATT_INCLUDE_UUID )
};

// Characteristic UUID
const uint8_t characterUUID[ATT_BT_UUID_SIZE] =
{
  LSB( GATT_CHARACTER_UUID ), MSB( GATT_CHARACTER_UUID )
};

/**
 * GATT Characteristic Descriptors
 */
// Characteristic Extended Properties UUID
const uint8_t charExtPropsUUID[ATT_BT_UUID_SIZE] =
{
  LSB( GATT_CHAR_EXT_PROPS_UUID ), MSB( GATT_CHAR_EXT_PROPS_UUID )
};

// Characteristic User Description UUID
const uint8_t charUserDescUUID[ATT_BT_UUID_SIZE] =
{
  LSB( GATT_CHAR_USER_DESC_UUID ), MSB( GATT_CHAR_USER_DESC_UUID )
};

// Client Characteristic Configuration UUID
const uint8_t clientCharCfgUUID[ATT_BT_UUID_SIZE] =
{
  LSB( GATT_CLIENT_CHAR_CFG_UUID ), MSB( GATT_CLIENT_CHAR_CFG_UUID )
};

// Server Characteristic Configuration UUID
const uint8_t servCharCfgUUID[ATT_BT_UUID_SIZE] =
{
  LSB( GATT_SERV_CHAR_CFG_UUID ), MSB( GATT_SERV_CHAR_CFG_UUID )
};

// Characteristic Format UUID
const uint8_t charFormatUUID[ATT_BT_UUID_SIZE] =
{
  LSB( GATT_CHAR_FORMAT_UUID ), MSB( GATT_CHAR_FORMAT_UUID )
};

// Characteristic Aggregate Format UUID
const uint8_t charAggFormatUUID[ATT_BT_UUID_SIZE] =
{
  LSB( GATT_CHAR_AGG_FORMAT_UUID ), MSB( GATT_CHAR_AGG_FORMAT_UUID )
};

/**
 * GATT Characteristic Types
 */
// Device Name UUID
const uint8_t deviceNameUUID[ATT_BT_UUID_SIZE] =
{
  LSB( DEVICE_NAME_UUID ), MSB( DEVICE_NAME_UUID )
};

// Appearance UUID
const uint8_t appearanceUUID[ATT_BT_UUID_SIZE] =
{
  LSB( APPEARANCE_UUID ), MSB( APPEARANCE_UUID )
};

// Peripheral Privacy Flag UUID
const uint8_t periPrivacyFlagUUID[ATT_BT_UUID_SIZE] =
{
  LSB( PERI_PRIVACY_FLAG_UUID ), MSB( PERI_PRIVACY_FLAG_UUID )
};

// Reconnection Address UUID
const uint8_t reconnectAddrUUID[ATT_BT_UUID_SIZE] =
{
  LSB( RECONNECT_ADDR_UUID ), MSB( RECONNECT_ADDR_UUID )
};

// Peripheral Preferred Connection Parameters UUID
const uint8_t periConnParamUUID[ATT_BT_UUID_SIZE] =
{
  LSB( PERI_CONN_PARAM_UUID ), MSB( PERI_CONN_PARAM_UUID )
};

// Service Changed UUID
const uint8_t serviceChangedUUID[ATT_BT_UUID_SIZE] =
{
  LSB( SERVICE_CHANGED_UUID ), MSB( SERVICE_CHANGED_UUID )
};

