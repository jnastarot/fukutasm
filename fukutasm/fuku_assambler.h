#pragma once

enum fuku_t0_types {
    FUKU_T0_REGISTER,
    FUKU_T0_OPERAND,
    FUKU_T0_IMMEDIATE,
};


class fuku_type {
    fuku_t0_types type;
    fuku_register reg;
    const fuku_operand *op;
    const fuku_immediate *imm;
public:
    fuku_type(fuku_register reg);
    fuku_type(const fuku_operand& op);
    fuku_type(const fuku_immediate& imm);
    ~fuku_type();
public:
    fuku_t0_types get_type();
    fuku_register get_register();
    const fuku_operand  &get_operand();
    const fuku_immediate &get_immediate();
};

class fuku_assambler {
public:
    fuku_assambler();
    ~fuku_assambler();

public:
    fuku_instruction mov(fuku_type dst, fuku_type src);
    fuku_instruction cmovcc(fuku_condition cond, fuku_type dst, fuku_type src);

    /*
        asm_def_cond_2op(cmovcc, _w, fuasm_reg, fuasm_reg)
        asm_def_cond_2op(cmovcc, _w, fuasm_reg, fuasm_op)
        asm_def_cond_2op(cmovcc, _dw, fuasm_reg, fuasm_reg)
        asm_def_cond_2op(cmovcc, _dw, fuasm_reg, fuasm_op)
        asm_def_cond_2op(cmovcc, _qw, fuasm_reg, fuasm_reg)
        asm_def_cond_2op(cmovcc, _qw, fuasm_reg, fuasm_op)
        asm_def_direct_mov(xchg)
        asm_def_1op(bswap_dw, , fuasm_reg)
        asm_def_1op(bswap_qw, , fuasm_reg)
        asm_def_direct_mov(xadd)
        asm_def_direct_mov(cmpxchg)
        asm_def_1op(cmpxchg8b, , fuasm_op)
        asm_def_1op(cmpxchg16b, , fuasm_op)
        asm_def_wdq_one_op(push)
        asm_def_1op(pop, _w, fuasm_reg)
        asm_def_1op(pop, _dw, fuasm_reg)
        asm_def_1op(pop, _qw, fuasm_reg)
        asm_def_1op(pop, _w, fuasm_op)
        asm_def_1op(pop, _dw, fuasm_op)
        asm_def_1op(pop, _qw, fuasm_op)
        asm_def_noop(cwd, )
        asm_def_noop(cdq, )
        asm_def_noop(cqo, )
        asm_def_movsxz(movzx)
        asm_def_movsxz(movsx)
        asm_def_2op(movsx, _dword_dw, fuasm_reg, fuasm_reg)
        asm_def_2op(movsx, _dword_dw, fuasm_reg, fuasm_op)
        asm_def_2op(movsx, _dword_qw, fuasm_reg, fuasm_reg)
        asm_def_2op(movsx, _dword_qw, fuasm_reg, fuasm_op)
        //Binary Arithmetic Instructions
        asm_def_2op(adcx, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(adcx, _dw, fuasm_reg, fuasm_op)
        asm_def_2op(adcx, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(adcx, _qw, fuasm_reg, fuasm_op)
        asm_def_2op(adox, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(adox, _dw, fuasm_reg, fuasm_op)
        asm_def_2op(adox, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(adox, _qw, fuasm_reg, fuasm_op)
        asm_def_full(add)
        asm_def_full(adc)
        asm_def_full(sub)
        asm_def_full(sbb)
        asm_def_r_op_one_op(imul)
        asm_def_r_op_one_op(mul)
        asm_def_r_op_one_op(idiv)
        asm_def_r_op_one_op(div)
        asm_def_r_op_one_op(inc)
        asm_def_r_op_one_op(dec)
        asm_def_r_op_one_op(neg)
        asm_def_full(cmp)
        //Decimal Arithmetic Instructions
        asm_def_noop(daa, )
        asm_def_noop(das, )
        asm_def_noop(aaa, )
        asm_def_noop(aas, )
        asm_def_noop_imm(aam)
        asm_def_noop_imm(aad)
        //Logical Instructions Instructions
        asm_def_full(and)
        asm_def_full(or )
        asm_def_full(xor)
        asm_def_r_op_one_op(not)
        //Shift and Rotate Instructions
        asm_def_full_shift(sar)
        asm_def_full_shift(shr)
        asm_def_full_shift(shl)
        asm_def_shXd_inst(shrd)
        asm_def_shXd_inst(shld)
        asm_def_full_shift(ror)
        asm_def_full_shift(rol)
        asm_def_full_shift(rcr)
        asm_def_full_shift(rcl)
        //Bit and Byte Instructions
        gen_func_body_bit(bt)
        gen_func_body_bit(bts)
        gen_func_body_bit(btr)
        gen_func_body_bit(btc)
        asm_def_cond_1op(setcc, , fuasm_reg)
        asm_def_cond_1op(setcc, , fuasm_op)
        asm_def_2op(test, _b, fuasm_op, fuasm_reg)
        asm_def_2op(test, _b, fuasm_reg, fuasm_reg)
        asm_def_2op(test, _b, fuasm_reg, fuasm_imm)
        asm_def_2op(test, _b, fuasm_op, fuasm_imm)
        asm_def_2op(test, _w, fuasm_op, fuasm_reg)
        asm_def_2op(test, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(test, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(test, _w, fuasm_op, fuasm_imm)
        asm_def_2op(test, _dw, fuasm_op, fuasm_reg)
        asm_def_2op(test, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(test, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(test, _dw, fuasm_op, fuasm_imm)
        asm_def_2op(test, _qw, fuasm_op, fuasm_reg)
        asm_def_2op(test, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(test, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(test, _qw, fuasm_op, fuasm_imm)
        asm_def_2op(popcnt, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(popcnt, _w, fuasm_reg, fuasm_op)
        asm_def_2op(popcnt, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(popcnt, _dw, fuasm_reg, fuasm_op)
        asm_def_2op(popcnt, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(popcnt, _qw, fuasm_reg, fuasm_op)
        //Control Transfer Instructions
        asm_def_eip_one_op(jmp)
        asm_def_cond_1op(jcc, , fuasm_imm)
        asm_def_eip_one_op(call)
        asm_def_noop_imm(ret)
        asm_def_noop(int3, )
        asm_def_2op(enter, , fuasm_imm, uint8_t)//enter size, nestinglevel
        asm_def_noop(, leave_)
        //String Instructions
        asm_def_noop(outsb, )
        asm_def_noop(outsw, )
        asm_def_noop(outsd, )
        asm_def_string_inst(movs)
        asm_def_string_inst(cmps)
        asm_def_string_inst(scas)
        asm_def_string_inst(lods)
        asm_def_string_inst(stos)
        //Flag Control (EFLAG) Instructions
        asm_def_noop(stc, )
        asm_def_noop(clc, )
        asm_def_noop(cmc, )
        asm_def_noop(cld, )
        asm_def_noop(std, )
        asm_def_noop(lahf, )
        asm_def_noop(sahf, )
        asm_def_noop(pusha, )       //pusha  w  regs
        asm_def_noop(pushad, )      //pushad dw/qw regs
        asm_def_noop(popa, )        //popa   w  regs
        asm_def_noop(popad, )       //popad  dw/qw regs
        asm_def_noop(pushf, )       //pushf  w  flags
        asm_def_noop(pushfd, )      //pushfd dw flags
        asm_def_noop(pushfq, )      //pushfq qw flags
        asm_def_noop(popf, )        //popf   w  flags
        asm_def_noop(popfd, )       //popfd  dw flags
        asm_def_noop(popfq, )       //popfq  qw flags
    //Miscellaneous Instructions
        asm_def_2op(lea, _w, fuasm_reg, fuasm_op)
        asm_def_2op(lea, _dw, fuasm_reg, fuasm_op)
        asm_def_2op(lea, _qw, fuasm_reg, fuasm_op)
        asm_def_1op(nop, , uint8_t)
        asm_def_noop(ud2, )
        asm_def_noop(cpuid, )
        //Random Number Generator Instructions
        asm_def_1op(rdrand, _w, fuasm_reg)
        asm_def_1op(rdrand, _dw, fuasm_reg)
        asm_def_1op(rdrand, _qw, fuasm_reg)
        asm_def_1op(rdseed, _w, fuasm_reg)
        asm_def_1op(rdseed, _dw, fuasm_reg)
        asm_def_1op(rdseed, _qw, fuasm_reg)
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
        asm_def_noop(hlt, )
        asm_def_noop(rdtsc, )
        asm_def_noop(lfence, )
        */
};

