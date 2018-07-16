
/**
* guid.h
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definition of the GUID data type.
*/

#ifndef INCLUDE_AEF_CUTILS_GUID_H_
#define INCLUDE_AEF_CUTILS_GUID_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/**
* GUID definition
*/
typedef struct _GUID
{
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} GUID;

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_AEF_CUTILS_GUID_H_ */
