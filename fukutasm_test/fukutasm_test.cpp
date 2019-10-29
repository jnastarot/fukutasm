#include "pch.h"
#include <iostream>


#define FUKU_TESTMOD_86 0

csh cap_handle;
cs_insn *instruction;

#include "fuku_checks.h"
#include "fuku_macro_test.h"


void test_no_arg(fuku_inst inst, fuku_operand_size size) {

    size_t count = cs_disasm(cap_handle, inst.get_opcode(), inst.get_oplength(), 0, 1, &instruction);
    std::string error;

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) {
        error += ("type error  ");
    }
    if (inst.get_cpu_flags() != instruction[0].detail->x86.eflags) {
        error += ("eflags error  ");
    }

    if (instruction[0].detail->x86.op_count) {
    //    printf("op count error  ");
    }

    if (error.size()) {
        printf("%s %s %s\n", error.c_str(), instruction[0].mnemonic, instruction[0].op_str);
    }

    cs_free(instruction, count);
}


void test_1_arg(fuku_inst inst, fuku_operand op, fuku_operand_size size) {

    size_t count = cs_disasm(cap_handle, inst.get_opcode(), inst.get_oplength(), 0, 1, &instruction);
    std::string error;

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { 
        error += ("type error  ");
    }
    if (inst.get_cpu_flags() != instruction[0].detail->x86.eflags) {
        error += ("eflags error  ");
    }

    if (instruction[0].detail->x86.op_count == 1) {
        check_operand(error, op, &instruction[0].detail->x86.operands[0], instruction[0].detail->x86.encoding.disp_size, size);
    }
    else {
        error += ("op count error  ");
    }

    if (error.size()) {
        printf("%s %s %s\n", error.c_str(), instruction[0].mnemonic, instruction[0].op_str);
    }

    cs_free(instruction, count);
}

void test_1_arg_eip(fuku_inst inst, fuku_immediate imm, fuku_operand_size size) {

    size_t count = cs_disasm(cap_handle, inst.get_opcode(), inst.get_oplength(), 0, 1, &instruction);
    std::string error;
    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) {
        error += ("type error  ");
    }
    if (inst.get_cpu_flags() != instruction[0].detail->x86.eflags) {
        error += ("eflags error  ");
    }

    if (instruction[0].detail->x86.op_count == 1) {
       // check_immediate(error, imm, &instruction[0].detail->x86.operands[0], instruction[0].detail->x86.encoding.imm_size, size);
    }
    else {
        error += ("op count error  ");
    }

    if (error.size()) {
        printf("%s %s %s\n", error.c_str(), instruction[0].mnemonic, instruction[0].op_str);
    }


    cs_free(instruction, count);
}

void test_1_arg(fuku_inst inst, fuku_register reg, fuku_operand_size size) {

    size_t count = cs_disasm(cap_handle, inst.get_opcode(), inst.get_oplength(), 0, 1, &instruction);
    std::string error;

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { 
        error += ("type error  ");
    }
    if (inst.get_cpu_flags() != instruction[0].detail->x86.eflags) {
        error += ("eflags error  ");
    }

    if (instruction[0].detail->x86.op_count == 1) {
        check_register(error, reg, &instruction[0].detail->x86.operands[0], size);
    }
    else {
        error += ("op count error  ");
    }

    if (error.size()) {
        printf("%s %s %s\n", error.c_str(), instruction[0].mnemonic, instruction[0].op_str);
    }


    cs_free(instruction, count);
}
void test_1_arg(fuku_inst inst, fuku_immediate imm, fuku_operand_size size) {

    size_t count = cs_disasm(cap_handle, inst.get_opcode(), inst.get_oplength(), 0, 1, &instruction);
    std::string error;
    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) {
        error += ("type error  ");
    }
    if (inst.get_cpu_flags() != instruction[0].detail->x86.eflags) {
        error += ("eflags error  ");
    }

    if (instruction[0].detail->x86.op_count == 1) {
        check_immediate(error, imm, &instruction[0].detail->x86.operands[0], instruction[0].detail->x86.encoding.imm_size, size);
    }
    else {
        error += ("op count error  ");
    }

    if (error.size()) {
        printf("%s %s %s\n", error.c_str(), instruction[0].mnemonic, instruction[0].op_str);
    }


    cs_free(instruction, count);
}

void test_2_arg(fuku_inst inst, fuku_operand op, fuku_register reg, fuku_operand_size size) {

    size_t count = cs_disasm(cap_handle, inst.get_opcode(), inst.get_oplength(), 0, 1, &instruction);
    std::string error;
    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { 
        error += ("type error  ");
    }
    if (inst.get_cpu_flags() != instruction[0].detail->x86.eflags) {
        error += ("eflags error  ");
    }

    if (instruction[0].detail->x86.op_count == 2) {
        check_operand(error, op, &instruction[0].detail->x86.operands[0], instruction[0].detail->x86.encoding.disp_size, size);
        check_register(error, reg, &instruction[0].detail->x86.operands[1], size);
    }
    else {
        printf("op count error  ");
    }

    if (error.size()) {
        printf("%s %s %s\n", error.c_str(), instruction[0].mnemonic, instruction[0].op_str);
    }
    

    cs_free(instruction, count);
}
void test_2_arg(fuku_inst inst, fuku_register reg1, fuku_register reg2, fuku_operand_size size) {

    size_t count = cs_disasm(cap_handle, inst.get_opcode(), inst.get_oplength(), 0, 1, &instruction);
    std::string error;
    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { 
        error += ("type error  ");
    }
    if (inst.get_cpu_flags() != instruction[0].detail->x86.eflags) {
        error += ("eflags error  ");
    }

    if (instruction[0].detail->x86.op_count == 2) {
        check_register(error, reg1, &instruction[0].detail->x86.operands[0], size);
        check_register(error, reg2, &instruction[0].detail->x86.operands[1], size);
    }
    else {
        error += ("op count error  ");
    }

    if (error.size()) {
        printf("%s %s %s\n", error.c_str(), instruction[0].mnemonic, instruction[0].op_str);
    }


    cs_free(instruction, count);
}
void test_2_arg(fuku_inst inst, fuku_register reg, fuku_operand op, fuku_operand_size size) {

    size_t count = cs_disasm(cap_handle, inst.get_opcode(), inst.get_oplength(), 0, 1, &instruction);
    std::string error;
    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { 
        error += ("type error  ");
    }
    if (inst.get_cpu_flags() != instruction[0].detail->x86.eflags) {
        error += ("eflags error  ");
    }

    if (instruction[0].detail->x86.op_count == 2) {
        check_register(error, reg, &instruction[0].detail->x86.operands[0], size);
        check_operand(error, op, &instruction[0].detail->x86.operands[1], instruction[0].detail->x86.encoding.disp_size, size);
    }
    else {
        error += ("op count error  ");
    }

    if (error.size()) {
        printf("%s %s %s\n", error.c_str(), instruction[0].mnemonic, instruction[0].op_str);
    }


    cs_free(instruction, count);
}
void test_2_arg(fuku_inst inst, fuku_register reg, fuku_immediate imm, fuku_operand_size size) {

    size_t count = cs_disasm(cap_handle, inst.get_opcode(), inst.get_oplength(), 0, 1, &instruction);
    std::string error;
    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { 
        error += ("type error  ");
    }
    if (inst.get_cpu_flags() != instruction[0].detail->x86.eflags) {
        error += ("eflags error  ");
    }

    if (instruction[0].detail->x86.op_count == 2) {
        check_register(error, reg, &instruction[0].detail->x86.operands[0], size);
        check_immediate(error, imm, &instruction[0].detail->x86.operands[1], instruction[0].detail->x86.encoding.imm_size, size);
    }
    else {
        error += ("op count error  ");
    }

    if (error.size()) {
        printf("%s %s %s\n", error.c_str(), instruction[0].mnemonic, instruction[0].op_str);
    }


    cs_free(instruction, count);
}
void test_2_arg(fuku_inst inst,fuku_operand op, fuku_immediate imm, fuku_operand_size size) {

    size_t count = cs_disasm(cap_handle, inst.get_opcode(), inst.get_oplength(), 0, 1, &instruction);
    std::string error;
    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { 
        error += ("type error  ");
    }
    if (inst.get_cpu_flags() != instruction[0].detail->x86.eflags) {
        error += ("eflags error  ");
    }

    if (instruction[0].detail->x86.op_count == 2) {
        check_operand(error, op, &instruction[0].detail->x86.operands[0], instruction[0].detail->x86.encoding.disp_size, size);
        check_immediate(error, imm, &instruction[0].detail->x86.operands[1], instruction[0].detail->x86.encoding.imm_size, size);
    }
    else {
        error += ("op count error  ");
    }

    if (error.size()) {
        printf("%s %s %s\n", error.c_str(), instruction[0].mnemonic, instruction[0].op_str);
    }


    cs_free(instruction, count);
}

using namespace fukutasm;

#include "x32_test.h"
#include "x64_test.h"

#include <vector>

int main() {

    x32_test();
    x64_test();


    return 0;
}

