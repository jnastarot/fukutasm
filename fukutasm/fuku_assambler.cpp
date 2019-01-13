#include "stdafx.h"
#include "fuku_assambler.h"
#include "fuku_assambler_macro.h"

#define set_modrm(mod, reg_idx) \
    FUKU_ASSERT_EQ(mod & -4, 0)\
    raw_operand[0] = (uint8_t(mod) << 6) | reg_idx;\
    operand_size = 1;


#define set_sib(scale,is_index_ext,reg_idx_index,reg_idx_base)\
    FUKU_ASSERT_EQ(operand_size, 1);\
    FUKU_ASSERT_EQ(scale & -4, 0);\
    FUKU_ASSERT( (!is_index_ext && reg_idx_index == FUKU_REG_INDEX_SP) || reg_idx_base == FUKU_REG_INDEX_SP);\
    raw_operand[1] = (scale << 6) | (reg_idx_index << 3) | reg_idx_base;\
    operand_size = 2;

#define set_disp8(disp)\
    raw_operand[operand_size] = (uint8_t)disp;\
    displacment_offset = length + operand_size;\
    operand_size += sizeof(int8_t);

#define set_dispr(disp)\
    *(uint32_t*)&raw_operand[operand_size] = disp;\
    displacment_offset = length + operand_size;\
    operand_size += sizeof(uint32_t);


fuku_assambler::fuku_assambler()
    : short_cfg(FUKU_ASM_SHORT_CFG_USE_EAX_SHORT | FUKU_ASM_SHORT_CFG_USE_DISP_SHORT | FUKU_ASM_SHORT_CFG_USE_IMM_SHORT) {}

fuku_assambler::~fuku_assambler() {}

uint8_t fuku_assambler::get_displacment_offset() {
    return this->displacment_offset;
}

uint8_t fuku_assambler::get_immediate_offset() {
    return this->immediate_offset;
}

bool fuku_assambler::is_used_short_eax() {
    return this->short_cfg & FUKU_ASM_SHORT_CFG_USE_EAX_SHORT;
}

bool fuku_assambler::is_used_short_disp() {
    return this->short_cfg & FUKU_ASM_SHORT_CFG_USE_DISP_SHORT;
}

bool fuku_assambler::is_used_short_imm() {
    return this->short_cfg & FUKU_ASM_SHORT_CFG_USE_IMM_SHORT;
}


void fuku_assambler::clear_space() {
    memset(bytecode, 0, sizeof(bytecode));
    this->length = 0;
    this->displacment_offset = 0;
    this->immediate_offset = 0;
}

void fuku_assambler::emit_b(uint8_t x) {
    bytecode[length] = x;
    length++;
}

void fuku_assambler::emit_w(uint16_t x) {
    *(uint16_t*)&bytecode[length] = x;
    length += sizeof(uint16_t);
}

void fuku_assambler::emit_dw(uint32_t x) {
    *(uint32_t*)&bytecode[length] = x;
    length += sizeof(uint32_t);
}

void fuku_assambler::emit_qw(uint64_t x) {
    *(uint64_t*)&bytecode[length] = x;
    length += sizeof(uint64_t);
}

void fuku_assambler::emit_immediate_b(const fuku_immediate& src) {
    bytecode[length] = src.get_immediate8();
    length++;
}

void fuku_assambler::emit_immediate_w(const fuku_immediate& src) {
    *(uint16_t*)&bytecode[length] = src.get_immediate16();
    length += sizeof(uint16_t);
}

void fuku_assambler::emit_immediate_dw(const fuku_immediate& src) {
    *(uint32_t*)&bytecode[length] = src.get_immediate32();
    length += sizeof(uint32_t);
}

void fuku_assambler::emit_immediate_qw(const fuku_immediate& src) {
    *(uint64_t*)&bytecode[length] = src.get_immediate64();
    length += sizeof(uint64_t);
}


/*
                          is base 64 ext(B)
         is reg 64 ext(R)  /
                 |        /
    REX 0100 0   0   0   0
             |        \
      is 64bit size(W) \
                      is index 64 ext(X)
*/
void fuku_assambler::emit_rex_64() {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    emit_b(0x48);
}

void fuku_assambler::emit_rex_64(fuku_register rm_reg, fuku_register reg) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    emit_b(0x48 | fuku_is_x64arch_ext_reg(reg) << 2 | fuku_is_x64arch_ext_reg(rm_reg));
}

void fuku_assambler::emit_rex_64(const fuku_operand& rm_reg, fuku_register reg) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    emit_b(0x48 | fuku_is_x64arch_ext_reg(reg) << 2 | rm_reg.get_low_rex());
}

void fuku_assambler::emit_rex_64(fuku_register rm_reg) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    emit_b(0x48 | fuku_is_x64arch_ext_reg(rm_reg));
}

void fuku_assambler::emit_rex_64(const fuku_operand& op) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    emit_b(0x48 | op.get_low_rex());
}

void fuku_assambler::emit_rex_32(fuku_register rm_reg, fuku_register reg) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    emit_b(0x40 | fuku_is_x64arch_ext_reg(reg) << 2 | fuku_is_x64arch_ext_reg(rm_reg));
}

void fuku_assambler::emit_rex_32(const fuku_operand& rm_reg, fuku_register reg) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    emit_b(0x40 | fuku_is_x64arch_ext_reg(reg) << 2 | rm_reg.get_low_rex());
}


void fuku_assambler::emit_rex_32(fuku_register rm_reg) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    emit_b(0x40 | fuku_is_x64arch_ext_reg(rm_reg));
}

void fuku_assambler::emit_rex_32(const fuku_operand& rm_reg) { 
    if (arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    emit_b(0x40 | rm_reg.get_low_rex()); 
}

void fuku_assambler::emit_optional_rex_32(fuku_register rm_reg, fuku_register reg) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    uint8_t rex_bits = fuku_is_x64arch_ext_reg(reg) << 2 | fuku_is_x64arch_ext_reg(rm_reg);
    if (rex_bits != 0) { emit_b(0x40 | rex_bits); }
}

void fuku_assambler::emit_optional_rex_32(const fuku_operand& rm_reg, fuku_register reg) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    uint8_t rex_bits = fuku_is_x64arch_ext_reg(reg) << 2 | rm_reg.get_low_rex();
    if (rex_bits != 0) { emit_b(0x40 | rex_bits); }
}


void fuku_assambler::emit_optional_rex_32(fuku_register rm_reg) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    if (fuku_is_x64arch_ext_reg(rm_reg)) { emit_b(0x41); }
}

void fuku_assambler::emit_optional_rex_32(const fuku_operand& rm_reg) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    if (rm_reg.get_low_rex() != 0) { emit_b(0x40 | rm_reg.get_low_rex()); }
}

void fuku_assambler::emit_rex(fuku_operand_size size) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    if (size == fuku_operand_size::FUKU_OPERAND_SIZE_64) {
        emit_rex_64();
    }
}

void fuku_assambler::emit_rex(const fuku_operand& rm_reg, fuku_operand_size size) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    if (size == fuku_operand_size::FUKU_OPERAND_SIZE_64) {
        emit_rex_64(rm_reg);
    }
    else {
        emit_optional_rex_32(rm_reg);
    }
}
void fuku_assambler::emit_rex(fuku_register rm_reg, fuku_operand_size size) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    if (size == fuku_operand_size::FUKU_OPERAND_SIZE_64) {
        emit_rex_64(rm_reg);
    }
    else {
        emit_optional_rex_32(rm_reg);
    }
}

void fuku_assambler::emit_rex(fuku_register rm_reg, fuku_register reg, fuku_operand_size size) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    if (size == fuku_operand_size::FUKU_OPERAND_SIZE_64) {
        emit_rex_64(reg, rm_reg);
    }
    else {
        emit_optional_rex_32(reg, rm_reg);
    }
}

void fuku_assambler::emit_rex(const fuku_operand& rm_reg, fuku_register reg, fuku_operand_size size) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    if (size == fuku_operand_size::FUKU_OPERAND_SIZE_64) {
        emit_rex_64(rm_reg, reg);
    }
    else {
        emit_optional_rex_32(rm_reg, reg);
    }
}

void fuku_assambler::emit_modrm(fuku_register rm_reg, fuku_register reg) {
    emit_b(0xC0 | fuku_get_index_by_register(reg) << 3 | fuku_get_index_by_register(rm_reg));
}

void fuku_assambler::emit_modrm(int code, fuku_register rm_reg) {
    emit_b(0xC0 | code << 3 | fuku_get_index_by_register(rm_reg));
}


void fuku_assambler::emit_operand_x64(fuku_register_index reg, const fuku_operand& rm_reg) {

    uint8_t raw_operand[6] = { 0 };
    uint8_t operand_size = 0;

    uint8_t base_idx = fuku_get_index_by_register(rm_reg.get_base());
    uint8_t index_idx = fuku_get_index_by_register(rm_reg.get_index());

    switch (rm_reg.get_type()) {

    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_DISP_ONLY: {
        set_modrm(0, FUKU_REG_INDEX_BP);
        set_dispr(rm_reg.get_disp().get_immediate32());
        break;
    }

    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_ONLY:
    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_DISP: {

        // [base + disp/r]
        if (rm_reg.get_disp().get_immediate32() == 0 && base_idx != FUKU_REG_INDEX_BP) {

            // [base]
            set_modrm(0, base_idx);
        }
        else if (is_used_short_disp() && rm_reg.get_disp().is_8()) {

            // [base + disp8]
            set_modrm(1, base_idx);
            set_disp8(rm_reg.get_disp().get_immediate8());
        }
        else {

            // [base + disp/r]
            set_modrm(2, base_idx);
            set_dispr(rm_reg.get_disp().get_immediate32());
        }

        break;
    }

    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_INDEX:
    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_INDEX_DISP: {

        FUKU_ASSERT(index_idx != FUKU_REG_INDEX_SP);

        set_sib(rm_reg.get_scale(), rm_reg.get_index(), index_idx, base_idx);

        // [base + index*scale + disp/r]
        if (rm_reg.get_disp().get_immediate32() == 0 && base_idx != FUKU_REG_INDEX_BP) {
            // [base + index*scale]
            set_modrm(0, FUKU_REG_INDEX_SP);
        }
        else if (is_used_short_disp() && rm_reg.get_disp().is_8()) {
            // [base + index*scale + disp8]
            set_modrm(1, FUKU_REG_INDEX_SP);
            set_disp8(rm_reg.get_disp().get_immediate8());
        }
        else {
            // [base + index*scale + disp/r]
            set_modrm(2, FUKU_REG_INDEX_SP);
            set_dispr(rm_reg.get_disp().get_immediate32());
        }

        break;
    }
    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_INDEX_DISP: {
        FUKU_ASSERT(index_idx != FUKU_REG_INDEX_SP);

        // [index*scale + disp/r]
        set_modrm(0, FUKU_REG_INDEX_SP);
        set_sib(rm_reg.get_scale(), rm_reg.get_index(), index_idx, FUKU_REG_INDEX_BP);
        set_dispr(rm_reg.get_disp().get_immediate32());
        break;
    }


    default:FUKU_DEBUG;
    }

    bytecode[length] = raw_operand[0] | reg << 3;

    for (unsigned i = 1; i < operand_size; i++) { bytecode[length + i] = raw_operand[i]; }
    length += operand_size;
}

void fuku_assambler::emit_operand_x86(fuku_register_index reg, const fuku_operand& adr) {
    FUKU_ASSERT_GT(length, 0);

    uint8_t raw_operand[6] = { 0 };
    uint8_t operand_size = 0;

    uint8_t base_idx = fuku_get_index_by_register(adr.get_base());
    uint8_t index_idx = fuku_get_index_by_register(adr.get_index());

    switch (adr.get_type()) {

    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_DISP_ONLY: {
        set_modrm(0, FUKU_REG_INDEX_BP);
        set_dispr(adr.get_disp().get_immediate32());
        break;
    }

    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_ONLY:
    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_DISP: {

        // [base + disp/r]
        if (adr.get_disp().get_immediate32() == 0 && base_idx != FUKU_REG_INDEX_BP) {

            // [base]
            set_modrm(0, base_idx);
            if (base_idx == FUKU_REG_INDEX_SP) {
                set_sib(FUKU_OPERAND_SCALE_1,0, FUKU_REG_INDEX_SP, base_idx);
            }
        }
        else if (is_used_short_disp() && adr.get_disp().is_8()) {

            // [base + disp8]
            set_modrm(1, base_idx);
            if (fuku_get_index_by_register(adr.get_base()) == FUKU_REG_INDEX_SP) {
                set_sib(FUKU_OPERAND_SCALE_1,0, FUKU_REG_INDEX_SP, base_idx);
            }
            set_disp8(adr.get_disp().get_immediate8());
        }
        else {

            // [base + disp/r]
            set_modrm(2, base_idx);
            if (base_idx == FUKU_REG_INDEX_SP) {
                set_sib(FUKU_OPERAND_SCALE_1,0, FUKU_REG_INDEX_SP, base_idx);
            }
            set_dispr(adr.get_disp().get_immediate32());
        }

        break;
    }

    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_INDEX:
    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_INDEX_DISP: {

        FUKU_ASSERT(index_idx != FUKU_REG_INDEX_SP);

        // [base + index*scale + disp/r]
        if (adr.get_disp().get_immediate32() == 0 && base_idx != FUKU_REG_INDEX_BP) {
            // [base + index*scale]
            set_modrm(0, FUKU_REG_INDEX_SP);
            set_sib(adr.get_scale(),0, index_idx, base_idx);
        }
        else if (is_used_short_disp() && adr.get_disp().is_8()) {
            // [base + index*scale + disp8]
            set_modrm(1, FUKU_REG_INDEX_SP);
            set_sib(adr.get_scale(),0, index_idx, base_idx);
            set_disp8(adr.get_disp().get_immediate8());
        }
        else {
            // [base + index*scale + disp/r]
            set_modrm(2, FUKU_REG_INDEX_SP);
            set_sib(adr.get_scale(),0, index_idx, base_idx);
            set_dispr(adr.get_disp().get_immediate32());
        }

        break;
    }
    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_INDEX_DISP: {
        FUKU_ASSERT(index_idx != FUKU_REG_INDEX_SP);

        // [index*scale + disp/r]
        set_modrm(0, FUKU_REG_INDEX_SP);
        set_sib(adr.get_scale(),0, index_idx, FUKU_REG_INDEX_BP);
        set_dispr(adr.get_disp().get_immediate32());
        break;
    }


    default:FUKU_DEBUG;
    }


    bytecode[length] = (raw_operand[0] & ~0x38) | (reg << 3);

    for (unsigned i = 1; i < operand_size; i++) {
        bytecode[length + i] = raw_operand[i];
    }

    length += operand_size;
}

void fuku_assambler::emit_operand(fuku_register_index reg, const fuku_operand& rm_reg) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) {
        emit_operand_x86(reg, rm_reg);
    }
    else {
        emit_operand_x64(reg, rm_reg);
    }
}

//Data Transfer Instructions
//MOV
//MOVcc
//XCHG
//BSWAP
//XADD
//CMPXCHG
//CMPXCHG8B
//PUSH
//POP
gen_func_body_onebyte_no_arg(cwd,  FUKU_PREFIX_OVERRIDE_DATA,0x99, X86_INS_CDQ, 0)
gen_func_body_onebyte_no_arg(cdq,  0x99, X86_INS_CDQ, 0)
gen_func_body_twobyte_no_arg(cqo,  0x48, 0x99, X86_INS_CDQ, 0)
//CBW/CWDE
//MOVSX
//MOVZX
//Binary Arithmetic Instructions
//ADCX
//ADOX
gen_func_body_arith(add, fuku_assambler_arith_add, X86_INS_ADD, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF    | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_arith(adc, fuku_assambler_arith_adc, X86_INS_ADC, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF    | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_arith(sub, fuku_assambler_arith_sub, X86_INS_SUB, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF    | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_arith(sbb, fuku_assambler_arith_sbb, X86_INS_SBB, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF    | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_arith_ex_one_op(imul, fuku_assambler_arith_ex_imul, X86_INS_IMUL, X86_EFLAGS_MODIFY_SF    | X86_EFLAGS_MODIFY_CF    | X86_EFLAGS_MODIFY_OF    | X86_EFLAGS_UNDEFINED_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF)
gen_func_body_arith_ex_one_op(mul,  fuku_assambler_arith_ex_mul,  X86_INS_MUL,  X86_EFLAGS_MODIFY_SF    | X86_EFLAGS_MODIFY_CF    | X86_EFLAGS_MODIFY_OF    | X86_EFLAGS_UNDEFINED_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF)
gen_func_body_arith_ex_one_op(idiv, fuku_assambler_arith_ex_idiv, X86_INS_IDIV, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_UNDEFINED_CF)
gen_func_body_arith_ex_one_op(div,  fuku_assambler_arith_ex_div,  X86_INS_DIV,  X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_UNDEFINED_CF)
gen_func_body_arith_incdec(inc, fuku_assambler_arith_inc, X86_INS_INC, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF)
gen_func_body_arith_incdec(dec, fuku_assambler_arith_dec, X86_INS_DEC, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF)
gen_func_body_arith_ex_one_op(neg,  fuku_assambler_arith_ex_neg,  X86_INS_NEG,  X86_EFLAGS_MODIFY_OF    | X86_EFLAGS_MODIFY_SF    | X86_EFLAGS_MODIFY_ZF    | X86_EFLAGS_MODIFY_AF    | X86_EFLAGS_MODIFY_PF    | X86_EFLAGS_MODIFY_CF)
gen_func_body_arith(cmp, fuku_assambler_arith_cmp, X86_INS_CMP, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF    | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
//Decimal Arithmetic Instructions
//asm_def_noop(daa,)
//asm_def_noop(das,)
//asm_def_noop(aaa,)
//asm_def_noop(aas,)
//asm_def_noop_imm(aam,)
//asm_def_noop_imm(aad,)
//Logical Instructions Instructions
gen_func_body_arith(and, fuku_assambler_arith_and, X86_INS_AND, X86_EFLAGS_RESET_OF  | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
gen_func_body_arith(or , fuku_assambler_arith_or,  X86_INS_OR,  X86_EFLAGS_RESET_OF  | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
gen_func_body_arith(xor, fuku_assambler_arith_xor, X86_INS_XOR, X86_EFLAGS_RESET_OF  | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
gen_func_body_arith_ex_one_op(not,  fuku_assambler_arith_ex_not,  X86_INS_NOT,  0)
//Shift and Rotate Instructions
gen_func_body_shift(sar, fuku_assambler_shift_sar, X86_INS_SAR, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_shift(shr, fuku_assambler_shift_shr, X86_INS_SHR, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_shift(shl, fuku_assambler_shift_shl, X86_INS_SHL, X86_EFLAGS_MODIFY_OF    | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
//asm_def_shXd_inst(shrd)
//asm_def_shXd_inst(shld)
gen_func_body_shift(ror, fuku_assambler_shift_ror, X86_INS_ROR, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_MODIFY_CF)
gen_func_body_shift(rol, fuku_assambler_shift_rol, X86_INS_ROL, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_MODIFY_CF)
gen_func_body_shift(rcr, fuku_assambler_shift_rcr, X86_INS_RCR, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_MODIFY_CF)
gen_func_body_shift(rcl, fuku_assambler_shift_rcl, X86_INS_RCL, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_MODIFY_CF)
//Bit and Byte Instructions
gen_func_body_bit(bt , fuku_assambler_bittest_bt,  X86_INS_BT,  X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_bit(bts, fuku_assambler_bittest_bts, X86_INS_BTS, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_bit(btr, fuku_assambler_bittest_btr, X86_INS_BTR, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_bit(btc, fuku_assambler_bittest_btc, X86_INS_BTC, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_MODIFY_CF)
//asm_def_cond_1op(_,setcc, fuasm_reg)
//asm_def_cond_1op(_,setcc, fuasm_op)
//TEST
//CRC32
//POPCNT
//Control Transfer Instructions
//asm_def_eip_one_op(_jmp)   
//asm_def_cond_1op(_,jcc, fuasm_imm)
//asm_def_eip_one_op(_call)
//asm_def_noop_imm(_ret)
gen_func_body_onebyte_no_arg(int3, 0xCC, X86_INS_INT3, 0)
//asm_def_2op(enter,,fuasm_op, uint8_t)//enter size, nestinglevel
gen_func_body_onebyte_no_arg(leave_, 0xC9, X86_INS_LEAVE, 0)
//String Instructions
gen_func_body_string_inst(movs, fuku_assambler_string_mov, X86_INS_MOVS, 0)
gen_func_body_string_inst(cmps, fuku_assambler_string_cmp, X86_INS_CMPS, 0)
gen_func_body_string_inst(stos, fuku_assambler_string_sto, X86_INS_STOS, 0)
gen_func_body_string_inst(lods, fuku_assambler_string_lod, X86_INS_LODS, 0)
gen_func_body_string_inst(scas, fuku_assambler_string_sca, X86_INS_SCAS, 0)
//Flag Control (EFLAG) Instructions
gen_func_body_onebyte_no_arg(stc,  0xF9, X86_INS_STC, X86_EFLAGS_SET_CF)
gen_func_body_onebyte_no_arg(clc,  0xF8, X86_INS_CLC, X86_EFLAGS_RESET_CF)
gen_func_body_onebyte_no_arg(cmc,  0xF5, X86_INS_CMC, X86_EFLAGS_MODIFY_CF)
gen_func_body_onebyte_no_arg(cld,  0xFC, X86_INS_CLD, X86_EFLAGS_RESET_DF)
gen_func_body_onebyte_no_arg(std,  0xFD, X86_INS_STD, X86_EFLAGS_SET_DF)
gen_func_body_onebyte_no_arg(lahf, 0x9F, X86_INS_LAHF, 0)
gen_func_body_onebyte_no_arg(sahf, 0x9E, X86_INS_SAHF, X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
asm_def_noop(pusha,)   //pusha  w  regs
asm_def_noop(pushad,)  //pushad dw regs
asm_def_noop(popa,)//popa   w  regs
asm_def_noop(popad,)   //popad  dw regs
asm_def_noop(pushf,)   //pushf  w  flags
asm_def_noop(pushfd,)  //pushfd dw flags
asm_def_noop(popf,)//popf   w  flags
asm_def_noop(popfd,)   //popfd  dw flags
//Miscellaneous Instructions
//LEA
//asm_def_1op(nop,,uint8_t)
gen_func_body_twobyte_no_arg(ud2, 0x0F, 0x0B, X86_INS_UD2, 0)
gen_func_body_twobyte_no_arg(cpuid, 0x0F, 0xA2, X86_INS_CPUID, 0)
//Random Number Generator Instructions
//RDRAND
//RDSEED
//BMI1, BMI2
//ANDN
//BEXTR
//BLSI
//BLSMSK
//BLSR
//BZHI
//LZCNT
//MULX
//PDEP
//PEXT
//RORX
//SARX
//SHLX
//SHRX
//SYSTEM INSTRUCTIONS
gen_func_body_onebyte_no_arg(hlt, 0xF4, X86_INS_HLT, 0)
gen_func_body_twobyte_no_arg(rdtsc, 0x0F, 0x31, X86_INS_RDTSC, 0)
gen_func_body_threebyte_no_arg(lfence, 0x0F, 0xAE, 0xE8, X86_INS_LFENCE, 0)





fuku_asm_ret_type fuku_assambler::_nop(uint8_t n) {

    clear_space();
    while (n > 0) {
        switch (n) {
        case 2:
            emit_b(0x66);
        case 1:
            emit_b(0x90);
            return fuku_instruction().set_op_code(bytecode, length).set_id(X86_INS_NOP).set_eflags(0);
        case 3:
            emit_b(0x0F);
            emit_b(0x1F);
            emit_b(0x00);
            return fuku_instruction().set_op_code(bytecode, length).set_id(X86_INS_NOP).set_eflags(0);
        case 4:
            emit_b(0x0F);
            emit_b(0x1F);
            emit_b(0x40);
            emit_b(0x00);
            return fuku_instruction().set_op_code(bytecode, length).set_id(X86_INS_NOP).set_eflags(0);
        case 6:
            emit_b(0x66);
        case 5:
            emit_b(0x0F);
            emit_b(0x1F);
            emit_b(0x44);
            emit_b(0x00);
            emit_b(0x00);
            return fuku_instruction().set_op_code(bytecode, length).set_id(X86_INS_NOP).set_eflags(0);
        case 7:
            emit_b(0x0F);
            emit_b(0x1F);
            emit_b(0x80);
            emit_b(0x00);
            emit_b(0x00);
            emit_b(0x00);
            emit_b(0x00);
            return fuku_instruction().set_op_code(bytecode, length).set_id(X86_INS_NOP).set_eflags(0);
        default:
        case 11:
            emit_b(0x66);
            n--;
        case 10:
            emit_b(0x66);
            n--;
        case 9:
            emit_b(0x66);
            n--;
        case 8:
            emit_b(0x0F);
            emit_b(0x1F);
            emit_b(0x84);
            emit_b(0x00);
            emit_b(0x00);
            emit_b(0x00);
            emit_b(0x00);
            emit_b(0x00);
            n -= 8;
        }
    }
    return fuku_instruction().set_op_code(bytecode, length).set_id(X86_INS_NOP).set_eflags(0);
}
