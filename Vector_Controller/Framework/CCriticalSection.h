
/**
* CCriticalSection.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definition of the CCriticalSection class
*
*/

#ifndef CCRITICALSECTION_H_
#define CCRITICALSECTION_H_

#include "aef/embedded/osal/critical_section.h"

/***********************************************************
 * Critical section class definition
 */
class CCriticalSection {

public:

	/*******************************************************
	 * CCriticalSection constructor
	 */
	CCriticalSection ()
	{
		ctx.mutex     = -1;
		ctx.ref_count = 0;
		critical_section_create (&ctx);
	}

	/*******************************************************
	 * CCriticalSection destructor
	 */
	virtual ~CCriticalSection ()
	{
		critical_section_destroy (&ctx);
	}

	/*******************************************************
	 * CCriticalSection::acquire
	 *   Request ownership of the critical section
	 */
	void acquire ( void )
	{
		critical_section_acquire (&ctx);
	}

	/*******************************************************
	 * CCriticalSection::TryAcquire
	 *   Request ownership of the critical section
	 *   Fail if already owned
	 */
	bool TryAcquire ( void )
	{
		return critical_section_try_acquire (&ctx);
	}

	/*******************************************************
	 * CCriticalSection::release
	 *   Relinquish ownership of the critical section
	 */
	void release ( void )
	{
		critical_section_release (&ctx);
	}

private:

	critical_section_ctx_t ctx;
	enum { m_uiSpinCount = 3000ul };

} ;

/***********************************************************
 * Class template that can be used with CCriticalSection classes
 *   or other classes that contain acquire() and release() methods
 */
template<class T>
class LockRegion {

public:

	LockRegion() { m_t.acquire (); }
	~LockRegion() { m_t.release (); }

private:

	T m_t;

} ;

#endif /* CCRITICALSECTION_H_ */
