#pragma once

#include "fuku_internal_assambler.h"

enum fuku_t0_types {
    FUKU_T0_NONE,
    FUKU_T0_REGISTER,
    FUKU_T0_OPERAND,
    FUKU_T0_IMMEDIATE,
};


class fuku_type {
    fuku_t0_types type;
    
    fuku_register base;
    fuku_register index;
    fuku_operand_scale scale;
    fuku_immediate disp;
    fuku_operand_size size;

public:
    fuku_type();
    fuku_type(const fuku_register& reg);
    fuku_type(const fuku_operand& op);
    fuku_type(const fuku_immediate& imm);
    ~fuku_type();

    fuku_type& operator=(const fuku_register& _reg);
    fuku_type& operator=(const fuku_operand& _op);
    fuku_type& operator=(const fuku_immediate& _imm);
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

    void on_emit();
    fukutasm::fuku_assambler_ctx& on_new_chain_item();
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
    fukutasm::fuku_assambler_ctx& mov(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& cmovcc(fuku_condition cond, const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& xchg(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& bswap(const fuku_type& dst);
    fukutasm::fuku_assambler_ctx& xadd(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& cmpxchg(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& cmpxchg8b(const fuku_type& dst);
    fukutasm::fuku_assambler_ctx& cmpxchg16b(const fuku_type& dst);
    fukutasm::fuku_assambler_ctx& push(const fuku_type& src);
    fukutasm::fuku_assambler_ctx& pop(const fuku_type& dst);
    fukutasm::fuku_assambler_ctx& cwd();
    fukutasm::fuku_assambler_ctx& cdq();
    fukutasm::fuku_assambler_ctx& cqo();
    fukutasm::fuku_assambler_ctx& cbw();
    fukutasm::fuku_assambler_ctx& cwde();
    fukutasm::fuku_assambler_ctx& cdqe();
    fukutasm::fuku_assambler_ctx& movzx(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& movsx(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& movsxd(const fuku_type& dst, const fuku_type& src);

//Binary Arithmetic Instructions
    fukutasm::fuku_assambler_ctx& adcx(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& adox(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& add(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& adc(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& sub(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& sbb(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& imul(const fuku_type& src);
    fukutasm::fuku_assambler_ctx& mul(const fuku_type& dst);
    fukutasm::fuku_assambler_ctx& idiv(const fuku_type& src);
    fukutasm::fuku_assambler_ctx& div(const fuku_type& dst);
    fukutasm::fuku_assambler_ctx& inc(const fuku_type& src);
    fukutasm::fuku_assambler_ctx& dec(const fuku_type& dst);
    fukutasm::fuku_assambler_ctx& neg(const fuku_type& src);
    fukutasm::fuku_assambler_ctx& cmp(const fuku_type& dst, const fuku_type& src);

//Decimal Arithmetic Instructions
    fukutasm::fuku_assambler_ctx& daa();
    fukutasm::fuku_assambler_ctx& das();
    fukutasm::fuku_assambler_ctx& aaa();
    fukutasm::fuku_assambler_ctx& aas();
    fukutasm::fuku_assambler_ctx& aam(const fuku_type& src);
    fukutasm::fuku_assambler_ctx& aad(const fuku_type& src);

//Logical Instructions Instructions
    fukutasm::fuku_assambler_ctx& and_(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& or_(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& xor_(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& not_(const fuku_type& dst);

//Shift and Rotate Instructions
    fukutasm::fuku_assambler_ctx& sar(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& shr(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& shl(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& shrd(const fuku_type& dst, const fuku_type& src, const fuku_type& shift);
    fukutasm::fuku_assambler_ctx& shld(const fuku_type& dst, const fuku_type& src, const fuku_type& shift);
    fukutasm::fuku_assambler_ctx& ror(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& rol(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& rcr(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& rcl(const fuku_type& dst, const fuku_type& src);

//Bit and Byte Instructions
    fukutasm::fuku_assambler_ctx& bt(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& bts(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& btr(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& btc(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& setcc(fuku_condition cond, const fuku_type& dst);
    fukutasm::fuku_assambler_ctx& test(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& popcnt(const fuku_type& dst, const fuku_type& src);

//Control Transfer Instructions
    fukutasm::fuku_assambler_ctx& jmp(const fuku_type& src);
    fukutasm::fuku_assambler_ctx& jcc(fuku_condition cond, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& call(const fuku_type& src);
    fukutasm::fuku_assambler_ctx& ret(const fuku_type& src);
    fukutasm::fuku_assambler_ctx& int3();
    fukutasm::fuku_assambler_ctx& enter(const fuku_type& size, const fuku_type& nestinglevel);
    fukutasm::fuku_assambler_ctx& leave_();

//String Instructions
    fukutasm::fuku_assambler_ctx& outsb();
    fukutasm::fuku_assambler_ctx& outsw();
    fukutasm::fuku_assambler_ctx& outsd();
    fukutasm::fuku_assambler_ctx& movsb();
    fukutasm::fuku_assambler_ctx& movsw();
    fukutasm::fuku_assambler_ctx& movsd();
    fukutasm::fuku_assambler_ctx& movsq();
    fukutasm::fuku_assambler_ctx& cmpsb();
    fukutasm::fuku_assambler_ctx& cmpsw();
    fukutasm::fuku_assambler_ctx& cmpsd();
    fukutasm::fuku_assambler_ctx& cmpsq();
    fukutasm::fuku_assambler_ctx& scasb();
    fukutasm::fuku_assambler_ctx& scasw();
    fukutasm::fuku_assambler_ctx& scasd();
    fukutasm::fuku_assambler_ctx& scasq();
    fukutasm::fuku_assambler_ctx& lodsb();
    fukutasm::fuku_assambler_ctx& lodsw();
    fukutasm::fuku_assambler_ctx& lodsd();
    fukutasm::fuku_assambler_ctx& lodsq();
    fukutasm::fuku_assambler_ctx& stosb();
    fukutasm::fuku_assambler_ctx& stosw();
    fukutasm::fuku_assambler_ctx& stosd();
    fukutasm::fuku_assambler_ctx& stosq();

//Flag Control (EFLAG) Instructions
    fukutasm::fuku_assambler_ctx& stc();
    fukutasm::fuku_assambler_ctx& clc();
    fukutasm::fuku_assambler_ctx& cmc();
    fukutasm::fuku_assambler_ctx& cld();
    fukutasm::fuku_assambler_ctx& std();
    fukutasm::fuku_assambler_ctx& lahf();
    fukutasm::fuku_assambler_ctx& sahf();
    fukutasm::fuku_assambler_ctx& pusha();
    fukutasm::fuku_assambler_ctx& pushad();
    fukutasm::fuku_assambler_ctx& popa();
    fukutasm::fuku_assambler_ctx& popad();
    fukutasm::fuku_assambler_ctx& pushf();
    fukutasm::fuku_assambler_ctx& pushfd();
    fukutasm::fuku_assambler_ctx& pushfq();
    fukutasm::fuku_assambler_ctx& popf();
    fukutasm::fuku_assambler_ctx& popfd();
    fukutasm::fuku_assambler_ctx& popfq();

//Miscellaneous Instructions
    fukutasm::fuku_assambler_ctx& lea(const fuku_type& dst, const fuku_type& src);
    fukutasm::fuku_assambler_ctx& nop(uint8_t size = 1);
    fukutasm::fuku_assambler_ctx& ud2();
    fukutasm::fuku_assambler_ctx& cpuid();

//Random Number Generator Instructions
    fukutasm::fuku_assambler_ctx& rdrand(const fuku_type& dst);
    fukutasm::fuku_assambler_ctx& rdseed(const fuku_type& dst);

//SYSTEM INSTRUCTIONS
    fukutasm::fuku_assambler_ctx& hlt();
    fukutasm::fuku_assambler_ctx& rdtsc();
    fukutasm::fuku_assambler_ctx& lfence();
};

