#include "stdafx.h"
#include "fuku_register_math.h"
#include "fuku_register_math_tables.h"



fuku_register_index get_index_by_register(fuku_register_enum reg) {

    if (reg <= FUKU_REG_NONE || reg >= FUKU_REG_MAX) {
        return FUKU_REG_INDEX_INVALID;
    }

    return ext_register_info[reg].register_index_;
}

fuku_register_enum get_register_by_index(fuku_register_index idx, bool x64ext, fuku_operand_size size) {

    if ((idx < FUKU_REG_INDEX_AX || idx > FUKU_REG_INDEX_DI) ||
        (size <= FUKU_OPERAND_SIZE_0 || size > FUKU_OPERAND_SIZE_64)) {
        return FUKU_REG_NONE;
    }

    switch (size) {

    case FUKU_OPERAND_SIZE_8:
        return registers8[4 + idx + (x64ext == true ? 8 : 0)];

    case FUKU_OPERAND_SIZE_16:
        return registers16[idx + (x64ext == true ? 8 : 0)];

    case FUKU_OPERAND_SIZE_32:
        return registers32[idx + (x64ext == true ? 8 : 0)];

    case FUKU_OPERAND_SIZE_64:
        return registers64[idx + (x64ext == true ? 8 : 0)];


    default:
        return FUKU_REG_NONE;
    }
}

fuku_operand_size get_register_size(fuku_register_enum reg) {

    if (reg <= FUKU_REG_NONE || reg >= FUKU_REG_MAX) {
        return FUKU_OPERAND_SIZE_0;
    }

    return ext_register_info[reg].register_size_;
}

bool is_x64arch_register(fuku_register_enum reg) {

    if (reg <= FUKU_REG_NONE || reg >= FUKU_REG_MAX) {
        return false;
    }

    return ext_register_info[reg].is_x64_arch;
}

bool is_x32arch_register(fuku_register_enum reg) {
    return !is_x64arch_register(reg);
}

uint8_t is_x64arch_ext_register(fuku_register_enum reg) {

    if (reg <= FUKU_REG_NONE || reg >= FUKU_REG_MAX) {
        return 0;
    }

    return ext_register_info[reg].is_x64_arch_ext;
}


x86_insn fuku_to_capstone_cc(fuku_condition cond, fuku_to_cap_convert_type type) {

    if (cond >= FUKU_CONDITION_MAX || cond < 0) {
        return X86_INS_INVALID;
    }
    if (type == fuku_to_cap_convert_type::CONVERT_TYPE_CMOVCC) {
        return capstone_movcc[cond];
    }
    else if (type == fuku_to_cap_convert_type::CONVERT_TYPE_SETCC) {
        return capstone_setcc[cond];
    }
    else {
        return capstone_jcc[cond];
    }

}


fuku_register_enum capstone_to_fuku_reg(x86_reg reg) {
    return cap_to_fuku_table[reg];
}

fuku_operand capstone_to_fuku_op(cs_x86& x86, uint8_t op_idx) {

    fuku_operand_size size = FUKU_OPERAND_SIZE_0;
    fuku_register_enum base = FUKU_REG_NONE;
    fuku_register_enum index = FUKU_REG_NONE;
    fuku_operand_scale scale = FUKU_OPERAND_SCALE_1;
    fuku_immediate imm = 0;

    auto& op = x86.operands[op_idx];

    if (op.type == X86_OP_MEM) {
        size = (fuku_operand_size)op.size;

        if (op.mem.base != X86_REG_INVALID) {
            base = cap_to_fuku_table[op.mem.base];
        }

        if (op.mem.index != X86_REG_INVALID) {
            index = cap_to_fuku_table[op.mem.index];
            scale = (fuku_operand_scale)op.mem.scale;
        }

        imm = op.mem.disp;
    }

    return fuku_operand(base, index, scale, imm, size);
}

fuku_condition capstone_to_fuku_cond(x86_insn cond) {


    switch (cond) {
    case  X86_INS_JO:  case X86_INS_SETO:   case X86_INS_CMOVO:  return fuku_condition::jo;
    case  X86_INS_JNO: case X86_INS_SETNO:  case X86_INS_CMOVNO: return fuku_condition::jno;
    case  X86_INS_JB:  case X86_INS_SETB:   case X86_INS_CMOVB:  return fuku_condition::jb;
    case  X86_INS_JAE: case X86_INS_SETAE:  case X86_INS_CMOVAE: return fuku_condition::jae;
    case  X86_INS_JE:  case X86_INS_SETE:   case X86_INS_CMOVE:  return fuku_condition::je;
    case  X86_INS_JNE: case X86_INS_SETNE:  case X86_INS_CMOVNE: return fuku_condition::jne;
    case  X86_INS_JBE: case X86_INS_SETBE:  case X86_INS_CMOVBE: return fuku_condition::jbe;
    case  X86_INS_JA:  case X86_INS_SETA:   case X86_INS_CMOVA:  return fuku_condition::ja;
    case  X86_INS_JS:  case X86_INS_SETS:   case X86_INS_CMOVS:  return fuku_condition::js;
    case  X86_INS_JNS: case X86_INS_SETNS:  case X86_INS_CMOVNS: return fuku_condition::jns;
    case  X86_INS_JP:  case X86_INS_SETP:   case X86_INS_CMOVP:  return fuku_condition::jp;
    case  X86_INS_JNP: case X86_INS_SETNP:  case X86_INS_CMOVNP: return fuku_condition::jnp;
    case  X86_INS_JL:  case X86_INS_SETL:   case X86_INS_CMOVL:  return fuku_condition::jl;
    case  X86_INS_JGE: case X86_INS_SETGE:  case X86_INS_CMOVGE: return fuku_condition::jge;
    case  X86_INS_JLE: case X86_INS_SETLE:  case X86_INS_CMOVLE: return fuku_condition::jle;
    case  X86_INS_JG:  case X86_INS_SETG:   case X86_INS_CMOVG:  return fuku_condition::jg;

    default: {
        return fuku_condition::jmp;
    }
    }
}


inline bool bit_scan_forward(uint32_t& index, uint64_t mask) {

    for (; index < 64; index++) {

        if (mask & ((uint64_t)1 << index)) {

            return true;
        }
    }

    return false;
}

inline bool bit_scan_backward(uint32_t& index, uint64_t mask) {

    for (; index != -1; index--) {

        if (mask & ((uint64_t)1 << index)) {
            return true;
        }
    }

    return false;
}

uint8_t get_random_bit_by_mask(uint64_t mask, uint8_t min_index, uint8_t max_index) {

    uint32_t index = min_index;

    uint32_t rand_idx = FUKU_GET_RAND(min_index, max_index);

    index = rand_idx;

    if (rand_idx == min_index) {

        bit_scan_forward(index, mask);

        return index;
    }
    else if (rand_idx == max_index) {

        bit_scan_backward(index, mask);

        return index;
    }
    else {

        if (!bit_scan_forward(index, mask) || index > max_index) {

            index = rand_idx;
            bit_scan_backward(index, mask);
        }
    }

    return index;
}



uint8_t get_flag_register_size(uint64_t reg) {

    uint32_t index = 0;

    if (!bit_scan_forward(index, reg)) {

        return 0;
    }

    return indexsz_to_size[((index) / 16) + 1];
}

uint8_t get_flag_register_index(uint64_t reg) {

    uint32_t index = 0;

    if (!bit_scan_forward(index, reg)) {

        return 0;
    }

    return (index) % 8;
}

uint8_t get_flag_register_ext64(uint64_t reg) {

    uint32_t index = 0;

    if (!bit_scan_forward(index, reg)) {

        return 0;
    }

    return (((index) / 16) > 7) ? 1 : 0;
}


uint64_t fuku_to_flag_register(fuku_register_enum reg) {
    return (uint64_t)1 << CONVERT_FUKU_REGISTER_TO_FLAG[reg];
}


fuku_register_enum flag_to_fuku_register(uint64_t reg) {

    uint32_t index = 0;

    if (bit_scan_forward(index, reg)) {
        return CONVERT_FLAG_REGISTER_TO_FUKU[index];
    }
    else {
        return FUKU_REG_NONE;
    }
}

fuku_register_enum set_fuku_register_grade(fuku_register_enum reg, uint8_t needed_size) {
    return flag_to_fuku_register(((uint64_t)1 << ((size_to_indexsz[needed_size] * 16) + (CONVERT_FUKU_REGISTER_TO_FLAG[reg] % 16))));
}


bool has_flag_free_register(uint64_t regs_flags, uint64_t reg) {
    return GET_BITES(regs_flags, reg) == reg;
}

bool has_inst_free_register(const fuku_inst& inst, x86_reg reg) {

    if (CONVERT_CAPSTONE_REGISTER_TO_FLAG[reg] != -2) {
        return has_flag_free_register(inst.get_cpu_registers(), CONVERT_CAPSTONE_REGISTER_TO_FLAG[reg]);
    }

    return false;
}

bool has_inst_free_eflags(uint64_t inst_eflags, uint64_t flags) {

    if (GET_BITES(flags, X86_EFLAGS_MODIFY_CF)) {
        if (!GET_BITES(inst_eflags, EFLAGS_MOD_CF)) {
            return false;
        }
    }

    if (GET_BITES(flags, X86_EFLAGS_MODIFY_OF)) {
        if (!GET_BITES(inst_eflags, EFLAGS_MOD_OF)) {
            return false;
        }
    }

    if (GET_BITES(flags, X86_EFLAGS_MODIFY_ZF)) {
        if (!GET_BITES(inst_eflags, EFLAGS_MOD_ZF)) {
            return false;
        }
    }

    if (GET_BITES(flags, X86_EFLAGS_MODIFY_DF)) {
        if (!GET_BITES(inst_eflags, EFLAGS_MOD_DF)) {
            return false;
        }
    }

    if (GET_BITES(flags, X86_EFLAGS_MODIFY_SF)) {
        if (!GET_BITES(inst_eflags, EFLAGS_MOD_SF)) {
            return false;
        }
    }

    if (GET_BITES(flags, X86_EFLAGS_MODIFY_PF)) {
        if (!GET_BITES(inst_eflags, EFLAGS_MOD_PF)) {
            return false;
        }
    }
    if (GET_BITES(flags, X86_EFLAGS_MODIFY_AF)) {
        if (!GET_BITES(inst_eflags, EFLAGS_MOD_AF)) {
            return false;
        }
    }


    return true;
}

uint64_t get_flag_complex_by_fuku_register(const fuku_register& reg, uint8_t size) {

    switch (size) {

    case 1: {
        return FULL_INCLUDE_FLAGS_TABLE[reg.get_index() + (reg.is_ext64() ? 8 : 0)] & 0xFFFF;
    }

    case 2: {
        return FULL_INCLUDE_FLAGS_TABLE[reg.get_index() + (reg.is_ext64() ? 8 : 0)] & 0xFFFFFFFF;
    }

    case 4: {
        return FULL_INCLUDE_FLAGS_TABLE[reg.get_index() + (reg.is_ext64() ? 8 : 0)] & 0xFFFFFFFFFFFF;
    }

    }

    return FULL_INCLUDE_FLAGS_TABLE[reg.get_index() + (reg.is_ext64() ? 8 : 0)];
}

uint64_t get_flag_complex_register(uint64_t flag_reg) {

    uint32_t index = 0;

    if (!bit_scan_forward(index, flag_reg)) {

        return 0;
    }

    uint8_t size = ((index) / 16) + 1;
    uint8_t reg_index = (index) % 16;

    return FULL_INCLUDE_FLAGS_TABLE[reg_index];
}

uint64_t get_flag_complex_register_by_size(uint64_t flag_reg) {

    uint32_t index = 0;

    if (!bit_scan_forward(index, flag_reg)) {

        return 0;
    }

    uint8_t size = ((index) / 16) + 1;
    uint8_t reg_index = (index) % 16;


    switch (size) {

    case 1: {
        return FULL_INCLUDE_FLAGS_TABLE[reg_index] & 0xFFFF;
    }

    case 2: {
        return FULL_INCLUDE_FLAGS_TABLE[reg_index] & 0xFFFFFFFF;
    }

    case 3: {
        return FULL_INCLUDE_FLAGS_TABLE[reg_index] & 0xFFFFFFFFFFFF;
    }

    case 4: {
        return FULL_INCLUDE_FLAGS_TABLE[reg_index] & 0xFFFFFFFFFFFFFFFF;
    }

    }

    return FULL_INCLUDE_FLAGS_TABLE[reg_index];
}




uint32_t get_rand_free_reg_(uint64_t inst_regs, uint32_t min_idx, uint32_t max_idx) {

    uint32_t index = min_idx;

    if (bit_scan_forward(index, inst_regs)) {

        if (index > max_idx) {

            if (max_idx + 16 < 63) {

                uint32_t idx = get_rand_free_reg_(inst_regs, min_idx + 16, max_idx + 16);

                if (idx == -1) {

                    return -1;
                }

                return idx - 16;
            }

            return -1;
        }

        return get_random_bit_by_mask(inst_regs, min_idx, max_idx);
    }

    return -1;
}

void exclude_flag_register(uint64_t& reg_flags, uint32_t reg_flag_idx) {

    for (; reg_flag_idx < 63; reg_flag_idx += 16) {

        reg_flags &= ~((uint64_t)1 << reg_flag_idx);
    }
}

fuku_register_enum get_random_register(uint32_t reg_size, bool x86_only, uint64_t exclude_regs) {

    switch (reg_size) {

    case 1: {
        return get_random_free_register(0xFFFFFFFFFFFFFFFF, 1, x86_only, exclude_regs);
    }

    case 2: {
        return get_random_free_register(0xFFFFFFFFFFFF0000, 2, x86_only, exclude_regs);
    }

    case 4: {
        return get_random_free_register(0xFFFFFFFF00000000, 4, x86_only, exclude_regs);
    }

    case 8: {
        return get_random_free_register(0xFFFF000000000000, 8, x86_only, exclude_regs);
    }

    }

    return FUKU_REG_NONE;
}

fuku_register_enum get_random_free_register(uint64_t reg_flags, uint32_t reg_size, bool x86_only, uint64_t exclude_regs) {


    reg_flags &= ~(exclude_regs);

    uint32_t returned_idx = -1;

    if (reg_flags) {

        switch (reg_size) {

        case 1: {

            if (x86_only) {
                returned_idx = get_rand_free_reg_(reg_flags, FLAG_REGISTER_IDX_AL, FLAG_REGISTER_IDX_BL);
            }
            else {
                returned_idx = get_rand_free_reg_(reg_flags, FLAG_REGISTER_IDX_AL, FLAG_REGISTER_IDX_R15B);
            }

            break;
        }
        case 2: {

            if (x86_only) {
                returned_idx = get_rand_free_reg_(reg_flags, FLAG_REGISTER_IDX_AX, FLAG_REGISTER_IDX_DI);
            }
            else {
                returned_idx = get_rand_free_reg_(reg_flags, FLAG_REGISTER_IDX_AX, FLAG_REGISTER_IDX_R15W);
            }

            break;
        }
        case 4: {

            if (x86_only) {
                returned_idx = get_rand_free_reg_(reg_flags, FLAG_REGISTER_IDX_EAX, FLAG_REGISTER_IDX_EDI);
            }
            else {
                returned_idx = get_rand_free_reg_(reg_flags, FLAG_REGISTER_IDX_EAX, FLAG_REGISTER_IDX_R15D);
            }

            break;
        }
        case 8: {

            if (x86_only) {
                returned_idx = get_rand_free_reg_(reg_flags, FLAG_REGISTER_IDX_RAX, FLAG_REGISTER_IDX_RDI);
            }
            else {
                returned_idx = get_rand_free_reg_(reg_flags, FLAG_REGISTER_IDX_RAX, FLAG_REGISTER_IDX_R15);
            }

            break;
        }
        }
    }

    if (returned_idx != -1) {

        return fuku_register_enum(CONVERT_FLAG_REGISTER_TO_FUKU[returned_idx]);
    }

    return FUKU_REG_NONE;
}

fuku_register_enum get_random_free_register(const fuku_inst& inst, uint32_t reg_size, bool x86_only, uint64_t exclude_regs) {

    return get_random_free_register(inst.get_cpu_registers(), reg_size, x86_only, exclude_regs);
}

fuku_register_enum get_random_free_register_x64(uint64_t reg_flags, uint8_t reg_size, uint64_t exclude_regs) {

    fuku_register_enum reg_ = get_random_free_register(reg_flags, reg_size == 4 ? 8 : reg_size, false, exclude_regs);

    if (reg_ != FUKU_REG_NONE && reg_size == 4) {
        return set_fuku_register_grade(reg_, 4);
    }

    return reg_;
}

uint64_t get_operand_mask_register(const fuku_type& op) {

    switch (op.get_type()) {

    case FUKU_T0_REGISTER: {
        return get_flag_complex_by_fuku_register(op.get_register().get_reg(), 8);
    }

    case FUKU_T0_OPERAND: {

        auto _op = op.get_operand();

        uint64_t result = 0;

        if (_op.get_base().get_reg() != FUKU_REG_NONE) {
            result &= get_flag_complex_by_fuku_register(_op.get_base(), 8);
        }
        if (_op.get_index().get_reg() != FUKU_REG_NONE) {
            result &= get_flag_complex_by_fuku_register(_op.get_index(), 8);
        }

        return result;
    }

    }

    return 0;
}

uint64_t get_operand_mask_register(const fuku_type& op1, const fuku_type& op2) {
    return get_operand_mask_register(op1) | get_operand_mask_register(op2);
}

fuku_immediate get_immediate_x86(uint8_t size) {

    uint8_t sw_ = FUKU_GET_RAND(0, size * 4);

    switch (sw_) {

    case 0: {
        return fuku_immediate(FUKU_GET_RAND(1, size * 0xFF) * 4);
    }

    case 1: {
        return fuku_immediate(FUKU_GET_RAND(1, 0xFFFFFFFF));
    }

    case 2:case 3:
    case 4:case 5:
    case 6:case 7:
    case 8:case 9:
    case 10:case 11:
    case 12:case 13:
    case 14:case 15:
    case 16: {

        return fuku_immediate(FUKU_GET_RAND(1, 0xF) * (1 << ((sw_ - 2) * 4)));
    }

    default:
        break;
    }

    return fuku_immediate(FUKU_GET_RAND(1, 0xFFFFFFFF));
}

bool get_operand_src_x86(fuku_type& op, uint8_t allow_inst, uint8_t size, uint64_t disallow_regs) {

    if (!allow_inst) { return false; }

    uint8_t target_type = get_random_bit_by_mask(allow_inst, 0, 2);

    switch (target_type) {

    case 0: {
        op = reg_(get_random_register(size, true, disallow_regs));
        return op.get_register().get_reg() != FUKU_REG_NONE;
    }

    case 1: {
        break;
    }

    case 2: {
        op = get_immediate_x86(size);
        return op.get_type() != FUKU_T0_NONE;
    }

    default: {break; }

    }

    return false;
}

bool get_operand_dst_x86(fuku_type& op, uint8_t allow_inst, uint8_t size, uint64_t allow_regs, uint64_t disallow_regs) {

    if (!allow_inst) { return false; }

    uint8_t target_type = get_random_bit_by_mask(allow_inst, 0, 2);

    switch (target_type) {

    case 0: {
        op = reg_(get_random_free_register(allow_regs, size, true, disallow_regs));
        return op.get_register().get_reg() != FUKU_REG_NONE;
    }

    case 1: {
        break;
    }

    default: {break; }
    }

    return false;
}

fuku_immediate get_immediate_x64(uint8_t size) {

    uint8_t sw_ = FUKU_GET_RAND(0, size * 4);

    switch (sw_) {

    case 0: {
        return fuku_immediate(FUKU_GET_RAND(1, size * 0xFF) * 4);
    }

    case 1: {
        return fuku_immediate(FUKU_GET_RAND(1, 0xFFFFFFFF));
    }

    case 2:case 3:
    case 4:case 5:
    case 6:case 7:
    case 8:case 9:
    case 10:case 11:
    case 12:case 13:
    case 14:case 15:
    case 16: {
        return fuku_immediate(FUKU_GET_RAND(1, 0xF) * (1 << ((sw_ - 2) * 4)));
    }

    default: {
        break;
    }

    }

    return fuku_immediate(FUKU_GET_RAND(1, 0xFFFFFFFF));
}


bool get_operand_src_x64(fuku_type& op, uint8_t allow_inst, uint8_t size, uint64_t disallow_regs) {

    if (!allow_inst) { return false; }

    uint8_t target_type = get_random_bit_by_mask(allow_inst, 0, 2);

    switch (target_type) {
    case 0: {
        op = reg_(get_random_register(size, false, disallow_regs));
        return op.get_register().get_reg() != FUKU_REG_NONE;
    }

    case 1: {
        break;
    }

    case 2: {
        op = get_immediate_x64(size);
        return op.get_type() != FUKU_T0_NONE;
    }

    default: {break; }
    }

    return false;
}

bool get_operand_dst_x64(fuku_type& op, uint8_t allow_inst, uint8_t size, uint64_t allow_regs, uint64_t disallow_regs) {

    if (!allow_inst) { return false; }

    uint8_t target_type = get_random_bit_by_mask(allow_inst, 0, 2);

    switch (target_type) {

    case 0: {
        op = reg_(get_random_free_register_x64(allow_regs, size, disallow_regs));
        return op.get_register().get_reg() != FUKU_REG_NONE;
    }

    case 1: {
        break;
    }

    default: {break; }
    }

    return false;
}