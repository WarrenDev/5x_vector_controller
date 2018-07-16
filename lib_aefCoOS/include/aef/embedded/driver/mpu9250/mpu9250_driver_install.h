
/**
* mpu9250_driver_install.h
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions of MPU-9250 driver installation routines.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_DRIVER_MPU9250_MPU9250_DRIVER_INSTALL_H_
#define INCLUDE_AEF_EMBEDDED_DRIVER_MPU9250_MPU9250_DRIVER_INSTALL_H_

#include <aef/embedded/driver/device_driver_status.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

/**
* Install the MPU-9250 device driver.
*
* \param    none
*
* \returns  DRIVER_STATUS_SUCCESSS if the device driver is successfully installed.
* 			DRIVER_FAILURE_GENERAL if device driver installation failed.
*/
driver_status_t mpu9250_driver_install (void);

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif /* INCLUDE_AEF_EMBEDDED_DRIVER_MPU9250_MPU9250_DRIVER_INSTALL_H_ */
