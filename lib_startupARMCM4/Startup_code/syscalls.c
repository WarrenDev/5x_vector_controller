
/**
* syscalls.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief O/S services for libc (newlib).  Non-reentrant newlib
*        system call functions.
*/

#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>

#include "ARMCM4.h"

#undef errno
extern int errno;

int _write(int file, char *ptr, int len);

/**
 * environ
 * A poitner to a list of environment variables and their values.
 * For minimal environment, an empty list is adequate.
 */
char *__env[1] = { 0 };
char **environ = __env;

/***********************************************************
* Close a file that was opened
*
* /param	file			File pointer
*
* /return	0 if successful
* 			-1 on error
*/
int _close(int file)
{
    return -1;
}

/***********************************************************
* Transfer control to a new process.
*
* /param	name			Process neame
* /param	argv			Pointer to parameter list
* /param	env				Pointer to environment variables
*
* /return	0 if successful
* 			-1 on error
*/
int _execve(char *name, char **argv, char **env)
{
    errno = ENOMEM;
    return -1;
}

/***********************************************************
* Create a new process.
*
* /param	none
*
* /return	0 if successful
* 			-1 on error
*/
int _fork(void)
{
    errno = EAGAIN;
    return -1;
}

/***********************************************************
* Status of an open file.
*
* /param	file			File pointer
* /param	st				Pointer to the stat structure
*
* /return	0 if successful
* 			-1 on error
*/
int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

/***********************************************************
* Get the id of the current process
*
* /param	none
*
* /return	Process id
*/
int _getpid(void)
{
    return 1;
}

/***********************************************************
* Query whether output stream is a terminal.
*
* /param	file			File pointer
*
* /return	none zero if a terminal
*/
int _isatty(int file)
{
	return 1;
}

/***********************************************************
* Send a signal to a process
*
* /param	pid				Process id
* /param	sig				Signal
*
* /return	0 if successful
* 			-1 on error
*/
int _kill(int pid, int sig)
{
    errno = EINVAL;
    return (-1);
}

/***********************************************************
* Establish a new name for an existing file.
*
* /param	old				Old name
* /param	new				New name
*
* /return	0 if successful
* 			-1 on error
*/
int _link(char *old, char *new)
{
    errno = EMLINK;
    return -1;
}

/***********************************************************
* Set position in a file.
*
* /param	file			File pointer
* /param	ptr				Position
* /param	dir				Direction to seek
*
* /return	0 if successful
* 			-1 on error
*/
int _lseek(int file, int ptr, int dir)
{
    return 0;
}

/***********************************************************
* Increase program data space. Used by malloc and related routines.
*
* /param	incr			Value to increase by
*
* /return	Pointer to heap end if successful
* 			-1 on error
*/
caddr_t _sbrk(int incr)
{
	// Heap block pointers defined by linker script
    extern unsigned long __bss_end__; 	// heap begin
    extern unsigned long __heap_limit;	// heap end
    static caddr_t heap_end;
    caddr_t prev_heap_end;

    if (heap_end == 0) {
        heap_end = (char*)&__bss_end__;
    }
    prev_heap_end = heap_end;

    caddr_t stack = (caddr_t)&__heap_limit;
    if (heap_end + incr > stack)
    {
    	_write (1, "Heap and stack collision\n", 25);
    	errno = ENOMEM;
    	return  (caddr_t) -1;
    	//abort ();
    }

    heap_end += incr;
    return prev_heap_end;
}

/***********************************************************
* Read characters from a file.  Blocks until the number of
* characters has been read.
*
* /param	file			File pointer
* /param	ptr				Pointer to buffer to receive characters
* /param	len				Number of characters to read
*
* /return	0 if successful
* 			-1 on error
*/
int _read(int file, char *ptr, int len)
{
    return 0;
}

/***********************************************************
* Status of a file by name
*
* /param	filepath		File name
* /param	st				Pointer to the stat structure
*
* /return	0if successful
* 			-1 on error
*/
int _stat(const char *filepath, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

/***********************************************************
* Timing information for the current process.
*
* /param	buf				Buffer pointer
*
* /return	0 on success
* 			-1 on error
*/
int _times(struct tms *buf)
{
    return -1;
}

/***********************************************************
* Remove the directory entry for a file.
*
* /param	name			File name
*
* /return	0 if successful
* 			-1 on error
*/
int _unlink(char *name)
{
    errno = ENOENT;
    return -1;
}

/***********************************************************
* Wait for a child process
*
* /param	status			Pointer to status buffer
*
* /return	0 if successful
* 			-1 on error
*/
int _wait(int *status)
{
    errno = ECHILD;
    return -1;
}

/***********************************************************
* Write characters to a file.
*
* /param	file			File pointer
* /param	ptr				Pointer to buffer to write
* /param	len				Number of bytes to write
*
* /return	Number of bytes sent if successful
* 			-1 on error
*/
int _write(int file, char *ptr, int len)
{
    return 0;
}

/***********************************************************
* Exit the system
*
* /param	status			Exit code
*
* /return	None
*/
extern void _exit(int status)
{
	_write (1, "Exiting the system\n", 19);

	for (;;);
}
