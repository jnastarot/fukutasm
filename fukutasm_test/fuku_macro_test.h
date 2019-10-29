#pragma once

#define fuku_asm_gen_name(prefix, sname, postfix) prefix##sname##postfix

#define asm_def_noop_test(name, postfix, size) \
        fuku_asm_gen_name(_,name,postfix)(ctx);\
        test_no_arg( *ctx.inst , size);

#define asm_def_1op_test(name, postfix, op, size) \
        fuku_asm_gen_name(_,name,postfix)(ctx, op);\
        test_1_arg( *ctx.inst, op, size);

#define asm_def_2op_test(name, postfix, op1, op2, size) \
        fuku_asm_gen_name(_,name,postfix)(ctx,op1, op2);\
        test_2_arg( *ctx.inst ,op1, op2, size);

#define asm_def_cond_1op_test(name, postfix, op, size) \
        for(uint8_t i = 0; i < 0x10;i ++){\
            fuku_asm_gen_name(_,name,postfix)(ctx, (fuku_condition)i, op);\
            test_1_arg(*ctx.inst , op, size);\
        }

#define asm_def_cond_2op_test(name, postfix, op1,op2, size) \
        for(uint8_t i = 0; i < 0x10;i ++){\
            fuku_asm_gen_name(_,name,postfix)(ctx, (fuku_condition)i, op1, op2);\
            test_2_arg(*ctx.inst, op1, op2, size);\
        }

#define test_asm_def_full(name, op, reg1, reg2, imm)\
    asm_def_2op_test(name,_b, reg1, reg2, FUKU_OPERAND_SIZE_8)\
    asm_def_2op_test(name,_b, reg1, imm, FUKU_OPERAND_SIZE_8)\
    asm_def_2op_test(name,_b, reg1, byte_ptr(op), FUKU_OPERAND_SIZE_8)\
    asm_def_2op_test(name,_b, byte_ptr(op), reg2, FUKU_OPERAND_SIZE_8)\
    asm_def_2op_test(name,_b, byte_ptr(op), imm, FUKU_OPERAND_SIZE_8)\
\
    asm_def_2op_test(name,_w, reg1, reg2, FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_w, reg1, imm, FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_w, reg1, word_ptr(op), FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_w, word_ptr(op), reg2, FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_w, word_ptr(op), imm, FUKU_OPERAND_SIZE_16)\
\
    asm_def_2op_test(name,_dw, reg1, reg2, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_dw, reg1, imm, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_dw, reg1, dword_ptr(op), FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_dw, dword_ptr(op), reg2, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_dw, dword_ptr(op), imm, FUKU_OPERAND_SIZE_32)\
\
    asm_def_2op_test(name,_qw, reg1, reg2, FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_qw, reg1, imm, FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_qw, reg1, qword_ptr(op), FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_qw, qword_ptr(op), reg2, FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_qw, qword_ptr(op), imm, FUKU_OPERAND_SIZE_64)

#define test_asm_def_full_shift(name, op, reg1, imm)\
    fuku_asm_gen_name(_,name,_cl_b)(ctx, reg1);\
    test_2_arg( *ctx.inst ,reg1, FUKU_REG_ECX, FUKU_OPERAND_SIZE_8);\
    fuku_asm_gen_name(_,name,_cl_b)(ctx, byte_ptr(op));\
    test_2_arg( *ctx.inst ,byte_ptr(op), FUKU_REG_ECX, FUKU_OPERAND_SIZE_8);\
    asm_def_2op_test(name,_b, reg1, imm, FUKU_OPERAND_SIZE_8)\
    asm_def_2op_test(name,_b, byte_ptr(op), imm, FUKU_OPERAND_SIZE_8)\
\
    fuku_asm_gen_name(_,name,_cl_w)(ctx, reg1);\
    test_2_arg( *ctx.inst ,reg1, FUKU_REG_ECX, FUKU_OPERAND_SIZE_16);\
    fuku_asm_gen_name(_,name,_cl_w)(ctx, word_ptr(op));\
    test_2_arg( *ctx.inst ,word_ptr(op), FUKU_REG_ECX, FUKU_OPERAND_SIZE_16);\
    asm_def_2op_test(name,_w, reg1, imm, FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_w, word_ptr(op), imm,FUKU_OPERAND_SIZE_16)\
\
    fuku_asm_gen_name(_,name,_cl_dw)(ctx, reg1);\
    test_2_arg( *ctx.inst ,reg1, FUKU_REG_ECX, FUKU_OPERAND_SIZE_32);\
    fuku_asm_gen_name(_,name,_cl_dw)(ctx, dword_ptr(op));\
    test_2_arg( *ctx.inst ,dword_ptr(op), FUKU_REG_ECX, FUKU_OPERAND_SIZE_32);\
    asm_def_2op_test(name,_dw, reg1, imm, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_dw, dword_ptr(op), imm, FUKU_OPERAND_SIZE_32)\
\
    fuku_asm_gen_name(_,name,_cl_qw)(ctx, reg1);\
    test_2_arg( *ctx.inst ,reg1, FUKU_REG_ECX, FUKU_OPERAND_SIZE_64);\
    fuku_asm_gen_name(_,name,_cl_qw)(ctx, qword_ptr(op));\
    test_2_arg( *ctx.inst , qword_ptr(op), FUKU_REG_ECX, FUKU_OPERAND_SIZE_64);\
    asm_def_2op_test(name,_qw, reg1, imm, FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_qw, qword_ptr(op), imm, FUKU_OPERAND_SIZE_64)

#define test_gen_func_body_bit(name, op, reg1, reg2, imm) \
    asm_def_2op_test(name,_w, reg1, reg2,FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_w, reg1, imm,FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_w, word_ptr(op), reg2,FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_w, word_ptr(op), imm,FUKU_OPERAND_SIZE_16)\
\
    asm_def_2op_test(name,_dw, reg1, reg2, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_dw, reg1, imm, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_dw, dword_ptr(op), reg2, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_dw, dword_ptr(op), imm, FUKU_OPERAND_SIZE_32)\
\
    asm_def_2op_test(name,_qw, reg1, reg2, FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_qw, reg1, imm, FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_qw, qword_ptr(op), reg2, FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_qw, qword_ptr(op), imm, FUKU_OPERAND_SIZE_64)

#define test_asm_def_r_op_one_op(name, op, reg1) \
    asm_def_1op_test(name,_b, reg1, FUKU_OPERAND_SIZE_8)\
    asm_def_1op_test(name,_b, byte_ptr(op), FUKU_OPERAND_SIZE_8)\
\
    asm_def_1op_test(name,_w, reg1,FUKU_OPERAND_SIZE_16)\
    asm_def_1op_test(name,_w, word_ptr(op),FUKU_OPERAND_SIZE_16)\
\
    asm_def_1op_test(name,_dw, reg1, FUKU_OPERAND_SIZE_32)\
    asm_def_1op_test(name,_dw, dword_ptr(op), FUKU_OPERAND_SIZE_32)\
\
    asm_def_1op_test(name,_qw, reg1, FUKU_OPERAND_SIZE_64)\
    asm_def_1op_test(name,_qw, qword_ptr(op), FUKU_OPERAND_SIZE_64)

#define test_asm_def_wdq_one_op(name, op, reg1, imm) \
    asm_def_1op_test(name,_w, reg1, FUKU_OPERAND_SIZE_16)\
    asm_def_1op_test(name,_w, byte_ptr(op), FUKU_OPERAND_SIZE_16)\
    asm_def_1op_test(name,_w, imm, FUKU_OPERAND_SIZE_16)\
\
    asm_def_1op_test(name,_dw, reg1,FUKU_OPERAND_SIZE_32)\
    asm_def_1op_test(name,_dw, dword_ptr(op),FUKU_OPERAND_SIZE_32)\
    asm_def_1op_test(name,_dw, imm,FUKU_OPERAND_SIZE_32)\
\
    asm_def_1op_test(name,_qw, reg1, FUKU_OPERAND_SIZE_64)\
    asm_def_1op_test(name,_qw, qword_ptr(op), FUKU_OPERAND_SIZE_64)\
    asm_def_1op_test(name,_qw, imm, FUKU_OPERAND_SIZE_64)\

#define test_asm_def_eip_one_op(name, op, reg1, imm, size) \
    asm_def_1op_test(name,, reg1, size)\
    asm_def_1op_test(name,, dword_ptr(op), size)\
    asm_def_1op_test(name,, imm, size)\

#define test_asm_def_noop_imm(name, imm) \
    asm_def_noop_test(name,,FUKU_OPERAND_SIZE_32)\
    asm_def_1op_test(name,,imm,FUKU_OPERAND_SIZE_32)\

#define test_asm_def_string_inst(name) \
    asm_def_noop_test(name,b, FUKU_OPERAND_SIZE_8)\
    asm_def_noop_test(name,w, FUKU_OPERAND_SIZE_16)\
    asm_def_noop_test(name,d,FUKU_OPERAND_SIZE_32)\
    asm_def_noop_test(name,q, FUKU_OPERAND_SIZE_64)\

#define test_asm_def_shXd_inst(name) \
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


#define test_asm_def_direct_mov(name, op, reg1, reg2) \
    asm_def_2op_test(name,_b, reg1, reg2, FUKU_OPERAND_SIZE_8)\
    asm_def_2op_test(name,_b, byte_ptr(op), reg2, FUKU_OPERAND_SIZE_8)\
\
    asm_def_2op_test(name,_w, reg1, reg2, FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_w, word_ptr(op), reg2, FUKU_OPERAND_SIZE_16)\
\
    asm_def_2op_test(name,_dw, reg1, reg2, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_dw, dword_ptr(op), reg2, FUKU_OPERAND_SIZE_32)\
\
    asm_def_2op_test(name,_qw, reg1, reg2, FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_qw, qword_ptr(op), reg2, FUKU_OPERAND_SIZE_64)

#define test_asm_def_movsxz(name, op, reg1, reg2) \
    asm_def_2op_test(name,_byte_w, reg1, reg2, FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_byte_w, reg1 ,  word_ptr(op), FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_byte_dw, reg1, reg2, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_byte_dw, reg1 , dword_ptr(op), FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_byte_qw, reg1, reg2, FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_byte_qw, reg1 , qword_ptr(op), FUKU_OPERAND_SIZE_64)\
\
    asm_def_2op_test(name,_word_dw, reg1, reg2, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_word_dw, reg1 , dword_ptr(op), FUKU_OPERAND_SIZE_32)\
\
    asm_def_2op_test(name,_word_qw, reg1, reg2, FUKU_OPERAND_SIZE_64)\
    asm_def_2op_test(name,_word_qw, reg1 , qword_ptr(op), FUKU_OPERAND_SIZE_64)
