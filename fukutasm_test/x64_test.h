#pragma once


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

#define asm_def_cond_jcc_1op_test(name, postfix, op, size) \
        for(uint8_t i = 0; i < 0x10;i ++){\
            fuku_asm_gen_name(_,name,postfix)(ctx, (fuku_condition)i, op);\
            test_1_arg_eip(*ctx.inst ,op, size);\
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
    test_2_arg( *ctx.inst ,reg1, reg_(FUKU_REG_ECX), FUKU_OPERAND_SIZE_8);\
    fuku_asm_gen_name(_,name,_cl_b)(ctx, byte_ptr(op));\
    test_2_arg( *ctx.inst ,byte_ptr(op), reg_(FUKU_REG_ECX), FUKU_OPERAND_SIZE_8);\
    asm_def_2op_test(name,_b, reg1, imm, FUKU_OPERAND_SIZE_8)\
    asm_def_2op_test(name,_b, byte_ptr(op), imm, FUKU_OPERAND_SIZE_8)\
\
    fuku_asm_gen_name(_,name,_cl_w)(ctx, reg1);\
    test_2_arg( *ctx.inst ,reg1, reg_(FUKU_REG_ECX), FUKU_OPERAND_SIZE_16);\
    fuku_asm_gen_name(_,name,_cl_w)(ctx, word_ptr(op));\
    test_2_arg( *ctx.inst ,word_ptr(op), reg_(FUKU_REG_ECX), FUKU_OPERAND_SIZE_16);\
    asm_def_2op_test(name,_w, reg1, imm, FUKU_OPERAND_SIZE_16)\
    asm_def_2op_test(name,_w, word_ptr(op), imm,FUKU_OPERAND_SIZE_16)\
\
    fuku_asm_gen_name(_,name,_cl_dw)(ctx, reg1);\
    test_2_arg( *ctx.inst ,reg1, reg_(FUKU_REG_ECX), FUKU_OPERAND_SIZE_32);\
    fuku_asm_gen_name(_,name,_cl_dw)(ctx, dword_ptr(op));\
    test_2_arg( *ctx.inst ,dword_ptr(op), reg_(FUKU_REG_ECX), FUKU_OPERAND_SIZE_32);\
    asm_def_2op_test(name,_dw, reg1, imm, FUKU_OPERAND_SIZE_32)\
    asm_def_2op_test(name,_dw, dword_ptr(op), imm, FUKU_OPERAND_SIZE_32)\
\
    fuku_asm_gen_name(_,name,_cl_qw)(ctx, reg1);\
    test_2_arg( *ctx.inst ,reg1, reg_(FUKU_REG_ECX), FUKU_OPERAND_SIZE_64);\
    fuku_asm_gen_name(_,name,_cl_qw)(ctx, qword_ptr(op));\
    test_2_arg( *ctx.inst , qword_ptr(op), reg_(FUKU_REG_ECX), FUKU_OPERAND_SIZE_64);\
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
    fuku_asm_gen_name(_,name,)(ctx, imm);\
    test_1_arg_eip(*ctx.inst ,imm, size);

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


void x64_test_internl(fukutasm::fuku_assambler_ctx& ctx) {

    fuku_register reg1 = FUKU_REG_EDX;
    fuku_register reg2 = FUKU_REG_EDX;
    fuku_immediate imm(0x12345678);


    uint64_t vall = 0x1000000;

        //operand test
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EAX), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ECX), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EDX), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EBX), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EBP), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ESP), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EDI), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ESI), reg1, FUKU_OPERAND_SIZE_32)

        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EAX, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ECX, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EDX, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EBX, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EBP, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ESP, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EDI, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ESI, vall), reg1, FUKU_OPERAND_SIZE_32)

        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EAX, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ECX, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EDX, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EBX, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EBP, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
        //asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ESP, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EDI, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ESI, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)

        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EAX, FUKU_REG_EAX, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ECX, FUKU_REG_EAX, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EDX, FUKU_REG_EAX, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EBX, FUKU_REG_EAX, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EBP, FUKU_REG_EAX, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ESP, FUKU_REG_EAX, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_EDI, FUKU_REG_EAX, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(mov, _dw, dword_ptr(FUKU_REG_ESI, FUKU_REG_EAX, FUKU_OPERAND_SCALE_2, vall), reg1, FUKU_OPERAND_SIZE_32)

        //Data Transfer Instructions
        test_asm_def_full(mov, op, reg1, reg2, imm)
        asm_def_cond_2op_test(cmovcc, _w, reg1, reg2, FUKU_OPERAND_SIZE_16)
        asm_def_cond_2op_test(cmovcc, _w, reg1, word_ptr(op), FUKU_OPERAND_SIZE_16)
        asm_def_cond_2op_test(cmovcc, _dw, reg1, reg2, FUKU_OPERAND_SIZE_32)
        asm_def_cond_2op_test(cmovcc, _dw, reg1, dword_ptr(op), FUKU_OPERAND_SIZE_32)
        asm_def_cond_2op_test(cmovcc, _qw, reg1, reg2, FUKU_OPERAND_SIZE_64)
        asm_def_cond_2op_test(cmovcc, _qw, reg1, qword_ptr(op), FUKU_OPERAND_SIZE_64)
        test_asm_def_direct_mov(xchg, op, reg1, reg2)
        asm_def_1op_test(bswap_dw, , reg1, FUKU_OPERAND_SIZE_32)
        asm_def_1op_test(bswap_qw, , reg1, FUKU_OPERAND_SIZE_64)
        test_asm_def_direct_mov(xadd, op, reg1, reg2)
        test_asm_def_direct_mov(cmpxchg, op, reg1, reg2)
        asm_def_1op_test(cmpxchg8b, , dword_ptr(op), FUKU_OPERAND_SIZE_32)
        asm_def_1op_test(cmpxchg16b, , qword_ptr(op), FUKU_OPERAND_SIZE_64)

        test_asm_def_wdq_one_op(push, op, reg1, imm)
        asm_def_1op_test(pop, _w, reg1, FUKU_OPERAND_SIZE_16)
        asm_def_1op_test(pop, _dw, reg1, FUKU_OPERAND_SIZE_32)
        asm_def_1op_test(pop, _qw, reg1, FUKU_OPERAND_SIZE_64)
        asm_def_1op_test(pop, _w, word_ptr(op), FUKU_OPERAND_SIZE_16)
        asm_def_1op_test(pop, _dw, dword_ptr(op), FUKU_OPERAND_SIZE_32)
        asm_def_1op_test(pop, _qw, qword_ptr(op), FUKU_OPERAND_SIZE_64)
        asm_def_noop_test(cwd, , FUKU_OPERAND_SIZE_16)
        asm_def_noop_test(cdq, , FUKU_OPERAND_SIZE_32)
        asm_def_noop_test(cqo, , FUKU_OPERAND_SIZE_64)
        test_asm_def_movsxz(movzx, op, reg1, reg2)
        test_asm_def_movsxz(movsx, op, reg1, reg2)
        // asm_def_2op_test(movsx,_dword_dw, reg1, reg2, FUKU_OPERAND_SIZE_32)
        // asm_def_2op_test(movsx,_dword_dw, reg1, dword_ptr(op), FUKU_OPERAND_SIZE_32)
       //  asm_def_2op_test(movsx,_dword_qw, reg1, reg2, FUKU_OPERAND_SIZE_64)
       //  asm_def_2op_test(movsx,_dword_qw, reg1, qword_ptr(op), FUKU_OPERAND_SIZE_64)

        //Binary Arithmetic Instructions
        asm_def_2op_test(adcx, _dw, reg1, reg2, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(adcx, _dw, reg1, dword_ptr(op), FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(adcx, _qw, reg1, reg2, FUKU_OPERAND_SIZE_64)
        asm_def_2op_test(adcx, _qw, reg1, qword_ptr(op), FUKU_OPERAND_SIZE_64)
        asm_def_2op_test(adox, _dw, reg1, reg2, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(adox, _dw, reg1, dword_ptr(op), FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(adox, _qw, reg1, reg2, FUKU_OPERAND_SIZE_64)
        asm_def_2op_test(adox, _qw, reg1, qword_ptr(op), FUKU_OPERAND_SIZE_64)
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

        //Logical Instructions Instructions
        test_asm_def_full(and, op, reg1, reg2, imm)
        test_asm_def_full(or , op, reg1, reg2, imm)
        test_asm_def_full(xor, op, reg1, reg2, imm)
        test_asm_def_r_op_one_op(not, op, reg1)

        //Shift and Rotate Instructions
        test_asm_def_full_shift(sar, op, reg2, imm)
        test_asm_def_full_shift(shr, op, reg2, imm)
        test_asm_def_full_shift(shl, op, reg2, imm)
            //asm_def_shXd_inst(shrd)
            //asm_def_shXd_inst(shld)
        test_asm_def_full_shift(ror, op, reg2, imm)
        test_asm_def_full_shift(rol, op, reg2, imm)
        test_asm_def_full_shift(rcr, op, reg2, imm)
        test_asm_def_full_shift(rcl, op, reg2, imm)

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
        asm_def_2op_test(popcnt, _dw, reg1, reg2, FUKU_OPERAND_SIZE_16)
        asm_def_2op_test(popcnt, _dw, reg1, word_ptr(op), FUKU_OPERAND_SIZE_16)
        asm_def_2op_test(popcnt, _dw, reg1, reg2, FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(popcnt, _dw, reg1, dword_ptr(op), FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(popcnt, _qw, reg1, reg2, FUKU_OPERAND_SIZE_64)
        asm_def_2op_test(popcnt, _qw, reg1, qword_ptr(op), FUKU_OPERAND_SIZE_64)

        //Control Transfer Instructions
        test_asm_def_eip_one_op(jmp, op, reg1, imm, FUKU_OPERAND_SIZE_64)
        asm_def_cond_jcc_1op_test(jcc, , imm, FUKU_OPERAND_SIZE_32)
        test_asm_def_eip_one_op(call, op, reg1, imm, FUKU_OPERAND_SIZE_64)
        test_asm_def_noop_imm(ret, imm)
        asm_def_noop_test(int3, , FUKU_OPERAND_SIZE_32)

        // asm_def_2op_test(enter,,fuasm_imm, uint8_t)//enter size, nestinglevel
        asm_def_noop_test(, leave_, FUKU_OPERAND_SIZE_32)

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
        asm_def_noop_test(pushf, , FUKU_OPERAND_SIZE_16)       //pushf  w  flags
        asm_def_noop_test(pushfq, , FUKU_OPERAND_SIZE_64)      //pushfq qw flags
        asm_def_noop_test(popf, , FUKU_OPERAND_SIZE_16)        //popf   w  flags
        asm_def_noop_test(popfq, , FUKU_OPERAND_SIZE_64)       //popfq  qw flags

        //Miscellaneous Instructions
        asm_def_2op_test(lea, _w, reg1, word_ptr(op), FUKU_OPERAND_SIZE_16)
        asm_def_2op_test(lea, _dw, reg1, dword_ptr(op), FUKU_OPERAND_SIZE_32)
        asm_def_2op_test(lea, _qw, reg1, qword_ptr(op), FUKU_OPERAND_SIZE_64)

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
}

void x64_test() {

    fuku_assambler fasm(FUKU_ASSAMBLER_ARCH_X64);

    fuku_inst inst;

    fukutasm::fuku_assambler_ctx& ctx = fasm.get_context();
    ctx.arch = FUKU_ASSAMBLER_ARCH_X64;
    ctx.short_cfg = 0xFF;
    ctx.inst = &inst;


    cs_open(CS_ARCH_X86, CS_MODE_64, &cap_handle);
    cs_option(cap_handle, CS_OPT_DETAIL, CS_OPT_ON);


    for (size_t i_1 = 0; i_1 < 2; i_1++) {
        for (size_t i_2 = 0; i_2 < 2; i_2++) {
            for (size_t i_3 = 0; i_3 < 2; i_3++) {

                ctx.short_cfg =
                    (i_1 ? FUKU_ASM_SHORT_CFG_USE_EAX_SHORT : 0) |
                    (i_2 ? FUKU_ASM_SHORT_CFG_USE_DISP_SHORT : 0) |
                    (i_3 ? FUKU_ASM_SHORT_CFG_USE_IMM_SHORT : 0);

                printf("START CHECK [X64] [EAX_SHORT : %s] [DISP_SHORT : %s] [IMM_SHORT : %s] \n",
                    (i_1 ? "ON " : "OFF"),
                    (i_2 ? "ON " : "OFF"),
                    (i_3 ? "ON " : "OFF")
                );

                x64_test_internl(ctx);
            }
        }
    }

    cs_close(&cap_handle);
}