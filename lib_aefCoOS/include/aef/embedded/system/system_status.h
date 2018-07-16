
/**
*  system_status.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Service status codes.
*/

#ifndef INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_STATUS_H_
#define INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_STATUS_H_


/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

    /**
    * System status codes.
    *
    * Regular status codes have the high bit of the 32-bit word unset.
    * Failure codes have the high bit set.
    */
    typedef enum system_status
    {
        /******** STATUS CODES ********/

        /**
        * The operation completed successfully.
        */
        SYSTEM_STATUS_SUCCESS = 0x00000000,

        /******** FAILURE CODES ********/

        /**
        * A general failure.
        */
        SYSTEM_FAILURE_GENERAL = 0x80000000,

        /**
        * The operation requested is not supported.
        */
        SYSTEM_FAILURE_UNSUPPORTED_OPERATION = 0x80000001,

        /**
        * The size of the data block provided for this operation was not
        * sufficient.
        */
        SYSTEM_FAILURE_INSUFFICIENT_SIZE = 0x80000002,

        /**
        * Incorrect mode for this operation
        */
        SYSTEM_FAILURE_INCORRECT_MODE = 0x80000003,

        /**
        * System feature unavailable
        */
        SYSTEM_FAILURE_FEATURE_UNAVAILABLE = 0x80000004,

		/**
		* System failure timeout
		*/
		SYSTEM_FAILURE_TIMEOUT = 0x80000005,

        /**
        * System feature offline
        */
        SYSTEM_FAILURE_FEATURE_OFFLINE = 0x80000006,

		/**
		* System failure invalid parameter
		*/
		SYSTEM_FAILURE_INVALID_PARAMETER = 0x80000007,

		/**
		* System failure operation error
		*/
		SYSTEM_FAILURE_OPERATION_ERROR = 0x80000008,

		/**
		* System failure not ready
		*/
		SYSTEM_FAILURE_NOT_READY = 0x80000009,

        /**
        * The requested self test failed.
        */
        SYSTEM_FAILURE_SELF_TEST = 0x80800000,

        /**
        * General failure during initialization.
        */
        SYSTEM_FAILURE_INITIALIZATION = 0x81000001,

    } system_status_t;

    /**
    * Helper macro which returns true if the given system_status_t value indicates
    * failure.
    */
#define SYSTEM_FAILURE(x) (((x) & 0x80000000) != 0)

    /**
    * Helper macro which returns true if the given system_status_t value indicates
    * success.
    */
#define SYSTEM_SUCCESS(x) (((x) & 0x80000000) == 0)

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_STATUS_H_
