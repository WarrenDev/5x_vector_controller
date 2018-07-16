
/**
* system_descriptors.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief System descriptor table definitions.
*/

#ifndef INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_DESCRIPTORS_H_
#define INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_DESCRIPTORS_H_

#include <stdint.h>

#define	SDT_PROPERTIES_OFFSET		0
#define	SDT_EVENTS_OFFSET			1
#define SDT_DEVICE_MANAGER_OFFSET	2
#define SDT_SERVICE_MANAGER_OFFSET	3
#define	SDT_FAULT_MANAGER_OFFSET	4

#define	UDT_PROPERTIES_OFFSET		0
#define	UDT_EVENTS_OFFSET			1
#define UDT_SERVICE_MANAGER_OFFSET	2

# ifdef   __cplusplus
extern "C" {
# endif

	/**
	 *  Descriptor property structure definition
	 */
	typedef struct descriptor_properties_def
	{
		/**
		 * Segment type
		 *    0: 32-bit, 1: 64-bit
		 */
		int32_t	segment_type:1;

		/*
		 * Privilege level
		 *    0: ring 0 (highest), 3: ring 3 (lowest)
		 */
		int32_t	privilege_level:2;

		/*
		 * Descriptor type
		 *    0: data, 1: code
		 */
		int32_t	type:1;

		/*
		 * Read only
		 *    0:  read/write, 1: read only
		 */
		int32_t	read_only:1;

		/*
		 * Reserved
		 */
		int32_t	reserved1:3;

		/*
		 * Access (used with virtual memory)
		 */
		int32_t	access:1;

		/*
		 * In memory (used with virtual memory)
		 *    0: segment is not in memory, 1: segment is in memory
		 */
		int32_t in_memory:1;

		/*
		 * Granularity
		 *    0: none, 1: limits multiplied by 4K
		 */
		int32_t	granularity:1;

		/*
		 * Reserved
		 */
		int32_t	reserved2:5;

		/*
		 * Padding
		 */
		int32_t pad:16;
	} descriptor_properties_t;

	/**
	 *  Descriptor entry structure definition
	 */
	typedef struct descriptor_entry_def
	{
		union
		{
			uint32_t _value;
			descriptor_properties_t _bits;
		} type;
		const void* address;
	} descriptor_entry_t, *p_descriptor_entry_t;

	/**
	 *  System Descriptor Table structure definition
	 */
	typedef struct system_descriptor_table_def
	{
		descriptor_entry_t	property_manager;
		descriptor_entry_t	event_manager;
		descriptor_entry_t	device_manager;
		descriptor_entry_t	service_manager;
		descriptor_entry_t	fault_manager;
	} system_descriptor_table_t;

	/**
	 *  User Descriptor Table structure definition
	 */
	typedef struct user_descriptor_table_def
	{
		descriptor_entry_t	property_manager;
		descriptor_entry_t	event_manager;
		descriptor_entry_t	service_manager;
	} user_descriptor_table_t;

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif /* INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_DESCRIPTORS_H_ */
