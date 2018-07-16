
/**
* CDemo.h
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definition of the CDemo class.
*
*/

#ifndef APPLICATION_CDEMO_H_
#define APPLICATION_CDEMO_H_

#include <stdint.h>
#include "derivative.h" /* include peripheral declarations */
#include "fsl_gpio.h"
#include "versioninfo.h"
#include "aef/embedded/system/system_core.h"
#include "aef/embedded/system/system_init.h"
#include "aef/embedded/osal/time_delay.h"
#include "aef/embedded/driver/device_manager.h"
#include "aef/embedded/driver/stream_driver.h"
#include "CCriticalSection.h"
#include "CThread.h"
#include "CConsole.h"
#include "bsp.h"
#include "frdmk64f.h"

#define DEMO_TASK_STACK_SIZE	512

/***********************************************************
 * CDemo - Perform demonstration
 */
class CDemo : public CThread
{
public:

	/*******************************************************
	 * Construction / Destruction
	 */
	CDemo(uint32_t stackSize = DEMO_TASK_STACK_SIZE);
	virtual ~CDemo();

	void Run ( void );

private:

	void threadRun ( void );

	device_manager_vtable_t* 	device_manager;

};

#endif /* APPLICATION_CDEMO_H_ */
