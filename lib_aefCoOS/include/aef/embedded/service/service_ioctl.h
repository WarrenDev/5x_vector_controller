
/**
* service_ioctl.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief List of service control codes.
*
*/
#ifndef  AEF_SERVICE_IOCTL_HEADER_GUARD
# define AEF_SERVICE_IOCTL_HEADER_GUARD

#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

    /**
    * Enumeration of service control codes.
    */
    typedef enum service_ioctl
    {

        /**
        * Start the service
        */
        IOCTL_SERVICE_START     = 0x00000001,

        /**
        * Restart the service
        */
        IOCTL_SERVICE_RESTART   = 0x00000002,

        /**
        * Stop the service 
        */
        IOCTL_SERVICE_STOP      = 0x00000003,

        /**
        * Query service status 
        */
        IOCTL_SERVICE_STATUS    = 0x00000004,

        /**
        * Pause the service
        */
        IOCTL_SERVICE_PAUSE     = 0x00000005,

        /**
        * Continue service execution
        */
        IOCTL_SERVICE_CONTINUE  = 0x00000006

    } service_ioctl_t;

    /* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //AEF_SERVICE_IOCTL_HEADER_GUARD
