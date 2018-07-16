
/**
* kvs.h
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definitions used by the key/value store code module
*/

#ifndef INCLUDE_AEF_CUTILS_KVS_H_
#define INCLUDE_AEF_CUTILS_KVS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "aef/cutils/jsmn.h"

#define MAX_KVS_ENTRIES			10

typedef struct
{
    const void* key;
    uint16_t 	key_length;
    void*		value;
    uint16_t 	value_length;
} kvs_pair_t;

typedef struct
{
    kvs_pair_t*	pairs;
    uint16_t	max_length;
    uint16_t 	length;
} kvs_store_t;

/**
* @fn      kvs_create
*
* @brief   Create and initialize a new key/value store
*
* @param   entries			Maximum number of key/value pairs
*
* @return  Pointer to a new kvs_store_t data structure on success
*          NULL on error
*/
kvs_store_t* kvs_create(uint16_t entries);

/**
* @fn      kvs_destroy
*
* @brief   Delete a key/value store
*
* @param   store			Pointer to a kvs_store_t data structure
*
* @return  none
*/
void kvs_destroy (kvs_store_t* store);

/**
* @fn      kvs_empty
*
* @brief   Remove key/value entries and initialize store
*
* @param   store			Pointer to a kvs_store_t data structure
*
* @return  none
*/
void kvs_empty (kvs_store_t* store);

/**
* @fn      kvs_put
*
* @brief   Add or update a key/value pair entry in the store
*
* @param   store			Pointer to a kvs_store_t data structure
* @param   key				Pointer to the key
* @param   key_size			Length of the key
* @param   value	    	Pointer to the value
* @param   value_size		Length of the value
*
* @return  TRUE on success
*          FALSE on failure
*/
bool kvs_put (kvs_store_t* store, const void *key, uint16_t key_size, void* value, uint16_t value_size);

/**
* @fn      kvs_get
*
* @brief   Look up a key/value pair entry in the store
*
* @param   store			Pointer to a kvs_store_t data structure
* @param   key				Pointer to the key
* @param   key_size			Length of the key
* @param   value	    	Pointer to the value
* @param   value_size		Length of the value
*
* @return  Pointer to a kvs_pair_t data structure on success
*          NULL if not found
*/
kvs_pair_t* kvs_get (kvs_store_t* store, const void *key);

/**
* @fn      kvs_jsmn_load
*
* @brief   Helper routine to support the JSMN embedded JSON parser.
*  		   Load a KVS store from JSMN tokens.
*
* @param   store			Pointer to a kvs_store_t data structure
* @param   message			Pointer to original message
* @param   jsmn_tokens		Pointer to jsmntok_t memory
* @param   size				Number of JSMN tokens
*
* @return  TRUE on success
*          FALSE on failure
*/
bool kvs_jsmn_load (kvs_store_t* store, void* message, jsmntok_t* tokens, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_AEF_CUTILS_KVS_H_ */
