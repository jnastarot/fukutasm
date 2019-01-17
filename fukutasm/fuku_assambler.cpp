#include "stdafx.h"
#include "fuku_assambler.h"
#include "fuku_assambler_func_macro.h"

#define set_modrm(mod, reg_idx) \
    FUKU_ASSERT_GT(mod , 3);\
    raw_operand[0] = (uint8_t(mod) << 6) | reg_idx;\
    if(operand_size < 1){operand_size = 1;}


#define set_sib(scale,is_index_ext,reg_idx_index,reg_idx_base)\
    FUKU_ASSERT_GT(operand_size, 1);\
    FUKU_ASSERT_GT(scale , 3);\
    FUKU_ASSERT_NEQ( ((!is_index_ext && reg_idx_index == FUKU_REG_INDEX_SP) || reg_idx_base == FUKU_REG_INDEX_SP), 0);\
    raw_operand[1] = (scale << 6) | (reg_idx_index << 3) | reg_idx_base;\
    if(operand_size < 2){operand_size = 2;}

#define set_disp8(disp)\
    raw_operand[operand_size] = (uint8_t)disp;\
    displacment_offset = length + operand_size;\
    operand_size += sizeof(int8_t);

#define set_dispr(disp)\
    *(uint32_t*)&raw_operand[operand_size] = disp;\
    displacment_offset = length + operand_size;\
    operand_size += sizeof(uint32_t);


fuku_assambler::fuku_assambler(fuku_assambler_arch arch)
    : short_cfg(FUKU_ASM_SHORT_CFG_USE_EAX_SHORT | FUKU_ASM_SHORT_CFG_USE_DISP_SHORT | FUKU_ASM_SHORT_CFG_USE_IMM_SHORT), arch(arch){}

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

void fuku_assambler::emit_modrm(fuku_register rm_reg, fuku_register reg) {
    emit_b(0xC0 | fuku_get_index_by_register(reg) << 3 | fuku_get_index_by_register(rm_reg));
}

void fuku_assambler::emit_modrm(fuku_register rm_reg, int code) {
    emit_b(0xC0 | code << 3 | fuku_get_index_by_register(rm_reg));
}


void fuku_assambler::emit_operand_x64(const fuku_operand& rm_reg, fuku_register_index reg) {

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

void fuku_assambler::emit_operand_x86(const fuku_operand& rm_reg, fuku_register_index reg) {
    FUKU_ASSERT_EQ(length, 0);

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
            if (base_idx == FUKU_REG_INDEX_SP) {
                set_sib(FUKU_OPERAND_SCALE_1,0, FUKU_REG_INDEX_SP, base_idx);
            }
        }
        else if (is_used_short_disp() && rm_reg.get_disp().is_8()) {

            // [base + disp8]
            set_modrm(1, base_idx);
            if (fuku_get_index_by_register(rm_reg.get_base()) == FUKU_REG_INDEX_SP) {
                set_sib(FUKU_OPERAND_SCALE_1,0, FUKU_REG_INDEX_SP, base_idx);
            }
            set_disp8(rm_reg.get_disp().get_immediate8());
        }
        else {

            // [base + disp/r]
            set_modrm(2, base_idx);
            if (base_idx == FUKU_REG_INDEX_SP) {
                set_sib(FUKU_OPERAND_SCALE_1,0, FUKU_REG_INDEX_SP, base_idx);
            }
            set_dispr(rm_reg.get_disp().get_immediate32());
        }

        break;
    }

    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_INDEX:
    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_INDEX_DISP: {

        FUKU_ASSERT(index_idx != FUKU_REG_INDEX_SP);

        // [base + index*scale + disp/r]
        if (rm_reg.get_disp().get_immediate32() == 0 && base_idx != FUKU_REG_INDEX_BP) {
            // [base + index*scale]
            set_modrm(0, FUKU_REG_INDEX_SP);
            set_sib(rm_reg.get_scale(),0, index_idx, base_idx);
        }
        else if (is_used_short_disp() && rm_reg.get_disp().is_8()) {
            // [base + index*scale + disp8]
            set_modrm(1, FUKU_REG_INDEX_SP);
            set_sib(rm_reg.get_scale(),0, index_idx, base_idx);
            set_disp8(rm_reg.get_disp().get_immediate8());
        }
        else {
            // [base + index*scale + disp/r]
            set_modrm(2, FUKU_REG_INDEX_SP);
            set_sib(rm_reg.get_scale(),0, index_idx, base_idx);
            set_dispr(rm_reg.get_disp().get_immediate32());
        }

        break;
    }
    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_INDEX_DISP: {
        FUKU_ASSERT(index_idx != FUKU_REG_INDEX_SP);

        // [index*scale + disp/r]
        set_modrm(0, FUKU_REG_INDEX_SP);
        set_sib(rm_reg.get_scale(),0, index_idx, FUKU_REG_INDEX_BP);
        set_dispr(rm_reg.get_disp().get_immediate32());
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

void fuku_assambler::emit_operand(const fuku_operand& rm_reg, fuku_register reg) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) {
        emit_operand_x86(rm_reg, fuku_get_index_by_register(reg));
    }
    else {
        emit_operand_x64(rm_reg, fuku_get_index_by_register(reg));
    }
}
void fuku_assambler::emit_operand(const fuku_operand& rm_reg, int code) {
    if (arch == FUKU_ASSAMBLER_ARCH_X86) {
        emit_operand_x86(rm_reg, (fuku_register_index)code);
    }
    else {
        emit_operand_x64(rm_reg, (fuku_register_index)code);
    }
}

//Data Transfer Instructions
//MOV
//MOVcc
fuku_asm_ret_type fuku_assambler::_xchg_b(const fuku_operand& dst, fuku_register src) {
    gencleanerdata
    gen_pattern32_1em_op_r(0x86, dst, src)
    gen_func_return(X86_INS_XCHG, 0)
}
fuku_asm_ret_type fuku_assambler::_xchg_b(fuku_register dst, fuku_register src) {
    gencleanerdata
    gen_pattern32_1em_rm_r(0x86, dst, src)
    gen_func_return(X86_INS_XCHG, 0)
}
fuku_asm_ret_type fuku_assambler::_xchg_w(const fuku_operand& dst, fuku_register src) {
    gencleanerdata
    gen_pattern32_1em_op_r_word(0x87, dst, src)
    gen_func_return(X86_INS_XCHG, 0)
}
fuku_asm_ret_type fuku_assambler::_xchg_w(fuku_register dst, fuku_register src) {
    gencleanerdata
    gen_pattern32_1em_rm_r_word(0x87, dst, src)
    gen_func_return(X86_INS_XCHG, 0)
}
fuku_asm_ret_type fuku_assambler::_xchg_dw(const fuku_operand& dst, fuku_register src) {
    gencleanerdata
    gen_pattern32_1em_op_r(0x87, dst, src)
    gen_func_return(X86_INS_XCHG, 0)
}
fuku_asm_ret_type fuku_assambler::_xchg_dw(fuku_register dst, fuku_register src) {
    gencleanerdata
    gen_pattern32_1em_rm_r(0x87, dst, src)
    gen_func_return(X86_INS_XCHG, 0)
}
fuku_asm_ret_type fuku_assambler::_xchg_qw(const fuku_operand& dst, fuku_register src) {
    gencleanerdata
    gen_pattern64_1em_op_r(0x87, dst, src)
    gen_func_return(X86_INS_XCHG, 0)
}
fuku_asm_ret_type fuku_assambler::_xchg_qw(fuku_register dst, fuku_register src) {
    gencleanerdata
    gen_pattern64_1em_rm_r(0x87, dst, src)
    gen_func_return(X86_INS_XCHG, 0)
}
fuku_asm_ret_type fuku_assambler::_bswap_dw(fuku_register dst) {
    gencleanerdata
    emit_optional_rex_32(dst);
    emit_b(0x0F);
    emit_b(0xC8 | fuku_get_index_by_register(dst));
    gen_func_return(X86_INS_BSWAP, 0)
}
fuku_asm_ret_type fuku_assambler::_bswap_qw(fuku_register dst) {
    gencleanerdata
    emit_rex_64(dst);
    emit_b(0x0F);
    emit_b(0xC8 | fuku_get_index_by_register(dst));
    gen_func_return(X86_INS_BSWAP, 0)
}
//XADD
//CMPXCHG
//CMPXCHG8B
fuku_asm_ret_type fuku_assambler::_push_w(const fuku_immediate& imm) {
    gencleanerdata
    emit_b(FUKU_PREFIX_OVERRIDE_DATA);
    if (is_used_short_imm() && imm.is_8()) {
        emit_b(0x6A);
        emit_immediate_b(imm);
    }
    else {
        emit_b(0x68);
        emit_immediate_w(imm);
    }
    gen_func_return(X86_INS_PUSH, 0)
}

fuku_asm_ret_type fuku_assambler::_push_dw(const fuku_immediate& imm) {
    gencleanerdata
        if (is_used_short_imm() && imm.is_8()) {
            emit_b(0x6A);
            emit_immediate_b(imm);
        }
        else {
            emit_b(0x68);
            emit_immediate_dw(imm);
        }
    gen_func_return(X86_INS_PUSH, 0)
}
fuku_asm_ret_type fuku_assambler::_push_qw(const fuku_immediate& imm) {
    return _push_dw(imm);
}
fuku_asm_ret_type fuku_assambler::_push_w(fuku_register src) {
    gencleanerdata
    emit_b(FUKU_PREFIX_OVERRIDE_DATA);
    emit_optional_rex_32(src);
    emit_b(0x50 | fuku_get_index_by_register(src));
    gen_func_return(X86_INS_PUSH, 0)
}
fuku_asm_ret_type fuku_assambler::_push_dw(fuku_register src) {
    gencleanerdata
    emit_optional_rex_32(src);
    emit_b(0x50 | fuku_get_index_by_register(src));
    gen_func_return(X86_INS_PUSH, 0)
}
fuku_asm_ret_type fuku_assambler::_push_qw(fuku_register src) {
    return _push_dw(src);
}
fuku_asm_ret_type fuku_assambler::_push_w(const fuku_operand& src) {
    gencleanerdata
    gen_pattern32_1em_op_idx_word(0xFF, src, 4);
    gen_func_return(X86_INS_PUSH, 0)
}
fuku_asm_ret_type fuku_assambler::_push_dw(const fuku_operand& src) {
    gencleanerdata
    gen_pattern32_1em_op_idx(0xFF, src, 4);
    gen_func_return(X86_INS_PUSH, 0)
}
fuku_asm_ret_type fuku_assambler::_push_qw(const fuku_operand& src) {
    return _push_dw(src);
}
fuku_asm_ret_type fuku_assambler::_pop_w(fuku_register dst) {
    gencleanerdata
    emit_b(FUKU_PREFIX_OVERRIDE_DATA);
    emit_optional_rex_32(dst);
    emit_b(0x58 | fuku_get_index_by_register(dst));
    gen_func_return(X86_INS_POP, 0)
}
fuku_asm_ret_type fuku_assambler::_pop_dw(fuku_register dst) {
    gencleanerdata
    emit_optional_rex_32(dst);
    emit_b(0x58 | fuku_get_index_by_register(dst));
    gen_func_return(X86_INS_POP, 0)
}
fuku_asm_ret_type fuku_assambler::_pop_qw(fuku_register dst) {
    return _pop_dw(dst);
}
fuku_asm_ret_type fuku_assambler::_pop_w(const fuku_operand& dst) {
    gencleanerdata
    gen_pattern32_1em_op_idx_word(0xFF, dst, 0);
    gen_func_return(X86_INS_POP, 0)
}
fuku_asm_ret_type fuku_assambler::_pop_dw(const fuku_operand& dst) {
    gencleanerdata
    gen_pattern32_1em_op_idx(0xFF, dst, 0);
    gen_func_return(X86_INS_POP, 0)
}
fuku_asm_ret_type fuku_assambler::_pop_qw(const fuku_operand& dst) {
    return _pop_dw(dst);
}
gen_func_body_twobyte_no_arg(cwd,  FUKU_PREFIX_OVERRIDE_DATA,0x99, X86_INS_CDQ, 0)
gen_func_body_onebyte_no_arg(cdq,  0x99, X86_INS_CDQ, 0)
gen_func_body_twobyte_no_arg(cqo,  0x48, 0x99, X86_INS_CDQ, 0)
gen_func_body_movxx(movzx, 0xB6, X86_INS_MOVZX)
gen_func_body_movxx(movsx, 0xBE, X86_INS_MOVSX)
fuku_asm_ret_type fuku_assambler::_movsx_dword_dw(fuku_register dst, fuku_register src) {
    gencleanerdata
    gen_pattern32_1em_rm_r(0x63, dst, src);
    gen_func_return(X86_INS_MOVSXD, 0)
}
fuku_asm_ret_type fuku_assambler::_movsx_dword_dw(const fuku_operand& dst, fuku_register src) {
    gencleanerdata
    gen_pattern32_1em_op_r(0x63, dst, src);
    gen_func_return(X86_INS_MOVSXD, 0)
}
fuku_asm_ret_type fuku_assambler::_movsx_dword_qw(fuku_register dst, fuku_register src) {
    gencleanerdata
    gen_pattern64_1em_rm_r(0x63, dst, src);
    gen_func_return(X86_INS_MOVSXD, 0)
}
fuku_asm_ret_type fuku_assambler::_movsx_dword_qw(const fuku_operand& dst, fuku_register src) {
    gencleanerdata
    gen_pattern32_1em_op_r(0x63, dst, src);
    gen_func_return(X86_INS_MOVSXD, 0)
}
//Binary Arithmetic Instructions
//ADCX
//ADOX
gen_func_body_arith(add, fuku_assambler_arith_add, X86_INS_ADD, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF    | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_arith(adc, fuku_assambler_arith_adc, X86_INS_ADC, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF    | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_arith(sub, fuku_assambler_arith_sub, X86_INS_SUB, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF    | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_arith(sbb, fuku_assambler_arith_sbb, X86_INS_SBB, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF    | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_arith_ex_one_op(imul, fuku_assambler_arith_ex_imul, X86_INS_IMUL, X86_EFLAGS_MODIFY_SF    | X86_EFLAGS_MODIFY_CF    | X86_EFLAGS_MODIFY_OF    | X86_EFLAGS_UNDEFINED_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF)
gen_func_body_arith_ex_one_op(mul,  fuku_assambler_arith_ex_mul,  X86_INS_MUL,  X86_EFLAGS_MODIFY_OF    | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_arith_ex_one_op(idiv, fuku_assambler_arith_ex_idiv, X86_INS_IDIV, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_UNDEFINED_CF)
gen_func_body_arith_ex_one_op(div,  fuku_assambler_arith_ex_div,  X86_INS_DIV,  X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_UNDEFINED_CF)
gen_func_body_arith_incdec(inc, fuku_assambler_arith_inc, X86_INS_INC, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF)
gen_func_body_arith_incdec(dec, fuku_assambler_arith_dec, X86_INS_DEC, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF)
gen_func_body_arith_ex_one_op(neg,  fuku_assambler_arith_ex_neg,  X86_INS_NEG,  X86_EFLAGS_MODIFY_OF    | X86_EFLAGS_MODIFY_SF    | X86_EFLAGS_MODIFY_ZF    | X86_EFLAGS_MODIFY_AF    | X86_EFLAGS_MODIFY_PF    | X86_EFLAGS_MODIFY_CF)
gen_func_body_arith(cmp, fuku_assambler_arith_cmp, X86_INS_CMP, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF    | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
//Decimal Arithmetic Instructions
gen_func_body_onebyte_no_arg(daa, 0x27, X86_INS_DAA, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_onebyte_no_arg(das, 0x2F, X86_INS_DAS, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_onebyte_no_arg(aaa, 0x37, X86_INS_AAA, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_onebyte_no_arg(aas, 0x3F, X86_INS_AAS, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_MODIFY_CF)
//AAM
//AAD
//Logical Instructions Instructions
gen_func_body_arith(and, fuku_assambler_arith_and, X86_INS_AND, X86_EFLAGS_RESET_OF  | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
gen_func_body_arith(or , fuku_assambler_arith_or,  X86_INS_OR,  X86_EFLAGS_RESET_OF  | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
gen_func_body_arith(xor, fuku_assambler_arith_xor, X86_INS_XOR, X86_EFLAGS_RESET_OF  | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
gen_func_body_arith_ex_one_op(not,  fuku_assambler_arith_ex_not,  X86_INS_NOT,  0)
//Shift and Rotate Instructions
gen_func_body_shift(sar, fuku_assambler_shift_sar, X86_INS_SAR, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_shift(shr, fuku_assambler_shift_shr, X86_INS_SHR, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_shift(shl, fuku_assambler_shift_shl, X86_INS_SHL, X86_EFLAGS_MODIFY_OF    | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
//SHRD
//SHLD
gen_func_body_shift(ror, fuku_assambler_shift_ror, X86_INS_ROR, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_MODIFY_CF)
gen_func_body_shift(rol, fuku_assambler_shift_rol, X86_INS_ROL, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_MODIFY_CF)
gen_func_body_shift(rcr, fuku_assambler_shift_rcr, X86_INS_RCR, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_MODIFY_CF)
gen_func_body_shift(rcl, fuku_assambler_shift_rcl, X86_INS_RCL, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_MODIFY_CF)
//Bit and Byte Instructions
gen_func_body_bit(bt , fuku_assambler_bittest_bt,  X86_INS_BT,  X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_bit(bts, fuku_assambler_bittest_bts, X86_INS_BTS, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_bit(btr, fuku_assambler_bittest_btr, X86_INS_BTR, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_bit(btc, fuku_assambler_bittest_btc, X86_INS_BTC, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_MODIFY_CF)
fuku_asm_ret_type fuku_assambler::_setcc(fuku_condition cond, fuku_register dst) {
    gencleanerdata
    FUKU_ASSERT(cond < 0 || cond >= fuku_condition::FUKU_CONDITION_MAX);
    gen_pattern32_2em_rm_idx(0x0F, 0x80 | cond, dst, 0);
    gen_func_return(fuku_to_capstone_cc(cond, CONVERT_TYPE_SETCC), di_fl_jcc[cond])
}
fuku_asm_ret_type fuku_assambler::_setcc(fuku_condition cond, const fuku_operand& dst) {
    gencleanerdata
    FUKU_ASSERT(cond < 0 || cond >= fuku_condition::FUKU_CONDITION_MAX);
    gen_pattern32_2em_op_idx(0x0F, 0x80 | cond, dst, 0);
    gen_func_return(fuku_to_capstone_cc(cond, CONVERT_TYPE_SETCC), di_fl_jcc[cond])
}
fuku_asm_ret_type fuku_assambler::_test_b(fuku_register dst, fuku_register src) {
    gencleanerdata
    gen_pattern32_1em_rm_r(0x84, dst, src)
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type fuku_assambler::_test_b(fuku_register dst, const fuku_immediate& src) {
    gencleanerdata
    emit_optional_rex_32(dst);
    if (is_used_short_eax() && dst == fuku_register::FUKU_REG_AL) {
        emit_b(0xA8);
    }
    else {
        emit_b(0xF6);
        emit_modrm(dst, 0);
    }
    emit_immediate_b(src);
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type fuku_assambler::_test_b(const fuku_operand& dst, fuku_register src) {
    gencleanerdata
    gen_pattern32_1em_op_r(0x84, dst, src)
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type fuku_assambler::_test_b(const fuku_operand& dst, const fuku_immediate& src) {
    gencleanerdata
    emit_optional_rex_32(dst);
    emit_b(0xF6);
    emit_operand(dst, 0);
    emit_immediate_b(src);
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type fuku_assambler::_test_w(fuku_register dst, fuku_register src) {
    gencleanerdata
    gen_pattern32_1em_rm_r_word(0x85, dst, src)
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type fuku_assambler::_test_w(fuku_register dst, const fuku_immediate& src) {
    gencleanerdata
    emit_b(FUKU_PREFIX_OVERRIDE_DATA);
    emit_optional_rex_32(dst);
    if (is_used_short_eax() && dst == fuku_register::FUKU_REG_AX) {
        emit_b(0xA9);
    }
    else {
        emit_b(0xF7);
        emit_modrm(dst, 0);
    }
    emit_immediate_w(src);
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type fuku_assambler::_test_w(const fuku_operand& dst, fuku_register src) {
    gencleanerdata
    gen_pattern32_1em_op_r_word(0x85, dst, src)
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type fuku_assambler::_test_w(const fuku_operand& dst, const fuku_immediate& src) {
    gencleanerdata
    emit_b(FUKU_PREFIX_OVERRIDE_DATA);
    emit_optional_rex_32(dst);
    emit_b(0xF7);
    emit_operand(dst, 0);
    emit_immediate_w(src);
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type fuku_assambler::_test_dw(fuku_register dst, fuku_register src) {
    gencleanerdata
    gen_pattern32_1em_rm_r(0x85, dst, src)
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type fuku_assambler::_test_dw(fuku_register dst, const fuku_immediate& src) {
    gencleanerdata
    emit_optional_rex_32(src, dst);
    if (is_used_short_eax() && dst == fuku_register::FUKU_REG_EAX) {
        emit_b(0xA9);
    }
    else {
        emit_b(0xF7);
        emit_modrm(dst, 0);
    }
    emit_immediate_dw(src);
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type fuku_assambler::_test_dw(const fuku_operand& dst, fuku_register src) {
    gencleanerdata
    gen_pattern32_1em_op_r(0x85, dst, src)
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type fuku_assambler::_test_dw(const fuku_operand& dst, const fuku_immediate& src) {
    gencleanerdata
    emit_optional_rex_32(dst);
    emit_b(0xF7);
    emit_operand(dst, 0);
    emit_immediate_dw(src);
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type fuku_assambler::_test_qw(fuku_register dst, fuku_register src) {
    gencleanerdata
    gen_pattern64_1em_rm_r(0x85, dst, src);
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type fuku_assambler::_test_qw(fuku_register dst, const fuku_immediate& src) {
    gencleanerdata
    emit_rex_64(dst);
    if (is_used_short_eax() && dst == fuku_register::FUKU_REG_EAX) {
        emit_b(0xA9);
    }
    else {
        emit_b(0xF7);
        emit_modrm(dst, 0);
    }
    emit_immediate_dw(src);
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type fuku_assambler::_test_qw(const fuku_operand& dst, fuku_register src) {
    gencleanerdata
    gen_pattern64_1em_op_r(0x85, dst, src)
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type fuku_assambler::_test_qw(const fuku_operand& dst, const fuku_immediate& src) {
    gencleanerdata
    emit_rex_64(dst);
    emit_b(0xF7);
    emit_operand(dst, 0);
    emit_immediate_dw(src);
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
//CRC32
//POPCNT
//Control Transfer Instructions
gen_func_body_ff_offset(jmp,  0xF9, X86_INS_JMP,  0)
gen_func_body_ff_r(     jmp,  4   , X86_INS_JMP,  0)
gen_func_body_ff_op(    jmp,  4   , X86_INS_JMP,  0) 
fuku_asm_ret_type fuku_assambler::_jcc(fuku_condition cond, const fuku_immediate& imm) {
    gencleanerdata
    FUKU_ASSERT(cond < 0 || cond >= fuku_condition::FUKU_CONDITION_MAX);
    emit_b(0x0F);
    emit_b(0x80 | cond);
    emit_immediate_dw(imm);
    gen_func_return(fuku_to_capstone_cc(cond, CONVERT_TYPE_JCC), di_fl_jcc[cond])
}
gen_func_body_ff_offset(call, 0xF8, X86_INS_CALL, 0)
gen_func_body_ff_r(     call, 2   , X86_INS_CALL, 0)
gen_func_body_ff_op(    call, 2   , X86_INS_CALL, 0)
gen_func_body_onebyte_no_arg(ret, 0xC3, X86_INS_RET, 0)
fuku_asm_ret_type fuku_assambler::_ret(const fuku_immediate& imm) {
    gencleanerdata
    emit_b(0xC2);
    emit_immediate_w(imm);
    gen_func_return(X86_INS_RET, 0)
}
gen_func_body_onebyte_no_arg(int3, 0xCC, X86_INS_INT3, 0)
fuku_asm_ret_type fuku_assambler::_enter(const fuku_immediate& size, uint8_t nestinglevel) {
    gencleanerdata
    emit_b(0xC8);
    emit_immediate_w(size);
    emit_b(nestinglevel);
    gen_func_return(X86_INS_ENTER, 0)
}
gen_func_body_onebyte_no_arg(leave_, 0xC9, X86_INS_LEAVE, 0)
//String Instructions
gen_func_body_string_inst_withoutq(outs, fuku_assambler_string_out, X86_INS_OUTS, X86_EFLAGS_TEST_DF)
gen_func_body_string_inst(movs, fuku_assambler_string_mov, X86_INS_MOVS, X86_EFLAGS_TEST_DF)
gen_func_body_string_inst(cmps, fuku_assambler_string_cmp, X86_INS_CMPS, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_string_inst(stos, fuku_assambler_string_sto, X86_INS_STOS, X86_EFLAGS_TEST_DF)
gen_func_body_string_inst(lods, fuku_assambler_string_lod, X86_INS_LODS, X86_EFLAGS_TEST_DF)
gen_func_body_string_inst(scas, fuku_assambler_string_sca, X86_INS_SCAS, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
//Flag Control (EFLAG) Instructions
gen_func_body_onebyte_no_arg(stc,  0xF9, X86_INS_STC, X86_EFLAGS_SET_CF)
gen_func_body_onebyte_no_arg(clc,  0xF8, X86_INS_CLC, X86_EFLAGS_RESET_CF)
gen_func_body_onebyte_no_arg(cmc,  0xF5, X86_INS_CMC, X86_EFLAGS_MODIFY_CF)
gen_func_body_onebyte_no_arg(cld,  0xFC, X86_INS_CLD, X86_EFLAGS_RESET_DF)
gen_func_body_onebyte_no_arg(std,  0xFD, X86_INS_STD, X86_EFLAGS_SET_DF)
gen_func_body_onebyte_no_arg(lahf, 0x9F, X86_INS_LAHF, 0)
gen_func_body_onebyte_no_arg(sahf, 0x9E, X86_INS_SAHF, X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_twobyte_no_arg(pusha,  FUKU_PREFIX_OVERRIDE_DATA, 0x60, X86_INS_PUSHAW, 0)
gen_func_body_onebyte_no_arg(pushad, 0x60, X86_INS_PUSHAL, 0)
gen_func_body_twobyte_no_arg(popa,   FUKU_PREFIX_OVERRIDE_DATA, 0x61, X86_INS_POPAW, 0)
gen_func_body_onebyte_no_arg(popad,  0x61, X86_INS_POPAL, 0)
gen_func_body_twobyte_no_arg(pushf,  FUKU_PREFIX_OVERRIDE_DATA, 0x9C, X86_INS_PUSHF, 0)
gen_func_body_onebyte_no_arg(pushfd, 0x9C, X86_INS_PUSHFD, 0)
gen_func_body_onebyte_no_arg(pushfq, 0x9C, X86_INS_PUSHFQ, 0)
gen_func_body_twobyte_no_arg(popf,   FUKU_PREFIX_OVERRIDE_DATA, 0x9D, X86_INS_POPF, X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_CF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_TF | X86_EFLAGS_MODIFY_IF | X86_EFLAGS_MODIFY_DF | X86_EFLAGS_MODIFY_NT)
gen_func_body_onebyte_no_arg(popfd,  0x9D, X86_INS_POPFD, X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_CF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_TF | X86_EFLAGS_MODIFY_IF | X86_EFLAGS_MODIFY_DF | X86_EFLAGS_MODIFY_NT | X86_EFLAGS_MODIFY_RF)
gen_func_body_onebyte_no_arg(popfq,  0x9D, X86_INS_POPFQ, X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_CF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_TF | X86_EFLAGS_MODIFY_IF | X86_EFLAGS_MODIFY_DF | X86_EFLAGS_MODIFY_NT | X86_EFLAGS_MODIFY_RF)
//Miscellaneous Instructions
fuku_asm_ret_type fuku_assambler::_lea_w(fuku_register dst, const fuku_operand& src) {
    gencleanerdata
    gen_pattern32_1em_op_r_word(0x8D, src, dst)
    gen_func_return(X86_INS_LEA, 0)
}
fuku_asm_ret_type fuku_assambler::_lea_dw(fuku_register dst, const fuku_operand& src) {
    gencleanerdata
    gen_pattern32_1em_op_r(0x8D, src, dst)
    gen_func_return(X86_INS_LEA, 0)
}
fuku_asm_ret_type fuku_assambler::_lea_qw(fuku_register dst, const fuku_operand& src) {
    gencleanerdata
    gen_pattern64_1em_op_r(0x8D, src, dst)
    gen_func_return(X86_INS_LEA, 0)
}
gen_func_body_twobyte_no_arg(ud2, 0x0F, 0x0B, X86_INS_UD2, 0)
gen_func_body_twobyte_no_arg(cpuid, 0x0F, 0xA2, X86_INS_CPUID, 0)
//Random Number Generator Instructions
fuku_asm_ret_type fuku_assambler::_rdrand_w(fuku_register dst) {
    gencleanerdata
    emit_b(FUKU_PREFIX_OVERRIDE_DATA);
    emit_optional_rex_32(dst);
    emit_b(0x0F);
    emit_b(0xC7);
    emit_b(0xF0 | fuku_get_index_by_register(dst));
    gen_func_return(X86_INS_RDRAND, X86_EFLAGS_MODIFY_CF | X86_EFLAGS_RESET_OF | X86_EFLAGS_RESET_SF | X86_EFLAGS_RESET_ZF | X86_EFLAGS_RESET_AF | X86_EFLAGS_RESET_PF)
}
fuku_asm_ret_type fuku_assambler::_rdrand_dw(fuku_register dst) {
    gencleanerdata
    emit_optional_rex_32(dst);
    emit_b(0x0F);
    emit_b(0xC7);
    emit_b(0xF0 | fuku_get_index_by_register(dst));
    gen_func_return(X86_INS_RDRAND, X86_EFLAGS_MODIFY_CF | X86_EFLAGS_RESET_OF | X86_EFLAGS_RESET_SF | X86_EFLAGS_RESET_ZF | X86_EFLAGS_RESET_AF | X86_EFLAGS_RESET_PF)
}
fuku_asm_ret_type fuku_assambler::_rdrand_qw(fuku_register dst) {
    gencleanerdata
    emit_rex_64(dst);
    emit_b(0x0F);
    emit_b(0xC7);
    emit_b(0xF0 | fuku_get_index_by_register(dst));
    gen_func_return(X86_INS_RDRAND, X86_EFLAGS_MODIFY_CF | X86_EFLAGS_RESET_OF | X86_EFLAGS_RESET_SF | X86_EFLAGS_RESET_ZF | X86_EFLAGS_RESET_AF | X86_EFLAGS_RESET_PF)
}
fuku_asm_ret_type fuku_assambler::_rdseed_w(fuku_register dst) {
    gencleanerdata
    emit_b(FUKU_PREFIX_OVERRIDE_DATA);
    emit_optional_rex_32(dst);
    emit_b(0x0F);
    emit_b(0xC7);
    emit_b(0xF8 | fuku_get_index_by_register(dst));
    gen_func_return(X86_INS_RDRAND, X86_EFLAGS_MODIFY_CF | X86_EFLAGS_RESET_OF | X86_EFLAGS_RESET_SF | X86_EFLAGS_RESET_ZF | X86_EFLAGS_RESET_AF | X86_EFLAGS_RESET_PF)
}
fuku_asm_ret_type fuku_assambler::_rdseed_dw(fuku_register dst) {
    gencleanerdata
    emit_optional_rex_32(dst);
    emit_b(0x0F);
    emit_b(0xC7);
    emit_b(0xF8 | fuku_get_index_by_register(dst));
    gen_func_return(X86_INS_RDRAND, X86_EFLAGS_MODIFY_CF | X86_EFLAGS_RESET_OF | X86_EFLAGS_RESET_SF | X86_EFLAGS_RESET_ZF | X86_EFLAGS_RESET_AF | X86_EFLAGS_RESET_PF)
}
fuku_asm_ret_type fuku_assambler::_rdseed_qw(fuku_register dst) {
    gencleanerdata
    emit_rex_64(dst);
    emit_b(0x0F);
    emit_b(0xC7);
    emit_b(0xF8 | fuku_get_index_by_register(dst));
    gen_func_return(X86_INS_RDRAND, X86_EFLAGS_MODIFY_CF | X86_EFLAGS_RESET_OF | X86_EFLAGS_RESET_SF | X86_EFLAGS_RESET_ZF | X86_EFLAGS_RESET_AF | X86_EFLAGS_RESET_PF)
}
//BMI1, BMI2 Instructions
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
