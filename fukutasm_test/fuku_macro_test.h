#pragma once

#define asm_def_noop_test(name, postfix, size) \
        test_no_arg( fuku_asm. fuku_asm_gen_name(_,name,postfix), size);

#define asm_def_1op_test(name, postfix, op, size) \
        test_1_arg( fuku_asm. fuku_asm_gen_name(_,name,postfix)(op), op, size);

#define asm_def_2op_test(name, postfix, op1, op2, size) \
        test_2_arg( fuku_asm. fuku_asm_gen_name(_,name,postfix)(op1, op2),op1, op2, size);


#define test_asm_def_full(name, op, reg1, reg2, imm)\
    asm_def_2op_test(name,_b, reg1, reg2, FUKU_OPERAND_SIZE_8)\
    asm_def_2op_test(name,_b, reg1, imm, FUKU_OPERAND_SIZE_8)\
    asm_def_2op_test(name,_b, reg1, op, FUKU_OPERAND_SIZE_8)\
    asm_def_2op_test(name,_b, op, reg2, FUKU_OPERAND_SIZE_8)\
    asm_def_2op_test(name,_b, op, imm, FUKU_OPERAND_SIZE_8)\
\
    asm_def_2op_test(name,_w, reg1, reg2, FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_w, reg1, imm, FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_w, reg1, op, FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_w, op, reg2, FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_w, op, imm, FUKU_OPERAND_SIZE_16)\
\
    asm_def_2op_test(name,_dw, reg1, reg2, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_dw, reg1, imm, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_dw, reg1, op, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_dw, op, reg2, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_dw, op, imm, FUKU_OPERAND_SIZE_32)\
\
    asm_def_2op_test(name,_qw, reg1, reg2, FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_qw, reg1, imm, FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_qw, reg1, op, FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_qw, op, reg2, FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_qw, op, imm, FUKU_OPERAND_SIZE_64)

#define test_asm_def_full_shift(name, op, reg1, imm)\
    test_2_arg( fuku_asm. _##name##_cl_b(reg1),reg1, FUKU_REG_ECX, FUKU_OPERAND_SIZE_8);\
    test_2_arg( fuku_asm. _##name##_cl_b(op),op, FUKU_REG_ECX, FUKU_OPERAND_SIZE_8);\
    asm_def_2op_test(name,_b, reg1, imm, FUKU_OPERAND_SIZE_8)\
    asm_def_2op_test(name,_b, op, imm, FUKU_OPERAND_SIZE_8)\
\
    test_2_arg( fuku_asm. _##name##_cl_w(reg1),reg1, FUKU_REG_ECX, FUKU_OPERAND_SIZE_16);\
    test_2_arg( fuku_asm. _##name##_cl_w(op),op, FUKU_REG_ECX, FUKU_OPERAND_SIZE_16);\
    asm_def_2op_test(name,_w, reg1, imm, FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_w, op, imm,FUKU_OPERAND_SIZE_16)\
\
    test_2_arg( fuku_asm. _##name##_cl_dw(reg1),reg1, FUKU_REG_ECX, FUKU_OPERAND_SIZE_32);\
    test_2_arg( fuku_asm. _##name##_cl_dw(op),op, FUKU_REG_ECX, FUKU_OPERAND_SIZE_32);\
    asm_def_2op_test(name,_dw, reg1, imm, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_dw, op, imm, FUKU_OPERAND_SIZE_32)\
\
    test_2_arg( fuku_asm. _##name##_cl_qw(reg1),reg1, FUKU_REG_ECX, FUKU_OPERAND_SIZE_64);\
    test_2_arg( fuku_asm. _##name##_cl_qw(op),op, FUKU_REG_ECX, FUKU_OPERAND_SIZE_64);\
    asm_def_2op_test(name,_qw, reg1, imm, FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_qw, op, imm, FUKU_OPERAND_SIZE_64)

#define test_gen_func_body_bit(name, op, reg1, reg2, imm) \
    asm_def_2op_test(name,_w, reg1, reg2,FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_w, reg1, imm,FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_w, op, reg2,FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_w, op, imm,FUKU_OPERAND_SIZE_16)\
\
    asm_def_2op_test(name,_dw, reg1, reg2, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_dw, reg1, imm, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_dw, op, reg2, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_dw, op, imm, FUKU_OPERAND_SIZE_32)\
\
    asm_def_2op_test(name,_qw, reg1, reg2, FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_qw, reg1, imm, FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_qw, op, reg2, FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_qw, op, imm, FUKU_OPERAND_SIZE_64)

#define test_asm_def_r_op_one_op(name, op, reg1) \
    asm_def_1op_test(name,_b, reg1, FUKU_OPERAND_SIZE_8)\
    asm_def_1op_test(name,_b, op, FUKU_OPERAND_SIZE_8)\
\
    asm_def_1op_test(name,_w, reg1,FUKU_OPERAND_SIZE_16)\
    asm_def_1op_test(name,_w, op,FUKU_OPERAND_SIZE_16)\
\
    asm_def_1op_test(name,_dw, reg1, FUKU_OPERAND_SIZE_32)\
    asm_def_1op_test(name,_dw, op, FUKU_OPERAND_SIZE_32)\
\
    asm_def_1op_test(name,_qw, reg1, FUKU_OPERAND_SIZE_64)\
    asm_def_1op_test(name,_qw, op, FUKU_OPERAND_SIZE_64)

#define asm_def_wdq_one_op(name) \
    asm_def_1op_test(name,_w, fuasm_reg)\
    asm_def_1op_test(name,_w, fuasm_op)\
    asm_def_1op_test(name,_w, fuasm_imm)\
\
    asm_def_1op_test(name,_dw, fuasm_reg)\
    asm_def_1op_test(name,_dw, fuasm_op)\
    asm_def_1op_test(name,_dw, fuasm_imm)\
\
    asm_def_1op_test(name,_qw, fuasm_reg)\
    asm_def_1op_test(name,_qw, fuasm_op)\
    asm_def_1op_test(name,_qw, fuasm_imm)\

#define asm_def_eip_one_op(name) \
    asm_def_1op_test(name,, fuasm_reg)\
    asm_def_1op_test(name,, fuasm_op)\
    asm_def_1op_test(name,, fuasm_imm)\

#define asm_def_noop_imm(name) \
    asm_def_noop_test(name,)\
    asm_def_1op_test(name,,fuasm_imm)\

#define asm_def_string_inst(name) \
    asm_def_noop_test(name,b)\
    asm_def_noop_test(name,w)\
    asm_def_noop_test(name,d)\
    asm_def_noop_test(name,q)\

#define asm_def_shXd_inst(name) \
    asm_def_3op(name,_w,fuasm_reg, fuasm_reg, fuasm_imm)\
    asm_def_3op(name,_w,fuasm_op, fuasm_reg, fuasm_imm)\
    asm_def_3op(name##_cl,_w,fuasm_reg, fuasm_reg)\
    asm_def_3op(name##_cl,_w,fuasm_op, fuasm_reg)\
\
    asm_def_3op(name,_dw,fuasm_reg, fuasm_reg, fuasm_imm)\
    asm_def_3op(name,_dw,fuasm_op, fuasm_reg, fuasm_imm)\
    asm_def_3op(name##_cl,_dw,fuasm_reg, fuasm_reg)\
    asm_def_3op(name##_cl,_dw,fuasm_op, fuasm_reg)\
\
    asm_def_3op(name,_qw,fuasm_reg, fuasm_reg, fuasm_imm)\
    asm_def_3op(name,_qw,fuasm_op, fuasm_reg, fuasm_imm)\
    asm_def_3op(name##_cl,_qw,fuasm_reg, fuasm_reg)\
    asm_def_3op(name##_cl,_qw,fuasm_op, fuasm_reg)


#define asm_def_direct_mov(name) \
    asm_def_2op_test(name,_b, fuasm_reg, fuasm_reg)\
    asm_def_2op_test(name,_b, fuasm_op, fuasm_reg)\
\
    asm_def_2op_test(name,_w, fuasm_reg, fuasm_reg)\
    asm_def_2op_test(name,_w, fuasm_op, fuasm_reg)\
\
    asm_def_2op_test(name,_dw, fuasm_reg, fuasm_reg)\
    asm_def_2op_test(name,_dw, fuasm_op, fuasm_reg)\
\
    asm_def_2op_test(name,_qw, fuasm_reg, fuasm_reg)\
    asm_def_2op_test(name,_qw, fuasm_op, fuasm_reg)

#define asm_def_movsxz(name) \
    asm_def_2op_test(name,_byte_w, fuasm_reg, fuasm_reg)\
    asm_def_2op_test(name,_byte_w, fuasm_op, fuasm_reg)\
    asm_def_2op_test(name,_byte_dw, fuasm_reg, fuasm_reg)\
    asm_def_2op_test(name,_byte_dw, fuasm_op, fuasm_reg)\
    asm_def_2op_test(name,_byte_qw, fuasm_reg, fuasm_reg)\
    asm_def_2op_test(name,_byte_qw, fuasm_op, fuasm_reg)\
\
    asm_def_2op_test(name,_word_dw, fuasm_reg, fuasm_reg)\
    asm_def_2op_test(name,_word_dw, fuasm_op, fuasm_reg)\
\
    asm_def_2op_test(name,_word_qw, fuasm_reg, fuasm_reg)\
    asm_def_2op_test(name,_word_qw, fuasm_op, fuasm_reg)
