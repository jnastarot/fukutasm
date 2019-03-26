#include "pch.h"
#include <iostream>


#define FUKU_TESTMOD_86 0

csh cap_handle;
cs_insn *instruction;

#include "fuku_checks.h"
#include "fuku_macro_test.h"


void test_no_arg(fuku_instruction inst, fuku_operand_size size) {
    if (FUKU_TESTMOD_86 && size == FUKU_OPERAND_SIZE_64) { return; }

    size_t count = cs_disasm(cap_handle, inst.get_op_code(), inst.get_op_length(), 0, 1, &instruction);

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) {
        printf("type error  "); 
    }
    if (inst.get_used_eflags() != instruction[0].detail->x86.eflags) {
        printf("eflags error  "); 
    }

    if (instruction[0].detail->x86.op_count) {
    //    printf("op count error  ");
    }

    printf("%s %s\n", instruction[0].mnemonic, instruction[0].op_str);


    cs_free(instruction, count);
}


void test_1_arg(fuku_instruction inst, fuku_operand op, fuku_operand_size size) {
    if (FUKU_TESTMOD_86 && size == FUKU_OPERAND_SIZE_64) { return; }

    size_t count = cs_disasm(cap_handle, inst.get_op_code(), inst.get_op_length(), 0, 1, &instruction);

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { 
        printf("type error  ");
    }
    if (inst.get_used_eflags() != instruction[0].detail->x86.eflags) {
        printf("eflags error  ");
    }

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
    if (FUKU_TESTMOD_86 && size == FUKU_OPERAND_SIZE_64) { return; }

    size_t count = cs_disasm(cap_handle, inst.get_op_code(), inst.get_op_length(), 0, 1, &instruction);

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { 
        printf("type error  ");
    }
    if (inst.get_used_eflags() != instruction[0].detail->x86.eflags) {
        printf("eflags error  ");
    }

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
    if (FUKU_TESTMOD_86 && size == FUKU_OPERAND_SIZE_64) { return; }

    size_t count = cs_disasm(cap_handle, inst.get_op_code(), inst.get_op_length(), 0, 1, &instruction);

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) {
        printf("type error  "); 
    }
    if (inst.get_used_eflags() != instruction[0].detail->x86.eflags) {
        printf("eflags error  "); 
    }

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
    if (FUKU_TESTMOD_86 && size == FUKU_OPERAND_SIZE_64) { return; }

    size_t count = cs_disasm(cap_handle, inst.get_op_code(), inst.get_op_length(), 0, 1, &instruction);

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { 
        printf("type error  "); 
    }
    if (inst.get_used_eflags() != instruction[0].detail->x86.eflags) {
        printf("eflags error  "); 
    }

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
    if (FUKU_TESTMOD_86 && size == FUKU_OPERAND_SIZE_64) { return; }

    size_t count = cs_disasm(cap_handle, inst.get_op_code(), inst.get_op_length(), 0, 1, &instruction);

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { 
        printf("type error  ");
    }
    if (inst.get_used_eflags() != instruction[0].detail->x86.eflags) {
        printf("eflags error  "); 
    }

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
    if (FUKU_TESTMOD_86 && size == FUKU_OPERAND_SIZE_64) { return; }

    size_t count = cs_disasm(cap_handle, inst.get_op_code(), inst.get_op_length(), 0, 1, &instruction);

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { 
        printf("type error  "); 
    }
    if (inst.get_used_eflags() != instruction[0].detail->x86.eflags) {
        printf("eflags error  "); 
    }

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
    if (FUKU_TESTMOD_86 && size == FUKU_OPERAND_SIZE_64) { return; }

    size_t count = cs_disasm(cap_handle, inst.get_op_code(), inst.get_op_length(), 0, 1, &instruction);

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { 
        printf("type error  "); 
    }
    if (inst.get_used_eflags() != instruction[0].detail->x86.eflags) {
        printf("eflags error  ");
    }

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
    if (FUKU_TESTMOD_86 && size == FUKU_OPERAND_SIZE_64) { return; }

    size_t count = cs_disasm(cap_handle, inst.get_op_code(), inst.get_op_length(), 0, 1, &instruction);

    if (!count) {
        __debugbreak();
    }

    if (inst.get_id() != instruction[0].id) { 
        printf("type error  ");
    }
    if (inst.get_used_eflags() != instruction[0].detail->x86.eflags) {
        printf("eflags error  ");
    }

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

using namespace fukutasm;

int main() {

    cs_open(CS_ARCH_X86, FUKU_TESTMOD_86 == 1 ? CS_MODE_32 : CS_MODE_64, &cap_handle);
    cs_option(cap_handle, CS_OPT_DETAIL, CS_OPT_ON);

#define op FUKU_REG_EAX

   // fuku_internal_assambler fuku_asm(FUKU_TESTMOD_86 == 1 ? FUKU_ASSAMBLER_ARCH_X86 : FUKU_ASSAMBLER_ARCH_X64);
  //  fuku_operand  op = qword_ptr(FUKU_REG_EAX);// fuku_operand(FUKU_REG_EAX, FUKU_REG_ECX, FUKU_OPERAND_SCALE_2, fuku_immediate(0x12345678));
    fuku_register reg1 = FUKU_REG_EDX;
    fuku_register reg2 = FUKU_REG_EDX;
    fuku_immediate imm(0x12345678);

    fuku_assambler fasm(FUKU_TESTMOD_86 == 1 ? FUKU_ASSAMBLER_ARCH_X86 : FUKU_ASSAMBLER_ARCH_X64);

    fuku_instruction inst;

    fukutasm::fuku_assambler_ctx & ctx = fasm.get_context();
    ctx.arch = FUKU_TESTMOD_86 == 1 ? FUKU_ASSAMBLER_ARCH_X86 : FUKU_ASSAMBLER_ARCH_X64;
    ctx.short_cfg = 0xFF;
    ctx.inst = &inst;
  //  fasm.xchg(op, FUKU_REG_RCX); test_2_arg(*ctx.inst, op, FUKU_REG_RCX, FUKU_OPERAND_SIZE_64);
 //   fasm.xchg(op, FUKU_REG_ECX); test_2_arg(inst, op, FUKU_REG_ECX, FUKU_OPERAND_SIZE_32);
  //  fasm.xchg(op, FUKU_REG_CX); test_2_arg(inst, op, FUKU_REG_CX, FUKU_OPERAND_SIZE_16);
   // test_2_arg(inst, op , reg1, FUKU_OPERAND_SIZE_32);

    for (fuku_register_enum i = FUKU_REG_NONE; i < FUKU_REG_MAX; i = fuku_register_enum(i + 1) ) {
        test_asm_def_r_op_one_op(not, op, reg_((fuku_register_enum)i));
    }
   // return 0;z
    uint64_t vall = 0x1000000;

    fasm.shld(reg_(FUKU_REG_SP), reg_(FUKU_REG_BP), reg_(FUKU_REG_CL));
    test_2_arg(inst, dword_ptr(FUKU_REG_EAX), reg_(FUKU_REG_ESP), FUKU_OPERAND_SIZE_32);

    fasm.shld(reg_(FUKU_REG_SP), reg_(FUKU_REG_BP), imm);
    test_2_arg(inst, dword_ptr(FUKU_REG_RAX), reg_(FUKU_REG_ESP), FUKU_OPERAND_SIZE_64);

    //operand test
    asm_def_2op_test(mov,_dw,  dword_ptr(FUKU_REG_EAX), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ECX), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EDX), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EBX), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EBP), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ESP), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EDI), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ESI), reg1, FUKU_OPERAND_SIZE_32)

    asm_def_2op_test(mov,_dw,  dword_ptr(FUKU_REG_EAX, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ECX, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EDX, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EBX, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EBP, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ESP, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EDI, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ESI, vall), reg1, FUKU_OPERAND_SIZE_32)

    asm_def_2op_test(mov,_dw,  dword_ptr(FUKU_REG_EAX, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ECX, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EDX, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EBX, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EBP, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ESP, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EDI, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ESI, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)

    asm_def_2op_test(mov,_dw,  dword_ptr(FUKU_REG_EAX, FUKU_REG_ESP, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ECX, FUKU_REG_ESP, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EDX, FUKU_REG_ESP, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EBX, FUKU_REG_ESP, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EBP, FUKU_REG_ESP, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ESP, FUKU_REG_ESP, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EDI, FUKU_REG_ESP, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ESI, FUKU_REG_ESP, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)

//Data Transfer Instructions
    test_asm_def_full(mov, op, reg1, reg2, imm)
    asm_def_cond_2op_test(cmovcc,_w, reg1, reg2, FUKU_OPERAND_SIZE_16)
    asm_def_cond_2op_test(cmovcc,_w, reg1, word_ptr(op), FUKU_OPERAND_SIZE_16)
    asm_def_cond_2op_test(cmovcc,_dw, reg1, reg2, FUKU_OPERAND_SIZE_32)
    asm_def_cond_2op_test(cmovcc,_dw, reg1, dword_ptr(op), FUKU_OPERAND_SIZE_32)
    asm_def_cond_2op_test(cmovcc,_qw, reg1, reg2, FUKU_OPERAND_SIZE_64)
    asm_def_cond_2op_test(cmovcc,_qw, reg1, qword_ptr(op), FUKU_OPERAND_SIZE_64)
    test_asm_def_direct_mov(xchg, op, reg1, reg2)
    asm_def_1op_test(bswap_dw,, reg1, FUKU_OPERAND_SIZE_32)
    asm_def_1op_test(bswap_qw,, reg1, FUKU_OPERAND_SIZE_64)
    test_asm_def_direct_mov(xadd, op, reg1, reg2)
    test_asm_def_direct_mov(cmpxchg, op, reg1, reg2)
    asm_def_1op_test(cmpxchg8b,, dword_ptr(op), FUKU_OPERAND_SIZE_32)
#if ( FUKU_TESTMOD_86 == 0 )
    asm_def_1op_test(cmpxchg16b,, qword_ptr(op), FUKU_OPERAND_SIZE_64)
#endif
    test_asm_def_wdq_one_op(push, op, reg1, imm)
    asm_def_1op_test(pop,_w, reg1, FUKU_OPERAND_SIZE_16)
    asm_def_1op_test(pop,_dw, reg1, FUKU_OPERAND_SIZE_32)
    asm_def_1op_test(pop,_qw, reg1, FUKU_OPERAND_SIZE_64)
    asm_def_1op_test(pop, _w, word_ptr(op), FUKU_OPERAND_SIZE_16)
    asm_def_1op_test(pop, _dw, dword_ptr(op), FUKU_OPERAND_SIZE_32)
    asm_def_1op_test(pop, _qw, qword_ptr(op), FUKU_OPERAND_SIZE_64)
    asm_def_noop_test(cwd,, FUKU_OPERAND_SIZE_16)
    asm_def_noop_test(cdq,, FUKU_OPERAND_SIZE_32)
    asm_def_noop_test(cqo,, FUKU_OPERAND_SIZE_64)
    test_asm_def_movsxz(movzx, op, reg1, reg2)
    test_asm_def_movsxz(movsx, op, reg1, reg2)
#if ( FUKU_TESTMOD_86 == 0 )
   // asm_def_2op_test(movsx,_dword_dw, reg1, reg2, FUKU_OPERAND_SIZE_32)
   // asm_def_2op_test(movsx,_dword_dw, reg1, dword_ptr(op), FUKU_OPERAND_SIZE_32)
  //  asm_def_2op_test(movsx,_dword_qw, reg1, reg2, FUKU_OPERAND_SIZE_64)
  //  asm_def_2op_test(movsx,_dword_qw, reg1, qword_ptr(op), FUKU_OPERAND_SIZE_64)
#endif
//Binary Arithmetic Instructions
    asm_def_2op_test(adcx,_dw, reg1, reg2, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(adcx,_dw, reg1, dword_ptr(op), FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(adcx,_qw, reg1, reg2, FUKU_OPERAND_SIZE_64)
    asm_def_2op_test(adcx,_qw, reg1, qword_ptr(op), FUKU_OPERAND_SIZE_64)
    asm_def_2op_test(adox,_dw, reg1, reg2, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(adox,_dw, reg1, dword_ptr(op), FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(adox,_qw, reg1, reg2, FUKU_OPERAND_SIZE_64)
    asm_def_2op_test(adox,_qw, reg1, qword_ptr(op), FUKU_OPERAND_SIZE_64)
    test_asm_def_full(add, op, reg1, reg2, imm)
    test_asm_def_full(adc, op, reg1, reg2, imm)
    test_asm_def_full(sub, op, reg1, reg2, imm)
    test_asm_def_full(sbb, op, reg1, reg2, imm)
    test_asm_def_r_op_one_op(imul, op, reg1)
    test_asm_def_r_op_one_op(mul, op, reg1)
    test_asm_def_r_op_one_op(idiv, op, reg1)
    test_asm_def_r_op_one_op(div, op, reg1)
    test_asm_def_r_op_one_op(inc, op, reg1)
    test_asm_def_r_op_one_op(dec, op, reg1)
    test_asm_def_r_op_one_op(neg, op, reg1)
    test_asm_def_full(cmp, op, reg1, reg2, imm)
//Decimal Arithmetic Instructions
#if ( FUKU_TESTMOD_86 == 1 )
    asm_def_noop_test(daa, , FUKU_OPERAND_SIZE_32)
    asm_def_noop_test(das, , FUKU_OPERAND_SIZE_32)
    asm_def_noop_test(aaa, , FUKU_OPERAND_SIZE_32)
    asm_def_noop_test(aas, , FUKU_OPERAND_SIZE_32)
//    asm_def_1op_test(aam,, reg1, FUKU_OPERAND_SIZE_32)
 //   asm_def_1op_test(aad,, reg1, FUKU_OPERAND_SIZE_32)
#endif
//Logical Instructions Instructions
    test_asm_def_full(and, op, reg1, reg2, imm)
    test_asm_def_full(or , op, reg1, reg2, imm)
    test_asm_def_full(xor, op, reg1, reg2, imm)
    test_asm_def_r_op_one_op(not, op, reg1)
//Shift and Rotate Instructions
//    test_asm_def_full_shift(sar, op, reg2, imm)
 //   test_asm_def_full_shift(shr, op, reg2, imm)
 //   test_asm_def_full_shift(shl, op, reg2, imm)
    //asm_def_shXd_inst(shrd)
    //asm_def_shXd_inst(shld)
 //   test_asm_def_full_shift(ror, op, reg2, imm)
 //   test_asm_def_full_shift(rol, op, reg2, imm)
 //   test_asm_def_full_shift(rcr, op, reg2, imm)
 //   test_asm_def_full_shift(rcl, op, reg2, imm)
//Bit and Byte Instructions
    test_gen_func_body_bit(bt, op, reg1, reg2, imm)
    test_gen_func_body_bit(bts, op, reg1, reg2, imm)
    test_gen_func_body_bit(btr, op, reg1, reg2, imm)
    test_gen_func_body_bit(btc, op, reg1, reg2, imm)
    asm_def_2op_test(test, _b, byte_ptr(op), reg2, FUKU_OPERAND_SIZE_8)
    asm_def_2op_test(test, _b, reg1, reg2, FUKU_OPERAND_SIZE_8)
    asm_def_2op_test(test, _b, reg1, imm, FUKU_OPERAND_SIZE_8)
    asm_def_2op_test(test, _b, byte_ptr(op), imm, FUKU_OPERAND_SIZE_8)
    asm_def_2op_test(test, _w, word_ptr(op), reg2, FUKU_OPERAND_SIZE_16)
    asm_def_2op_test(test, _w, reg1, reg2, FUKU_OPERAND_SIZE_16)
    asm_def_2op_test(test, _w, reg1, imm, FUKU_OPERAND_SIZE_16)
    asm_def_2op_test(test, _w, word_ptr(op), imm, FUKU_OPERAND_SIZE_16)
    asm_def_2op_test(test, _dw, dword_ptr(op), reg2, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(test, _dw, reg1, reg2, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(test, _dw, reg1, imm, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(test, _dw, dword_ptr(op), imm, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(test, _qw, qword_ptr(op), reg2, FUKU_OPERAND_SIZE_64)
    asm_def_2op_test(test, _qw, reg1, reg2, FUKU_OPERAND_SIZE_64)
    asm_def_2op_test(test, _qw, reg1, imm, FUKU_OPERAND_SIZE_64)
    asm_def_2op_test(test, _qw, qword_ptr(op), imm, FUKU_OPERAND_SIZE_64)
    asm_def_2op_test(popcnt,_dw, reg1, reg2, FUKU_OPERAND_SIZE_16)
    asm_def_2op_test(popcnt,_dw, reg1, word_ptr(op), FUKU_OPERAND_SIZE_16)
    asm_def_2op_test(popcnt,_dw, reg1, reg2, FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(popcnt,_dw, reg1, dword_ptr(op), FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(popcnt,_qw, reg1, reg2, FUKU_OPERAND_SIZE_64)
    asm_def_2op_test(popcnt,_qw, reg1, qword_ptr(op), FUKU_OPERAND_SIZE_64)
//Control Transfer Instructions
#if ( FUKU_TESTMOD_86 == 1 )
    test_asm_def_eip_one_op(jmp, op, reg1, imm, FUKU_OPERAND_SIZE_32)
#else
    test_asm_def_eip_one_op(jmp, op, reg1, imm, FUKU_OPERAND_SIZE_64)
#endif
    asm_def_cond_1op_test(jcc,, imm, FUKU_OPERAND_SIZE_32)
#if ( FUKU_TESTMOD_86 == 1 )
    test_asm_def_eip_one_op(call, op, reg1, imm, FUKU_OPERAND_SIZE_32)
#else
    test_asm_def_eip_one_op(call, op, reg1, imm, FUKU_OPERAND_SIZE_64)
#endif
    test_asm_def_noop_imm(ret, imm)
    asm_def_noop_test(int3, , FUKU_OPERAND_SIZE_32)
   // asm_def_2op_test(enter,,fuasm_imm, uint8_t)//enter size, nestinglevel
    asm_def_noop_test(,leave_, FUKU_OPERAND_SIZE_32)
//String Instructions
    asm_def_noop_test(outsb, , FUKU_OPERAND_SIZE_8)
    asm_def_noop_test(outsw, , FUKU_OPERAND_SIZE_16)
    asm_def_noop_test(outsd, , FUKU_OPERAND_SIZE_32)
    test_asm_def_string_inst(movs)
    test_asm_def_string_inst(cmps)
    test_asm_def_string_inst(scas)
    test_asm_def_string_inst(lods)
    test_asm_def_string_inst(stos)
//Flag Control (EFLAG) Instructions
    asm_def_noop_test(stc, , FUKU_OPERAND_SIZE_32)
    asm_def_noop_test(clc, , FUKU_OPERAND_SIZE_32)
    asm_def_noop_test(cmc, , FUKU_OPERAND_SIZE_32)
    asm_def_noop_test(cld, , FUKU_OPERAND_SIZE_32)
    asm_def_noop_test(std, , FUKU_OPERAND_SIZE_32)
#if ( FUKU_TESTMOD_86 == 1 )
    asm_def_noop_test(lahf, , FUKU_OPERAND_SIZE_32)
    asm_def_noop_test(sahf, , FUKU_OPERAND_SIZE_32)
#endif
#if ( FUKU_TESTMOD_86 == 1 )
    asm_def_noop_test(pusha, , FUKU_OPERAND_SIZE_16)       //pusha  w  regs
    asm_def_noop_test(pushad, , FUKU_OPERAND_SIZE_32)      //pushad dw regs
    asm_def_noop_test(popa, , FUKU_OPERAND_SIZE_16)        //popa   w  regs
    asm_def_noop_test(popad, , FUKU_OPERAND_SIZE_32)       //popad  dw regs
#endif
    asm_def_noop_test(pushf, , FUKU_OPERAND_SIZE_16)       //pushf  w  flags
#if ( FUKU_TESTMOD_86 == 1 )
    asm_def_noop_test(pushfd, , FUKU_OPERAND_SIZE_32)      //pushfd dw flags
#else
    asm_def_noop_test(pushfq, , FUKU_OPERAND_SIZE_64)      //pushfq qw flags
#endif
    asm_def_noop_test(popf, , FUKU_OPERAND_SIZE_16)        //popf   w  flags
#if ( FUKU_TESTMOD_86 == 1 )
    asm_def_noop_test(popfd, , FUKU_OPERAND_SIZE_32)       //popfd  dw flags
#else
    asm_def_noop_test(popfq, , FUKU_OPERAND_SIZE_64)       //popfq  qw flags
#endif
//Miscellaneous Instructions
    asm_def_2op_test(lea,_w, reg1,  word_ptr(op), FUKU_OPERAND_SIZE_16)
    asm_def_2op_test(lea,_dw, reg1, dword_ptr(op), FUKU_OPERAND_SIZE_32)
    asm_def_2op_test(lea,_qw, reg1, qword_ptr(op), FUKU_OPERAND_SIZE_64)
   // test_no_arg(fuku_asm.fuku_asm_gen_name(_,nop,)(1), FUKU_OPERAND_SIZE_32);
    asm_def_noop_test(ud2, , FUKU_OPERAND_SIZE_32)
    asm_def_noop_test(cpuid, , FUKU_OPERAND_SIZE_32)
//Random Number Generator Instructions
    asm_def_1op_test(rdrand, _w, reg1, FUKU_OPERAND_SIZE_16)
    asm_def_1op_test(rdrand, _dw, reg1, FUKU_OPERAND_SIZE_32)
    asm_def_1op_test(rdrand, _qw, reg1, FUKU_OPERAND_SIZE_64)
    asm_def_1op_test(rdseed, _w, reg1, FUKU_OPERAND_SIZE_16)
    asm_def_1op_test(rdseed, _dw, reg1, FUKU_OPERAND_SIZE_32)
    asm_def_1op_test(rdseed, _qw, reg1, FUKU_OPERAND_SIZE_64)
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
    asm_def_noop_test(hlt, , FUKU_OPERAND_SIZE_32)
    asm_def_noop_test(rdtsc, , FUKU_OPERAND_SIZE_32)
    asm_def_noop_test(lfence, , FUKU_OPERAND_SIZE_32)
    
    return 0;
}

