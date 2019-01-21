#pragma once

enum fuku_t0_types {
    FUKU_T0_REGISTER = 1,
    FUKU_T0_OPERAND  = 2,
    FUKU_T0_IMMEDIATE = 4,
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
    const fuku_t0_types get_type() const;
    const fuku_register get_register() const;
    const fuku_operand  &get_operand() const;
    const fuku_immediate &get_immediate() const;
};

class fuku_assambler {
    fuku_assambler_ctx context;
public:
    fuku_assambler(fuku_assambler_arch arch);
    ~fuku_assambler();

    void set_optimization_flags(uint8_t flags); //fuku_asm_short_cfg
    void set_arch(fuku_assambler_arch arch);
    fuku_assambler_arch get_arch();
public:
//Data Transfer Instructions
    fuku_instruction mov(fuku_type dst, fuku_type src);
    fuku_instruction cmovcc(fuku_condition cond, fuku_type dst, fuku_type src);
    fuku_instruction xchg(fuku_type dst, fuku_type src);
    fuku_instruction bswap(fuku_type dst);
    fuku_instruction xadd(fuku_type dst, fuku_type src);
    fuku_instruction cmpxchg(fuku_type dst, fuku_type src);
    fuku_instruction cmpxchg8b(fuku_type dst);
    fuku_instruction cmpxchg16b(fuku_type dst);
    fuku_instruction push(fuku_type src);
    fuku_instruction pop(fuku_type dst);
    fuku_instruction cwd();
    fuku_instruction cdq();
    fuku_instruction cqo();
    fuku_instruction movzx(fuku_type dst, fuku_type src);
    fuku_instruction movsx(fuku_type dst, fuku_type src);

//Binary Arithmetic Instructions
    fuku_instruction adcx(fuku_type dst, fuku_type src);
    fuku_instruction adox(fuku_type dst, fuku_type src);
    fuku_instruction add(fuku_type dst, fuku_type src);
    fuku_instruction adc(fuku_type dst, fuku_type src);
    fuku_instruction sub(fuku_type dst, fuku_type src);
    fuku_instruction sbb(fuku_type dst, fuku_type src);
    fuku_instruction imul(fuku_type src);
    fuku_instruction mul(fuku_type dst);
    fuku_instruction idiv(fuku_type src);
    fuku_instruction div(fuku_type dst);
    fuku_instruction inc(fuku_type src);
    fuku_instruction dec(fuku_type dst);
    fuku_instruction neg(fuku_type src);
    fuku_instruction cmp(fuku_type dst, fuku_type src);

//Decimal Arithmetic Instructions
    fuku_instruction daa();
    fuku_instruction das();
    fuku_instruction aaa();
    fuku_instruction aas();
    fuku_instruction aam(fuku_type src);
    fuku_instruction aad(fuku_type src);

//Logical Instructions Instructions
    fuku_instruction and_(fuku_type dst, fuku_type src);
    fuku_instruction or_(fuku_type dst, fuku_type src);
    fuku_instruction xor_(fuku_type dst, fuku_type src);
    fuku_instruction not_(fuku_type dst);

//Shift and Rotate Instructions
    fuku_instruction sar(fuku_type dst, fuku_type src);
    fuku_instruction shr(fuku_type dst, fuku_type src);
    fuku_instruction shl(fuku_type dst, fuku_type src);
    fuku_instruction shrd(fuku_type dst, fuku_type src, fuku_type shift);
    fuku_instruction shld(fuku_type dst, fuku_type src, fuku_type shift);
    fuku_instruction ror(fuku_type dst, fuku_type src);
    fuku_instruction rol(fuku_type dst, fuku_type src);
    fuku_instruction rcr(fuku_type dst, fuku_type src);
    fuku_instruction rcl(fuku_type dst, fuku_type src);

//Bit and Byte Instructions
    fuku_instruction bt(fuku_type dst, fuku_type src);
    fuku_instruction bts(fuku_type dst, fuku_type src);
    fuku_instruction btr(fuku_type dst, fuku_type src);
    fuku_instruction btc(fuku_type dst, fuku_type src);
    fuku_instruction setcc(fuku_condition cond, fuku_type dst);
    fuku_instruction test(fuku_type dst, fuku_type src);
    fuku_instruction popcnt(fuku_type dst, fuku_type src);

//Control Transfer Instructions
    fuku_instruction jmp(fuku_type src);
    fuku_instruction jcc(fuku_condition cond, fuku_type src);
    fuku_instruction call(fuku_type src);
    fuku_instruction ret(fuku_type src);
    fuku_instruction int3();
    fuku_instruction enter(fuku_type size, fuku_type nestinglevel);
    fuku_instruction leave_();

//String Instructions
    fuku_instruction outsb();
    fuku_instruction outsw();
    fuku_instruction outsd();
    fuku_instruction movsb();
    fuku_instruction movsw();
    fuku_instruction movsd();
    fuku_instruction movsq();
    fuku_instruction cmpsb();
    fuku_instruction cmpsw();
    fuku_instruction cmpsd();
    fuku_instruction cmpsq();
    fuku_instruction scasb();
    fuku_instruction scasw();
    fuku_instruction scasd();
    fuku_instruction scasq();
    fuku_instruction lodsb();
    fuku_instruction lodsw();
    fuku_instruction lodsd();
    fuku_instruction lodsq();
    fuku_instruction stosb();
    fuku_instruction stosw();
    fuku_instruction stosd();
    fuku_instruction stosq();

//Flag Control (EFLAG) Instructions
    fuku_instruction stc();
    fuku_instruction clc();
    fuku_instruction cmc();
    fuku_instruction cld();
    fuku_instruction std();
    fuku_instruction lahf();
    fuku_instruction sahf();
    fuku_instruction pusha();
    fuku_instruction pushad();
    fuku_instruction popa();
    fuku_instruction popad();
    fuku_instruction pushf();
    fuku_instruction pushfd();
    fuku_instruction pushfq();
    fuku_instruction popf();
    fuku_instruction popfd();
    fuku_instruction popfq();

//Miscellaneous Instructions
    fuku_instruction lea(fuku_type dst, fuku_type src);
    fuku_instruction nop(uint8_t size = 1);
    fuku_instruction ud2();
    fuku_instruction cpuid();

//Random Number Generator Instructions
    fuku_instruction rdrand(fuku_type dst);
    fuku_instruction rdseed(fuku_type dst);

//SYSTEM INSTRUCTIONS
    fuku_instruction hlt();
    fuku_instruction rdtsc();
    fuku_instruction lfence();
};

