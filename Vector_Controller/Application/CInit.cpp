
/**
* CInit.cpp
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Implementation of the CInit class.
*
*/
#include "string.h"
#include "aef/embedded/osal/time_delay.h"
#include "CInit.h"
#include "CConsole.h"
#include "versioninfo.h"

#define	INTERFACE_STARTUP_DELAY			5000	// Startup delay in ms
#define	INTERFACE_HEARTBEAT_INTERVAL	5000	// Heartbeat interval

CInit* CInit::instance = nullptr;

/**
* CInit - Class constructor
*/
CInit::CInit(uint32_t stackSize)
	: CThread( stackSize )
	, property_manager(nullptr)
	, m_pCUnitTest(nullptr)
	, m_pCDemo(nullptr)
{
}

/**
* CInit - Class destructor
*/
CInit::~CInit()
{
}

/**
* getInstance - Return an instance of CInit
*/
CInit* CInit::getInstance(void)
{
	if ( instance == nullptr )
	{
		instance = new CInit;
	}
	return instance;
}

/**
* CInit::Run
*
* Start the internal thread
*/
void CInit::Run(void)
{
	/**
	* Initialize
	*/
	property_manager = system_get_property_manager();

	/**
	* Start the thread
	*/
	Start();
}

/**
* CInit::threadRun
*
* Implementation of the threadRun method of CThread
*
* This is the initialization process.
* It's responsibility is to create and start all processes and services
* used by the application.  It also manages each process by monitoring
* the termination signal that will be sent by each process upon termination.
*/
void CInit::threadRun(void)
{
	/**
	* Delay to allow time for the network interface to initialize and settle
	*/
	time_delay (INTERFACE_STARTUP_DELAY);

	/**
	* Identify ourselves on the system console
	*/
	char buffer[80];
	uint16_t bufferLength = sprintf ( buffer, "%s v%02d.%02d.%02d.%04d\r\n", DEVICE_INFO, MAJOR_VER, MINOR_VER, REVISION, BUILD );
	systemConsole->write ( buffer, bufferLength );
	systemConsole->write ( "\r\n", 2 );
	systemConsole->write ( "CInit: Init process starting...", 31 );

	/**
	* Start the demo
	*/
//	(m_pCDemo = new CDemo)->Run();

	/**
	* Start the Unit Test
	*/
	(m_pCUnitTest = new CUnitTest)->Run();

	THREAD_RUN_LOOP
	{
    	time_delay (INTERFACE_HEARTBEAT_INTERVAL);
    	systemConsole->write ( "CInit: heartbeat...", 19 );
	}
}
