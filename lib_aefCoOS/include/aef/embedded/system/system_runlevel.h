
/**
* system_runlevel.h
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief List of supported system run level identifiers.
*
*/
#ifndef INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_RUNLEVEL_H_
#define INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_RUNLEVEL_H_

#include <stdint.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

    /**
    * Enumeration of supported system run level identifiers.
    */
    typedef enum system_runlevel
    {

        /**
        * System run levels
        */
        SYS_RUNLEVEL0 = 0x0000,
        SYS_RUNLEVEL1 = 0x0001,
        SYS_RUNLEVEL2 = 0x0002,
        SYS_RUNLEVEL3 = 0x0003,
        SYS_RUNLEVEL4 = 0x0004,
        SYS_RUNLEVEL5 = 0x0005,
        SYS_RUNLEVEL6 = 0x0006,
        SYS_RUNLEVEL7 = 0x0007,
        SYS_RUNLEVEL8 = 0x0008,
        SYS_RUNLEVEL9 = 0x0009,
        SYS_RUNLEVEL10 = 0x000A,

    } system_runlevel_t;

    /* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif /* INCLUDE_AEF_EMBEDDED_SYSTEM_SYSTEM_RUNLEVEL_H_ */
