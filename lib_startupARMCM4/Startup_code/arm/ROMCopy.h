
/**
* ROMCopy.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief Routine to copy form ROM to RAM.
*/

#ifndef __ROMCOPY_H__
#define __ROMCOPY_H__

/* format of the ROM table info entry ... */
typedef struct RomInfo {
	unsigned long	Source;
	unsigned long	Target;
	unsigned long 	Size;
} RomInfo;


/* exported routines */

extern void __copy_rom_section(unsigned long dst, unsigned long src, unsigned long size);
extern void __copy_rom_sections_to_ram(void);
extern void __flush_cache(unsigned long dst, unsigned long size);

#endif
