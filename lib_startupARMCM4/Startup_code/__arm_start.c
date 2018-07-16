
/**
* __arm_start.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Entry point for ARM programs.
*/

#include <string.h>
#include <stdlib.h>
#include <runtime_configuration.h>

#if SUPPORT_SEMIHOST_ARGC_ARGV
#define __MAX_CMDLINE_ARGS 10
static char *argv[__MAX_CMDLINE_ARGS] = { 0 };
//#else
//static char *argv[] = { 0 };
#endif

#if __GNUC__
#define __call_static_initializers __init_cpp
#endif

#if SUPPORT_SEMIHOST_ARGC_ARGV
extern int __argc_argv(int, char **);
#endif /* SUPPORT_SEMIHOST_ARGC_ARGV */

extern void __call_static_initializers(void);
extern int main(void);

#ifdef __VFPV4__
extern void __fp_init(void);
#endif /* __VFPV4__ */

extern void __init_registers();
extern void __init_hardware();
extern void __init_user();

#if defined(__APCS_ROPI)
extern void __init_pic();
#endif

#if defined(__APCS_RWPI)
extern void __init_pid();
#endif

#if defined(__APCS_ROPI) || defined(__APCS_RWPI)
extern void __load_static_base();
#endif

#if defined(__SEMIHOSTING)
extern void __init_semihost(void) __attribute__((weak));
#endif

#if SUPPORT_ROM_TO_RAM
extern void __copy_rom_sections_to_ram(void);
extern char __S_romp[];
#endif

static void zero_fill_bss(void)
{

	extern char __START_BSS[];
	extern char __END_BSS[];

	memset(__START_BSS, 0, (__END_BSS - __START_BSS));

}

void toolchain_startup(void) __attribute__((naked));
void toolchain_startup(void)
{

		//  Setup registers
		__init_registers();

		//  Setup hardware
		__init_hardware();

#if defined(__APCS_ROPI) || defined(__APCS_RWPI)
		//	static base register initialization
		__load_static_base();
#endif

#if defined(__APCS_RWPI)
		//	-pid
		//	setup static base for SB relative position independent data
		//	perform runtime relocation
		__init_pid();
#endif

#if defined(__APCS_ROPI)
		//	-pic
		//	perform runtime relocation for position independent code
		__init_pic();
#endif

		//	Zero-fill the .bss section
		zero_fill_bss();

#if SUPPORT_ROM_TO_RAM
		if (__S_romp != 0L)
			__copy_rom_sections_to_ram();
#endif

		//  Initialize the floating-point library
#ifdef __VFPV4__
		__fp_init();
#endif

		//	Call C++ static initializers
		__call_static_initializers();

		//  Initializations before main, user specific
		__init_user();

#if defined(__SEMIHOSTING)
		//  semihost initializations
		__init_semihost();
#endif

		//	Call main()
#if SUPPORT_SEMIHOST_ARGC_ARGV
		exit(main(__argc_argv(__MAX_CMDLINE_ARGS, argv), argv));
#else
		main();
#endif

		//	Should never get here
		while (1);

} // toolchain_startup

