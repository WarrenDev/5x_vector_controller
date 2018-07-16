
/**
* CDemo.cpp
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Implementation of the CDemo class.
*
*/

#include "string.h"
#include "aef/embedded/osal/time_delay.h"
#include "CDemo.h"
#include "CConsole.h"
#include "versioninfo.h"

/***********************************************************
 * CDemo - Class constructor
 */
CDemo::CDemo(uint32_t stackSize)
	: CThread( stackSize )
	, device_manager(nullptr)
{
}

/***********************************************************
 * CDemo - Class destructor
 */
CDemo::~CDemo()
{
}

/***********************************************************
 * CDemo::Run
 *
 * Start the internal thread
 */
void CDemo::Run(void)
{
	/*******************************************************
	 * Initialize
	 */

	/*******************************************************
	 * Start the thread
	 */
	Start();
}

/***********************************************************
 * CDemo::threadRun
 *
 * Implementation of the threadRun method of CThread
 */
void CDemo::threadRun(void)
{
	/**
	 * Identify ourselves on the system console
	 */
	systemConsole->write ( "CDemo: starting...", 18 );

	THREAD_RUN_LOOP
	{
    	time_delay (10000);
	}
}
