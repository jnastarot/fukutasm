#pragma once

#include "fuku_internal_assambler.h"

enum fuku_t0_types {
    FUKU_T0_REGISTER = 1,
    FUKU_T0_OPERAND  = 2,
    FUKU_T0_IMMEDIATE = 4,
};


class fuku_type {
    fuku_t0_types type;
    union {
        const fuku_register* reg;
        const fuku_operand *op;
        const fuku_immediate *imm;
    };
public:
    fuku_type(const fuku_register& reg);
    fuku_type(const fuku_operand& op);
    fuku_type(const fuku_immediate& imm);
    ~fuku_type();
public:
    const fuku_t0_types get_type() const;
    const fuku_register &get_register() const;
    const fuku_operand  &get_operand() const;
    const fuku_immediate &get_immediate() const;
};

enum fuku_assambler_hold_type {
    ASSAMBLER_HOLD_TYPE_NOOVERWRITE,
    ASSAMBLER_HOLD_TYPE_FIRST_OVERWRITE,
    ASSAMBLER_HOLD_TYPE_FULL_OVERWRITE,
};

class fuku_assambler {
    fuku_instruction inst;
    fukutasm::fuku_assambler_ctx context;

    fuku_assambler_hold_type hold_type;
    fuku_code_holder * code_holder;
    linestorage::iterator position;
    bool first_emit;

    void on_new_chain_item();
public:
    fuku_assambler();
    fuku_assambler(fuku_assambler_arch arch);
    ~fuku_assambler();

    fukutasm::fuku_assambler_ctx& get_context();

    fuku_assambler& set_holder(fuku_code_holder * code_holder, fuku_assambler_hold_type hold_type);
    fuku_assambler& set_position(linestorage::iterator& position);
    fuku_assambler& set_first_emit(bool first_emit);
public:
//Data Transfer Instructions
    void mov(const fuku_type& dst, const fuku_type& src);
    void cmovcc(fuku_condition cond, const fuku_type& dst, const fuku_type& src);
    void xchg(const fuku_type& dst, const fuku_type& src);
    void bswap(const fuku_type& dst);
    void xadd(const fuku_type& dst, const fuku_type& src);
    void cmpxchg(const fuku_type& dst, const fuku_type& src);
    void cmpxchg8b(const fuku_type& dst);
    void cmpxchg16b(const fuku_type& dst);
    void push(const fuku_type& src);
    void pop(const fuku_type& dst);
    void cwd();
    void cdq();
    void cqo();
    void movzx(const fuku_type& dst, const fuku_type& src);
    void movsx(const fuku_type& dst, const fuku_type& src);

//Binary Arithmetic Instructions
    void adcx(const fuku_type& dst, const fuku_type& src);
    void adox(const fuku_type& dst, const fuku_type& src);
    void add(const fuku_type& dst, const fuku_type& src);
    void adc(const fuku_type& dst, const fuku_type& src);
    void sub(const fuku_type& dst, const fuku_type& src);
    void sbb(const fuku_type& dst, const fuku_type& src);
    void imul(const fuku_type& src);
    void mul(const fuku_type& dst);
    void idiv(const fuku_type& src);
    void div(const fuku_type& dst);
    void inc(const fuku_type& src);
    void dec(const fuku_type& dst);
    void neg(const fuku_type& src);
    void cmp(const fuku_type& dst, const fuku_type& src);

//Decimal Arithmetic Instructions
    void daa();
    void das();
    void aaa();
    void aas();
    void aam(const fuku_type& src);
    void aad(const fuku_type& src);

//Logical Instructions Instructions
    void and_(const fuku_type& dst, const fuku_type& src);
    void or_(const fuku_type& dst, const fuku_type& src);
    void xor_(const fuku_type& dst, const fuku_type& src);
    void not_(const fuku_type& dst);

//Shift and Rotate Instructions
    void sar(const fuku_type& dst, const fuku_type& src);
    void shr(const fuku_type& dst, const fuku_type& src);
    void shl(const fuku_type& dst, const fuku_type& src);
    void shrd(const fuku_type& dst, const fuku_type& src, const fuku_type& shift);
    void shld(const fuku_type& dst, const fuku_type& src, const fuku_type& shift);
    void ror(const fuku_type& dst, const fuku_type& src);
    void rol(const fuku_type& dst, const fuku_type& src);
    void rcr(const fuku_type& dst, const fuku_type& src);
    void rcl(const fuku_type& dst, const fuku_type& src);

//Bit and Byte Instructions
    void bt(const fuku_type& dst, const fuku_type& src);
    void bts(const fuku_type& dst, const fuku_type& src);
    void btr(const fuku_type& dst, const fuku_type& src);
    void btc(const fuku_type& dst, const fuku_type& src);
    void setcc(fuku_condition cond, const fuku_type& dst);
    void test(const fuku_type& dst, const fuku_type& src);
    void popcnt(const fuku_type& dst, const fuku_type& src);

//Control Transfer Instructions
    void jmp(const fuku_type& src);
    void jcc(fuku_condition cond, const fuku_type& src);
    void call(const fuku_type& src);
    void ret(const fuku_type& src);
    void int3();
    void enter(const fuku_type& size, const fuku_type& nestinglevel);
    void leave_();

//String Instructions
    void outsb();
    void outsw();
    void outsd();
    void movsb();
    void movsw();
    void movsd();
    void movsq();
    void cmpsb();
    void cmpsw();
    void cmpsd();
    void cmpsq();
    void scasb();
    void scasw();
    void scasd();
    void scasq();
    void lodsb();
    void lodsw();
    void lodsd();
    void lodsq();
    void stosb();
    void stosw();
    void stosd();
    void stosq();

//Flag Control (EFLAG) Instructions
    void stc();
    void clc();
    void cmc();
    void cld();
    void std();
    void lahf();
    void sahf();
    void pusha();
    void pushad();
    void popa();
    void popad();
    void pushf();
    void pushfd();
    void pushfq();
    void popf();
    void popfd();
    void popfq();

//Miscellaneous Instructions
    void lea(const fuku_type& dst, const fuku_type& src);
    void nop(uint8_t size = 1);
    void ud2();
    void cpuid();

//Random Number Generator Instructions
    void rdrand(const fuku_type& dst);
    void rdseed(const fuku_type& dst);

//SYSTEM INSTRUCTIONS
    void hlt();
    void rdtsc();
    void lfence();
};

