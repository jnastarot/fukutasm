#pragma once

enum fuku_condition {
    FUKU_NO_CONDITION = -1,                jmp = -1,

    FUKU_CONDITION_OVERFLOW        = 0,    jo   = 0,             //(OF == 1)
    FUKU_CONDITION_NO_OVERFLOW     = 1,    jno  = 1,             //(OF != 1)
    FUKU_CONDITION_BELOW           = 2,    jb   = 2,             //(CF == 1)
    FUKU_CONDITION_ABOVE_EQUAL     = 3,    jae  = 3,    jnc = 3, //(CF != 1)
    FUKU_CONDITION_EQUAL           = 4,    je   = 4,    jz  = 4, //(ZF == 1)
    FUKU_CONDITION_NOT_EQUAL       = 5,    jne  = 5,    jnz = 5, //(ZF != 1)
    FUKU_CONDITION_BELOW_EQUAL     = 6,    jbe  = 6,    jna = 6, //(CF == 1 || ZF == 1)
    FUKU_CONDITION_ABOVE           = 7,    jnbe = 7,    ja  = 7, //(CF != 1 && ZF != 1)
    FUKU_CONDITION_NEGATIVE        = 8,    js   = 8,             //(SF == 1)
    FUKU_CONDITION_POSITIVE        = 9,    jns  = 9,             //(SF != 1)
    FUKU_CONDITION_PARITY_EVEN     = 10,   jp   = 10,            //(PF == 1)
    FUKU_CONDITION_PARITY_ODD      = 11,   jnp  = 11,   jpo = 11,//(PF != 1)
    FUKU_CONDITION_LESS            = 12,   jnge = 12,   jl  = 12,//(SF != OF)
    FUKU_CONDITION_GREATER_EQUAL   = 13,   jge  = 13,   jnl = 13,//(SF == OF)
    FUKU_CONDITION_LESS_EQUAL      = 14,   jng  = 14,   jle = 14,//(ZF == 1 || (SF != OF) )
    FUKU_CONDITION_GREATER         = 15,   jnle = 15,   jg  = 15,//(ZF != 1 && (SF == OF) )

    FUKU_CONDITION_MAX
};

enum fuku_prefix {
    FUKU_PREFIX_NONE             = 0,
    FUKU_PREFIX_LOCK             = 0xF0,
    FUKU_PREFIX_REPE             = 0xF3,
    FUKU_PREFIX_REPNE            = 0xF2,
    FUKU_PREFIX_CS               = 0x2E,
    FUKU_PREFIX_SS               = 0x36,
    FUKU_PREFIX_DS               = 0x3E,
    FUKU_PREFIX_ES               = 0x26,
    FUKU_PREFIX_FS               = 0x64,
    FUKU_PREFIX_GS               = 0x65,
    FUKU_PREFIX_OVERRIDE_DATA    = 0x66,
    FUKU_PREFIX_OVERRIDE_ADDRESS = 0x67,
};

enum fuku_operand_size {
    FUKU_OPERAND_SIZE_0  = 0, //none
    FUKU_OPERAND_SIZE_8  = 1,
    FUKU_OPERAND_SIZE_16 = 2,
    FUKU_OPERAND_SIZE_32 = 4,
    FUKU_OPERAND_SIZE_64 = 8,
};

enum fuku_assambler_arch {
    FUKU_ASSAMBLER_ARCH_X86,
    FUKU_ASSAMBLER_ARCH_X64,
};

enum fuku_asm_short_cfg {
    FUKU_ASM_SHORT_CFG_USE_EAX_SHORT = 1,
    FUKU_ASM_SHORT_CFG_USE_DISP_SHORT = 2,
    FUKU_ASM_SHORT_CFG_USE_IMM_SHORT = 4,
};


#include "fuku_type.h"
#include "fuku_inst.h"

#include "fuku_register_math.h"
#include "fuku_code_holder.h"
#include "fuku_code_analyzer.h"
#include "fuku_code_profiler.h"

#include "fuku_assambler.h"
