
/**
* CUnitTest.h
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definition of the CUnitTest class.
*
*/

#ifndef APPLICATION_CUNITTEST_H_
#define APPLICATION_CUNITTEST_H_

#include <stdint.h>
#include "derivative.h" /* include peripheral declarations */
#include "aef/embedded/system/system_core.h"
#include <aef/embedded/driver/stream_driver.h>
#include <aef/embedded/system/system_management.h>
#include <aef/embedded/service/service_id.h>
#include "aef/embedded/driver/device_manager.h"
#include "CCriticalSection.h"
#include "CThread.h"
#include "CConsole.h"
#include "bsp.h"
#include "frdmk64f.h"

#define UNITTEST_TASK_STACK_SIZE	512

/**
* CUnitTest - Perform Unit Tests
*/
class CUnitTest : public CThread
{
public:

	/**
	* Construction / Destruction
	*/
	CUnitTest(uint32_t stackSize = UNITTEST_TASK_STACK_SIZE);
	virtual ~CUnitTest();

	void Run ( void );

private:

	void threadRun ( void );

	service_manager_vtable_t* 	service_manager;
	CCriticalSection			CLock;
};

#endif /* APPLICATION_CUNITTEST_H_ */
