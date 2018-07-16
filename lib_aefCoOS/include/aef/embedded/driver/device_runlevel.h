
/**
* device_runlevel.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief List of supported device driver run level identifiers.
*
*/
#ifndef  AEF_DEVICE_RUNLEVEL_HEADER_GUARD
# define AEF_DEVICE_RUNLEVEL_HEADER_GUARD

#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

    /**
    * Enumeration of supported device driver run level identifiers.
    */
    typedef enum device_runlevel
    {
        /**
        * Device driver run levels
        */
        DRV_RUNLEVEL0 = 0x0000,
        DRV_RUNLEVEL1 = 0x0001,
        DRV_RUNLEVEL2 = 0x0002,
        DRV_RUNLEVEL3 = 0x0003,
        DRV_RUNLEVEL4 = 0x0004,
        DRV_RUNLEVEL5 = 0x0005

    } device_runlevel_t;

    /* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif //AEF_DEVICE_RUNLEVEL_HEADER_GUARD
