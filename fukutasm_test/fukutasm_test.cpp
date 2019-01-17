#include "pch.h"
#include <iostream>


#define FUKU_TESTMOD_86 0

csh cap_handle;
cs_insn *instruction;

#include "fuku_checks.h"
#include "fuku_macro_test.h"


void test_no_arg(fuku_instruction& inst, fuku_operand_size size) {
    if (FUKU_TESTMOD_86 && size == fuku_operand_size::FUKU_OPERAND_SIZE_64) { return; }

}


void test_1_arg(fuku_instruction inst, fuku_operand op, fuku_operand_size size) {
    if (FUKU_TESTMOD_86 && size == fuku_operand_size::FUKU_OPERAND_SIZE_64) { return; }

    size_t count = cs_disasm(cap_handle, inst.get_op_code(), inst.get_op_length(), 0, 1, &instruction);

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { printf("type error  "); }
    if (inst.get_eflags() != instruction[0].detail->x86.eflags) { printf("eflags error  "); }

    if (instruction[0].detail->x86.op_count == 1) {
        check_operand(op, &instruction[0].detail->x86.operands[0], instruction[0].detail->x86.encoding.disp_size, size);
    }
    else {
        printf("op count error  ");
    }

    printf("%s %s\n", instruction[0].mnemonic, instruction[0].op_str);


    cs_free(instruction, count);
}

void test_1_arg(fuku_instruction inst, fuku_register reg, fuku_operand_size size) {
    if (FUKU_TESTMOD_86 && size == fuku_operand_size::FUKU_OPERAND_SIZE_64) { return; }

    size_t count = cs_disasm(cap_handle, inst.get_op_code(), inst.get_op_length(), 0, 1, &instruction);

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { printf("type error  "); }
    if (inst.get_eflags() != instruction[0].detail->x86.eflags) { printf("eflags error  "); }

    if (instruction[0].detail->x86.op_count == 1) {
        check_register(reg, &instruction[0].detail->x86.operands[0], size);
    }
    else {
        printf("op count error  ");
    }

    printf("%s %s\n", instruction[0].mnemonic, instruction[0].op_str);


    cs_free(instruction, count);
}
void test_1_arg(fuku_instruction inst, fuku_immediate imm, fuku_operand_size size) {
    if (FUKU_TESTMOD_86 && size == fuku_operand_size::FUKU_OPERAND_SIZE_64) { return; }

    size_t count = cs_disasm(cap_handle, inst.get_op_code(), inst.get_op_length(), 0, 1, &instruction);

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { printf("type error  "); }
    if (inst.get_eflags() != instruction[0].detail->x86.eflags) { printf("eflags error  "); }

    if (instruction[0].detail->x86.op_count == 1) {
        check_immediate(imm, &instruction[0].detail->x86.operands[0], instruction[0].detail->x86.encoding.imm_size, size);
    }
    else {
        printf("op count error  ");
    }

    printf("%s %s\n", instruction[0].mnemonic, instruction[0].op_str);


    cs_free(instruction, count);
}

void test_2_arg(fuku_instruction inst, fuku_operand op, fuku_register reg, fuku_operand_size size) {
    if (FUKU_TESTMOD_86 && size == fuku_operand_size::FUKU_OPERAND_SIZE_64) { return; }

    size_t count = cs_disasm(cap_handle, inst.get_op_code(), inst.get_op_length(), 0, 1, &instruction);

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { printf("type error  "); }
    if (inst.get_eflags() != instruction[0].detail->x86.eflags) { printf("eflags error  "); }

    if (instruction[0].detail->x86.op_count == 2) {
        check_operand(op, &instruction[0].detail->x86.operands[0], instruction[0].detail->x86.encoding.disp_size, size);
        check_register(reg, &instruction[0].detail->x86.operands[1], size);
    }
    else {
        printf("op count error  ");
    }

    printf("%s %s\n", instruction[0].mnemonic, instruction[0].op_str);
    

    cs_free(instruction, count);
}
void test_2_arg(fuku_instruction inst, fuku_register reg1, fuku_register reg2, fuku_operand_size size) {
    if (FUKU_TESTMOD_86 && size == fuku_operand_size::FUKU_OPERAND_SIZE_64) { return; }

    size_t count = cs_disasm(cap_handle, inst.get_op_code(), inst.get_op_length(), 0, 1, &instruction);

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { printf("type error  "); }
    if (inst.get_eflags() != instruction[0].detail->x86.eflags) { printf("eflags error  "); }

    if (instruction[0].detail->x86.op_count == 2) {
        check_register(reg1, &instruction[0].detail->x86.operands[0], size);
        check_register(reg2, &instruction[0].detail->x86.operands[1], size);
    }
    else {
        printf("op count error  ");
    }

    printf("%s %s\n", instruction[0].mnemonic, instruction[0].op_str);


    cs_free(instruction, count);
}
void test_2_arg(fuku_instruction inst, fuku_register reg, fuku_operand op, fuku_operand_size size) {
    if (FUKU_TESTMOD_86 && size == fuku_operand_size::FUKU_OPERAND_SIZE_64) { return; }

    size_t count = cs_disasm(cap_handle, inst.get_op_code(), inst.get_op_length(), 0, 1, &instruction);

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { printf("type error  "); }
    if (inst.get_eflags() != instruction[0].detail->x86.eflags) { printf("eflags error  "); }

    if (instruction[0].detail->x86.op_count == 2) {
        check_register(reg, &instruction[0].detail->x86.operands[0], size);
        check_operand(op, &instruction[0].detail->x86.operands[1], instruction[0].detail->x86.encoding.disp_size, size);
    }
    else {
        printf("op count error  ");
    }

    printf("%s %s\n", instruction[0].mnemonic, instruction[0].op_str);


    cs_free(instruction, count);
}
void test_2_arg(fuku_instruction inst, fuku_register reg, fuku_immediate imm, fuku_operand_size size) {
    if (FUKU_TESTMOD_86 && size == fuku_operand_size::FUKU_OPERAND_SIZE_64) { return; }

    size_t count = cs_disasm(cap_handle, inst.get_op_code(), inst.get_op_length(), 0, 1, &instruction);

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { printf("type error  "); }
    if (inst.get_eflags() != instruction[0].detail->x86.eflags) { printf("eflags error  "); }

    if (instruction[0].detail->x86.op_count == 2) {
        check_register(reg, &instruction[0].detail->x86.operands[0], size);
        check_immediate(imm, &instruction[0].detail->x86.operands[1], instruction[0].detail->x86.encoding.imm_size, size);
    }
    else {
        printf("op count error  ");
    }

    printf("%s %s\n", instruction[0].mnemonic, instruction[0].op_str);


    cs_free(instruction, count);
}
void test_2_arg(fuku_instruction inst,fuku_operand op, fuku_immediate imm, fuku_operand_size size) {
    if (FUKU_TESTMOD_86 && size == fuku_operand_size::FUKU_OPERAND_SIZE_64) { return; }

    size_t count = cs_disasm(cap_handle, inst.get_op_code(), inst.get_op_length(), 0, 1, &instruction);

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { printf("type error  "); }
    if (inst.get_eflags() != instruction[0].detail->x86.eflags) { printf("eflags error  "); }

    if (instruction[0].detail->x86.op_count == 2) {
        check_operand(op, &instruction[0].detail->x86.operands[0], instruction[0].detail->x86.encoding.disp_size, size);
        check_immediate(imm, &instruction[0].detail->x86.operands[1], instruction[0].detail->x86.encoding.imm_size, size);
    }
    else {
        printf("op count error  ");
    }

    printf("%s %s\n", instruction[0].mnemonic, instruction[0].op_str);


    cs_free(instruction, count);
}





int main() {

    cs_open(CS_ARCH_X86, FUKU_TESTMOD_86 == 1 ? CS_MODE_32 : CS_MODE_64, &cap_handle);
    cs_option(cap_handle, CS_OPT_DETAIL, CS_OPT_ON);

    fuku_assambler fuku_asm(FUKU_TESTMOD_86 == 1 ? FUKU_ASSAMBLER_ARCH_X86 : FUKU_ASSAMBLER_ARCH_X64);

    

    fuku_operand test_op = fuku_operand(FUKU_REG_EAX, FUKU_REG_ECX, FUKU_OPERAND_SCALE_2, fuku_immediate(0xDEADBEAF));
    test_asm_def_full(add, test_op, FUKU_REG_ECX, FUKU_REG_EDX, fuku_immediate(0xDEADBEAF))
    test_asm_def_full(adc, test_op, FUKU_REG_ECX, FUKU_REG_EDX, fuku_immediate(0xDEADBEAF))
    test_asm_def_full(sub, test_op, FUKU_REG_ECX, FUKU_REG_EDX, fuku_immediate(0xDEADBEAF))
    test_asm_def_full(sbb, test_op, FUKU_REG_ECX, FUKU_REG_EDX, fuku_immediate(0xDEADBEAF))
    test_asm_def_full(cmp, test_op, FUKU_REG_ECX, FUKU_REG_EDX, fuku_immediate(0xDEADBEAF))
    test_asm_def_full(and, test_op, FUKU_REG_ECX, FUKU_REG_EDX, fuku_immediate(0xDEADBEAF))
    test_asm_def_full(or , test_op, FUKU_REG_ECX, FUKU_REG_EDX, fuku_immediate(0xDEADBEAF))
    test_asm_def_full(xor, test_op, FUKU_REG_ECX, FUKU_REG_EDX, fuku_immediate(0xDEADBEAF))

    test_asm_def_full_shift(sar, test_op, FUKU_REG_EDX, fuku_immediate(0xAF))
    test_asm_def_full_shift(shr, test_op, FUKU_REG_EDX, fuku_immediate(0xAF))
    test_asm_def_full_shift(shl, test_op, FUKU_REG_EDX, fuku_immediate(0xAF))
    test_asm_def_full_shift(ror, test_op, FUKU_REG_EDX, fuku_immediate(0xAF))
    test_asm_def_full_shift(rol, test_op, FUKU_REG_EDX, fuku_immediate(0xAF))
    test_asm_def_full_shift(rcr, test_op, FUKU_REG_EDX, fuku_immediate(0xAF))
    test_asm_def_full_shift(rcl, test_op, FUKU_REG_EDX, fuku_immediate(0xAF))

    test_gen_func_body_bit(bt, test_op, FUKU_REG_ECX, FUKU_REG_EDX, fuku_immediate(0xDEADBEAF))
    test_gen_func_body_bit(bts, test_op, FUKU_REG_ECX, FUKU_REG_EDX, fuku_immediate(0xDEADBEAF))
    test_gen_func_body_bit(btr, test_op, FUKU_REG_ECX, FUKU_REG_EDX, fuku_immediate(0xDEADBEAF))
    test_gen_func_body_bit(btc, test_op, FUKU_REG_ECX, FUKU_REG_EDX, fuku_immediate(0xDEADBEAF))

    test_asm_def_r_op_one_op(imul, test_op, FUKU_REG_ECX)
    test_asm_def_r_op_one_op(mul, test_op, FUKU_REG_ECX)
    test_asm_def_r_op_one_op(idiv, test_op, FUKU_REG_ECX)
    test_asm_def_r_op_one_op(div, test_op, FUKU_REG_ECX)
    test_asm_def_r_op_one_op(inc, test_op, FUKU_REG_ECX)
    test_asm_def_r_op_one_op(dec, test_op, FUKU_REG_ECX)
    test_asm_def_r_op_one_op(neg, test_op, FUKU_REG_ECX)
    test_asm_def_r_op_one_op(not, test_op, FUKU_REG_ECX)
    return 0;
}

