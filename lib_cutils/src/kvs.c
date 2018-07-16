
/**
* kvs.c
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Implementation of the key/value store code module
*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bsp.h"
#include "aef/cutils/kvs.h"

static const size_t kvs_pair_size = sizeof(kvs_pair_t);

static const size_t kvs_store_size = sizeof(kvs_store_t);

static kvs_pair_t* kvs_get_pair (kvs_store_t *store, const void *key)
{
    if ( store && store->length )
    {
        for ( uint16_t index = 0; index < store->length; index++ )
        {
        	if ( memcmp ( store->pairs[index].key, key, store->pairs[index].key_length ) == 0 )
        	{
        		return ( store->pairs + index );
        	}
        }
    }
    return NULL;
}

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
kvs_store_t* kvs_create (uint16_t entries)
{
    kvs_store_t* store = malloc(kvs_store_size);
    if ( store )
    {
    	store->max_length = entries;
        store->pairs = malloc ( kvs_pair_size * store->max_length );
        if ( store->pairs == NULL )
        {
        	kvs_destroy (store);
        	store = NULL;
        }
        else
        {
        	kvs_empty ( store );
        }
    }
    return store;
}

/**
* @fn      kvs_destroy
*
* @brief   Delete a key/value store
*
* @param   store			Pointer to a kvs_store_t data structure
*
* @return  none
*/
void kvs_destroy (kvs_store_t* store)
{
    if (store)
    {
    	if (store->pairs)
    	{
    		free(store->pairs);
    	}
        free(store);
    }
}

/**
* @fn      kvs_empty
*
* @brief   Remove key/value entries and initialize store
*
* @param   store			Pointer to a kvs_store_t data structure
*
* @return  none
*/
void kvs_empty (kvs_store_t* store)
{
    if (store)
    {
    	if (store->pairs)
    	{
    		memset ( store->pairs, 0, kvs_pair_size * store->max_length);
    	}
    	store->length = 0;
    }
}

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
bool kvs_put (kvs_store_t* store, const void *key, uint16_t key_size, void* value, uint16_t value_size)
{
	kvs_pair_t* entry = kvs_get_pair ( store, key );
	if ( (entry == NULL) && (store->length < store->max_length) )
	{
		entry = store->pairs + store->length;
		store->length++;
	}
	if ( entry )
	{
		entry->key = key;
		entry->key_length = key_size;
		entry->value = value;
		entry->value_length = value_size;
		return TRUE;
	}
	return FALSE;
}

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
kvs_pair_t* kvs_get (kvs_store_t* store, const void *key)
{
	return kvs_get_pair ( store, key );
}

/**
* @fn      kvs_jsmn_load
*
* @brief   Helper routine to support the JSMN embedded JSON parser.
*  		   Load a KVS store from JSMN tokens.
*
* @param   store			Pointer to a kvs_store_t data structure
* @param   message			Pointer to original message
* @param   tokens			Pointer to jsmntok_t memory
* @param   size				Number of JSMN tokens
*
* @return  TRUE on success
*          FALSE on failure
*/
bool kvs_jsmn_load (kvs_store_t* store, void* message, jsmntok_t* tokens, uint16_t size)
{
	bool result = FALSE;
	if ( store )
	{
		/**
		* Clear the store.
		* Skip the first token in the list which is always the object identifier.
		*/
		kvs_empty ( store );
		tokens++;

		/**
		* Load the store
		*/
		for ( uint16_t token_index = 1; token_index < size; tokens += 2, token_index += 2 )
		{
			result = kvs_put ( store,
					           message + tokens->start, tokens->end - tokens->start,
							   message + (tokens+1)->start, (tokens+1)->end - (tokens+1)->start );
			if ( result == FALSE )
				break;
		}
	}
	return result;
}

