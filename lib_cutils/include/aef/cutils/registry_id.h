
/**
* registry_id.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Registry id codes.
*/
#ifndef REGISTRY_ID_H
#define REGISTRY_ID_H

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

	/**
	* Enumeration of supported registry identifiers.
	*/
	typedef enum registry_id
	{
		/**
		* vtable registry
		*/
		REGISTRY_ID_VTABLE = 0x0001,

		/**
		* UUID registry
		*/
		REGISTRY_ID_UUID = 0x0010,

		/**
		* String registry
		*/
		REGISTRY_ID_STRING = 0x0020

	} registry_id_t;

	/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //REGISTRY_ID_H
