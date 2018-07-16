
/**
* CUnitTest.cpp
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Implementation of the CUnitTest class.
*
*/

#include "string.h"
#include "aef/embedded/osal/time_delay.h"
#include "aef/embedded/driver/mpu9250/mpu9250_driver.h"
#include "CUnitTest.h"
#include "CConsole.h"
#include "CVIDLComm.h"

#define TEST_GPRMC	"$GPRMC,220516,A,5133.82,N,00042.24,W,173.8,231.8,130694,004.2,W*70\r\n"

/**
* CUnitTest - Class constructor
*/
CUnitTest::CUnitTest(uint32_t stackSize)
	: CThread( stackSize )
	, service_manager(nullptr)
{
}

/**
* CUnitTest - Class destructor
*/
CUnitTest::~CUnitTest()
{
}

/**
* CUnitTest::Run
*
* Start the internal thread
*/
void CUnitTest::Run(void)
{
	/**
	* Initialize
	*/

	/**
	* Start the thread
	*/
	Start();
}

/**
 * CUnitTest::threadRun
 *
 * Implementation of the threadRun method of CThread
 */
void CUnitTest::threadRun(void)
{
	device_manager_vtable_t* device_manager = system_get_device_manager ();
	stream_driver_vtable_t* mpu9250_drv = device_manager->getdevice (DRV_SENSOR_MPU9250);
	stream_driver_ctx_t* 	mpu9250_ctx;
	uint8_t device_id = 0xFF;

	CVIDLComm* VidlComm = CVIDLComm::getInstance();

	if ( mpu9250_drv )
	{
		/**
		 * Open the device driver
		 */
		mpu9250_ctx = mpu9250_drv->open (mpu9250_drv->getname(), 0, 0);
		uint32_t bytes_read = 0;
		driver_status_t status;
		status = mpu9250_drv->iocontrol (mpu9250_ctx, IOCTL_MPU9250_READ_ID, NULL, 0L, &device_id, 1, &bytes_read);
		status = mpu9250_drv->iocontrol (mpu9250_ctx, IOCTL_MPU9250_READ_ID2, NULL, 0L, &device_id, 1, &bytes_read);
	}

	systemConsole->write ( "CUnitTest: starting...", 18 );

	THREAD_RUN_LOOP
	{
    	time_delay (10000);
    	systemConsole->write ( "CUnitTest: sending VIDL message...", 34 );
    	VidlComm->sendVIDL( TEST_GPRMC, 0 );
	}
}

