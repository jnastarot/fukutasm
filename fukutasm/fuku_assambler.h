#pragma once


#define asm_def_full(name)\
    asm_def_2op(name,_b, fuasm_reg, fuasm_reg)\
    asm_def_2op(name,_b, fuasm_reg, fuasm_imm)\
    asm_def_2op(name,_b, fuasm_reg, fuasm_op)\
    asm_def_2op(name,_b, fuasm_op, fuasm_reg)\
    asm_def_2op(name,_b, fuasm_op, fuasm_imm)\
\
    asm_def_2op(name,_w, fuasm_reg, fuasm_reg)\
    asm_def_2op(name,_w, fuasm_reg, fuasm_imm)\
    asm_def_2op(name,_w, fuasm_reg, fuasm_op)\
    asm_def_2op(name,_w, fuasm_op, fuasm_reg)\
    asm_def_2op(name,_w, fuasm_op, fuasm_imm)\
\
    asm_def_2op(name,_dw, fuasm_reg, fuasm_reg)\
    asm_def_2op(name,_dw, fuasm_reg, fuasm_imm)\
    asm_def_2op(name,_dw, fuasm_reg, fuasm_op)\
    asm_def_2op(name,_dw, fuasm_op, fuasm_reg)\
    asm_def_2op(name,_dw, fuasm_op, fuasm_imm)\
\
    asm_def_2op(name,_qw, fuasm_reg, fuasm_reg)\
    asm_def_2op(name,_qw, fuasm_reg, fuasm_imm)\
    asm_def_2op(name,_qw, fuasm_reg, fuasm_op)\
    asm_def_2op(name,_qw, fuasm_op, fuasm_reg)\
    asm_def_2op(name,_qw, fuasm_op, fuasm_imm)

#define asm_def_full_shift(name)\
    asm_def_1op(name##_cl,_b, fuasm_reg)\
    asm_def_1op(name##_cl,_b, fuasm_op)\
    asm_def_2op(name,_b, fuasm_reg, fuasm_imm)\
    asm_def_2op(name,_b, fuasm_op, fuasm_imm)\
\
    asm_def_1op(name##_cl,_w, fuasm_reg)\
    asm_def_1op(name##_cl,_w, fuasm_op)\
    asm_def_2op(name,_w, fuasm_reg, fuasm_imm)\
    asm_def_2op(name,_w, fuasm_op, fuasm_imm)\
\
    asm_def_1op(name##_cl,_dw, fuasm_reg)\
    asm_def_1op(name##_cl,_dw, fuasm_op)\
    asm_def_2op(name,_dw, fuasm_reg, fuasm_imm)\
    asm_def_2op(name,_dw, fuasm_op, fuasm_imm)\
\
    asm_def_1op(name##_cl,_qw, fuasm_reg)\
    asm_def_1op(name##_cl,_qw, fuasm_op)\
    asm_def_2op(name,_qw, fuasm_reg, fuasm_imm)\
    asm_def_2op(name,_qw, fuasm_op, fuasm_imm)

#define gen_func_body_bit(name) \
    asm_def_2op(name,_w, fuasm_reg, fuasm_reg)\
    asm_def_2op(name,_w, fuasm_reg, fuasm_imm)\
    asm_def_2op(name,_w, fuasm_op, fuasm_reg)\
    asm_def_2op(name,_w, fuasm_op, fuasm_imm)\
\
    asm_def_2op(name,_dw, fuasm_reg, fuasm_reg)\
    asm_def_2op(name,_dw, fuasm_reg, fuasm_imm)\
    asm_def_2op(name,_dw, fuasm_op, fuasm_reg)\
    asm_def_2op(name,_dw, fuasm_op, fuasm_imm)\
\
    asm_def_2op(name,_qw, fuasm_reg, fuasm_reg)\
    asm_def_2op(name,_qw, fuasm_reg, fuasm_imm)\
    asm_def_2op(name,_qw, fuasm_op, fuasm_reg)\
    asm_def_2op(name,_qw, fuasm_op, fuasm_imm)

#define asm_def_r_op_one_op(name) \
    asm_def_1op(name,_b, fuasm_reg)\
    asm_def_1op(name,_b, fuasm_op)\
\
    asm_def_1op(name,_w, fuasm_reg)\
    asm_def_1op(name,_w, fuasm_op)\
\
    asm_def_1op(name,_dw, fuasm_reg)\
    asm_def_1op(name,_dw, fuasm_op)\
\
    asm_def_1op(name,_qw, fuasm_reg)\
    asm_def_1op(name,_qw, fuasm_op)

#define asm_def_wdq_one_op(name) \
    asm_def_1op(name,_w, fuasm_reg)\
    asm_def_1op(name,_w, fuasm_op)\
    asm_def_1op(name,_w, fuasm_imm)\
\
    asm_def_1op(name,_dw, fuasm_reg)\
    asm_def_1op(name,_dw, fuasm_op)\
    asm_def_1op(name,_dw, fuasm_imm)\
\
    asm_def_1op(name,_qw, fuasm_reg)\
    asm_def_1op(name,_qw, fuasm_op)\
    asm_def_1op(name,_qw, fuasm_imm)\

#define asm_def_eip_one_op(name) \
    asm_def_1op(name,, fuasm_reg)\
    asm_def_1op(name,, fuasm_op)\
    asm_def_1op(name,, fuasm_imm)\

#define asm_def_noop_imm(name) \
    asm_def_noop(name,)\
    asm_def_1op(name,,fuasm_imm)\

#define asm_def_string_inst(name) \
    asm_def_noop(name,b)\
    asm_def_noop(name,w)\
    asm_def_noop(name,d)\
    asm_def_noop(name,q)\

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
    asm_def_2op(name,_b, fuasm_reg, fuasm_reg)\
    asm_def_2op(name,_b, fuasm_op, fuasm_reg)\
\
    asm_def_2op(name,_w, fuasm_reg, fuasm_reg)\
    asm_def_2op(name,_w, fuasm_op, fuasm_reg)\
\
    asm_def_2op(name,_dw, fuasm_reg, fuasm_reg)\
    asm_def_2op(name,_dw, fuasm_op, fuasm_reg)\
\
    asm_def_2op(name,_qw, fuasm_reg, fuasm_reg)\
    asm_def_2op(name,_qw, fuasm_op, fuasm_reg)

#define asm_def_movsxz(name) \
    asm_def_2op(name,_b, fuasm_reg, fuasm_reg)\
    asm_def_2op(name,_b, fuasm_op, fuasm_reg)\
\
    asm_def_2op(name,_w, fuasm_reg, fuasm_reg)\
    asm_def_2op(name,_w, fuasm_op, fuasm_reg)

class fuku_assambler{
    fuku_assambler_arch arch;

    uint8_t bytecode[16];
    uint8_t length;

    uint8_t displacment_offset;
    uint8_t immediate_offset;

    uint8_t short_cfg;
    
    void clear_space();

    void emit_b(uint8_t x);
    void emit_w(uint16_t x);
    void emit_dw(uint32_t x);
    void emit_qw(uint64_t x);

    void emit_immediate_b(const fuku_immediate& src);
    void emit_immediate_w(const fuku_immediate& src);
    void emit_immediate_dw(const fuku_immediate& src);
    void emit_immediate_qw(const fuku_immediate& src);

    void emit_rex_64();
    void emit_rex_64(fuku_register rm_reg, fuku_register reg);
    void emit_rex_64(const fuku_operand& rm_reg, fuku_register reg);
    void emit_rex_64(fuku_register rm_reg);
    void emit_rex_64(const fuku_operand& rm_reg);
    void emit_rex_32(fuku_register rm_reg, fuku_register reg);
    void emit_rex_32(const fuku_operand& rm_reg, fuku_register reg);
    void emit_rex_32(fuku_register rm_reg);
    void emit_rex_32(const fuku_operand& rm_reg);
    void emit_optional_rex_32(fuku_register rm_reg, fuku_register reg);
    void emit_optional_rex_32(const fuku_operand& rm_reg, fuku_register reg);
    void emit_optional_rex_32(fuku_register rm_reg);
    void emit_optional_rex_32(const fuku_operand& rm_reg);

    void emit_modrm(fuku_register rm_reg, fuku_register reg);
    void emit_modrm(fuku_register rm_reg, int code);

    void emit_operand_x64(const fuku_operand& rm_reg,fuku_register_index reg);
    void emit_operand_x86(const fuku_operand& rm_reg, fuku_register_index reg);
    void emit_operand(const fuku_operand& rm_reg, fuku_register reg);
    void emit_operand(const fuku_operand& rm_reg, int code);
public:
    fuku_assambler();
    ~fuku_assambler();

    uint8_t get_displacment_offset();
    uint8_t get_immediate_offset();

    bool is_used_short_eax();
    bool is_used_short_disp();
    bool is_used_short_imm();
public:

//Data Transfer Instructions
    asm_def_full(mov)
    asm_def_cond_2op(movcc,, fuasm_reg, fuasm_reg)
    asm_def_cond_2op(movcc,, fuasm_reg, fuasm_op)
    asm_def_direct_mov(xchg)
    asm_def_1op(bswap_dw,, fuasm_reg)
    asm_def_1op(bswap_qw,, fuasm_reg) 
    //XADD
    //CMPXCHG
    //CMPXCHG8B
    asm_def_wdq_one_op(push)
    asm_def_wdq_one_op(pop)
    asm_def_noop(cwd)
    asm_def_noop(cdq)
    asm_def_noop(cqo)
    asm_def_movsxz(movsx)
    asm_def_movsxz(movzx)
//Binary Arithmetic Instructions
    //ADCX
    //ADOX
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
    asm_def_noop(daa,)
    asm_def_noop(das,)
    asm_def_noop(aaa,)
    asm_def_noop(aas,)
    asm_def_noop_imm(aam,)
    asm_def_noop_imm(aad,)
//Logical Instructions Instructions
    asm_def_full(and)
    asm_def_full(or)
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
    asm_def_cond_1op(setcc,, fuasm_reg)
    asm_def_cond_1op(setcc,, fuasm_op)
    //TEST
    //CRC32
    //POPCNT
//Control Transfer Instructions
    asm_def_eip_one_op(jmp)   
    asm_def_cond_1op(jcc,, fuasm_imm)
    asm_def_eip_one_op(call)
    asm_def_noop_imm(ret)
    asm_def_noop(int3,)
    asm_def_2op(enter,,fuasm_imm, uint8_t)//enter size, nestinglevel
    asm_def_noop(,leave_)
//String Instructions
    asm_def_string_inst(outs)
    asm_def_string_inst(movs)
    asm_def_string_inst(cmps)
    asm_def_string_inst(scas)
    asm_def_string_inst(lods)
    asm_def_string_inst(stos)
//Flag Control (EFLAG) Instructions
    asm_def_noop(stc,)
    asm_def_noop(clc,)
    asm_def_noop(cmc,)
    asm_def_noop(cld,)
    asm_def_noop(std,)
    asm_def_noop(lahf,)
    asm_def_noop(sahf,)
    asm_def_noop(pusha,)       //pusha  w  regs
    asm_def_noop(pushad,)      //pushad dw regs
    asm_def_noop(popa,)        //popa   w  regs
    asm_def_noop(popad,)       //popad  dw regs
    asm_def_noop(pushf,)       //pushf  w  flags
    asm_def_noop(pushfd,)      //pushfd dw flags
    asm_def_noop(popf,)        //popf   w  flags
    asm_def_noop(popfd,)       //popfd  dw flags
//Miscellaneous Instructions
    //LEA
    asm_def_1op(nop,,uint8_t)
    asm_def_noop(ud2,)
    asm_def_noop(cpuid,)
//Random Number Generator Instructions
    //RDRAND
    //RDSEED
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
    asm_def_noop(hlt,)
    asm_def_noop(rdtsc,)
    asm_def_noop(lfence,)
};

#undef asm_def_full
#undef asm_def_full_shift
#undef gen_func_body_bit
#undef asm_def_r_op_one_op
#undef asm_def_wdq_one_op
#undef asm_def_eip_one_op
#undef asm_def_noop_imm
#undef asm_def_string_inst

