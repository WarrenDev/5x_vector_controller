
/**
* device_driver_status.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Device driver status codes.
*/
#ifndef  AEF_DEVICE_DRIVER_STATUS_HEADER_GUARD
# define AEF_DEVICE_DRIVER_STATUS_HEADER_GUARD


/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

    /**
    * Device driver status codes.  These codes represent various success and failure
    * codes that can arise when using device drivers and the device manager.
    *
    * Regular status codes have the high bit of the 32-bit word unset.
    * Failure codes have the high bit set.
    */
    typedef enum driver_status
    {
        /******** STATUS CODES ********/

        /**
        * The operation completed successfully.
        */
        DRIVER_STATUS_SUCCESS = 0x00000000,

        /******** FAILURE CODES ********/

        /**
        * A general failure.
        */
        DRIVER_FAILURE_GENERAL = 0x80000000,

        /**
        * The operation requested is not supported.
        */
        DRIVER_FAILURE_UNSUPPORTED_OPERATION = 0x80000001,

        /**
        * The size of the data buffer provided for this operation was not
        * sufficient.
        */
        DRIVER_FAILURE_INSUFFICIENT_SIZE = 0x80000002,

        /**
        * Incorrect mode for this operation
        */
        DRIVER_FAILURE_INCORRECT_MODE = 0x80000003,

        /**
        * Invalid parameter for this operation
        */
        DRIVER_FAILURE_INVALID_PARAMETER = 0x80000004,

        /**
        * Operation timed out
        */
        DRIVER_FAILURE_TIMEOUT = 0x80000005,

        /**
        * The requested self test failed.
        */
        DRIVER_FAILURE_SELF_TEST = 0x80800000,

        /**
        * General failure during initialization.
        */
        DRIVER_FAILURE_INITIALIZATION = 0x81000001,

        /**
        * The context buffer size is insufficient for initialization.
        */
        DRIVER_FAILURE_INSUFFICIENT_CONTEXT_SIZE = 0x81000003,

    } driver_status_t;

    /**
    * Helper macro which returns true if the given service_status_t value indicates
    * failure.
    */
#define DRIVER_FAILURE(x) (((x) & 0x80000000) != 0)

    /**
    * Helper macro which returns true if the given service_status_t value indicates
    * success.
    */
#define DRIVER_SUCCESS(x) (((x) & 0x80000000) == 0)

    /* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //AEF_DEVICE_DRIVER_STATUS_HEADER_GUARD
