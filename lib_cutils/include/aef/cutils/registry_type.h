
/**
* registry_type.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Registry type codes.
*/
#ifndef REGISTRY_TYPE_H
#define REGISTRY_TYPE_H

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

	/**
	* Enumeration of supported registry type identifiers.
	*/
	typedef enum registry_type
	{
		/**
		* vtable registry
		*/
		REGISTRY_TYPE_VTABLE = 0x0001,

		/**
		* UUID registry
		*/
		REGISTRY_TYPE_UUID = 0x0010,

		/**
		* String registry
		*/
		REGISTRY_TYPE_STRING = 0x0020

	} registry_type_t;

	/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //REGISTRY_TYPE_H
