
/**
* app-main.cpp
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Application entry point
*/

#include "derivative.h" /* include peripheral declarations */
#include "aef/embedded/system/system_init.h"
#include "CConsole.h"

#include "bsp.h"
#include "frdmk64f.h"
#include "CInit.h"

CConsole* systemConsole = nullptr;

/*
* Main application entry point.
*   "This is where it begins and ends at the very same time."
*/
int main(void)
{
	SystemCoreClockUpdate();

	/**
	 * Initialize the O/S
	 */
	CoInitOS();

	/**
	 * Initialize the advanced embedded framework
	 */
	aef_system_init ();

	/**
	 * Initialize the system console
	 */
	systemConsole = CConsole::getInstance();

	/**
	 * Initialize the application layer
	 */
    CInit *appInit = CInit::getInstance();
    appInit->Run();

	/**
	 * Start the O/S
	 */
	CoStartOS ();

	/**
	 * Should never get here
	 */
	while ( 1 ) ;

  return 0;
}
