
/**
* registry_status.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Registry status codes.
*/
#ifndef REGISTRY_STATUS_H
#define REGISTRY_STATUS_H

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

	/**
	* Registry status codes.  These codes represent various success and failure
	* codes that can arise when using the registry component of the UniKey cutils library.
	*
	* Regular status codes have the high bit of the 32-bit word unset.
	* Failure codes have the high bit set.
	*/
	typedef enum registry_status
	{
		/******** STATUS CODES ********/

		/**
		* The operation completed successfully.
		*/
		REGISTRY_STATUS_SUCCESS = 0x00000000,

		/******** FAILURE CODES ********/

		/**
		* A general failure.
		*/
		REGISTRY_FAILURE_GENERAL = 0x80000000,

		/**
		* The operation requested is not supported.
		*/
		REGISTRY_FAILURE_UNSUPPORTED_OPERATION = 0x80000001,

		/**
		* The size of the registry item provided for this operation was incorrect.
		*/
		REGISTRY_FAILURE_UNSUPPORTED_SIZE = 0x80000002,

		/**
		* Incorrect mode for this operation
		*/
		REGISTRY_FAILURE_INCORRECT_MODE = 0x80000003,

		/**
		* The requested self test failed.
		*/
		REGISTRY_FAILURE_SELF_TEST = 0x80800000,

		/**
		* General failure during initialization.
		*/
		REGISTRY_FAILURE_INITIALIZATION = 0x81000001,

	} registry_status_t;

	/**
	* Helper macro which returns true if the given registry_status_t value indicates
	* failure.
	*/
#define REGISTRY_FAILURE(x) (((x) & 0x80000000) != 0)

	/**
	* Helper macro which returns true if the given registry_status_t value indicates
	* success.
	*/
#define REGISTRY_SUCCESS(x) (((x) & 0x80000000) == 0)

	/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //REGISTRY_STATUS_H
