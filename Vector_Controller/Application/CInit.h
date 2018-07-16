
/**
* CInit.h
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definition of the CInit class.
*
*/

#ifndef APPLICATION_CINIT_H_
#define APPLICATION_CINIT_H_

#include <stdint.h>
#include "derivative.h" /* include peripheral declarations */
#include "fsl_gpio.h"
#include "versioninfo.h"
#include "aef/embedded/system/system_core.h"
#include <aef/embedded/system/system_property_manager.h>
#include "CThread.h"
#include "CConsole.h"
#include "CCriticalSection.h"

#include "bsp.h"
#include "frdmk64f.h"
#include "CUnitTest.h"
#include "CDemo.h"

#define DEFAULT_BANNER		DEVICE_INFO
#define DEFAULT_BANNER2		"\r\nUsing lib_aefCoOS, the Advanced Embedded Framework built for CoOS"
#define COPYRIGHT			"\r\nCopyright 2018, Advanced Embedded Frameworks, LLC\r\n\n"

#define INIT_TASK_STACK_SIZE	512

/**
* CInit - Perform application initialization
*/
class CInit : public CThread
{
public:

	/**
	* Destructor
	*/
	virtual ~CInit();

	/**
	* getInstance - Return an instance of CInit
	*/
	static CInit* getInstance(void);

	/**
	* Run - Start the CInit process
	*/
	void Run ( void );

private:

	/**
	* Constructor
	*/
	CInit(uint32_t stackSize = INIT_TASK_STACK_SIZE);

	void threadRun ( void );

	static CInit*		instance;

	CCriticalSection			m_CLock;
	property_manager_vtable_t* 	property_manager;
	CUnitTest* 					m_pCUnitTest;
	CDemo*						m_pCDemo;

};

#endif /* APPLICATION_CINIT_H_ */
