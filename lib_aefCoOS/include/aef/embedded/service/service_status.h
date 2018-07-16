
/**
* service_status.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Service status codes.
*/

#ifndef  AEF_SERVICE_STATUS_HEADER_GUARD
# define AEF_SERVICE_STATUS_HEADER_GUARD

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

    /**
    * Service status codes.  These codes represent various success and failure
    * codes that can arise when using services and the service manager.
    *
    * Regular status codes have the high bit of the 32-bit word unset.
    * Failure codes have the high bit set.
    */
    typedef enum service_status
    {
        /******** STATUS CODES ********/

        /**
        * The operation completed successfully.
        */
        SERVICE_STATUS_SUCCESS = 0x00000000,

        /******** FAILURE CODES ********/

        /**
        * A general failure.
        */
        SERVICE_FAILURE_GENERAL = 0x80000000,

        /**
        * The operation requested is not supported.
        */
        SERVICE_FAILURE_UNSUPPORTED_OPERATION = 0x80000001,

        /**
        * The size of the data block provided for this operation was not
        * sufficient.
        */
        SERVICE_FAILURE_INSUFFICIENT_SIZE = 0x80000002,

        /**
        * Incorrect mode for this operation
        */
        SERVICE_FAILURE_INCORRECT_MODE = 0x80000003,

        /**
        * Service unavailable
        */
        SERVICE_FAILURE_UNAVAILABLE = 0x80000004,

        /**
        * Service offline
        */
        SERVICE_FAILURE_OFFLINE = 0x80000005,

        /**
        * Invalid parameter for this operation
        */
        SERVICE_FAILURE_INVALID_PARAMETER = 0x80000006,

		/**
		* No connection or connection dropped
		*/
		SERVICE_FAILURE_NO_CONNECTION = 0x80000007,

        /**
        * The requested self test failed.
        */
        SERVICE_FAILURE_SELF_TEST = 0x80800000,

        /**
        * General failure during initialization.
        */
        SERVICE_FAILURE_INITIALIZATION = 0x81000001,

    } service_status_t;

    /**
    * Helper macro which returns true if the given service_status_t value indicates
    * failure.
    */
#define SERVICE_FAILURE(x) (((x) & 0x80000000) != 0)

    /**
    * Helper macro which returns true if the given service_status_t value indicates
    * success.
    */
#define SERVICE_SUCCESS(x) (((x) & 0x80000000) == 0)

    /* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //AEF_SERVICE_STATUS_HEADER_GUARD
