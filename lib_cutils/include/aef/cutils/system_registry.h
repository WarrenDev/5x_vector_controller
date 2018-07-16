
/**
* system_registry.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions use by the system registry.
*/

#ifndef SYSTEM_REGISTRY_H
#define SYSTEM_REGISTRY_H

#include <aef/cutils/registry_id.h>
#include <aef/cutils/registry_status.h>
#include <aef/cutils/registry_type.h>
#include <aef/cutils/string_binary_search.h>
#include <aef/cutils/uuid_binary_search.h>
#include <aef/cutils/vtable_binary_search.h>
#include <stdint.h>

# ifdef   __cplusplus
extern "C" {
# endif

#define SYSTEM_REGISTRY_DEFAULT_ID		0x00000001
#define	SYSTEM_REGISTRY_SIZE_INITIAL	50
#define SYSTEM_REGISTRY_SIZE_INCREMENT	50
#define SYSTEM_REGISTRY_SIZE_MAX		200

typedef void* registry_table_t;
typedef void* registry_entry_t;

////////////////////////////////////////////////////////
// System registry container structure definition
typedef struct system_registry_def
{
	uint32_t			registry_id;
	uint32_t			registry_type;
	uint32_t			registry_entries;
	registry_table_t	registry;
} system_registry_t, *p_system_registry_t;

////////////////////////////////////////////////////////
// System registry routines
system_registry_t* system_registry_create(registry_type_t type);
registry_status_t  system_registry_destroy(system_registry_t* registry_container);
registry_status_t  system_registry_add(system_registry_t* registry_container, registry_entry_t registry_item);
registry_entry_t   system_registry_get_item(system_registry_t* registry_container, void* item);
uint32_t		   system_registry_get_item_count(system_registry_t* registry_container);
registry_entry_t   system_registry_iterator(system_registry_t* registry_container, void* callback, uint32_t param);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif // SYSTEM_REGISTRY_H
