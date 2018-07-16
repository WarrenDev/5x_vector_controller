
/**
* MK_System_control.h
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief MK64FN system control block register definitions
*/

#ifndef KBL_CODE_MK_SYSTEM_CONTROL_H_
#define KBL_CODE_MK_SYSTEM_CONTROL_H_

#include <stdint.h>

/* ----------------------------------------------------------------------------
   -- SCB
   ---------------------------------------------------------------------------- */

/* SCB - Peripheral register structure */
typedef struct MK_SCB_MemMap
{
    volatile uint8_t RESERVED_0[8];
    volatile uint32_t ACTLR;                                  /* Auxiliary Control Register,, offset: 0x8 */
    volatile uint8_t RESERVED_1[3316];
    volatile uint32_t CPUID;                                  /* CPUID Base Register, offset: 0xD00 */
    volatile uint32_t ICSR;                                   /* Interrupt Control and State Register, offset: 0xD04 */
    volatile uint32_t VTOR;                                   /* Vector Table Offset Register, offset: 0xD08 */
    volatile uint32_t AIRCR;                                  /* Application Interrupt and Reset Control Register, offset: 0xD0C */
    volatile uint32_t SCR;                                    /* System Control Register, offset: 0xD10 */
    volatile uint32_t CCR;                                    /* Configuration and Control Register, offset: 0xD14 */
    volatile uint32_t SHPR1;                                  /* System Handler Priority Register 1, offset: 0xD18 */
    volatile uint32_t SHPR2;                                  /* System Handler Priority Register 2, offset: 0xD1C */
    volatile uint32_t SHPR3;                                  /* System Handler Priority Register 3, offset: 0xD20 */
    volatile uint32_t SHCSR;                                  /* System Handler Control and State Register, offset: 0xD24 */
    volatile uint32_t CFSR;                                   /* Configurable Fault Status Registers, offset: 0xD28 */
    volatile uint32_t HFSR;                                   /* HardFault Status register, offset: 0xD2C */
    volatile uint32_t DFSR;                                   /* Debug Fault Status Register, offset: 0xD30 */
    volatile uint32_t MMFAR;                                  /* MemManage Address Register, offset: 0xD34 */
    volatile uint32_t BFAR;                                   /* BusFault Address Register, offset: 0xD38 */
    volatile uint32_t AFSR;                                   /* Auxiliary Fault Status Register, offset: 0xD3C */
} volatile *MK_SCB_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- SCB - Register accessor macros
   ---------------------------------------------------------------------------- */

/* SCB - Register accessors */
#define MK_SCB_ACTLR_REG(base)                      ((base)->ACTLR)
#define MK_SCB_CPUID_REG(base)                      ((base)->CPUID)
#define MK_SCB_ICSR_REG(base)                       ((base)->ICSR)
#define MK_SCB_VTOR_REG(base)                       ((base)->VTOR)
#define MK_SCB_AIRCR_REG(base)                      ((base)->AIRCR)
#define MK_SCB_SCR_REG(base)                        ((base)->SCR)
#define MK_SCB_CCR_REG(base)                        ((base)->CCR)
#define MK_SCB_SHPR1_REG(base)                      ((base)->SHPR1)
#define MK_SCB_SHPR2_REG(base)                      ((base)->SHPR2)
#define MK_SCB_SHPR3_REG(base)                      ((base)->SHPR3)
#define MK_SCB_SHCSR_REG(base)                      ((base)->SHCSR)
#define MK_SCB_CFSR_REG(base)                       ((base)->CFSR)
#define MK_SCB_HFSR_REG(base)                       ((base)->HFSR)
#define MK_SCB_DFSR_REG(base)                       ((base)->DFSR)
#define MK_SCB_MMFAR_REG(base)                      ((base)->MMFAR)
#define MK_SCB_BFAR_REG(base)                       ((base)->BFAR)
#define MK_SCB_AFSR_REG(base)                       ((base)->AFSR)

/* ----------------------------------------------------------------------------
   -- SCB Register Masks
   ---------------------------------------------------------------------------- */

/* ACTLR Bit Fields */
#define MK_SCB_ACTLR_DISMCYCINT_MASK                0x1u
#define MK_SCB_ACTLR_DISMCYCINT_SHIFT               0
#define MK_SCB_ACTLR_DISDEFWBUF_MASK                0x2u
#define MK_SCB_ACTLR_DISDEFWBUF_SHIFT               1
#define MK_SCB_ACTLR_DISFOLD_MASK                   0x4u
#define MK_SCB_ACTLR_DISFOLD_SHIFT                  2
/* CPUID Bit Fields */
#define MK_SCB_CPUID_REVISION_MASK                  0xFu
#define MK_SCB_CPUID_REVISION_SHIFT                 0
#define MK_SCB_CPUID_REVISION(x)                    (((uint32_t)(((uint32_t)(x))<<MK_SCB_CPUID_REVISION_SHIFT))&MK_SCB_CPUID_REVISION_MASK)
#define MK_SCB_CPUID_PARTNO_MASK                    0xFFF0u
#define MK_SCB_CPUID_PARTNO_SHIFT                   4
#define MK_SCB_CPUID_PARTNO(x)                      (((uint32_t)(((fnrt_uint32)(x))<<MK_SCB_CPUID_PARTNO_SHIFT))&MK_SCB_CPUID_PARTNO_MASK)
#define MK_SCB_CPUID_VARIANT_MASK                   0xF00000u
#define MK_SCB_CPUID_VARIANT_SHIFT                  20
#define MK_SCB_CPUID_VARIANT(x)                     (((fnrt_uint32)(((uint32_t)(x))<<MK_SCB_CPUID_VARIANT_SHIFT))&MK_SCB_CPUID_VARIANT_MASK)
#define MK_SCB_CPUID_IMPLEMENTER_MASK               0xFF000000u
#define MK_SCB_CPUID_IMPLEMENTER_SHIFT              24
#define MK_SCB_CPUID_IMPLEMENTER(x)                 (((uint32_t)(((uint32_t)(x))<<MK_SCB_CPUID_IMPLEMENTER_SHIFT))&MK_SCB_CPUID_IMPLEMENTER_MASK)
/* ICSR Bit Fields */
#define MK_SCB_ICSR_VECTACTIVE_MASK                 0x1FFu
#define MK_SCB_ICSR_VECTACTIVE_SHIFT                0
#define MK_SCB_ICSR_VECTACTIVE(x)                   (((uint32_t)(((uint32_t)(x))<<MK_SCB_ICSR_VECTACTIVE_SHIFT))&MK_SCB_ICSR_VECTACTIVE_MASK)
#define MK_SCB_ICSR_RETTOBASE_MASK                  0x800u
#define MK_SCB_ICSR_RETTOBASE_SHIFT                 11
#define MK_SCB_ICSR_VECTPENDING_MASK                0x3F000u
#define MK_SCB_ICSR_VECTPENDING_SHIFT               12
#define MK_SCB_ICSR_VECTPENDING(x)                  (((uint32_t)(((uint32_t)(x))<<MK_SCB_ICSR_VECTPENDING_SHIFT))&MK_SCB_ICSR_VECTPENDING_MASK)
#define MK_SCB_ICSR_ISRPENDING_MASK                 0x400000u
#define MK_SCB_ICSR_ISRPENDING_SHIFT                22
#define MK_SCB_ICSR_ISRPREEMPT_MASK                 0x800000u
#define MK_SCB_ICSR_ISRPREEMPT_SHIFT                23
#define MK_SCB_ICSR_PENDSTCLR_MASK                  0x2000000u
#define MK_SCB_ICSR_PENDSTCLR_SHIFT                 25
#define MK_SCB_ICSR_PENDSTSET_MASK                  0x4000000u
#define MK_SCB_ICSR_PENDSTSET_SHIFT                 26
#define MK_SCB_ICSR_PENDSVCLR_MASK                  0x8000000u
#define MK_SCB_ICSR_PENDSVCLR_SHIFT                 27
#define MK_SCB_ICSR_PENDSVSET_MASK                  0x10000000u
#define MK_SCB_ICSR_PENDSVSET_SHIFT                 28
#define MK_SCB_ICSR_NMIPENDSET_MASK                 0x80000000u
#define MK_SCB_ICSR_NMIPENDSET_SHIFT                31
/* VTOR Bit Fields */
#define MK_SCB_VTOR_TBLOFF_MASK                     0xFFFFFF80u
#define MK_SCB_VTOR_TBLOFF_SHIFT                    7
#define MK_SCB_VTOR_TBLOFF(x)                       (((uint32_t)(((uint32_t)(x))<<MK_SCB_VTOR_TBLOFF_SHIFT))&MK_SCB_VTOR_TBLOFF_MASK)
/* AIRCR Bit Fields */
#define MK_SCB_AIRCR_VECTRESET_MASK                 0x1u
#define MK_SCB_AIRCR_VECTRESET_SHIFT                0
#define MK_SCB_AIRCR_VECTCLRACTIVE_MASK             0x2u
#define MK_SCB_AIRCR_VECTCLRACTIVE_SHIFT            1
#define MK_SCB_AIRCR_SYSRESETREQ_MASK               0x4u
#define MK_SCB_AIRCR_SYSRESETREQ_SHIFT              2
#define MK_SCB_AIRCR_PRIGROUP_MASK                  0x700u
#define MK_SCB_AIRCR_PRIGROUP_SHIFT                 8
#define MK_SCB_AIRCR_PRIGROUP(x)                    (((uint32_t)(((uint32_t)(x))<<MK_SCB_AIRCR_PRIGROUP_SHIFT))&MK_SCB_AIRCR_PRIGROUP_MASK)
#define MK_SCB_AIRCR_ENDIANNESS_MASK                0x8000u
#define MK_SCB_AIRCR_ENDIANNESS_SHIFT               15
#define MK_SCB_AIRCR_VECTKEY_MASK                   0xFFFF0000u
#define MK_SCB_AIRCR_VECTKEY_SHIFT                  16
#define MK_SCB_AIRCR_VECTKEY(x)                     (((uint32_t)(((uint32_t)(x))<<MK_SCB_AIRCR_VECTKEY_SHIFT))&MK_SCB_AIRCR_VECTKEY_MASK)
/* SCR Bit Fields */
#define MK_SCB_SCR_SLEEPONEXIT_MASK                 0x2u
#define MK_SCB_SCR_SLEEPONEXIT_SHIFT                1
#define MK_SCB_SCR_SLEEPDEEP_MASK                   0x4u
#define MK_SCB_SCR_SLEEPDEEP_SHIFT                  2
#define MK_SCB_SCR_SEVONPEND_MASK                   0x10u
#define MK_SCB_SCR_SEVONPEND_SHIFT                  4
/* CCR Bit Fields */
#define MK_SCB_CCR_NONBASETHRDENA_MASK              0x1u
#define MK_SCB_CCR_NONBASETHRDENA_SHIFT             0
#define MK_SCB_CCR_USERSETMPEND_MASK                0x2u
#define MK_SCB_CCR_USERSETMPEND_SHIFT               1
#define MK_SCB_CCR_UNALIGN_TRP_MASK                 0x8u
#define MK_SCB_CCR_UNALIGN_TRP_SHIFT                3
#define MK_SCB_CCR_DIV_0_TRP_MASK                   0x10u
#define MK_SCB_CCR_DIV_0_TRP_SHIFT                  4
#define MK_SCB_CCR_BFHFNMIGN_MASK                   0x100u
#define MK_SCB_CCR_BFHFNMIGN_SHIFT                  8
#define MK_SCB_CCR_STKALIGN_MASK                    0x200u
#define MK_SCB_CCR_STKALIGN_SHIFT                   9
/* SHPR1 Bit Fields */
#define MK_SCB_SHPR1_PRI_4_MASK                     0xFFu
#define MK_SCB_SHPR1_PRI_4_SHIFT                    0
#define MK_SCB_SHPR1_PRI_4(x)                       (((uint32_t)(((uint32_t)(x))<<MK_SCB_SHPR1_PRI_4_SHIFT))&MK_SCB_SHPR1_PRI_4_MASK)
#define MK_SCB_SHPR1_PRI_5_MASK                     0xFF00u
#define MK_SCB_SHPR1_PRI_5_SHIFT                    8
#define MK_SCB_SHPR1_PRI_5(x)                       (((uint32_t)(((uint32_t)(x))<<MK_SCB_SHPR1_PRI_5_SHIFT))&MK_SCB_SHPR1_PRI_5_MASK)
#define MK_SCB_SHPR1_PRI_6_MASK                     0xFF0000u
#define MK_SCB_SHPR1_PRI_6_SHIFT                    16
#define MK_SCB_SHPR1_PRI_6(x)                       (((uint32_t)(((uint32_t)(x))<<MK_SCB_SHPR1_PRI_6_SHIFT))&MK_SCB_SHPR1_PRI_6_MASK)
/* SHPR2 Bit Fields */
#define MK_SCB_SHPR2_PRI_11_MASK                    0xFF000000u
#define MK_SCB_SHPR2_PRI_11_SHIFT                   24
#define MK_SCB_SHPR2_PRI_11(x)                      (((uint32_t)(((uint32_t)(x))<<MK_SCB_SHPR2_PRI_11_SHIFT))&MK_SCB_SHPR2_PRI_11_MASK)
/* SHPR3 Bit Fields */
#define MK_SCB_SHPR3_PRI_14_MASK                    0xFF0000u
#define MK_SCB_SHPR3_PRI_14_SHIFT                   16
#define MK_SCB_SHPR3_PRI_14(x)                      (((uint32_t)(((uint32_t)(x))<<MK_SCB_SHPR3_PRI_14_SHIFT))&MK_SCB_SHPR3_PRI_14_MASK)
#define MK_SCB_SHPR3_PRI_15_MASK                    0xFF000000u
#define MK_SCB_SHPR3_PRI_15_SHIFT                   24
#define MK_SCB_SHPR3_PRI_15(x)                      (((uint32_t)(((uint32_t)(x))<<MK_SCB_SHPR3_PRI_15_SHIFT))&MK_SCB_SHPR3_PRI_15_MASK)
/* SHCSR Bit Fields */
#define MK_SCB_SHCSR_MEMFAULTACT_MASK               0x1u
#define MK_SCB_SHCSR_MEMFAULTACT_SHIFT              0
#define MK_SCB_SHCSR_BUSFAULTACT_MASK               0x2u
#define MK_SCB_SHCSR_BUSFAULTACT_SHIFT              1
#define MK_SCB_SHCSR_USGFAULTACT_MASK               0x8u
#define MK_SCB_SHCSR_USGFAULTACT_SHIFT              3
#define MK_SCB_SHCSR_SVCALLACT_MASK                 0x80u
#define MK_SCB_SHCSR_SVCALLACT_SHIFT                7
#define MK_SCB_SHCSR_MONITORACT_MASK                0x100u
#define MK_SCB_SHCSR_MONITORACT_SHIFT               8
#define MK_SCB_SHCSR_PENDSVACT_MASK                 0x400u
#define MK_SCB_SHCSR_PENDSVACT_SHIFT                10
#define MK_SCB_SHCSR_SYSTICKACT_MASK                0x800u
#define MK_SCB_SHCSR_SYSTICKACT_SHIFT               11
#define MK_SCB_SHCSR_USGFAULTPENDED_MASK            0x1000u
#define MK_SCB_SHCSR_USGFAULTPENDED_SHIFT           12
#define MK_SCB_SHCSR_MEMFAULTPENDED_MASK            0x2000u
#define MK_SCB_SHCSR_MEMFAULTPENDED_SHIFT           13
#define MK_SCB_SHCSR_BUSFAULTPENDED_MASK            0x4000u
#define MK_SCB_SHCSR_BUSFAULTPENDED_SHIFT           14
#define MK_SCB_SHCSR_SVCALLPENDED_MASK              0x8000u
#define MK_SCB_SHCSR_SVCALLPENDED_SHIFT             15
#define MK_SCB_SHCSR_MEMFAULTENA_MASK               0x10000u
#define MK_SCB_SHCSR_MEMFAULTENA_SHIFT              16
#define MK_SCB_SHCSR_BUSFAULTENA_MASK               0x20000u
#define MK_SCB_SHCSR_BUSFAULTENA_SHIFT              17
#define MK_SCB_SHCSR_USGFAULTENA_MASK               0x40000u
#define MK_SCB_SHCSR_USGFAULTENA_SHIFT              18
/* CFSR Bit Fields */
#define MK_SCB_CFSR_IACCVIOL_MASK                   0x1u
#define MK_SCB_CFSR_IACCVIOL_SHIFT                  0
#define MK_SCB_CFSR_DACCVIOL_MASK                   0x2u
#define MK_SCB_CFSR_DACCVIOL_SHIFT                  1
#define MK_SCB_CFSR_MUNSTKERR_MASK                  0x8u
#define MK_SCB_CFSR_MUNSTKERR_SHIFT                 3
#define MK_SCB_CFSR_MSTKERR_MASK                    0x10u
#define MK_SCB_CFSR_MSTKERR_SHIFT                   4
#define MK_SCB_CFSR_MLSPERR_MASK                    0x20u
#define MK_SCB_CFSR_MLSPERR_SHIFT                   5
#define MK_SCB_CFSR_MMARVALID_MASK                  0x80u
#define MK_SCB_CFSR_MMARVALID_SHIFT                 7
#define MK_SCB_CFSR_IBUSERR_MASK                    0x100u
#define MK_SCB_CFSR_IBUSERR_SHIFT                   8
#define MK_SCB_CFSR_PRECISERR_MASK                  0x200u
#define MK_SCB_CFSR_PRECISERR_SHIFT                 9
#define MK_SCB_CFSR_IMPRECISERR_MASK                0x400u
#define MK_SCB_CFSR_IMPRECISERR_SHIFT               10
#define MK_SCB_CFSR_UNSTKERR_MASK                   0x800u
#define MK_SCB_CFSR_UNSTKERR_SHIFT                  11
#define MK_SCB_CFSR_STKERR_MASK                     0x1000u
#define MK_SCB_CFSR_STKERR_SHIFT                    12
#define MK_SCB_CFSR_LSPERR_MASK                     0x2000u
#define MK_SCB_CFSR_LSPERR_SHIFT                    13
#define MK_SCB_CFSR_BFARVALID_MASK                  0x8000u
#define MK_SCB_CFSR_BFARVALID_SHIFT                 15
#define MK_SCB_CFSR_UNDEFINSTR_MASK                 0x10000u
#define MK_SCB_CFSR_UNDEFINSTR_SHIFT                16
#define MK_SCB_CFSR_INVSTATE_MASK                   0x20000u
#define MK_SCB_CFSR_INVSTATE_SHIFT                  17
#define MK_SCB_CFSR_INVPC_MASK                      0x40000u
#define MK_SCB_CFSR_INVPC_SHIFT                     18
#define MK_SCB_CFSR_NOCP_MASK                       0x80000u
#define MK_SCB_CFSR_NOCP_SHIFT                      19
#define MK_SCB_CFSR_UNALIGNED_MASK                  0x1000000u
#define MK_SCB_CFSR_UNALIGNED_SHIFT                 24
#define MK_SCB_CFSR_DIVBYZERO_MASK                  0x2000000u
#define MK_SCB_CFSR_DIVBYZERO_SHIFT                 25
/* HFSR Bit Fields */
#define MK_SCB_HFSR_VECTTBL_MASK                    0x2u
#define MK_SCB_HFSR_VECTTBL_SHIFT                   1
#define MK_SCB_HFSR_FORCED_MASK                     0x40000000u
#define MK_SCB_HFSR_FORCED_SHIFT                    30
#define MK_SCB_HFSR_DEBUGEVT_MASK                   0x80000000u
#define MK_SCB_HFSR_DEBUGEVT_SHIFT                  31
/* DFSR Bit Fields */
#define MK_SCB_DFSR_HALTED_MASK                     0x1u
#define MK_SCB_DFSR_HALTED_SHIFT                    0
#define MK_SCB_DFSR_BKPT_MASK                       0x2u
#define MK_SCB_DFSR_BKPT_SHIFT                      1
#define MK_SCB_DFSR_DWTTRAP_MASK                    0x4u
#define MK_SCB_DFSR_DWTTRAP_SHIFT                   2
#define MK_SCB_DFSR_VCATCH_MASK                     0x8u
#define MK_SCB_DFSR_VCATCH_SHIFT                    3
#define MK_SCB_DFSR_EXTERNAL_MASK                   0x10u
#define MK_SCB_DFSR_EXTERNAL_SHIFT                  4
/* MMFAR Bit Fields */
#define MK_SCB_MMFAR_ADDRESS_MASK                   0xFFFFFFFFu
#define MK_SCB_MMFAR_ADDRESS_SHIFT                  0
#define MK_SCB_MMFAR_ADDRESS(x)                     (((uint32_t)(((uint32_t)(x))<<MK_SCB_MMFAR_ADDRESS_SHIFT))&MK_SCB_MMFAR_ADDRESS_MASK)
/* BFAR Bit Fields */
#define MK_SCB_BFAR_ADDRESS_MASK                    0xFFFFFFFFu
#define MK_SCB_BFAR_ADDRESS_SHIFT                   0
#define MK_SCB_BFAR_ADDRESS(x)                      (((uint32_t)(((uint32_t)(x))<<MK_SCB_BFAR_ADDRESS_SHIFT))&MK_SCB_BFAR_ADDRESS_MASK)
/* AFSR Bit Fields */
#define MK_SCB_AFSR_AUXFAULT_MASK                   0xFFFFFFFFu
#define MK_SCB_AFSR_AUXFAULT_SHIFT                  0
#define MK_SCB_AFSR_AUXFAULT(x)                     (((uint32_t)(((uint32_t)(x))<<MK_SCB_AFSR_AUXFAULT_SHIFT))&MK_SCB_AFSR_AUXFAULT_MASK)


/* SCB - Peripheral instance base addresses */
/* Peripheral SystemControl base pointer */
#define MK_SystemControl_BASE_PTR                   ((MK_SCB_MemMapPtr)0xE000E000u)

/* ----------------------------------------------------------------------------
   -- SCB - Register accessor macros
   ---------------------------------------------------------------------------- */

/* SCB - Register instance definitions */
/* SystemControl */
#define MK_SCB_ACTLR                                MK_SCB_ACTLR_REG(MK_SystemControl_BASE_PTR)
#define MK_SCB_CPUID                                MK_SCB_CPUID_REG(MK_SystemControl_BASE_PTR)
#define MK_SCB_ICSR                                 MK_SCB_ICSR_REG(MK_SystemControl_BASE_PTR)
#define MK_SCB_AIRCR                                MK_SCB_AIRCR_REG(MK_SystemControl_BASE_PTR)
#define MK_SCB_VTOR                                 MK_SCB_VTOR_REG(MK_SystemControl_BASE_PTR)
#define MK_SCB_SCR                                  MK_SCB_SCR_REG(MK_SystemControl_BASE_PTR)
#define MK_SCB_CCR                                  MK_SCB_CCR_REG(MK_SystemControl_BASE_PTR)
#define MK_SCB_SHPR1                                MK_SCB_SHPR1_REG(MK_SystemControl_BASE_PTR)
#define MK_SCB_SHPR2                                MK_SCB_SHPR2_REG(MK_SystemControl_BASE_PTR)
#define MK_SCB_SHPR3                                MK_SCB_SHPR3_REG(MK_SystemControl_BASE_PTR)
#define MK_SCB_SHCSR                                MK_SCB_SHCSR_REG(MK_SystemControl_BASE_PTR)
#define MK_SCB_CFSR                                 MK_SCB_CFSR_REG(MK_SystemControl_BASE_PTR)
#define MK_SCB_HFSR                                 MK_SCB_HFSR_REG(MK_SystemControl_BASE_PTR)
#define MK_SCB_DFSR                                 MK_SCB_DFSR_REG(MK_SystemControl_BASE_PTR)
#define MK_SCB_MMFAR                                MK_SCB_MMFAR_REG(MK_SystemControl_BASE_PTR)
#define MK_SCB_BFAR                                 MK_SCB_BFAR_REG(MK_SystemControl_BASE_PTR)
#define MK_SCB_AFSR                                 MK_SCB_AFSR_REG(MK_SystemControl_BASE_PTR)

#endif /* KBL_CODE_MK_SYSTEM_CONTROL_H_ */
