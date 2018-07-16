
/**
* mini_cpp.cpp
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Implementation of the new and delete operators that
*        don't throw exceptions.
*/

#include "../../lib_bspMK64FN/include/bsp.h"

/**
* Operator new
*
* Implementation of the C++ new operator that uses the O/S's
* runtime heap and that do not throw exceptions
*/
void *operator new			(U32 size) throw() { return malloc(size); }
void *operator new[]		(U32 size) throw() { return malloc(size); }

/*
* Operator delete
*
* Implementation of the C++ delete operator that uses the O/S's
* runtime heap and that do not throw exceptions
*/
void operator delete		(void* __p) throw() { free(__p); }
void operator delete[]		(void* __p) throw() { free(__p); }

/**
* Default placement versions of operator new
*/
void* operator new			(U32 size, void* __p) throw() { return __p; }
void* operator new[]		(U32 size, void* __p) throw() { return __p; }

/**
* Default placement versions of operator delete
*/
void operator delete  		(void*, void*) throw() { }
void operator delete[]		(void*, void*) throw() { }

/**
* __aeabi_atexit
*
* Implementation of an empty __aeabi_atexit routine.
* The embedded application never actually returns, so static
* destructors are never called.
*/
extern "C"
int __aeabi_atexit (void *object, void (*destructor)(void *), void *dso_handle)
{
	return 0;
}

/**
* __cxa_pure_virtual
*
* Pure virtual function error handler.  This is a required routine.
* This routine is usually in libstdc++.a.  We do not want to pull in this
* library and its support for exception handling, so we so we implement
* this routine here.  This routine is called when an object is created with
* a pure virtual function pointer that is not filled in, and the
* corresponding function is called.
*/
extern "C"
void __cxa_pure_virtual ()
{
	while (1);
}

