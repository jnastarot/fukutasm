#include "stdafx.h"
#include "fuku_internal_assambler.h"
#include "fuku_internal_assambler_func_macro.h"

#define set_modrm(mod, reg, rm) \
    FUKU_ASSERT_GT(mod , 3);\
    raw_operand[0] = (mod << 6 | reg << 3 | rm);\
    if(operand_size < 1){operand_size = 1;}

#define set_sib(scale, reg_idx_index,reg_idx_base)\
    FUKU_ASSERT_GT(operand_size, 1);\
    FUKU_ASSERT_GT(scale , 3);\
    raw_operand[1] = (scale << 6) | (reg_idx_index << 3) | reg_idx_base;\
    if(operand_size < 2){operand_size = 2;}


#define set_disp8(disp)\
    raw_operand[operand_size] = (uint8_t)disp;\
    ctx.displacment_offset = ctx.length + operand_size;\
    operand_size += sizeof(int8_t);

#define set_dispr(disp)\
    *(uint32_t*)&raw_operand[operand_size] = disp;\
    ctx.displacment_offset = ctx.length + operand_size;\
    operand_size += sizeof(uint32_t);


namespace fukutasm {

inline void clear_space(fuku_assambler_ctx& ctx) {
    memset(ctx.bytecode, 0, sizeof(ctx.bytecode));
    ctx.length = 0;
    ctx.displacment_offset = 0;
    ctx.immediate_offset = 0;
    ctx.disp_reloc = 0;
    ctx.imm_reloc = 0;
}

inline void emit_b(fuku_assambler_ctx& ctx, uint8_t x) {
    ctx.bytecode[ctx.length] = x;
    ctx.length++;
}

inline void emit_w(fuku_assambler_ctx& ctx, uint16_t x) {
    *(uint16_t*)&ctx.bytecode[ctx.length] = x;
    ctx.length += sizeof(uint16_t);
}

inline void emit_dw(fuku_assambler_ctx& ctx, uint32_t x) {
    *(uint32_t*)&ctx.bytecode[ctx.length] = x;
    ctx.length += sizeof(uint32_t);
}

inline void emit_qw(fuku_assambler_ctx& ctx, uint64_t x) {
    *(uint64_t*)&ctx.bytecode[ctx.length] = x;
    ctx.length += sizeof(uint64_t);
}

inline void emit_immediate_b(fuku_assambler_ctx& ctx, const fuku_immediate& src) {
    ctx.bytecode[ctx.length] = src.get_immediate8();
    ctx.immediate_offset = ctx.length;
    ctx.length++;
    ctx.imm_reloc = src.is_relocate();
}

inline void emit_immediate_w(fuku_assambler_ctx& ctx, const fuku_immediate& src) {
    *(uint16_t*)&ctx.bytecode[ctx.length] = src.get_immediate16();
    ctx.immediate_offset = ctx.length;
    ctx.length += sizeof(uint16_t);
    ctx.imm_reloc = src.is_relocate();
}

inline void emit_immediate_dw(fuku_assambler_ctx& ctx, const fuku_immediate& src) {
    *(uint32_t*)&ctx.bytecode[ctx.length] = src.get_immediate32();
    ctx.immediate_offset = ctx.length;
    ctx.length += sizeof(uint32_t);
    ctx.imm_reloc = src.is_relocate();
}

inline void emit_immediate_qw(fuku_assambler_ctx& ctx, const fuku_immediate& src) {
    *(uint64_t*)&ctx.bytecode[ctx.length] = src.get_immediate64();
    ctx.immediate_offset = ctx.length;
    ctx.length += sizeof(uint64_t);
    ctx.imm_reloc = src.is_relocate();
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
inline void emit_rex_64(fuku_assambler_ctx& ctx) {
    if (ctx.arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    emit_b(ctx, 0x48);
}

inline void emit_rex_64(fuku_assambler_ctx& ctx, const fuku_register& rm_reg, const fuku_register& reg) {
    if (ctx.arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    emit_b(ctx, 0x48 | (reg.is_ext64() ? 1 : 0) << 2 | (rm_reg.is_ext64() ? 1 : 0));
}

inline void emit_rex_64(fuku_assambler_ctx& ctx, const fuku_operand& rm_reg, const fuku_register& reg) {
    if (ctx.arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    emit_b(ctx, 0x48 | (reg.is_ext64() ? 1 : 0) << 2 | rm_reg.get_low_rex());
}

inline void emit_rex_64(fuku_assambler_ctx& ctx, const fuku_register& rm_reg) {
    if (ctx.arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    emit_b(ctx, 0x48 | (rm_reg.is_ext64() ? 1 : 0));
}

inline void emit_rex_64(fuku_assambler_ctx& ctx, const fuku_operand& op) {
    if (ctx.arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    emit_b(ctx, 0x48 | op.get_low_rex());
}

inline void emit_optional_rex_32(fuku_assambler_ctx& ctx, const fuku_register& rm_reg, const fuku_register& reg) {
    if (ctx.arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    uint8_t rex_bits = (reg.is_ext64() ? 1 : 0) << 2 | (rm_reg.is_ext64() ? 1 : 0);
    if (rex_bits != 0 || rm_reg.is_arch64() || reg.is_arch64()) { emit_b(ctx, 0x40 | rex_bits); }
}

inline void emit_optional_rex_32(fuku_assambler_ctx& ctx, const fuku_operand& rm_reg, const fuku_register& reg) {
    if (ctx.arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    uint8_t rex_bits = (reg.is_ext64() ? 1 : 0) << 2 | rm_reg.get_low_rex();
    if (rex_bits != 0 || reg.is_arch64()) { emit_b(ctx, 0x40 | rex_bits); }
}

inline void emit_optional_rex_32(fuku_assambler_ctx& ctx, const fuku_register& rm_reg) {
    if (ctx.arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    if (rm_reg.is_ext64() || rm_reg.is_arch64()) { emit_b(ctx, 0x41); }
}

inline void emit_optional_rex_32(fuku_assambler_ctx& ctx, const fuku_operand& rm_reg) {
    if (ctx.arch == FUKU_ASSAMBLER_ARCH_X86) { return; }
    if (rm_reg.get_low_rex() != 0) { emit_b(ctx, 0x40 | rm_reg.get_low_rex()); }
}

inline void emit_modrm(fuku_assambler_ctx& ctx, const fuku_register& rm_reg, const fuku_register& reg) {
    emit_b(ctx, 0xC0 | reg.get_index() << 3 | rm_reg.get_index());
}

inline void emit_modrm(fuku_assambler_ctx& ctx, const fuku_register& rm_reg, int code) {
    emit_b(ctx, 0xC0 | code << 3 | rm_reg.get_index());
}

void emit_operand_x64(fuku_assambler_ctx& ctx, const fuku_operand& rm_reg, fuku_register_index reg) {

    uint8_t raw_operand[6] = { 0 };
    uint8_t operand_size = 0;

    uint8_t base_idx = rm_reg.get_base().get_index();
    uint8_t index_idx = rm_reg.get_index().get_index();

    switch (rm_reg.get_type()) {

    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_DISP_ONLY: {
        set_modrm(0, reg, FUKU_REG_INDEX_BP);
        set_dispr(rm_reg.get_disp().get_immediate32());
        break;
    }

    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_ONLY:
    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_DISP: {

        if (base_idx == FUKU_REG_INDEX_SP) {
            set_sib(FUKU_OPERAND_SCALE_1, FUKU_REG_INDEX_SP, base_idx);
        }

        auto& disp = rm_reg.get_disp();

        // [base + disp/r]
        if (disp.get_immediate32() == 0 && base_idx != FUKU_REG_INDEX_BP) {

            // [base]
            set_modrm(0, reg, base_idx);
            
        }
        else if (is_used_short_disp() && disp.is_8()) {

            // [base + disp8]
            set_modrm(1, reg, base_idx);
            set_disp8(disp.get_immediate8());
        }
        else {

            // [base + disp/r]
            set_modrm(2, reg, base_idx);
            set_dispr(disp.get_immediate32());
        }

        break;
    }

    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_INDEX:
    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_INDEX_DISP: {

        FUKU_ASSERT(index_idx != FUKU_REG_INDEX_SP);

        set_sib(rm_reg.get_scale(), index_idx, base_idx);

        // [base + index*scale + disp/r]
        if (rm_reg.get_disp().get_immediate32() == 0 && base_idx != FUKU_REG_INDEX_BP) {
            // [base + index*scale]
            set_modrm(0, reg, FUKU_REG_INDEX_SP);
        }
        else if (is_used_short_disp() && rm_reg.get_disp().is_8()) {
            // [base + index*scale + disp8]
            set_modrm(1, reg, FUKU_REG_INDEX_SP);
            set_disp8(rm_reg.get_disp().get_immediate8());
        }
        else {
            // [base + index*scale + disp/r]
            set_modrm(2, reg, FUKU_REG_INDEX_SP);
            set_dispr(rm_reg.get_disp().get_immediate32());
        }

        break;
    }
    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_INDEX_DISP: {
        FUKU_ASSERT(index_idx != FUKU_REG_INDEX_SP);

        // [index*scale + disp/r]
        set_modrm(0, reg, FUKU_REG_INDEX_SP);
        set_sib(rm_reg.get_scale(), index_idx, FUKU_REG_INDEX_BP);
        set_dispr(rm_reg.get_disp().get_immediate32());
        break;
    }


    default:FUKU_DEBUG;
    }

    ctx.bytecode[ctx.length] = raw_operand[0] | reg << 3;

    for (unsigned i = 1; i < operand_size; i++) { ctx.bytecode[ctx.length + i] = raw_operand[i]; }
    ctx.length += operand_size;
}

void emit_operand_x86(fuku_assambler_ctx& ctx, const fuku_operand& rm_reg, fuku_register_index reg) {
    FUKU_ASSERT_EQ(ctx.length, 0);

    uint8_t raw_operand[6] = { 0 };
    uint8_t operand_size = 0;

    uint8_t base_idx = rm_reg.get_base().get_index();
    uint8_t index_idx = rm_reg.get_index().get_index();

    switch (rm_reg.get_type()) {

    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_DISP_ONLY: {
        set_modrm(0, reg, FUKU_REG_INDEX_BP);
        set_dispr(rm_reg.get_disp().get_immediate32());
        break;
    }

    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_ONLY:
    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_DISP: {

        if (base_idx == FUKU_REG_INDEX_SP) {
            set_sib(FUKU_OPERAND_SCALE_1, FUKU_REG_INDEX_SP, base_idx);
        }

        auto& disp = rm_reg.get_disp();

        // [base + disp/r]
        if (disp.get_immediate32() == 0 && base_idx != FUKU_REG_INDEX_BP) {
            // [base]
            set_modrm(0, reg, base_idx);
        }
        else if (is_used_short_disp() && disp.is_8()) {
            // [base + disp8]
            set_modrm(1, reg, base_idx);
            set_disp8(disp.get_immediate8());
        }
        else {
            // [base + disp/r]
            set_modrm(2, reg, base_idx);
            set_dispr(disp.get_immediate32());
        }
        break;
    }

    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_INDEX:
    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_INDEX_DISP: {

        set_sib(rm_reg.get_scale(), index_idx, base_idx);

        // [base + index*scale + disp/r]
        if (rm_reg.get_disp().get_immediate32() == 0 && base_idx != FUKU_REG_INDEX_BP) {
            // [base + index*scale]
            set_modrm(0, reg, FUKU_REG_INDEX_SP);
        }
        else if (is_used_short_disp() && rm_reg.get_disp().is_8()) {
            // [base + index*scale + disp8]
            set_modrm(1, reg, FUKU_REG_INDEX_SP);
            set_disp8(rm_reg.get_disp().get_immediate8());
        }
        else {
            // [base + index*scale + disp/r]
            set_modrm(2, reg, FUKU_REG_INDEX_SP);
            set_dispr(rm_reg.get_disp().get_immediate32());
        }

        break;
    }
    case fuku_mem_opernad_type::FUKU_MEM_OPERAND_INDEX_DISP: {
        FUKU_ASSERT(index_idx != FUKU_REG_INDEX_SP);

        set_modrm(0, reg, FUKU_REG_INDEX_SP);
        set_sib(rm_reg.get_scale(), index_idx, FUKU_REG_INDEX_BP)
        set_dispr(rm_reg.get_disp().get_immediate32());
        break;
    }


    default:FUKU_DEBUG;
    }


    for (unsigned i = 0; i < operand_size; i++) { ctx.bytecode[ctx.length + i] = raw_operand[i];}
    ctx.length += operand_size;
}

void emit_operand(fuku_assambler_ctx& ctx, const fuku_operand& rm_reg, const fuku_register& reg) {
    if (ctx.arch == FUKU_ASSAMBLER_ARCH_X86) {
        emit_operand_x86(ctx, rm_reg, reg.get_index());
    }
    else {
        emit_operand_x64(ctx, rm_reg, reg.get_index());
    }
}
void emit_operand(fuku_assambler_ctx& ctx, const fuku_operand& rm_reg, int code) {
    if (ctx.arch == FUKU_ASSAMBLER_ARCH_X86) {
        emit_operand_x86(ctx, rm_reg, (fuku_register_index)code);
    }
    else {
        emit_operand_x64(ctx, rm_reg, (fuku_register_index)code);
    }
}

//Data Transfer Instructions
//MOV
fuku_asm_ret_type _mov_b(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) { 
    gencleanerdata
    gen_pattern32_1em_rm_r(0x88, dst, src)
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _mov_b(fuku_assambler_ctx& ctx, const fuku_register& dst,const fuku_immediate& src) {
    gencleanerdata
    gen_pattern32_1em_immb(0xB0 | dst.get_index(), dst, src)
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _mov_b(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_operand& src) {
    gencleanerdata  
    if(is_used_short_eax() && dst.get_reg() == FUKU_REG_AL &&
        src.get_type() == FUKU_MEM_OPERAND_DISP_ONLY ){
        emit_optional_rex_32(ctx, src, dst);
        emit_b(ctx, 0xA0);
        emit_dw(ctx, src.get_disp().get_immediate32());
        ctx.displacment_offset = ctx.length;//todo
    }
    else {
        gen_pattern32_1em_op_r(0x8A, src, dst)
    }
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _mov_b(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_immediate& src) {
    gencleanerdata
    gen_pattern32_1em_op_idx_immb(0xC6, dst, 0, src)
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _mov_b(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata 
    if(is_used_short_eax() && src.get_reg() == FUKU_REG_AL &&
        dst.get_type() == fuku_mem_opernad_type::FUKU_MEM_OPERAND_DISP_ONLY ){
        emit_optional_rex_32(ctx, dst, src);
        emit_b(ctx, 0xA2);
        emit_dw(ctx, dst.get_disp().get_immediate32());
        ctx.displacment_offset = ctx.length; //todo
    }
    else {
        gen_pattern32_1em_op_r(0x88, dst, src)
    }
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _mov_w(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_1em_rm_r_word(0x89, dst, src)
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _mov_w(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_immediate& src) {
    gencleanerdata
    emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);
    emit_optional_rex_32(ctx, dst);
    emit_b(ctx, 0xB8 | dst.get_index());
    emit_immediate_w(ctx, src);
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _mov_w(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_operand& src) {
    gencleanerdata
    if (is_used_short_eax() && dst.get_reg() == FUKU_REG_AX &&
        src.get_type() == fuku_mem_opernad_type::FUKU_MEM_OPERAND_DISP_ONLY) {
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);
        emit_optional_rex_32(ctx, src, dst);
        emit_b(ctx, 0xA1);
        emit_dw(ctx, src.get_disp().get_immediate32());
        ctx.displacment_offset = ctx.length;//todo
    }
    else {
        gen_pattern32_1em_op_r_word(0x8B, src, dst)
    }
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _mov_w(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata
    if (is_used_short_eax() && src.get_reg() == FUKU_REG_AX &&
        dst.get_type() == fuku_mem_opernad_type::FUKU_MEM_OPERAND_DISP_ONLY) {
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);
        emit_optional_rex_32(ctx, dst, src);
        emit_b(ctx, 0xA3);
        emit_dw(ctx, dst.get_disp().get_immediate32());
        ctx.displacment_offset = ctx.length;//todo
    }
    else {
        gen_pattern32_1em_op_r_word(0x89, dst, src)
    }
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _mov_w(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_immediate& src) {
    gencleanerdata
    gen_pattern32_1em_op_idx_immw_word(0xC7, dst, 0, src)
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _mov_dw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_immediate& src) {
    gencleanerdata
    emit_optional_rex_32(ctx, dst);
    emit_b(ctx, 0xB8 | dst.get_index());
    emit_immediate_dw(ctx, src);
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _mov_dw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_operand& src) {
    gencleanerdata
    if (is_used_short_eax() && dst.get_reg() == FUKU_REG_EAX &&
        src.get_type() == fuku_mem_opernad_type::FUKU_MEM_OPERAND_DISP_ONLY) {
        emit_optional_rex_32(ctx, dst);
        emit_b(ctx, 0xA3);
        emit_dw(ctx, src.get_disp().get_immediate32());
        ctx.displacment_offset = ctx.length;//todo
    }
    else {
        gen_pattern32_1em_op_r(0x8B, src, dst)
    }
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _mov_dw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_1em_rm_r(0x89, dst, src)
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _mov_dw(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_immediate& src) {
    gencleanerdata
    gen_pattern32_1em_op_idx_immdw(0xC7, dst, 0, src)
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _mov_dw(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata
    if (is_used_short_eax() && src.get_reg() == FUKU_REG_EAX &&
        dst.get_type() == fuku_mem_opernad_type::FUKU_MEM_OPERAND_DISP_ONLY) {
        emit_optional_rex_32(ctx, dst, src);
        emit_b(ctx, 0xA3);
        emit_dw(ctx, dst.get_disp().get_immediate32());
        ctx.displacment_offset = ctx.length;//todo
    }
    else {
        gen_pattern32_1em_op_r(0x89, dst, src)
    }
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _mov_qw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_immediate& src) {
    gencleanerdata
    emit_rex_64(ctx, dst);
    emit_b(ctx, 0xB8 | dst.get_index());
    emit_immediate_qw(ctx, src);
    gen_func_return(X86_INS_MOVABS, 0)
}
fuku_asm_ret_type _mov_qw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_operand& src) {
    gencleanerdata
    if (is_used_short_eax() && dst.get_reg() == FUKU_REG_RAX &&
        src.get_type() == fuku_mem_opernad_type::FUKU_MEM_OPERAND_DISP_ONLY) {
        emit_rex_64(ctx, dst);
        emit_b(ctx, 0xA3);
        emit_dw(ctx, src.get_disp().get_immediate32());
        ctx.displacment_offset = ctx.length;//todo
    }
    else {
        gen_pattern64_1em_op_r(0x8B, src, dst)
    }
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _mov_qw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern64_1em_rm_r(0x89, dst, src)
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _mov_qw(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_immediate& src) {
    gencleanerdata
    gen_pattern64_1em_op_idx_immdw(0xC7, dst, 0, src)
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _mov_qw(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata   
    if (is_used_short_eax() && src.get_reg() == FUKU_REG_RAX &&
        dst.get_type() == fuku_mem_opernad_type::FUKU_MEM_OPERAND_DISP_ONLY) {
        emit_rex_64(ctx, dst, src);
        emit_b(ctx, 0xA3);
        emit_dw(ctx, dst.get_disp().get_immediate32());
        ctx.displacment_offset = ctx.length;//todo
    }
    else {
        gen_pattern64_1em_op_r(0x89, dst, src)
    }
    gen_func_return(X86_INS_MOV, 0)
}
fuku_asm_ret_type _cmovcc_w(fuku_assambler_ctx& ctx, fuku_condition cond, const fuku_register& dst, const fuku_operand& src) {
    gencleanerdata
    gen_pattern32_2em_op_r_word(0x0F, 0x40 | cond, src, dst)
    gen_func_return(fuku_to_capstone_cc(cond, CONVERT_TYPE_CMOVCC), di_fl_jcc[cond])
}
fuku_asm_ret_type _cmovcc_w(fuku_assambler_ctx& ctx, fuku_condition cond, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_2em_rm_r_word(0x0F, 0x40 | cond, src, dst)
    gen_func_return(fuku_to_capstone_cc(cond, CONVERT_TYPE_CMOVCC), di_fl_jcc[cond])
}
fuku_asm_ret_type _cmovcc_dw(fuku_assambler_ctx& ctx, fuku_condition cond, const fuku_register& dst, const fuku_operand& src) {
    gencleanerdata
    gen_pattern32_2em_op_r(0x0F, 0x40 | cond, src, dst)
    gen_func_return(fuku_to_capstone_cc(cond, CONVERT_TYPE_CMOVCC), di_fl_jcc[cond])
}
fuku_asm_ret_type _cmovcc_dw(fuku_assambler_ctx& ctx, fuku_condition cond, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_2em_rm_r(0x0F, 0x40 | cond, src, dst)
    gen_func_return(fuku_to_capstone_cc(cond, CONVERT_TYPE_CMOVCC), di_fl_jcc[cond])
}
fuku_asm_ret_type _cmovcc_qw(fuku_assambler_ctx& ctx, fuku_condition cond, const fuku_register& dst, const fuku_operand& src) {
    gencleanerdata
    gen_pattern64_2em_op_r(0x0F, 0x40 | cond, src, dst)
    gen_func_return(fuku_to_capstone_cc(cond, CONVERT_TYPE_CMOVCC), di_fl_jcc[cond])
}
fuku_asm_ret_type _cmovcc_qw(fuku_assambler_ctx& ctx, fuku_condition cond, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern64_2em_rm_r(0x0F, 0x40 | cond, src, dst)
    gen_func_return(fuku_to_capstone_cc(cond, CONVERT_TYPE_CMOVCC), di_fl_jcc[cond])
}
fuku_asm_ret_type _xchg_b(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_1em_op_r(0x86, dst, src)
    gen_func_return(X86_INS_XCHG, 0)
}
fuku_asm_ret_type _xchg_b(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_1em_rm_r(0x86, dst, src)
    gen_func_return(X86_INS_XCHG, 0)
}
fuku_asm_ret_type _xchg_w(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_1em_op_r_word(0x87, dst, src)
    gen_func_return(X86_INS_XCHG, 0)
}
fuku_asm_ret_type _xchg_w(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_1em_rm_r_word(0x87, dst, src)
    gen_func_return(X86_INS_XCHG, 0)
}
fuku_asm_ret_type _xchg_dw(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_1em_op_r(0x87, dst, src)
    gen_func_return(X86_INS_XCHG, 0)
}
fuku_asm_ret_type _xchg_dw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_1em_rm_r(0x87, dst, src)
    gen_func_return(X86_INS_XCHG, 0)
}
fuku_asm_ret_type _xchg_qw(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern64_1em_op_r(0x87, dst, src)
    gen_func_return(X86_INS_XCHG, 0)
}
fuku_asm_ret_type _xchg_qw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern64_1em_rm_r(0x87, dst, src)
    gen_func_return(X86_INS_XCHG, 0)
}
fuku_asm_ret_type _bswap_w(fuku_assambler_ctx& ctx, const fuku_register& dst) {
    gencleanerdata
    gen_pattern32_1em_rm_idx_word(0x0F, dst, 1)
    gen_func_return(X86_INS_BSWAP, 0)
}
fuku_asm_ret_type _bswap_dw(fuku_assambler_ctx& ctx, const fuku_register& dst) {
    gencleanerdata
    gen_pattern32_1em_rm_idx(0x0F, dst, 1)
    gen_func_return(X86_INS_BSWAP, 0)
}
fuku_asm_ret_type _bswap_qw(fuku_assambler_ctx& ctx, const fuku_register& dst) {
    gencleanerdata
    gen_pattern64_1em_rm_idx(0x0F, dst, 1)
    gen_func_return(X86_INS_BSWAP, 0)
}
fuku_asm_ret_type _xadd_b(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_2em_op_r(0x0F, 0xC0, dst, src)
    gen_func_return(X86_INS_XADD, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _xadd_b(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_2em_rm_r(0x0F, 0xC0, dst, src)
    gen_func_return(X86_INS_XADD, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _xadd_w(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_2em_op_r_word(0x0F, 0xC1, dst, src)
    gen_func_return(X86_INS_XADD, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _xadd_w(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_2em_rm_r_word(0x0F, 0xC1, dst, src)
    gen_func_return(X86_INS_XADD, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _xadd_dw(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_2em_op_r(0x0F, 0xC1, dst, src)
    gen_func_return(X86_INS_XADD, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _xadd_dw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_2em_rm_r(0x0F, 0xC1, dst, src)
    gen_func_return(X86_INS_XADD, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _xadd_qw(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern64_2em_op_r(0x0F, 0xC1, dst, src)
    gen_func_return(X86_INS_XADD, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _xadd_qw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern64_2em_rm_r(0x0F, 0xC1, dst, src)
    gen_func_return(X86_INS_XADD, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _cmpxchg_b(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_2em_op_r(0x0F, 0xB0, dst, src)
    gen_func_return(X86_INS_CMPXCHG, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _cmpxchg_b(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_2em_rm_r(0x0F, 0xB0, dst, src)
    gen_func_return(X86_INS_CMPXCHG, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _cmpxchg_w(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_2em_op_r_word(0x0F, 0xB1, dst, src)
    gen_func_return(X86_INS_CMPXCHG, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _cmpxchg_w(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_2em_rm_r_word(0x0F, 0xB1, dst, src)
    gen_func_return(X86_INS_CMPXCHG, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _cmpxchg_dw(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_2em_op_r(0x0F, 0xB1, dst, src)
    gen_func_return(X86_INS_CMPXCHG, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _cmpxchg_dw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_2em_rm_r(0x0F, 0xB1, dst, src)
    gen_func_return(X86_INS_CMPXCHG, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _cmpxchg_qw(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern64_2em_op_r(0x0F, 0xB1, dst, src)
    gen_func_return(X86_INS_CMPXCHG, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _cmpxchg_qw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern64_2em_rm_r(0x0F, 0xB1, dst, src)
    gen_func_return(X86_INS_CMPXCHG, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _cmpxchg8b(fuku_assambler_ctx& ctx, const fuku_operand& dst) {
    gencleanerdata
    gen_pattern32_2em_op_idx(0x0F, 0xC7, dst, 1)
    gen_func_return(X86_INS_CMPXCHG8B, X86_EFLAGS_MODIFY_ZF)
}
fuku_asm_ret_type _cmpxchg16b(fuku_assambler_ctx& ctx, const fuku_operand& dst) {
    gencleanerdata
    gen_pattern64_2em_op_idx(0x0F, 0xC7, dst, 1)
    gen_func_return(X86_INS_CMPXCHG16B, X86_EFLAGS_MODIFY_ZF)
}
fuku_asm_ret_type _push_w(fuku_assambler_ctx& ctx, const fuku_immediate& imm) {
    gencleanerdata
    emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);
    if (is_used_short_imm() && imm.is_8()) {
        emit_b(ctx, 0x6A);
        emit_immediate_b(ctx, imm);
    }
    else {
        emit_b(ctx, 0x68);
        emit_immediate_w(ctx, imm);
    }
    gen_func_return(X86_INS_PUSH, 0)
}

fuku_asm_ret_type _push_dw(fuku_assambler_ctx& ctx, const fuku_immediate& imm) {
    gencleanerdata
        if (is_used_short_imm() && imm.is_8()) {
            gen_pattern32_1em_immb(0x6A, fuku_register(FUKU_REG_NONE), imm)
        }
        else {
            gen_pattern32_1em_immdw(0x68, fuku_register(FUKU_REG_NONE), imm)
        }
    gen_func_return(X86_INS_PUSH, 0)
}
fuku_asm_ret_type _push_qw(fuku_assambler_ctx& ctx, const fuku_immediate& imm) {
    return _push_dw(ctx, imm);
}
fuku_asm_ret_type _push_w(fuku_assambler_ctx& ctx, const fuku_register& src) {
    gencleanerdata
    emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);
    emit_optional_rex_32(ctx, src);
    emit_b(ctx, 0x50 | src.get_index());
    gen_func_return(X86_INS_PUSH, 0)
}
fuku_asm_ret_type _push_dw(fuku_assambler_ctx& ctx, const fuku_register& src) {
    gencleanerdata
    emit_optional_rex_32(ctx, src);
    emit_b(ctx, 0x50 | src.get_index());
    gen_func_return(X86_INS_PUSH, 0)
}
fuku_asm_ret_type _push_qw(fuku_assambler_ctx& ctx, const fuku_register& src) {
    return _push_dw(ctx, src);
}
fuku_asm_ret_type _push_w(fuku_assambler_ctx& ctx, const fuku_operand& src) {
    gencleanerdata
    gen_pattern32_1em_op_idx_word(0xFF, src, fuku_internal_assambler_push);
    gen_func_return(X86_INS_PUSH, 0)
}
fuku_asm_ret_type _push_dw(fuku_assambler_ctx& ctx, const fuku_operand& src) {
    gencleanerdata
    gen_pattern32_1em_op_idx(0xFF, src, fuku_internal_assambler_push);
    gen_func_return(X86_INS_PUSH, 0)
}
fuku_asm_ret_type _push_qw(fuku_assambler_ctx& ctx, const fuku_operand& src) {
    return _push_dw(ctx, src);
}
fuku_asm_ret_type _pop_w(fuku_assambler_ctx& ctx, const fuku_register& dst) {
    gencleanerdata
    emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);
    emit_optional_rex_32(ctx, dst);
    emit_b(ctx, 0x58 | dst.get_index());
    gen_func_return(X86_INS_POP, 0)
}
fuku_asm_ret_type _pop_dw(fuku_assambler_ctx& ctx, const fuku_register& dst) {
    gencleanerdata
    emit_optional_rex_32(ctx, dst);
    emit_b(ctx, 0x58 | dst.get_index());
    gen_func_return(X86_INS_POP, 0)
}
fuku_asm_ret_type _pop_qw(fuku_assambler_ctx& ctx, const fuku_register& dst) {
    return _pop_dw(ctx, dst);
}
fuku_asm_ret_type _pop_w(fuku_assambler_ctx& ctx, const fuku_operand& dst) {
    gencleanerdata
    gen_pattern32_1em_op_idx_word(0x8F, dst, fuku_internal_assambler_pop);
    gen_func_return(X86_INS_POP, 0)
}
fuku_asm_ret_type _pop_dw(fuku_assambler_ctx& ctx, const fuku_operand& dst) {
    gencleanerdata
    gen_pattern32_1em_op_idx(0x8F, dst, fuku_internal_assambler_pop);
    gen_func_return(X86_INS_POP, 0)
}
fuku_asm_ret_type _pop_qw(fuku_assambler_ctx& ctx, const fuku_operand& dst) {
    return _pop_dw(ctx, dst);
}
gen_func_body_twobyte_no_arg(cwd,  FUKU_PREFIX_OVERRIDE_DATA,0x99, X86_INS_CWD, 0)
gen_func_body_onebyte_no_arg(cdq,  0x99, X86_INS_CDQ, 0)
gen_func_body_twobyte_no_arg(cqo,  0x48, 0x99, X86_INS_CQO, 0)

gen_func_body_twobyte_no_arg(cbw, FUKU_PREFIX_OVERRIDE_DATA, 0x98, X86_INS_CBW, 0)
gen_func_body_onebyte_no_arg(cwde, 0x98, X86_INS_CWDE, 0)
gen_func_body_twobyte_no_arg(cdqe, 0x48, 0x98, X86_INS_CDQE, 0)

gen_func_body_movxx(movzx, 0xB6, X86_INS_MOVZX)
gen_func_body_movxx(movsx, 0xBE, X86_INS_MOVSX)
fuku_asm_ret_type _movsx_dword_dw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_1em_rm_r(0x63, src, dst);
    gen_func_return(X86_INS_MOVSXD, 0)
}
fuku_asm_ret_type _movsx_dword_dw(fuku_assambler_ctx& ctx, const fuku_register& src, const fuku_operand& dst) {
    gencleanerdata
    gen_pattern32_1em_op_r(0x63, dst, src);
    gen_func_return(X86_INS_MOVSXD, 0)
}
fuku_asm_ret_type _movsx_dword_qw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern64_1em_rm_r(0x63, src, dst);
    gen_func_return(X86_INS_MOVSXD, 0)
}
fuku_asm_ret_type _movsx_dword_qw(fuku_assambler_ctx& ctx, const fuku_register& src, const fuku_operand& dst) {
    gencleanerdata
    gen_pattern64_1em_op_r(0x63, dst, src);
    gen_func_return(X86_INS_MOVSXD, 0)
}
//Binary Arithmetic Instructions
fuku_asm_ret_type _adcx_dw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);
    emit_optional_rex_32(ctx, dst, src);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0x38);
    emit_b(ctx, 0xF6);
    emit_modrm(ctx, src, dst);
    gen_func_return(X86_INS_ADCX, X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _adcx_dw(fuku_assambler_ctx& ctx, const fuku_register& src, const fuku_operand& dst) {
    gencleanerdata
    emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);
    emit_optional_rex_32(ctx, dst, src);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0x38);
    emit_b(ctx, 0xF6);
    emit_operand(ctx, dst, src);
    gen_func_return(X86_INS_ADCX, X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _adcx_qw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);
    emit_rex_64(ctx, dst, src);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0x38);
    emit_b(ctx, 0xF6);
    emit_modrm(ctx, src, dst);
    gen_func_return(X86_INS_ADCX, X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _adcx_qw(fuku_assambler_ctx& ctx, const fuku_register& src, const fuku_operand& dst) {
    gencleanerdata
    emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);
    emit_rex_64(ctx, dst, src);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0x38);
    emit_b(ctx, 0xF6);
    emit_operand(ctx, dst, src);
    gen_func_return(X86_INS_ADCX, X86_EFLAGS_MODIFY_CF)
}
fuku_asm_ret_type _adox_dw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    emit_b(ctx, 0xF3);
    emit_optional_rex_32(ctx, dst, src);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0x38);
    emit_b(ctx, 0xF6);
    emit_modrm(ctx, src, dst);
    gen_func_return(X86_INS_ADOX, X86_EFLAGS_MODIFY_OF)
}
fuku_asm_ret_type _adox_dw(fuku_assambler_ctx& ctx, const fuku_register& src, const fuku_operand& dst) {
    gencleanerdata
    emit_b(ctx, 0xF3);
    emit_optional_rex_32(ctx, dst, src);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0x38);
    emit_b(ctx, 0xF6);
    emit_operand(ctx, dst, src);
    gen_func_return(X86_INS_ADOX, X86_EFLAGS_MODIFY_OF)
}
fuku_asm_ret_type _adox_qw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    emit_b(ctx, 0xF3);
    emit_rex_64(ctx, dst, src);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0x38);
    emit_b(ctx, 0xF6);
    emit_modrm(ctx, src, dst);
    gen_func_return(X86_INS_ADOX, X86_EFLAGS_MODIFY_OF)
}
fuku_asm_ret_type _adox_qw(fuku_assambler_ctx& ctx, const fuku_register& src, const fuku_operand& dst) {
    gencleanerdata
    emit_b(ctx, 0xF3);
    emit_rex_64(ctx, dst, src);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0x38);
    emit_b(ctx, 0xF6);
    emit_operand(ctx, dst, src);
    gen_func_return(X86_INS_ADOX, X86_EFLAGS_MODIFY_OF)
}
gen_func_body_arith(add, fuku_internal_assambler_arith_add, X86_INS_ADD, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF    | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_arith(adc, fuku_internal_assambler_arith_adc, X86_INS_ADC, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF    | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_arith(sub, fuku_internal_assambler_arith_sub, X86_INS_SUB, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF    | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_arith(sbb, fuku_internal_assambler_arith_sbb, X86_INS_SBB, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF    | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_arith_ex_one_op(imul, fuku_internal_assambler_arith_ex_imul, X86_INS_IMUL, X86_EFLAGS_MODIFY_SF    | X86_EFLAGS_MODIFY_CF    | X86_EFLAGS_MODIFY_OF    | X86_EFLAGS_UNDEFINED_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF)
gen_func_body_arith_ex_one_op(mul,  fuku_internal_assambler_arith_ex_mul,  X86_INS_MUL,  X86_EFLAGS_MODIFY_OF    | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_arith_ex_one_op(idiv, fuku_internal_assambler_arith_ex_idiv, X86_INS_IDIV, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_UNDEFINED_CF)
gen_func_body_arith_ex_one_op(div,  fuku_internal_assambler_arith_ex_div,  X86_INS_DIV,  X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_UNDEFINED_CF)
gen_func_body_arith_incdec(inc, fuku_internal_assambler_arith_inc, X86_INS_INC, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF)
gen_func_body_arith_incdec(dec, fuku_internal_assambler_arith_dec, X86_INS_DEC, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF)
gen_func_body_arith_ex_one_op(neg,  fuku_internal_assambler_arith_ex_neg,  X86_INS_NEG,  X86_EFLAGS_MODIFY_OF    | X86_EFLAGS_MODIFY_SF    | X86_EFLAGS_MODIFY_ZF    | X86_EFLAGS_MODIFY_AF    | X86_EFLAGS_MODIFY_PF    | X86_EFLAGS_MODIFY_CF)
gen_func_body_arith(cmp, fuku_internal_assambler_arith_cmp, X86_INS_CMP, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF    | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
//Decimal Arithmetic Instructions
gen_func_body_onebyte_no_arg(daa, 0x27, X86_INS_DAA, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_onebyte_no_arg(das, 0x2F, X86_INS_DAS, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_onebyte_no_arg(aaa, 0x37, X86_INS_AAA, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_onebyte_no_arg(aas, 0x3F, X86_INS_AAS, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_MODIFY_CF)
fuku_asm_ret_type _aam(fuku_assambler_ctx& ctx, const fuku_immediate& imm) {
    gencleanerdata
    gen_pattern32_1em_immb(0xD4, fuku_register(FUKU_REG_NONE), imm);
    gen_func_return(X86_INS_AAM, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_UNDEFINED_CF)
}
fuku_asm_ret_type _aad(fuku_assambler_ctx& ctx, const fuku_immediate& imm) {
    gencleanerdata
    gen_pattern32_1em_immb(0xD5, fuku_register(FUKU_REG_NONE), imm);
    gen_func_return(X86_INS_AAD, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_UNDEFINED_CF)
}
//Logical Instructions Instructions
gen_func_body_arith(and, fuku_internal_assambler_arith_and, X86_INS_AND, X86_EFLAGS_RESET_OF  | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
gen_func_body_arith(or , fuku_internal_assambler_arith_or,  X86_INS_OR,  X86_EFLAGS_RESET_OF  | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
gen_func_body_arith(xor, fuku_internal_assambler_arith_xor, X86_INS_XOR, X86_EFLAGS_RESET_OF  | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
gen_func_body_arith_ex_one_op(not,  fuku_internal_assambler_arith_ex_not,  X86_INS_NOT,  0)
//Shift and Rotate Instructions
gen_func_body_shift(sar, fuku_internal_assambler_shift_sar, X86_INS_SAR, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_shift(shr, fuku_internal_assambler_shift_shr, X86_INS_SHR, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_shift(shl, fuku_internal_assambler_shift_shl, X86_INS_SHL, X86_EFLAGS_MODIFY_OF    | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
//SHRD
//SHLD
gen_func_body_shift(ror, fuku_internal_assambler_shift_ror, X86_INS_ROR, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_MODIFY_CF)
gen_func_body_shift(rol, fuku_internal_assambler_shift_rol, X86_INS_ROL, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_MODIFY_CF)
gen_func_body_shift(rcr, fuku_internal_assambler_shift_rcr, X86_INS_RCR, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_MODIFY_CF)
gen_func_body_shift(rcl, fuku_internal_assambler_shift_rcl, X86_INS_RCL, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_MODIFY_CF)
//Bit and Byte Instructions
gen_func_body_bit(bt , fuku_internal_assambler_bittest_bt,  X86_INS_BT,  X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_bit(bts, fuku_internal_assambler_bittest_bts, X86_INS_BTS, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_bit(btr, fuku_internal_assambler_bittest_btr, X86_INS_BTR, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_bit(btc, fuku_internal_assambler_bittest_btc, X86_INS_BTC, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_bit_ex(bsf, fuku_internal_assambler_bittest_bsf, X86_INS_BSF, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_UNDEFINED_CF)
gen_func_body_bit_ex(bsr, fuku_internal_assambler_bittest_bsr, X86_INS_BSR, X86_EFLAGS_UNDEFINED_OF | X86_EFLAGS_UNDEFINED_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_UNDEFINED_PF | X86_EFLAGS_UNDEFINED_CF)

fuku_asm_ret_type _setcc(fuku_assambler_ctx& ctx, fuku_condition cond, const fuku_register& dst) {
    gencleanerdata
    FUKU_ASSERT(cond >= 0 && cond < fuku_condition::FUKU_CONDITION_MAX);
    gen_pattern32_2em_rm_idx(0x0F, 0x90 | cond, dst, 0);
    gen_func_return(fuku_to_capstone_cc(cond, CONVERT_TYPE_SETCC), di_fl_jcc[cond])
}
fuku_asm_ret_type _setcc(fuku_assambler_ctx& ctx, fuku_condition cond, const fuku_operand& dst) {
    gencleanerdata
    FUKU_ASSERT(cond >= 0 && cond < fuku_condition::FUKU_CONDITION_MAX);
    gen_pattern32_2em_op_idx(0x0F, 0x90 | cond, dst, 0);
    gen_func_return(fuku_to_capstone_cc(cond, CONVERT_TYPE_SETCC), di_fl_jcc[cond])
}
fuku_asm_ret_type _test_b(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_1em_rm_r(0x84, dst, src)
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _test_b(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_immediate& src) {
    gencleanerdata
    if (is_used_short_eax() && dst.get_reg() == FUKU_REG_AL) {
        gen_pattern32_1em_immb(0xA8, dst, src)
    }
    else {
        gen_pattern32_1em_rm_idx_immb(0xF6, dst, 0, src)
    }
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _test_b(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_1em_op_r(0x84, dst, src)
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _test_b(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_immediate& src) {
    gencleanerdata
    gen_pattern32_1em_op_idx_immb(0xF6, dst, 0, src)
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _test_w(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_1em_rm_r_word(0x85, dst, src)
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _test_w(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_immediate& src) {
    gencleanerdata    
    if (is_used_short_eax() && dst.get_reg() == FUKU_REG_AX) {
        gen_pattern32_1em_immw_word(0xA9, dst, src)
    }
    else {
        gen_pattern32_1em_rm_idx_immw_word(0xF7, dst, 0, src)
    }   
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _test_w(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_1em_op_r_word(0x85, dst, src)
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _test_w(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_immediate& src) {
    gencleanerdata
    gen_pattern32_1em_op_idx_immw_word(0xF7, dst, 0, src)
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _test_dw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_1em_rm_r(0x85, dst, src)
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _test_dw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_immediate& src) {
    gencleanerdata   
    if (is_used_short_eax() && dst.get_reg() == FUKU_REG_EAX) {
        gen_pattern32_1em_immdw(0xA9, dst, src)
    }
    else {
        gen_pattern32_1em_rm_idx_immdw(0xF7, dst, 0, src)
    } 
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _test_dw(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern32_1em_op_r(0x85, dst, src)
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _test_dw(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_immediate& src) {
    gencleanerdata
    gen_pattern32_1em_op_idx_immdw(0xF7, dst, 0, src)
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _test_qw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern64_1em_rm_r(0x85, dst, src);
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _test_qw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_immediate& src) {
    gencleanerdata
    if (is_used_short_eax() && dst.get_reg() == FUKU_REG_RAX) {
        emit_rex_64(ctx, dst);
        gen_pattern64_1em_immdw(0xA9, dst, src)
    }
    else {
        gen_pattern64_1em_rm_idx_immdw(0xF7, dst, 0, src)
    }
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _test_qw(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_register& src) {
    gencleanerdata
    gen_pattern64_1em_op_r(0x85, dst, src)
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _test_qw(fuku_assambler_ctx& ctx, const fuku_operand& dst, const fuku_immediate& src) {
    gencleanerdata
    gen_pattern64_1em_op_idx_immdw(0xF7, dst, 0, src)
    gen_func_return(X86_INS_TEST, X86_EFLAGS_RESET_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_UNDEFINED_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _popcnt_w(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);
    emit_b(ctx, 0xF3);
    emit_optional_rex_32(ctx, dst, src);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0xB8);
    emit_modrm(ctx, src, dst);
    gen_func_return(X86_INS_POPCNT, X86_EFLAGS_RESET_OF | X86_EFLAGS_RESET_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_RESET_AF | X86_EFLAGS_RESET_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _popcnt_w(fuku_assambler_ctx& ctx, const fuku_register& src, const fuku_operand& dst) {
    gencleanerdata
    emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);
    emit_b(ctx, 0xF3);
    emit_optional_rex_32(ctx, dst, src);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0xB8);
    emit_operand(ctx, dst, src);
    gen_func_return(X86_INS_POPCNT, X86_EFLAGS_RESET_OF | X86_EFLAGS_RESET_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_RESET_AF | X86_EFLAGS_RESET_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _popcnt_dw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    emit_b(ctx, 0xF3);
    emit_optional_rex_32(ctx, dst, src);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0xB8);
    emit_modrm(ctx, src, dst);
    gen_func_return(X86_INS_POPCNT, X86_EFLAGS_RESET_OF | X86_EFLAGS_RESET_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_RESET_AF | X86_EFLAGS_RESET_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _popcnt_dw(fuku_assambler_ctx& ctx, const fuku_register& src, const fuku_operand& dst) {
    gencleanerdata
    emit_b(ctx, 0xF3);
    emit_optional_rex_32(ctx, dst, src);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0xB8);
    emit_operand(ctx, dst, src);
    gen_func_return(X86_INS_POPCNT, X86_EFLAGS_RESET_OF | X86_EFLAGS_RESET_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_RESET_AF | X86_EFLAGS_RESET_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _popcnt_qw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_register& src) {
    gencleanerdata
    emit_b(ctx, 0xF3);
    emit_rex_64(ctx, dst, src);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0xB8);
    emit_modrm(ctx, src, dst);
    gen_func_return(X86_INS_POPCNT, X86_EFLAGS_RESET_OF | X86_EFLAGS_RESET_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_RESET_AF | X86_EFLAGS_RESET_PF | X86_EFLAGS_RESET_CF)
}
fuku_asm_ret_type _popcnt_qw(fuku_assambler_ctx& ctx, const fuku_register& src, const fuku_operand& dst) {
    gencleanerdata
    emit_b(ctx, 0xF3);
    emit_rex_64(ctx, dst, src);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0xB8);
    emit_operand(ctx, dst, src);
    gen_func_return(X86_INS_POPCNT, X86_EFLAGS_RESET_OF | X86_EFLAGS_RESET_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_RESET_AF | X86_EFLAGS_RESET_PF | X86_EFLAGS_RESET_CF)
}
//Control Transfer Instructions
gen_func_body_ff_offset(jmp,  0xE9, X86_INS_JMP,  0)
gen_func_body_ff_r(     jmp,  4   , X86_INS_JMP,  0)
gen_func_body_ff_op(    jmp,  4   , X86_INS_JMP,  0) 
fuku_asm_ret_type _jcc(fuku_assambler_ctx& ctx, fuku_condition cond, const fuku_immediate& imm) {
    gencleanerdata
    FUKU_ASSERT(cond >= 0 && cond < fuku_condition::FUKU_CONDITION_MAX);
    gen_pattern32_2em_immdw(0x0F, 0x80 | cond, fuku_register(FUKU_REG_NONE), imm)
    gen_func_return(fuku_to_capstone_cc(cond, CONVERT_TYPE_JCC), di_fl_jcc[cond])
}
gen_func_body_ff_offset(call, 0xE8, X86_INS_CALL, 0)
gen_func_body_ff_r(     call, 2   , X86_INS_CALL, 0)
gen_func_body_ff_op(    call, 2   , X86_INS_CALL, 0)
gen_func_body_onebyte_no_arg(ret, 0xC3, X86_INS_RET, 0)
fuku_asm_ret_type _ret(fuku_assambler_ctx& ctx, const fuku_immediate& imm) {
    gencleanerdata
    gen_pattern32_1em_immw(0xC2, fuku_register(FUKU_REG_NONE), imm)
    gen_func_return(X86_INS_RET, 0)
}
gen_func_body_onebyte_no_arg(int3, 0xCC, X86_INS_INT3, X86_EFLAGS_MODIFY_IF | X86_EFLAGS_MODIFY_TF | X86_EFLAGS_MODIFY_NT | X86_EFLAGS_MODIFY_RF)
fuku_asm_ret_type _enter(fuku_assambler_ctx& ctx, const fuku_immediate& size, uint8_t nestinglevel) {
    gencleanerdata
    gen_pattern32_1em_immw(0xC8, fuku_register(FUKU_REG_NONE), size)
    emit_b(ctx, nestinglevel);
    gen_func_return(X86_INS_ENTER, 0)
}
gen_func_body_onebyte_no_arg(leave_, 0xC9, X86_INS_LEAVE, 0)
//String Instructions
gen_func_body_string_inst_withoutq(outs, fuku_internal_assambler_string_out, X86_INS_OUTS, X86_EFLAGS_TEST_DF)
gen_func_body_string_inst(movs, fuku_internal_assambler_string_mov, X86_INS_MOVS, X86_EFLAGS_TEST_DF)
gen_func_body_string_inst(cmps, fuku_internal_assambler_string_cmp, X86_INS_CMPS, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
gen_func_body_string_inst(stos, fuku_internal_assambler_string_sto, X86_INS_STOS, X86_EFLAGS_TEST_DF)
gen_func_body_string_inst(lods, fuku_internal_assambler_string_lod, X86_INS_LODS, X86_EFLAGS_TEST_DF)
gen_func_body_string_inst(scas, fuku_internal_assambler_string_sca, X86_INS_SCAS, X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_CF)
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
gen_func_body_twobyte_no_arg(popf,   FUKU_PREFIX_OVERRIDE_DATA, 0x9D, X86_INS_POPF, X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_CF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_TF | X86_EFLAGS_MODIFY_IF | X86_EFLAGS_MODIFY_DF | X86_EFLAGS_MODIFY_NT | X86_EFLAGS_MODIFY_RF)
gen_func_body_onebyte_no_arg(popfd,  0x9D, X86_INS_POPFD, X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_CF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_TF | X86_EFLAGS_MODIFY_IF | X86_EFLAGS_MODIFY_DF | X86_EFLAGS_MODIFY_NT | X86_EFLAGS_MODIFY_RF)
gen_func_body_onebyte_no_arg(popfq,  0x9D, X86_INS_POPFQ, X86_EFLAGS_MODIFY_AF | X86_EFLAGS_MODIFY_CF | X86_EFLAGS_MODIFY_SF | X86_EFLAGS_MODIFY_ZF | X86_EFLAGS_MODIFY_PF | X86_EFLAGS_MODIFY_OF | X86_EFLAGS_MODIFY_TF | X86_EFLAGS_MODIFY_IF | X86_EFLAGS_MODIFY_DF | X86_EFLAGS_MODIFY_NT | X86_EFLAGS_MODIFY_RF)
//Miscellaneous Instructions
fuku_asm_ret_type _lea_w(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_operand& src) {
    gencleanerdata
    gen_pattern32_1em_op_r_word(0x8D, src, dst)
    gen_func_return(X86_INS_LEA, 0)
}
fuku_asm_ret_type _lea_dw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_operand& src) {
    gencleanerdata
    gen_pattern32_1em_op_r(0x8D, src, dst)
    gen_func_return(X86_INS_LEA, 0)
}
fuku_asm_ret_type _lea_qw(fuku_assambler_ctx& ctx, const fuku_register& dst, const fuku_operand& src) {
    gencleanerdata
    gen_pattern64_1em_op_r(0x8D, src, dst)
    gen_func_return(X86_INS_LEA, 0)
}
gen_func_body_twobyte_no_arg(ud2, 0x0F, 0x0B, X86_INS_UD2, 0)
gen_func_body_twobyte_no_arg(cpuid, 0x0F, 0xA2, X86_INS_CPUID, 0)
//Random Number Generator Instructions
fuku_asm_ret_type _rdrand_w(fuku_assambler_ctx& ctx, const fuku_register& dst) {
    gencleanerdata
    emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);
    emit_optional_rex_32(ctx, dst);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0xC7);
    emit_b(ctx, 0xF0 | dst.get_index());
    gen_func_return(X86_INS_RDRAND, X86_EFLAGS_MODIFY_CF | X86_EFLAGS_RESET_OF | X86_EFLAGS_RESET_SF | X86_EFLAGS_RESET_ZF | X86_EFLAGS_RESET_AF | X86_EFLAGS_RESET_PF)
}
fuku_asm_ret_type _rdrand_dw(fuku_assambler_ctx& ctx, const fuku_register& dst) {
    gencleanerdata
    emit_optional_rex_32(ctx, dst);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0xC7);
    emit_b(ctx, 0xF0 | dst.get_index());
    gen_func_return(X86_INS_RDRAND, X86_EFLAGS_MODIFY_CF | X86_EFLAGS_RESET_OF | X86_EFLAGS_RESET_SF | X86_EFLAGS_RESET_ZF | X86_EFLAGS_RESET_AF | X86_EFLAGS_RESET_PF)
}
fuku_asm_ret_type _rdrand_qw(fuku_assambler_ctx& ctx, const fuku_register& dst) {
    gencleanerdata
    emit_rex_64(ctx, dst);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0xC7);
    emit_b(ctx, 0xF0 | dst.get_index());
    gen_func_return(X86_INS_RDRAND, X86_EFLAGS_MODIFY_CF | X86_EFLAGS_RESET_OF | X86_EFLAGS_RESET_SF | X86_EFLAGS_RESET_ZF | X86_EFLAGS_RESET_AF | X86_EFLAGS_RESET_PF)
}
fuku_asm_ret_type _rdseed_w(fuku_assambler_ctx& ctx, const fuku_register& dst) {
    gencleanerdata
    emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);
    emit_optional_rex_32(ctx, dst);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0xC7);
    emit_b(ctx, 0xF8 | dst.get_index());
    gen_func_return(X86_INS_RDSEED, X86_EFLAGS_MODIFY_CF | X86_EFLAGS_RESET_OF | X86_EFLAGS_RESET_SF | X86_EFLAGS_RESET_ZF | X86_EFLAGS_RESET_AF | X86_EFLAGS_RESET_PF)
}
fuku_asm_ret_type _rdseed_dw(fuku_assambler_ctx& ctx, const fuku_register& dst) {
    gencleanerdata
    emit_optional_rex_32(ctx, dst);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0xC7);
    emit_b(ctx, 0xF8 | dst.get_index());
    gen_func_return(X86_INS_RDSEED, X86_EFLAGS_MODIFY_CF | X86_EFLAGS_RESET_OF | X86_EFLAGS_RESET_SF | X86_EFLAGS_RESET_ZF | X86_EFLAGS_RESET_AF | X86_EFLAGS_RESET_PF)
}
fuku_asm_ret_type _rdseed_qw(fuku_assambler_ctx& ctx, const fuku_register& dst) {
    gencleanerdata
    emit_rex_64(ctx, dst);
    emit_b(ctx, 0x0F);
    emit_b(ctx, 0xC7);
    emit_b(ctx, 0xF8 | dst.get_index());
    gen_func_return(X86_INS_RDSEED, X86_EFLAGS_MODIFY_CF | X86_EFLAGS_RESET_OF | X86_EFLAGS_RESET_SF | X86_EFLAGS_RESET_ZF | X86_EFLAGS_RESET_AF | X86_EFLAGS_RESET_PF)
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





fuku_asm_ret_type _nop(fuku_assambler_ctx& ctx, uint8_t n) {

    clear_space(ctx);

    switch (n) {
    case 2:
        emit_b(ctx, 0x66);

    default:
    case 1:
        emit_b(ctx, 0x90);
        break;
    case 3:
        emit_b(ctx, 0x0F);
        emit_b(ctx, 0x1F);
        emit_b(ctx, 0x00);
        break;
    case 4:
        emit_b(ctx, 0x0F);
        emit_b(ctx, 0x1F);
        emit_b(ctx, 0x40);
        emit_b(ctx, 0x00);
        break;
    case 6:
        emit_b(ctx, 0x66);
    case 5:
        emit_b(ctx, 0x0F);
        emit_b(ctx, 0x1F);
        emit_b(ctx, 0x44);
        emit_b(ctx, 0x00);
        emit_b(ctx, 0x00);
        break;
    case 7:
        emit_b(ctx, 0x0F);
        emit_b(ctx, 0x1F);
        emit_b(ctx, 0x80);
        emit_b(ctx, 0x00);
        emit_b(ctx, 0x00);
        emit_b(ctx, 0x00);
        emit_b(ctx, 0x00);
        break;
    case 11:
        emit_b(ctx, 0x66);
    case 10:
        emit_b(ctx, 0x66);
    case 9:
        emit_b(ctx, 0x66);
    case 8:
        emit_b(ctx, 0x0F);
        emit_b(ctx, 0x1F);
        emit_b(ctx, 0x84);
        emit_b(ctx, 0x00);
        emit_b(ctx, 0x00);
        emit_b(ctx, 0x00);
        emit_b(ctx, 0x00);
        emit_b(ctx, 0x00);
        break;
    }

    gen_func_return(X86_INS_NOP, 0)
}
};