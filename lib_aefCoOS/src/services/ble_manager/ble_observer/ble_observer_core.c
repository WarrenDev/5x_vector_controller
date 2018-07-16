
/**
* ble_observer_core.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  BLE observer core implementation.
*
*/

	#include "CoOS.h"
	#include "ble_observer_core.h"

	static hci_msg_observer_t observer_list[MAX_BLE_OBSERVERS];

/**
* Initialize the BLE observer container
*
* \param 	none
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t ble_observer_core_init (void)
{
	memset (observer_list, 0, sizeof(observer_list));
	return SERVICE_STATUS_SUCCESS;
}

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
service_status_t ble_observer_core_attach (void* instance, uint16_t hci_msg, hci_msg_notification_func_t noti)
{
	if ( instance != NULL && noti != NULL )
	{
		for ( uint32_t index = 1; index < MAX_BLE_OBSERVERS; index++ )
		{
			if ( observer_list[index].instance == NULL && observer_list[index].notification == NULL )
			{
				observer_list[index].instance = instance;
				observer_list[index].hci_msg = hci_msg;
				observer_list[index].notification = noti;
				return SERVICE_STATUS_SUCCESS;
			}
		}
		return SERVICE_FAILURE_GENERAL;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Detach a BLE observer
*
* \param	hci_msg		HCI message to be observed
* \param	noti		Observer notification function pointer
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t ble_observer_core_detach (uint16_t hci_msg, hci_msg_notification_func_t noti)
{
	if ( noti != NULL )
	{
		for ( uint32_t index = 1; index < MAX_BLE_OBSERVERS; index++ )
		{
			if ( observer_list[index].hci_msg == hci_msg && observer_list[index].notification == noti )
			{
				observer_list[index].instance = NULL;
				observer_list[index].hci_msg = 0x0000;
				observer_list[index].notification = NULL;
				return SERVICE_STATUS_SUCCESS;
			}
		}
		return SERVICE_FAILURE_GENERAL;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}

/**
* Notify BLE observers
*
* \param	hci_msg		HCI message to be observed
*
* \returns  SERVICE_STATUS_SUCCESS if successful.
*           SERVICE_FAILURE_GENERAL if unsuccessful.
*           SERVICE_FAILURE_INVALID_PARAMETER if an invalid parameter.
*/
service_status_t ble_observer_core_notify (uint16_t hci_msg, void* hci_msg_data)
{
	if ( hci_msg != 0 && hci_msg_data != NULL )
	{
		for ( uint32_t index = 0; index < MAX_BLE_OBSERVERS; index++ )
		{
			if ( index == 0 )
			{
				continue;
			}
			if ( observer_list[index].hci_msg == hci_msg && observer_list[index].notification != NULL )
			{
				(*observer_list[index].notification)(observer_list[index].instance, hci_msg_data);
			}
		}
		return SERVICE_STATUS_SUCCESS;
	}
	return SERVICE_FAILURE_INVALID_PARAMETER;
}
