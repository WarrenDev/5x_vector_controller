
/**
* PoewitCertificateMem.h
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definition of the certificate non-volatile memory area.
*/

#ifndef KBL_CODE_POEWITCERTIFICATEMEM_H_
#define KBL_CODE_POEWITCERTIFICATEMEM_H_

#include <stdint.h>

/**
* ROM memory area size definitions
*/
#define	KBL_CERTS_AREA_SIZE		0x800

/**
* ROM memory pointer identifier definitions
*/
extern const uint8_t __certsmem[];

#endif /* KBL_CODE_POEWITCERTIFICATEMEM_H_ */

