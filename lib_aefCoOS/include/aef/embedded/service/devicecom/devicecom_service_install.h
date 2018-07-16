
/**
* devicecom_service_install.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions of devicecom system service installation routines.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_SERVICE_DEVICECOM_DEVICECOM_SERVICE_INSTALL_H_
#define INCLUDE_AEF_EMBEDDED_SERVICE_DEVICECOM_DEVICECOM_SERVICE_INSTALL_H_

#include <aef/embedded/service/service_status.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

/**
* Install the devicecom system service.
*
* \param    none
*
* \returns  SERVICE_STATUS_SUCCESSS if the service is successfully installed.
* 			SERVICE_FAILURE_GENERAL if service installation failed.
*/
service_status_t devicecom_service_install (void);

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif /* INCLUDE_AEF_EMBEDDED_SERVICE_DEVICECOM_DEVICECOM_SERVICE_INSTALL_H_ */
