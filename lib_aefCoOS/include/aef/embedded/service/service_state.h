
/**
* service_state.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Service state codes.
*/

#ifndef INCLUDE_AEF_EMBEDDED_SERVICE_SERVICE_STATE_H_
#define INCLUDE_AEF_EMBEDDED_SERVICE_SERVICE_STATE_H_

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

    /**
    * Service state codes.  Service state values are used to indicate
    * the current state of a service.  Service state can be queried
    * using the IOCTL_SERVICE_STATUS system service control code.
    */
    typedef enum service_state
    {
        /******** STATE CODES ********/

        /**
        * The initial state of a service
        */
        SERVICE_UNINITIALIZED = 0x00000000,

        /**
        * The service is not running
        */
        SERVICE_STOPPED = 0x00000001,

        /**
        * The service is in the process of starting but is not yet ready
        * to respond to requests
        */
        SERVICE_START_PENDING = 0x00000002,

        /**
        * The service is stopping but has not completed shutdown
        */
        SERVICE_STOP_PENDING = 0x00000003,

        /**
        * The service is running
        */
        SERVICE_RUNNING = 0x00000004,

        /**
        * The service is in the process of resuming from a pause
        */
        SERVICE_CONTINUE_PENDING = 0x00000005,

        /**
        * The service pause is in progress, but yet safely paused
        */
        SERVICE_PAUSE_PENDING = 0x00000006,

        /**
        * The service is paused
        */
        SERVICE_PAUSED = 0x00000007,

        /**
        * The service has been disabled and is not running
        */
        SERVICE_DISABLED = 0x00000008,

    } service_state_t;

    /* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif /* INCLUDE_AEF_EMBEDDED_SERVICE_SERVICE_STATE_H_ */
