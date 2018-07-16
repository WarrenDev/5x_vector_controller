
/**
* __arm_eabi_init.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Board-level initialization.
*	If hardware initialization and pre-main user
*   initialization are required, customize this file
*   instead of customizing __start.c.
*/

extern char __SP_INIT[];

// __init_registers, __init_hardware, __init_user suggested by Kobler

void __init_registers(void) __attribute__((weak));
void __init_registers(void)
{
	int addr = (int)__SP_INIT;
		// setup the stack before we attempt anything else
		// skip stack setup if __SP_INIT is 0
		// assume sp is already setup.
	__asm (
	"mov	r0,%0\n\t"
	"cmp	r0,#0\n\t"
	"beq	skip_sp\n\t"
	"mov	sp,r0\n\t"
	"sub	sp,#4\n\t"
	"mov	r0,#0\n\t"
	"mvn	r0,r0\n\t"
	"str	r0,[sp,#0]\n\t"
	"add	sp,#4\n\t"
	"skip_sp:\n\t"
	::"r"(addr));

}

#ifdef __VFPV4__
void __fp_init(void) __attribute__((weak));
void __fp_init(void)
{
	int addr = (int)0xE000ED88;
	/* CPACR is located at address 0xE000ED88 */
	/* Read CPACR */
	/* Set bits 20-23 to enable CP10 and CP11 coprocessors */
	/* Write back the modified value to the CPACR */
	/* Reset pipeline */
	__asm (
	"mov	r0,%0\n\t"
	"LDR R1, [R0]\n\t"
	"ORR R1, R1, #(0xF << 20)\n\t"
	"STR R1, [R0]\n\t"
#ifndef __CWCC__
	"DSB\n\t"
	"ISB\n\t"
#else
	"nop\n\t"
	"nop\n\t"
#endif
	::"r"(addr));

}
#endif /* __VFPV4__ */

#ifdef EWL_VFP11
__declspec(weak) asm void __init_vfp11(void)
{
	
#define	VFPEnable 0x40000000
#define RF_Enable 0x03c00000

	MRC p15, 0, r1, c1, c0, 2 //  r1 = Access Control Register
	ORR r1, r1, #(0xf << 20)  //  enable full access for p10,11
	MCR p15, 0, r1, c1, c0, 2 // Access Control Register = r1
	MOV r1, #0
	MCR p15, 0, r1, c7, c5, 4 // flush prefetch buffer because of FMXR below
	// and CP 10 & 11 were only just enabled

	// Enable VFP itself
	MOV r0,#VFPEnable
	FMXR FPEXC, r0 // FPEXC = r0

	// Enable "Run Fast" Mode and round toward zero
	MOV r0,#RF_Enable
	FMXR FPSCR, r0
	BX LR
}
#endif

void __init_hardware(void) __attribute__((weak));
void __init_hardware(void)
{
#ifdef EWL_VFP11
	__asm volatile ("b	__init_vfp11");
#endif
	
}

void __init_user(void) __attribute__((weak));
void __init_user(void)
{

}

