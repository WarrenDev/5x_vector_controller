
/**
* service_runlevel.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief List of supported service run level identifiers.
*
*/
#ifndef  AEF_SERVICE_RUNLEVEL_HEADER_GUARD
# define AEF_SERVICE_RUNLEVEL_HEADER_GUARD

#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

    /**
    * Enumeration of supported service run level identifiers.
    */
    typedef enum service_runlevel
    {

        /**
        * Service run levels
        */
        SRV_RUNLEVEL0 = 0x0000,
        SRV_RUNLEVEL1 = 0x0001,
        SRV_RUNLEVEL2 = 0x0002,
        SRV_RUNLEVEL3 = 0x0003,
        SRV_RUNLEVEL4 = 0x0004,
        SRV_RUNLEVEL5 = 0x0005,
        SRV_RUNLEVEL6 = 0x0006,
        SRV_RUNLEVEL7 = 0x0007,
        SRV_RUNLEVEL8 = 0x0008,
        SRV_RUNLEVEL9 = 0x0009,
        SRV_RUNLEVEL10 = 0x000A,

    } service_runlevel_t;

    /* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //AEF_SERVICE_RUNLEVEL_HEADER_GUARD
