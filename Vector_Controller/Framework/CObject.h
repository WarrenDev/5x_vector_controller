
/**
* CObject.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definition of the CObject class
*/

#ifndef COBJECT_H_
#define COBJECT_H_

#define UNREFERENCED_PARAMETER(p) (p)=(p)

/***********************************************************
 * CObject class
 */
class CObject 
{
	
public:
	CObject() 			{ }
	virtual ~CObject()	{ }
	
	/*******************************************************
	 * Virtual methods - you may override
	 *   If overridden in a derived class, call the base class
	 *   method first
	 */
	virtual bool AssertValid 	( void ) const	{ return false;	}
	virtual bool IsSerializable ( void ) const	{ return false;	}
	virtual bool Serialize 		( void ) const	{ return false;	}
	virtual void Dump 			( void ) const	{ return;		}
	
};

#endif /* COBJECT_H_ */
