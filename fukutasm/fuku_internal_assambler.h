#pragma once

#define fuasm_reg  const fuku_register&
#define fuasm_op   const fuku_operand&
#define fuasm_imm  const fuku_immediate&

#define fuku_asm_gen_name(prefix, sname, postfix) prefix##sname##postfix
#define fuku_asm_ret_type void

#define asm_def_noop(name, postfix) \
        fuku_asm_ret_type fuku_asm_gen_name(_,name,postfix)(fuku_assambler_ctx& ctx);

#define asm_def_1op(name, postfix, type1) \
        fuku_asm_ret_type fuku_asm_gen_name(_,name,postfix)(fuku_assambler_ctx& ctx,type1 src);

#define asm_def_2op(name, postfix, type1, type2) \
        fuku_asm_ret_type fuku_asm_gen_name(_,name,postfix)(fuku_assambler_ctx& ctx,type1 dst, type2 src);

#define asm_def_3op(name, postfix, type1, type2, type3) \
        fuku_asm_ret_type fuku_asm_gen_name(_,name,postfix)(fuku_assambler_ctx& ctx,type1 dst, type2 src1, type3 src2);

#define asm_def_cond_1op(name, postfix, type1) \
        fuku_asm_ret_type fuku_asm_gen_name(_,name,postfix)(fuku_assambler_ctx& ctx,fuku_condition cond,type1 dst);

#define asm_def_cond_2op(name, postfix, type1, type2) \
        fuku_asm_ret_type fuku_asm_gen_name(_,name,postfix)(fuku_assambler_ctx& ctx,fuku_condition cond, type1 dst, type2 src);

namespace fukutasm {

    struct fuku_assambler_ctx {
        fuku_assambler_arch arch;

        uint8_t bytecode[16];
        uint8_t length;

        uint8_t displacment_offset;
        uint8_t immediate_offset;
        bool disp_reloc;
        bool imm_reloc;

        uint8_t short_cfg;

        fuku_inst * inst;
    };


        //Data Transfer Instructions
        asm_def_2op(mov, _b, fuasm_reg, fuasm_reg)
        asm_def_2op(mov, _b, fuasm_reg, fuasm_imm)
        asm_def_2op(mov, _b, fuasm_reg, fuasm_op)
        asm_def_2op(mov, _b, fuasm_op, fuasm_reg)
        asm_def_2op(mov, _b, fuasm_op, fuasm_imm)
        asm_def_2op(mov, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(mov, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(mov, _w, fuasm_reg, fuasm_op)
        asm_def_2op(mov, _w, fuasm_op, fuasm_reg)
        asm_def_2op(mov, _w, fuasm_op, fuasm_imm)
        asm_def_2op(mov, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(mov, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(mov, _dw, fuasm_reg, fuasm_op)
        asm_def_2op(mov, _dw, fuasm_op, fuasm_reg)
        asm_def_2op(mov, _dw, fuasm_op, fuasm_imm)
        asm_def_2op(mov, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(mov, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(mov, _qw, fuasm_reg, fuasm_op)
        asm_def_2op(mov, _qw, fuasm_op, fuasm_reg)
        asm_def_2op(mov, _qw, fuasm_op, fuasm_imm)

        asm_def_cond_2op(cmovcc, _w, fuasm_reg, fuasm_reg)
        asm_def_cond_2op(cmovcc, _w, fuasm_reg, fuasm_op)
        asm_def_cond_2op(cmovcc, _dw, fuasm_reg, fuasm_reg)
        asm_def_cond_2op(cmovcc, _dw, fuasm_reg, fuasm_op)
        asm_def_cond_2op(cmovcc, _qw, fuasm_reg, fuasm_reg)
        asm_def_cond_2op(cmovcc, _qw, fuasm_reg, fuasm_op)

        asm_def_2op(xchg, _b, fuasm_reg, fuasm_reg)
        asm_def_2op(xchg, _b, fuasm_op, fuasm_reg)
        asm_def_2op(xchg, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(xchg, _w, fuasm_op, fuasm_reg)
        asm_def_2op(xchg, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(xchg, _dw, fuasm_op, fuasm_reg)
        asm_def_2op(xchg, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(xchg, _qw, fuasm_op, fuasm_reg)

        asm_def_1op(bswap_w, , fuasm_reg)
        asm_def_1op(bswap_dw, , fuasm_reg)
        asm_def_1op(bswap_qw, , fuasm_reg)

        asm_def_2op(xadd, _b, fuasm_reg, fuasm_reg)
        asm_def_2op(xadd, _b, fuasm_op, fuasm_reg)
        asm_def_2op(xadd, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(xadd, _w, fuasm_op, fuasm_reg)
        asm_def_2op(xadd, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(xadd, _dw, fuasm_op, fuasm_reg)
        asm_def_2op(xadd, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(xadd, _qw, fuasm_op, fuasm_reg)

        asm_def_2op(cmpxchg, _b, fuasm_reg, fuasm_reg)
        asm_def_2op(cmpxchg, _b, fuasm_op, fuasm_reg)
        asm_def_2op(cmpxchg, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(cmpxchg, _w, fuasm_op, fuasm_reg)
        asm_def_2op(cmpxchg, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(cmpxchg, _dw, fuasm_op, fuasm_reg)
        asm_def_2op(cmpxchg, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(cmpxchg, _qw, fuasm_op, fuasm_reg)

        asm_def_1op(cmpxchg8b, , fuasm_op)
        asm_def_1op(cmpxchg16b, , fuasm_op)

        asm_def_1op(push, _w, fuasm_reg)
        asm_def_1op(push, _w, fuasm_op)
        asm_def_1op(push, _w, fuasm_imm)
        asm_def_1op(push, _dw, fuasm_reg)
        asm_def_1op(push, _dw, fuasm_op)
        asm_def_1op(push, _dw, fuasm_imm)
        asm_def_1op(push, _qw, fuasm_reg)
        asm_def_1op(push, _qw, fuasm_op)
        asm_def_1op(push, _qw, fuasm_imm)

        asm_def_1op(pop, _w, fuasm_reg)
        asm_def_1op(pop, _dw, fuasm_reg)
        asm_def_1op(pop, _qw, fuasm_reg)
        asm_def_1op(pop, _w, fuasm_op)
        asm_def_1op(pop, _dw, fuasm_op)
        asm_def_1op(pop, _qw, fuasm_op)

        asm_def_noop(pusha, )   //pusha  w  regs
        asm_def_noop(pushad, )  //pushad dw regs

        asm_def_noop(popa, )    //popa   w  regs
        asm_def_noop(popad, )   //popad  dw regs

        asm_def_noop(cwd, )
        asm_def_noop(cdq, )
        asm_def_noop(cqo, )

        asm_def_noop(cbw, )
        asm_def_noop(cwde, )
        asm_def_noop(cdqe, )

        asm_def_2op(movzx, _byte_w, fuasm_reg, fuasm_reg)
        asm_def_2op(movzx, _byte_w, fuasm_reg, fuasm_op)
        asm_def_2op(movzx, _byte_dw, fuasm_reg, fuasm_reg)
        asm_def_2op(movzx, _byte_dw, fuasm_reg, fuasm_op)
        asm_def_2op(movzx, _byte_qw, fuasm_reg, fuasm_reg)
        asm_def_2op(movzx, _byte_qw, fuasm_reg, fuasm_op)
        asm_def_2op(movzx, _word_dw, fuasm_reg, fuasm_reg)
        asm_def_2op(movzx, _word_dw, fuasm_reg, fuasm_op)
        asm_def_2op(movzx, _word_qw, fuasm_reg, fuasm_reg)
        asm_def_2op(movzx, _word_qw, fuasm_reg, fuasm_op)

        asm_def_2op(movsx, _byte_w, fuasm_reg, fuasm_reg)
        asm_def_2op(movsx, _byte_w, fuasm_reg, fuasm_op)
        asm_def_2op(movsx, _byte_dw, fuasm_reg, fuasm_reg)
        asm_def_2op(movsx, _byte_dw, fuasm_reg, fuasm_op)
        asm_def_2op(movsx, _byte_qw, fuasm_reg, fuasm_reg)
        asm_def_2op(movsx, _byte_qw, fuasm_reg, fuasm_op)
        asm_def_2op(movsx, _word_dw, fuasm_reg, fuasm_reg)
        asm_def_2op(movsx, _word_dw, fuasm_reg, fuasm_op)
        asm_def_2op(movsx, _word_qw, fuasm_reg, fuasm_reg)
        asm_def_2op(movsx, _word_qw, fuasm_reg, fuasm_op)
        asm_def_2op(movsxd, _word_w, fuasm_reg, fuasm_reg)
        asm_def_2op(movsxd, _word_w, fuasm_reg, fuasm_op)
        asm_def_2op(movsxd, _dword_dw, fuasm_reg, fuasm_reg)
        asm_def_2op(movsxd, _dword_dw, fuasm_reg, fuasm_op)
        asm_def_2op(movsxd, _dword_qw, fuasm_reg, fuasm_reg)
        asm_def_2op(movsxd, _dword_qw, fuasm_reg, fuasm_op)

        //Binary Arithmetic Instructions
        asm_def_2op(adcx, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(adcx, _dw, fuasm_reg, fuasm_op)
        asm_def_2op(adcx, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(adcx, _qw, fuasm_reg, fuasm_op)
        asm_def_2op(adox, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(adox, _dw, fuasm_reg, fuasm_op)
        asm_def_2op(adox, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(adox, _qw, fuasm_reg, fuasm_op)

        //ADOX TODO:

        asm_def_2op(add, _b, fuasm_reg, fuasm_reg)
        asm_def_2op(add, _b, fuasm_reg, fuasm_imm)
        asm_def_2op(add, _b, fuasm_reg, fuasm_op)
        asm_def_2op(add, _b, fuasm_op, fuasm_reg)
        asm_def_2op(add, _b, fuasm_op, fuasm_imm)
        asm_def_2op(add, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(add, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(add, _w, fuasm_reg, fuasm_op)
        asm_def_2op(add, _w, fuasm_op, fuasm_reg)
        asm_def_2op(add, _w, fuasm_op, fuasm_imm)
        asm_def_2op(add, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(add, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(add, _dw, fuasm_reg, fuasm_op)
        asm_def_2op(add, _dw, fuasm_op, fuasm_reg)
        asm_def_2op(add, _dw, fuasm_op, fuasm_imm)
        asm_def_2op(add, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(add, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(add, _qw, fuasm_reg, fuasm_op)
        asm_def_2op(add, _qw, fuasm_op, fuasm_reg)
        asm_def_2op(add, _qw, fuasm_op, fuasm_imm)

        asm_def_2op(adc, _b, fuasm_reg, fuasm_reg)
        asm_def_2op(adc, _b, fuasm_reg, fuasm_imm)
        asm_def_2op(adc, _b, fuasm_reg, fuasm_op)
        asm_def_2op(adc, _b, fuasm_op, fuasm_reg)
        asm_def_2op(adc, _b, fuasm_op, fuasm_imm)
        asm_def_2op(adc, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(adc, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(adc, _w, fuasm_reg, fuasm_op)
        asm_def_2op(adc, _w, fuasm_op, fuasm_reg)
        asm_def_2op(adc, _w, fuasm_op, fuasm_imm)
        asm_def_2op(adc, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(adc, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(adc, _dw, fuasm_reg, fuasm_op)
        asm_def_2op(adc, _dw, fuasm_op, fuasm_reg)
        asm_def_2op(adc, _dw, fuasm_op, fuasm_imm)
        asm_def_2op(adc, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(adc, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(adc, _qw, fuasm_reg, fuasm_op)
        asm_def_2op(adc, _qw, fuasm_op, fuasm_reg)
        asm_def_2op(adc, _qw, fuasm_op, fuasm_imm)

        asm_def_2op(sub, _b, fuasm_reg, fuasm_reg)
        asm_def_2op(sub, _b, fuasm_reg, fuasm_imm)
        asm_def_2op(sub, _b, fuasm_reg, fuasm_op)
        asm_def_2op(sub, _b, fuasm_op, fuasm_reg)
        asm_def_2op(sub, _b, fuasm_op, fuasm_imm)
        asm_def_2op(sub, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(sub, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(sub, _w, fuasm_reg, fuasm_op)
        asm_def_2op(sub, _w, fuasm_op, fuasm_reg)
        asm_def_2op(sub, _w, fuasm_op, fuasm_imm)
        asm_def_2op(sub, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(sub, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(sub, _dw, fuasm_reg, fuasm_op)
        asm_def_2op(sub, _dw, fuasm_op, fuasm_reg)
        asm_def_2op(sub, _dw, fuasm_op, fuasm_imm)
        asm_def_2op(sub, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(sub, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(sub, _qw, fuasm_reg, fuasm_op)
        asm_def_2op(sub, _qw, fuasm_op, fuasm_reg)
        asm_def_2op(sub, _qw, fuasm_op, fuasm_imm)

        asm_def_2op(sbb, _b, fuasm_reg, fuasm_reg)
        asm_def_2op(sbb, _b, fuasm_reg, fuasm_imm)
        asm_def_2op(sbb, _b, fuasm_reg, fuasm_op)
        asm_def_2op(sbb, _b, fuasm_op, fuasm_reg)
        asm_def_2op(sbb, _b, fuasm_op, fuasm_imm)
        asm_def_2op(sbb, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(sbb, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(sbb, _w, fuasm_reg, fuasm_op)
        asm_def_2op(sbb, _w, fuasm_op, fuasm_reg)
        asm_def_2op(sbb, _w, fuasm_op, fuasm_imm)
        asm_def_2op(sbb, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(sbb, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(sbb, _dw, fuasm_reg, fuasm_op)
        asm_def_2op(sbb, _dw, fuasm_op, fuasm_reg)
        asm_def_2op(sbb, _dw, fuasm_op, fuasm_imm)
        asm_def_2op(sbb, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(sbb, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(sbb, _qw, fuasm_reg, fuasm_op)
        asm_def_2op(sbb, _qw, fuasm_op, fuasm_reg)
        asm_def_2op(sbb, _qw, fuasm_op, fuasm_imm)

        asm_def_1op(imul, _b, fuasm_reg)
        asm_def_1op(imul, _b, fuasm_op)
        asm_def_1op(imul, _w, fuasm_reg)
        asm_def_1op(imul, _w, fuasm_op)
        asm_def_1op(imul, _dw, fuasm_reg)
        asm_def_1op(imul, _dw, fuasm_op)
        asm_def_1op(imul, _qw, fuasm_reg)
        asm_def_1op(imul, _qw, fuasm_op)

        asm_def_1op(mul, _b, fuasm_reg)
        asm_def_1op(mul, _b, fuasm_op)
        asm_def_1op(mul, _w, fuasm_reg)
        asm_def_1op(mul, _w, fuasm_op)
        asm_def_1op(mul, _dw, fuasm_reg)
        asm_def_1op(mul, _dw, fuasm_op)
        asm_def_1op(mul, _qw, fuasm_reg)
        asm_def_1op(mul, _qw, fuasm_op)

        asm_def_1op(idiv, _b, fuasm_reg)
        asm_def_1op(idiv, _b, fuasm_op)
        asm_def_1op(idiv, _w, fuasm_reg)
        asm_def_1op(idiv, _w, fuasm_op)
        asm_def_1op(idiv, _dw, fuasm_reg)
        asm_def_1op(idiv, _dw, fuasm_op)
        asm_def_1op(idiv, _qw, fuasm_reg)
        asm_def_1op(idiv, _qw, fuasm_op)

        asm_def_1op(div, _b, fuasm_reg)
        asm_def_1op(div, _b, fuasm_op)
        asm_def_1op(div, _w, fuasm_reg)
        asm_def_1op(div, _w, fuasm_op)
        asm_def_1op(div, _dw, fuasm_reg)
        asm_def_1op(div, _dw, fuasm_op)
        asm_def_1op(div, _qw, fuasm_reg)
        asm_def_1op(div, _qw, fuasm_op)

        asm_def_1op(inc, _b, fuasm_reg)
        asm_def_1op(inc, _b, fuasm_op)
        asm_def_1op(inc, _w, fuasm_reg)
        asm_def_1op(inc, _w, fuasm_op)
        asm_def_1op(inc, _dw, fuasm_reg)
        asm_def_1op(inc, _dw, fuasm_op)
        asm_def_1op(inc, _qw, fuasm_reg)
        asm_def_1op(inc, _qw, fuasm_op)

        asm_def_1op(dec, _b, fuasm_reg)
        asm_def_1op(dec, _b, fuasm_op)
        asm_def_1op(dec, _w, fuasm_reg)
        asm_def_1op(dec, _w, fuasm_op)
        asm_def_1op(dec, _dw, fuasm_reg)
        asm_def_1op(dec, _dw, fuasm_op)
        asm_def_1op(dec, _qw, fuasm_reg)
        asm_def_1op(dec, _qw, fuasm_op)

        asm_def_1op(neg, _b, fuasm_reg)
        asm_def_1op(neg, _b, fuasm_op)
        asm_def_1op(neg, _w, fuasm_reg)
        asm_def_1op(neg, _w, fuasm_op)
        asm_def_1op(neg, _dw, fuasm_reg)
        asm_def_1op(neg, _dw, fuasm_op)
        asm_def_1op(neg, _qw, fuasm_reg)
        asm_def_1op(neg, _qw, fuasm_op)

        asm_def_2op(cmp, _b, fuasm_reg, fuasm_reg)
        asm_def_2op(cmp, _b, fuasm_reg, fuasm_imm)
        asm_def_2op(cmp, _b, fuasm_reg, fuasm_op)
        asm_def_2op(cmp, _b, fuasm_op, fuasm_reg)
        asm_def_2op(cmp, _b, fuasm_op, fuasm_imm)
        asm_def_2op(cmp, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(cmp, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(cmp, _w, fuasm_reg, fuasm_op)
        asm_def_2op(cmp, _w, fuasm_op, fuasm_reg)
        asm_def_2op(cmp, _w, fuasm_op, fuasm_imm)
        asm_def_2op(cmp, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(cmp, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(cmp, _dw, fuasm_reg, fuasm_op)
        asm_def_2op(cmp, _dw, fuasm_op, fuasm_reg)
        asm_def_2op(cmp, _dw, fuasm_op, fuasm_imm)
        asm_def_2op(cmp, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(cmp, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(cmp, _qw, fuasm_reg, fuasm_op)
        asm_def_2op(cmp, _qw, fuasm_op, fuasm_reg)
        asm_def_2op(cmp, _qw, fuasm_op, fuasm_imm)

        //Decimal Arithmetic Instructions
        asm_def_noop(daa, )
        asm_def_noop(das, )
        asm_def_noop(aaa, )
        asm_def_noop(aas, )
        asm_def_1op(aam, , fuasm_imm)
        asm_def_1op(aad, , fuasm_imm)

        //Logical Instructions Instructions
        asm_def_2op(and, _b, fuasm_reg, fuasm_reg)
        asm_def_2op(and, _b, fuasm_reg, fuasm_imm)
        asm_def_2op(and, _b, fuasm_reg, fuasm_op)
        asm_def_2op(and, _b, fuasm_op, fuasm_reg)
        asm_def_2op(and, _b, fuasm_op, fuasm_imm)
        asm_def_2op(and, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(and, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(and, _w, fuasm_reg, fuasm_op)
        asm_def_2op(and, _w, fuasm_op, fuasm_reg)
        asm_def_2op(and, _w, fuasm_op, fuasm_imm)
        asm_def_2op(and, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(and, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(and, _dw, fuasm_reg, fuasm_op)
        asm_def_2op(and, _dw, fuasm_op, fuasm_reg)
        asm_def_2op(and, _dw, fuasm_op, fuasm_imm)
        asm_def_2op(and, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(and, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(and, _qw, fuasm_reg, fuasm_op)
        asm_def_2op(and, _qw, fuasm_op, fuasm_reg)
        asm_def_2op(and, _qw, fuasm_op, fuasm_imm)

        asm_def_2op(or , _b, fuasm_reg, fuasm_reg)
        asm_def_2op(or , _b, fuasm_reg, fuasm_imm)
        asm_def_2op(or , _b, fuasm_reg, fuasm_op)
        asm_def_2op(or , _b, fuasm_op, fuasm_reg)
        asm_def_2op(or , _b, fuasm_op, fuasm_imm)
        asm_def_2op(or , _w, fuasm_reg, fuasm_reg)
        asm_def_2op(or , _w, fuasm_reg, fuasm_imm)
        asm_def_2op(or , _w, fuasm_reg, fuasm_op)
        asm_def_2op(or , _w, fuasm_op, fuasm_reg)
        asm_def_2op(or , _w, fuasm_op, fuasm_imm)
        asm_def_2op(or , _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(or , _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(or , _dw, fuasm_reg, fuasm_op)
        asm_def_2op(or , _dw, fuasm_op, fuasm_reg)
        asm_def_2op(or , _dw, fuasm_op, fuasm_imm)
        asm_def_2op(or , _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(or , _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(or , _qw, fuasm_reg, fuasm_op)
        asm_def_2op(or , _qw, fuasm_op, fuasm_reg)
        asm_def_2op(or , _qw, fuasm_op, fuasm_imm)

        asm_def_2op(xor, _b, fuasm_reg, fuasm_reg)
        asm_def_2op(xor, _b, fuasm_reg, fuasm_imm)
        asm_def_2op(xor, _b, fuasm_reg, fuasm_op)
        asm_def_2op(xor, _b, fuasm_op, fuasm_reg)
        asm_def_2op(xor, _b, fuasm_op, fuasm_imm)
        asm_def_2op(xor, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(xor, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(xor, _w, fuasm_reg, fuasm_op)
        asm_def_2op(xor, _w, fuasm_op, fuasm_reg)
        asm_def_2op(xor, _w, fuasm_op, fuasm_imm)
        asm_def_2op(xor, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(xor, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(xor, _dw, fuasm_reg, fuasm_op)
        asm_def_2op(xor, _dw, fuasm_op, fuasm_reg)
        asm_def_2op(xor, _dw, fuasm_op, fuasm_imm)
        asm_def_2op(xor, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(xor, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(xor, _qw, fuasm_reg, fuasm_op)
        asm_def_2op(xor, _qw, fuasm_op, fuasm_reg)
        asm_def_2op(xor, _qw, fuasm_op, fuasm_imm)

        asm_def_1op(not, _b, fuasm_reg)
        asm_def_1op(not, _b, fuasm_op)
        asm_def_1op(not, _w, fuasm_reg)
        asm_def_1op(not, _w, fuasm_op)
        asm_def_1op(not, _dw, fuasm_reg)
        asm_def_1op(not, _dw, fuasm_op)
        asm_def_1op(not, _qw, fuasm_reg)
        asm_def_1op(not, _qw, fuasm_op)

        //Shift and Rotate Instructions
        asm_def_1op(sar_cl, _b, fuasm_reg)
        asm_def_1op(sar_cl, _b, fuasm_op)
        asm_def_2op(sar, _b, fuasm_reg, fuasm_imm)
        asm_def_2op(sar, _b, fuasm_op, fuasm_imm)
        asm_def_1op(sar_cl, _w, fuasm_reg)
        asm_def_1op(sar_cl, _w, fuasm_op)
        asm_def_2op(sar, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(sar, _w, fuasm_op, fuasm_imm)
        asm_def_1op(sar_cl, _dw, fuasm_reg)
        asm_def_1op(sar_cl, _dw, fuasm_op)
        asm_def_2op(sar, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(sar, _dw, fuasm_op, fuasm_imm)
        asm_def_1op(sar_cl, _qw, fuasm_reg)
        asm_def_1op(sar_cl, _qw, fuasm_op)
        asm_def_2op(sar, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(sar, _qw, fuasm_op, fuasm_imm)

        asm_def_1op(shr_cl, _b, fuasm_reg)
        asm_def_1op(shr_cl, _b, fuasm_op)
        asm_def_2op(shr, _b, fuasm_reg, fuasm_imm)
        asm_def_2op(shr, _b, fuasm_op, fuasm_imm)
        asm_def_1op(shr_cl, _w, fuasm_reg)
        asm_def_1op(shr_cl, _w, fuasm_op)
        asm_def_2op(shr, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(shr, _w, fuasm_op, fuasm_imm)
        asm_def_1op(shr_cl, _dw, fuasm_reg)
        asm_def_1op(shr_cl, _dw, fuasm_op)
        asm_def_2op(shr, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(shr, _dw, fuasm_op, fuasm_imm)
        asm_def_1op(shr_cl, _qw, fuasm_reg)
        asm_def_1op(shr_cl, _qw, fuasm_op)
        asm_def_2op(shr, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(shr, _qw, fuasm_op, fuasm_imm)

        asm_def_1op(shl_cl, _b, fuasm_reg)
        asm_def_1op(shl_cl, _b, fuasm_op)
        asm_def_2op(shl, _b, fuasm_reg, fuasm_imm)
        asm_def_2op(shl, _b, fuasm_op, fuasm_imm)
        asm_def_1op(shl_cl, _w, fuasm_reg)
        asm_def_1op(shl_cl, _w, fuasm_op)
        asm_def_2op(shl, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(shl, _w, fuasm_op, fuasm_imm)
        asm_def_1op(shl_cl, _dw, fuasm_reg)
        asm_def_1op(shl_cl, _dw, fuasm_op)
        asm_def_2op(shl, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(shl, _dw, fuasm_op, fuasm_imm)
        asm_def_1op(shl_cl, _qw, fuasm_reg)
        asm_def_1op(shl_cl, _qw, fuasm_op)
        asm_def_2op(shl, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(shl, _qw, fuasm_op, fuasm_imm)

        asm_def_3op(shrd, _w, fuasm_reg, fuasm_reg, fuasm_imm)
        asm_def_3op(shrd, _w, fuasm_op, fuasm_reg, fuasm_imm)
        asm_def_2op(shrd_cl, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(shrd_cl, _w, fuasm_op, fuasm_reg)
        asm_def_3op(shrd, _dw, fuasm_reg, fuasm_reg, fuasm_imm)
        asm_def_3op(shrd, _dw, fuasm_op, fuasm_reg, fuasm_imm)
        asm_def_2op(shrd_cl, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(shrd_cl, _dw, fuasm_op, fuasm_reg)
        asm_def_3op(shrd, _qw, fuasm_reg, fuasm_reg, fuasm_imm)
        asm_def_3op(shrd, _qw, fuasm_op, fuasm_reg, fuasm_imm)
        asm_def_2op(shrd_cl, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(shrd_cl, _qw, fuasm_op, fuasm_reg)

        asm_def_3op(shld, _w, fuasm_reg, fuasm_reg, fuasm_imm)
        asm_def_3op(shld, _w, fuasm_op, fuasm_reg, fuasm_imm)
        asm_def_2op(shld_cl, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(shld_cl, _w, fuasm_op, fuasm_reg)
        asm_def_3op(shld, _dw, fuasm_reg, fuasm_reg, fuasm_imm)
        asm_def_3op(shld, _dw, fuasm_op, fuasm_reg, fuasm_imm)
        asm_def_2op(shld_cl, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(shld_cl, _dw, fuasm_op, fuasm_reg)
        asm_def_3op(shld, _qw, fuasm_reg, fuasm_reg, fuasm_imm)
        asm_def_3op(shld, _qw, fuasm_op, fuasm_reg, fuasm_imm)
        asm_def_2op(shld_cl, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(shld_cl, _qw, fuasm_op, fuasm_reg)

        asm_def_1op(ror_cl, _b, fuasm_reg)
        asm_def_1op(ror_cl, _b, fuasm_op)
        asm_def_2op(ror, _b, fuasm_reg, fuasm_imm)
        asm_def_2op(ror, _b, fuasm_op, fuasm_imm)
        asm_def_1op(ror_cl, _w, fuasm_reg)
        asm_def_1op(ror_cl, _w, fuasm_op)
        asm_def_2op(ror, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(ror, _w, fuasm_op, fuasm_imm)
        asm_def_1op(ror_cl, _dw, fuasm_reg)
        asm_def_1op(ror_cl, _dw, fuasm_op)
        asm_def_2op(ror, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(ror, _dw, fuasm_op, fuasm_imm)
        asm_def_1op(ror_cl, _qw, fuasm_reg)
        asm_def_1op(ror_cl, _qw, fuasm_op)
        asm_def_2op(ror, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(ror, _qw, fuasm_op, fuasm_imm)

        asm_def_1op(rol_cl, _b, fuasm_reg)
        asm_def_1op(rol_cl, _b, fuasm_op)
        asm_def_2op(rol, _b, fuasm_reg, fuasm_imm)
        asm_def_2op(rol, _b, fuasm_op, fuasm_imm)
        asm_def_1op(rol_cl, _w, fuasm_reg)
        asm_def_1op(rol_cl, _w, fuasm_op)
        asm_def_2op(rol, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(rol, _w, fuasm_op, fuasm_imm)
        asm_def_1op(rol_cl, _dw, fuasm_reg)
        asm_def_1op(rol_cl, _dw, fuasm_op)
        asm_def_2op(rol, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(rol, _dw, fuasm_op, fuasm_imm)
        asm_def_1op(rol_cl, _qw, fuasm_reg)
        asm_def_1op(rol_cl, _qw, fuasm_op)
        asm_def_2op(rol, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(rol, _qw, fuasm_op, fuasm_imm)

        asm_def_1op(rcr_cl, _b, fuasm_reg)
        asm_def_1op(rcr_cl, _b, fuasm_op)
        asm_def_2op(rcr, _b, fuasm_reg, fuasm_imm)
        asm_def_2op(rcr, _b, fuasm_op, fuasm_imm)
        asm_def_1op(rcr_cl, _w, fuasm_reg)
        asm_def_1op(rcr_cl, _w, fuasm_op)
        asm_def_2op(rcr, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(rcr, _w, fuasm_op, fuasm_imm)
        asm_def_1op(rcr_cl, _dw, fuasm_reg)
        asm_def_1op(rcr_cl, _dw, fuasm_op)
        asm_def_2op(rcr, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(rcr, _dw, fuasm_op, fuasm_imm)
        asm_def_1op(rcr_cl, _qw, fuasm_reg)
        asm_def_1op(rcr_cl, _qw, fuasm_op)
        asm_def_2op(rcr, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(rcr, _qw, fuasm_op, fuasm_imm)

        asm_def_1op(rcl_cl, _b, fuasm_reg)
        asm_def_1op(rcl_cl, _b, fuasm_op)
        asm_def_2op(rcl, _b, fuasm_reg, fuasm_imm)
        asm_def_2op(rcl, _b, fuasm_op, fuasm_imm)
        asm_def_1op(rcl_cl, _w, fuasm_reg)
        asm_def_1op(rcl_cl, _w, fuasm_op)
        asm_def_2op(rcl, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(rcl, _w, fuasm_op, fuasm_imm)
        asm_def_1op(rcl_cl, _dw, fuasm_reg)
        asm_def_1op(rcl_cl, _dw, fuasm_op)
        asm_def_2op(rcl, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(rcl, _dw, fuasm_op, fuasm_imm)
        asm_def_1op(rcl_cl, _qw, fuasm_reg)
        asm_def_1op(rcl_cl, _qw, fuasm_op)
        asm_def_2op(rcl, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(rcl, _qw, fuasm_op, fuasm_imm)

        //Bit and Byte Instructions
        asm_def_2op(bt, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(bt, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(bt, _w, fuasm_op, fuasm_reg)
        asm_def_2op(bt, _w, fuasm_op, fuasm_imm)
        asm_def_2op(bt, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(bt, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(bt, _dw, fuasm_op, fuasm_reg)
        asm_def_2op(bt, _dw, fuasm_op, fuasm_imm)
        asm_def_2op(bt, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(bt, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(bt, _qw, fuasm_op, fuasm_reg)
        asm_def_2op(bt, _qw, fuasm_op, fuasm_imm)

        asm_def_2op(bts, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(bts, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(bts, _w, fuasm_op, fuasm_reg)
        asm_def_2op(bts, _w, fuasm_op, fuasm_imm)
        asm_def_2op(bts, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(bts, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(bts, _dw, fuasm_op, fuasm_reg)
        asm_def_2op(bts, _dw, fuasm_op, fuasm_imm)
        asm_def_2op(bts, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(bts, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(bts, _qw, fuasm_op, fuasm_reg)
        asm_def_2op(bts, _qw, fuasm_op, fuasm_imm)

        asm_def_2op(btr, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(btr, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(btr, _w, fuasm_op, fuasm_reg)
        asm_def_2op(btr, _w, fuasm_op, fuasm_imm)
        asm_def_2op(btr, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(btr, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(btr, _dw, fuasm_op, fuasm_reg)
        asm_def_2op(btr, _dw, fuasm_op, fuasm_imm)
        asm_def_2op(btr, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(btr, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(btr, _qw, fuasm_op, fuasm_reg)
        asm_def_2op(btr, _qw, fuasm_op, fuasm_imm)

        asm_def_2op(btc, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(btc, _w, fuasm_reg, fuasm_imm)
        asm_def_2op(btc, _w, fuasm_op, fuasm_reg)
        asm_def_2op(btc, _w, fuasm_op, fuasm_imm)
        asm_def_2op(btc, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(btc, _dw, fuasm_reg, fuasm_imm)
        asm_def_2op(btc, _dw, fuasm_op, fuasm_reg)
        asm_def_2op(btc, _dw, fuasm_op, fuasm_imm)
        asm_def_2op(btc, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(btc, _qw, fuasm_reg, fuasm_imm)
        asm_def_2op(btc, _qw, fuasm_op, fuasm_reg)
        asm_def_2op(btc, _qw, fuasm_op, fuasm_imm)

        asm_def_2op(bsf, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(bsf, _w, fuasm_op, fuasm_reg)
        asm_def_2op(bsf, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(bsf, _dw, fuasm_op, fuasm_reg)
        asm_def_2op(bsf, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(bsf, _qw, fuasm_op, fuasm_reg)

        asm_def_2op(bsr, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(bsr, _w, fuasm_op, fuasm_reg)
        asm_def_2op(bsr, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(bsr, _dw, fuasm_op, fuasm_reg)
        asm_def_2op(bsr, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(bsr, _qw, fuasm_op, fuasm_reg)

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

        //CRC32 TODO:

        asm_def_2op(popcnt, _w, fuasm_reg, fuasm_reg)
        asm_def_2op(popcnt, _w, fuasm_reg, fuasm_op)
        asm_def_2op(popcnt, _dw, fuasm_reg, fuasm_reg)
        asm_def_2op(popcnt, _dw, fuasm_reg, fuasm_op)
        asm_def_2op(popcnt, _qw, fuasm_reg, fuasm_reg)
        asm_def_2op(popcnt, _qw, fuasm_reg, fuasm_op)

        //Control Transfer Instructions
        asm_def_1op(jmp, , fuasm_reg)
        asm_def_1op(jmp, , fuasm_op)
        asm_def_1op(jmp, , fuasm_imm)

        asm_def_cond_1op(jcc, , fuasm_imm)

        //JCXZ/JECXZ TODO:
        //LOOP/LOOPZ/LOOPNZ TODO: 

        asm_def_1op(call, , fuasm_reg)
        asm_def_1op(call, , fuasm_op)
        asm_def_1op(call, , fuasm_imm)

        asm_def_noop(ret, )
        asm_def_1op(ret, , fuasm_imm)

        //IRET TODO:

        asm_def_noop(int3, )

        //INTO/BOUND TODO:

        asm_def_2op(enter, , fuasm_imm, uint8_t)//enter size, nestinglevel
        asm_def_noop(, leave_)

        //String Instructions
        asm_def_noop(movs, b)
        asm_def_noop(movs, w)
        asm_def_noop(movs, d)
        asm_def_noop(movs, q)

        asm_def_noop(cmps, b)
        asm_def_noop(cmps, w)
        asm_def_noop(cmps, d)
        asm_def_noop(cmps, q)

        asm_def_noop(scas, b)
        asm_def_noop(scas, w)
        asm_def_noop(scas, d)
        asm_def_noop(scas, q)

        asm_def_noop(lods, b)
        asm_def_noop(lods, w)
        asm_def_noop(lods, d)
        asm_def_noop(lods, q)

        asm_def_noop(stos, b)
        asm_def_noop(stos, w)
        asm_def_noop(stos, d)
        asm_def_noop(stos, q)

        // I/O Instructions
        
        //IN/OUT TODO:
        //INS TODO:
            
        asm_def_noop(outsb, )
        asm_def_noop(outsw, )
        asm_def_noop(outsd, )

        //Flag Control (EFLAG) Instructions
        asm_def_noop(stc, )
        asm_def_noop(clc, )
        asm_def_noop(cmc, )
        asm_def_noop(cld, )
        asm_def_noop(std, )

        asm_def_noop(lahf, )
        asm_def_noop(sahf, )

        asm_def_noop(pushf, )   //pushf  w  flags
        asm_def_noop(pushfd, )  //pushfd dw flags
        asm_def_noop(pushfq, )  //pushfq qw flags

        asm_def_noop(popf, )//popf   w  flags
        asm_def_noop(popfd, )   //popfd  dw flags
        asm_def_noop(popfq, )   //popfq  qw flags

        //STI/CLI TODO:

        //Segment Register Instructions

        //LDS/LES/LFS/LGS/LSS TODO:


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
        /*asm_def_3op(andn, _dw, fuasm_reg, fuasm_reg, fuasm_reg)
        asm_def_3op(andn, _dw, fuasm_reg, fuasm_reg, fuasm_op)
        asm_def_3op(andn, _qw, fuasm_reg, fuasm_reg, fuasm_reg)
        asm_def_3op(andn, _qw, fuasm_reg, fuasm_reg, fuasm_op)

        asm_def_3op(bextr, _dw, fuasm_reg, fuasm_reg, fuasm_reg)
        asm_def_3op(bextr, _dw, fuasm_reg, fuasm_op, fuasm_reg)
        asm_def_3op(bextr, _qw, fuasm_reg, fuasm_reg, fuasm_reg)
        asm_def_3op(bextr, _qw, fuasm_reg, fuasm_op, fuasm_reg)

        asm_def_3op(blsi, _dw, fuasm_reg, fuasm_reg)
        asm_def_3op(blsi, _dw, fuasm_reg, fuasm_op)
        asm_def_3op(blsi, _qw, fuasm_reg, fuasm_reg)
        asm_def_3op(blsi, _qw, fuasm_reg, fuasm_op)

        asm_def_3op(blsmsk, _dw, fuasm_reg, fuasm_reg)
        asm_def_3op(blsmsk, _dw, fuasm_reg, fuasm_op)
        asm_def_3op(blsmsk, _qw, fuasm_reg, fuasm_reg)
        asm_def_3op(blsmsk, _qw, fuasm_reg, fuasm_op)

        asm_def_3op(blsr, _dw, fuasm_reg, fuasm_reg)
        asm_def_3op(blsr, _dw, fuasm_reg, fuasm_op)
        asm_def_3op(blsr, _qw, fuasm_reg, fuasm_reg)
        asm_def_3op(blsr, _qw, fuasm_reg, fuasm_op)

        asm_def_3op(bzhi, _dw, fuasm_reg, fuasm_reg, fuasm_reg)
        asm_def_3op(bzhi, _dw, fuasm_reg, fuasm_op, fuasm_reg)
        asm_def_3op(bzhi, _qw, fuasm_reg, fuasm_reg, fuasm_reg)
        asm_def_3op(bzhi, _qw, fuasm_reg, fuasm_op, fuasm_reg)

        asm_def_2op(lzcnt,_w,  fuasm_reg, fuasm_reg)
        asm_def_2op(lzcnt,_w,  fuasm_reg, fuasm_op)
        asm_def_2op(lzcnt,_dw, fuasm_reg, fuasm_reg)
        asm_def_2op(lzcnt,_dw, fuasm_reg, fuasm_op)
        asm_def_2op(lzcnt,_qw, fuasm_reg, fuasm_reg)
        asm_def_2op(lzcnt,_qw, fuasm_reg, fuasm_op)

        asm_def_3op(mulx, _dw, fuasm_reg, fuasm_reg, fuasm_reg)
        asm_def_3op(mulx, _dw, fuasm_reg, fuasm_reg, fuasm_op)
        asm_def_3op(mulx, _qw, fuasm_reg, fuasm_reg, fuasm_reg)
        asm_def_3op(mulx, _qw, fuasm_reg, fuasm_reg, fuasm_op)

        asm_def_3op(pdep, _dw, fuasm_reg, fuasm_reg, fuasm_reg)
        asm_def_3op(pdep, _dw, fuasm_reg, fuasm_reg, fuasm_op)
        asm_def_3op(pdep, _qw, fuasm_reg, fuasm_reg, fuasm_reg)
        asm_def_3op(pdep, _qw, fuasm_reg, fuasm_reg, fuasm_op)

        asm_def_3op(pext, _dw, fuasm_reg, fuasm_reg, fuasm_reg)
        asm_def_3op(pext, _dw, fuasm_reg, fuasm_reg, fuasm_op)
        asm_def_3op(pext, _qw, fuasm_reg, fuasm_reg, fuasm_reg)
        asm_def_3op(pext, _qw, fuasm_reg, fuasm_reg, fuasm_op)*/
        //RORX
        //SARX
        //SHLX
        //SHRX

        //SYSTEM INSTRUCTIONS
        asm_def_noop(hlt, )
        asm_def_noop(rdtsc, )
        asm_def_noop(lfence, )


        //X87 Instructions

        //SSE Instructions

        //SSE2 Instructions

        //SSE3 Instructions

        //SSE4 Instructions
        //SSE4.1 Instructions
        //SSE4.2 Instructions

        //AVX Instructions
        //AVX2 Instructions
        //TSX Instructions
        //AVX-512 Instructions
};

#undef asm_def_noop
#undef asm_def_1op
#undef asm_def_2op
#undef asm_def_3op
#undef asm_def_cond_1op
#undef asm_def_cond_2op

#undef fuku_asm_ret_type
#undef fuku_asm_gen_name

#undef fuasm_reg
#undef fuasm_op
#undef fuasm_imm