
/**
* uart_driver_install.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Definitions of UART driver installation routines.
*
*/

#ifndef INCLUDE_AEF_EMBEDDED_DRIVER_UART_UART_DRIVER_INSTALL_H_
#define INCLUDE_AEF_EMBEDDED_DRIVER_UART_UART_DRIVER_INSTALL_H_

#include <aef/embedded/driver/device_driver_status.h>

/* C++ guard */
# ifdef   __cplusplus
extern "C" {
# endif //__cplusplus

/**
* Install the UARTA device driver.
*
* \param    none
*
* \returns  DRIVER_STATUS_SUCCESSS if the device driver is successfully installed.
* 			DRIVER_FAILURE_GENERAL if device driver installation failed.
*/
driver_status_t uarta_driver_install (void);

/**
* Install the UARTB device driver.
*
* \param    none
*
* \returns  DRIVER_STATUS_SUCCESSS if the device driver is successfully installed.
* 			DRIVER_FAILURE_GENERAL if device driver installation failed.
*/
driver_status_t uartb_driver_install (void);

/**
* Install the UARTC device driver.
*
* \param    none
*
* \returns  DRIVER_STATUS_SUCCESSS if the device driver is successfully installed.
* 			DRIVER_FAILURE_GENERAL if device driver installation failed.
*/
driver_status_t uartc_driver_install (void);

/**
* Install the UARTD device driver.
*
* \param    none
*
* \returns  DRIVER_STATUS_SUCCESSS if the device driver is successfully installed.
* 			DRIVER_FAILURE_GENERAL if device driver installation failed.
*/
driver_status_t uartd_driver_install (void);

/* end C++ guard */
# ifdef   __cplusplus
} /* extern "C" */
# endif //__cplusplus

#endif /* INCLUDE_AEF_EMBEDDED_DRIVER_UART_UART_DRIVER_INSTALL_H_ */
