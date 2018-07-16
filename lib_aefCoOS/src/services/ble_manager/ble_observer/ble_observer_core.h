
/**
* ble_observer_core.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  BLE Manager observer core definitions.
*
*/

#ifndef SRC_SERVICES_BLE_MANAGER_BLE_OBSERVER_BLE_OBSERVER_CORE_H_
#define SRC_SERVICES_BLE_MANAGER_BLE_OBSERVER_BLE_OBSERVER_CORE_H_

#include <aef/embedded/service/ble_manager/ble_manager_service.h>
#include <aef/embedded/service/service_status.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

# ifdef   __cplusplus
extern "C" {
# endif

#define	MAX_BLE_OBSERVERS	50

/**
* Initialize the BLE observer container
*
* \param 	none
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t ble_observer_core_init (void);

/**
* Attach a BLE observer
*
* \param 	instance	Observer instance data
* \param	hci_msg		HCI message to be observed
* \param	noti		Observer notification function pointer
* \param	index		Pointer to index
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t ble_observer_core_attach (void* instance, uint16_t hci_msg, hci_msg_notification_func_t noti);

/**
* Detach a BLE observer
*
* \param 	instance	Observer instance data
* \param	hci_msg		HCI message to be observed
* \param	noti		Observer notification function pointer
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t ble_observer_core_detach (uint16_t hci_msg, hci_msg_notification_func_t noti);

/**
* Notify BLE observers
*
* \param	hci_msg		HCI message to be observed
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t ble_observer_core_notify (uint16_t hci_msg, void* hci_msg_data);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif /* SRC_SERVICES_BLE_MANAGER_BLE_OBSERVER_BLE_OBSERVER_CORE_H_ */
