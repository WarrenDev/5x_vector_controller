
/**
* __arm_end.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Interface for board-level termination.
*/


#ifdef __cplusplus
extern "C"
{
#endif

#if defined(__SEMIHOSTING)
extern void sys_exit(int status);
#endif
/*
 *	_ExitProcess
 *
 */
extern void _ExitProcess(int status);
extern void _ExitProcess(int status)
{
#if defined(__SEMIHOSTING)
	sys_exit(status);
#endif
	while (1);
}
#ifdef __cplusplus
}
#endif
