#pragma once


fuku_register_index convert_cs_to_fuku(x86_reg reg) {

    switch (reg) {
    case X86_REG_AH: case X86_REG_AL: case X86_REG_AX: case X86_REG_EAX: case X86_REG_RAX: return FUKU_REG_INDEX_AX;
    case X86_REG_CH: case X86_REG_CL: case X86_REG_CX: case X86_REG_ECX: case X86_REG_RCX: return FUKU_REG_INDEX_CX;
    case X86_REG_BH: case X86_REG_BL: case X86_REG_BX: case X86_REG_EBX: case X86_REG_RBX: return FUKU_REG_INDEX_BX;
    case X86_REG_DH: case X86_REG_DL: case X86_REG_DX: case X86_REG_EDX: case X86_REG_RDX: return FUKU_REG_INDEX_DX;
    case X86_REG_BP: case X86_REG_BPL: case X86_REG_EBP: case X86_REG_RBP: return FUKU_REG_INDEX_BP;
    case X86_REG_SP: case X86_REG_SPL: case X86_REG_ESP: case X86_REG_RSP: return FUKU_REG_INDEX_SP;
    case X86_REG_DI: case X86_REG_DIL: case X86_REG_EDI: case X86_REG_RDI: return FUKU_REG_INDEX_DI;
    case X86_REG_SI: case X86_REG_SIL: case X86_REG_ESI: case X86_REG_RSI: return FUKU_REG_INDEX_SI;

    case X86_REG_R8B: case X86_REG_R8W: case X86_REG_R8D: case X86_REG_R8: return FUKU_REG_INDEX_R8;
    case X86_REG_R9B: case X86_REG_R9W: case X86_REG_R9D: case X86_REG_R9: return FUKU_REG_INDEX_R9;
    case X86_REG_R10B: case X86_REG_R10W: case X86_REG_R10D: case X86_REG_R10: return FUKU_REG_INDEX_R10;
    case X86_REG_R11B: case X86_REG_R11W: case X86_REG_R11D: case X86_REG_R11: return FUKU_REG_INDEX_R11;
    case X86_REG_R12B: case X86_REG_R12W: case X86_REG_R12D: case X86_REG_R12: return FUKU_REG_INDEX_R12;
    case X86_REG_R13B: case X86_REG_R13W: case X86_REG_R13D: case X86_REG_R13: return FUKU_REG_INDEX_R13;
    case X86_REG_R14B: case X86_REG_R14W: case X86_REG_R14D: case X86_REG_R14: return FUKU_REG_INDEX_R14;
    case X86_REG_R15B: case X86_REG_R15W: case X86_REG_R15D: case X86_REG_R15: return FUKU_REG_INDEX_R15;
    }

    return fuku_register_index::FUKU_REG_INDEX_INVALID;
}

bool check_imm(uint64_t cs_imm, fuku_immediate imm, uint32_t imm_size) {

    switch (imm_size) {
    case 1: {
        if (*(uint8_t*)&cs_imm != imm.get_immediate8()) { return false; }
        break;
    }
    case 2: {
        if (*(uint16_t*)&cs_imm != imm.get_immediate16()) { return false; }
        break;
    }
    case 4: {
        if (*(uint32_t*)&cs_imm != imm.get_immediate32()) { return false; }
        break;
    }
    case 8: {
        if (*(uint64_t*)&cs_imm != imm.get_immediate64()) { return false; }
        break;
    }
    default:
        return false;
    }

    return true;
}

void check_operand(fuku_operand& op, cs_x86_op * csop, uint32_t disp_size, fuku_operand_size size) {
    if (csop->type != x86_op_type::X86_OP_MEM) {
        printf("op error type  ");
        return;
    };

    switch (op.get_type()) {
    case FUKU_MEM_OPERAND_BASE_ONLY: {
        if (convert_cs_to_fuku(csop->mem.base) != fuku_get_index_by_register(op.get_base())) { printf("reg error type  "); }
        break;
    }
    case    FUKU_MEM_OPERAND_DISP_ONLY: {
        if (!check_imm(csop->mem.disp, op.get_disp(), disp_size)) {
            printf("disp error  ");
        }
        break;
    }
    case    FUKU_MEM_OPERAND_BASE_DISP: {
        if (convert_cs_to_fuku(csop->mem.base) != fuku_get_index_by_register(op.get_base())) { printf("reg error type  "); }
        if (!check_imm(csop->mem.disp, op.get_disp(), disp_size)) {
            printf("disp error  ");
        }
        break;
    }
    case    FUKU_MEM_OPERAND_INDEX_DISP: {
        if (convert_cs_to_fuku(csop->mem.index) != fuku_get_index_by_register(op.get_index())) { printf("idx error type  "); }
        if (!check_imm(csop->mem.disp, op.get_disp(), disp_size)) {
            printf("disp error  ");
        }
        break;
    }
    case    FUKU_MEM_OPERAND_BASE_INDEX: {
        if (convert_cs_to_fuku(csop->mem.base) != fuku_get_index_by_register(op.get_base())) { printf("reg error type  "); }
        if (convert_cs_to_fuku(csop->mem.index) != fuku_get_index_by_register(op.get_index())) { printf("idx error type  "); }
        break;
    }
    case    FUKU_MEM_OPERAND_BASE_INDEX_DISP: {
        if (convert_cs_to_fuku(csop->mem.base) != fuku_get_index_by_register(op.get_base())) { printf("reg error type  "); }
        if (convert_cs_to_fuku(csop->mem.index) != fuku_get_index_by_register(op.get_index())) { printf("idx error type  "); }
        if (!check_imm(csop->mem.disp, op.get_disp(), disp_size)) {
            printf("disp error  ");
        }
        break;
    }
    }
}

void check_register(fuku_register reg, cs_x86_op * csop, fuku_operand_size size) {
    if (csop->type != x86_op_type::X86_OP_REG) {
        printf("op error type  ");
        return;
    }

    if (convert_cs_to_fuku(csop->reg) != fuku_get_index_by_register(reg)) { printf("reg error type  "); }
}

void check_immediate(fuku_immediate& imm, cs_x86_op * csop, uint32_t imm_size, fuku_operand_size size) {
    if (csop->type != x86_op_type::X86_OP_IMM) {
        printf("op error type  ");
        return;
    };

    if (!check_imm(csop->imm, imm, imm_size)) { printf("imm error  "); }
}
