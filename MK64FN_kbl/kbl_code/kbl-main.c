
/**
* kbl-main.cpp
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief MK64FN kernel boot loader entry point
*/

#include "MK64F12.h"	/* peripheral declarations */

#include "bsp.h"
#include "kbl-app.h"

/*
* Main application entry point.
*   "This is where it begins and ends at the very same time."
*/
int main(void)
{
	/**
	* Run the boot loader
	*/
	kbl_run ();

	/**
	* Should never get here
	*/
	while ( 1 ) ;

  return 0;
}
