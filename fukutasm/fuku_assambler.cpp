#include "stdafx.h"
#include "fuku_assambler.h"

fuku_type::fuku_type(fuku_register reg)
    :reg(reg), type(FUKU_T0_REGISTER){}
fuku_type::fuku_type(const fuku_operand& op)
    : op(&op), type(FUKU_T0_OPERAND) {}
fuku_type::fuku_type(const fuku_immediate& imm)
    : imm(&imm), type(FUKU_T0_IMMEDIATE) {}
fuku_type::~fuku_type(){}

const fuku_t0_types fuku_type::get_type() const {
    return this->type;
}
const fuku_register fuku_type::get_register() const {
    return this->reg;
}
const fuku_operand  &fuku_type::get_operand() const {
    return *this->op;
}
const fuku_immediate &fuku_type::get_immediate() const {
    return *this->imm;
}

#include "fuku_assambler_misc.h"

fuku_assambler::fuku_assambler(fuku_assambler_arch arch) {

}

fuku_assambler::~fuku_assambler() {

}

void fuku_assambler::set_optimization_flags(uint8_t flags) {
    set_optimization_flags(flags);
}
void fuku_assambler::set_arch(fuku_assambler_arch arch) {
    set_arch(arch);
}
fuku_assambler_arch fuku_assambler::get_arch() {
    return get_arch();
}

fuku_instruction fuku_assambler::mov(fuku_type dst, fuku_type src) {
    FUKU_ASSERT_EQ(is_valid_op_r_i(dst, src), false);
    
    fuku_operand_size size = get_minimal_op_size(dst, src);

    switch (size) {
    case fuku_operand_size::FUKU_OPERAND_SIZE_8: {
        switch (dst.get_type()) {
        case FUKU_T0_REGISTER: {
            switch (src.get_type()) {

            case FUKU_T0_REGISTER: {
                return _mov_b(dst.get_register(), src.get_register());
            }
            case FUKU_T0_OPERAND: {
                return _mov_b(dst.get_register(), src.get_operand());
            }
            case FUKU_T0_IMMEDIATE: {
                return _mov_b(dst.get_register(), src.get_immediate());
            }
            default: {
                FUKU_DEBUG;
                break;
            }
            }
            break;
        }
        case FUKU_T0_OPERAND: {
            switch (src.get_type()) {

            case FUKU_T0_REGISTER: {
                return _mov_b(dst.get_operand(), src.get_register());
            }
            case FUKU_T0_IMMEDIATE: {
                return _mov_b(dst.get_operand(), src.get_immediate());
            }
            default: { FUKU_DEBUG; break; }
            }
            break;
        }
        default: { FUKU_DEBUG; break; }
        }
        break;
    }
    case fuku_operand_size::FUKU_OPERAND_SIZE_16: {
        switch (dst.get_type()) {
        case FUKU_T0_REGISTER: {
            switch (src.get_type()) {
            case FUKU_T0_REGISTER: {
                return _mov_w(context, dst.get_register(), src.get_register());
            }
            case FUKU_T0_OPERAND: {
                return _mov_w(dst.get_register(), src.get_operand());
            }
            case FUKU_T0_IMMEDIATE: {
                return _mov_w(dst.get_register(), src.get_immediate());
            }
            default: {
                FUKU_DEBUG;
                break;
            }
            }
            break;
        }
        case FUKU_T0_OPERAND: {
            switch (src.get_type()) {
            case FUKU_T0_REGISTER: {
                return _mov_w(dst.get_operand(), src.get_register());
            }
            case FUKU_T0_IMMEDIATE: {
                return _mov_w(dst.get_operand(), src.get_immediate());
            }
            default: { FUKU_DEBUG; break; }
            }
            break;
        }
        default: { FUKU_DEBUG; break; }
        }
        break;
    }
    case fuku_operand_size::FUKU_OPERAND_SIZE_32: {
        switch (dst.get_type()) {
        case FUKU_T0_REGISTER: {
            switch (src.get_type()) {
            case FUKU_T0_REGISTER: {
                return _mov_dw(dst.get_register(), src.get_register());
            }
            case FUKU_T0_OPERAND: {
                return _mov_dw(dst.get_register(), src.get_operand());
            }
            case FUKU_T0_IMMEDIATE: {
                return _mov_dw(dst.get_register(), src.get_immediate());
            }
            default: { FUKU_DEBUG; break; }
            }
            break;
        }
        case FUKU_T0_OPERAND: {
            switch (src.get_type()) {
            case FUKU_T0_REGISTER: {
                return _mov_dw(dst.get_operand(), src.get_register());
            }
            case FUKU_T0_IMMEDIATE: {
                return _mov_dw(dst.get_operand(), src.get_immediate());
            }
            default: { FUKU_DEBUG; break; }
            }
            break;
        }
        default: { FUKU_DEBUG; break; }
        }
        break;
    }
    case fuku_operand_size::FUKU_OPERAND_SIZE_64: {
        switch (dst.get_type()) {
        case FUKU_T0_REGISTER: {
            switch (src.get_type()) {
            case FUKU_T0_REGISTER: {
                return _mov_qw(dst.get_register(), src.get_register());
            }
            case FUKU_T0_OPERAND: {
                return _mov_qw(dst.get_register(), src.get_operand());
            }
            case FUKU_T0_IMMEDIATE: {
                return _mov_qw(dst.get_register(), src.get_immediate());
            }
            default: { FUKU_DEBUG; break; }
            }
            break;
        }
        case FUKU_T0_OPERAND: {
            switch (src.get_type()) {
            case FUKU_T0_REGISTER: {
                return _mov_qw(dst.get_operand(), src.get_register());
            }
            case FUKU_T0_IMMEDIATE: {
                return _mov_qw(dst.get_operand(), src.get_immediate());
            }
            default: { FUKU_DEBUG; break; }
            }
            break;
        }
        default: { FUKU_DEBUG; break; }
        }
        break;
    }
    default: { FUKU_DEBUG; break; }
    }
    return fuku_instruction();
}

fuku_instruction fuku_assambler::cmovcc(fuku_condition cond, fuku_type dst, fuku_type src) {
    fuku_instruction();
}
fuku_instruction fuku_assambler::xchg(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::bswap(fuku_type dst) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::xadd(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::cmpxchg(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::cmpxchg8b(fuku_type dst) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::cmpxchg16b(fuku_type dst) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::push(fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::pop(fuku_type dst) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::cwd() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::cdq() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::cqo() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::movzx(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::movsx(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}

//Binary Arithmetic Instructions
fuku_instruction fuku_assambler::adcx(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::adox(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::add(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::adc(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::sub(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::sbb(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::imul(fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::mul(fuku_type dst) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::idiv(fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::div(fuku_type dst) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::inc(fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::dec(fuku_type dst) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::neg(fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::cmp(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}

//Decimal Arithmetic Instructions
fuku_instruction fuku_assambler::daa() {
    _daa(context);
}
fuku_instruction fuku_assambler::das() {
    _das(context);
}
fuku_instruction fuku_assambler::aaa() {
    _aaa(context);
}
fuku_instruction fuku_assambler::aas() {
    _aas(context);
}
fuku_instruction fuku_assambler::aam(fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::aad(fuku_type src) {
    return fuku_instruction();
}

//Logical Instructions Instructions
fuku_instruction fuku_assambler::and_(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::or_(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::xor_(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::not_(fuku_type dst) {
    return fuku_instruction();
}

//Shift and Rotate Instructions
fuku_instruction fuku_assambler::sar(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::shr(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::shl(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::shrd(fuku_type dst, fuku_type src, fuku_type shift) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::shld(fuku_type dst, fuku_type src, fuku_type shift) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::ror(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::rol(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::rcr(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::rcl(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}

//Bit and Byte Instructions
fuku_instruction fuku_assambler::bt(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::bts(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::btr(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::btc(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::setcc(fuku_condition cond, fuku_type dst) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::test(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::popcnt(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}

//Control Transfer Instructions
fuku_instruction fuku_assambler::jmp(fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::jcc(fuku_condition cond, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::call(fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::ret(fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::int3() {
    _int3(context);
}
fuku_instruction fuku_assambler::enter(fuku_type size, fuku_type nestinglevel) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::leave_() {
    _leave_(context);
}

//String Instructions
fuku_instruction fuku_assambler::outsb() {
    _outsb(context);
}
fuku_instruction fuku_assambler::outsw() {
    _outsw(context);
}
fuku_instruction fuku_assambler::outsd() {
    _outsd(context);
}
fuku_instruction fuku_assambler::movsb() {
    _movsb(context);
}
fuku_instruction fuku_assambler::movsw() {
    _movsw(context);
}
fuku_instruction fuku_assambler::movsd() {
    _movsd(context);
}
fuku_instruction fuku_assambler::movsq() {
    _movsq(context);
}
fuku_instruction fuku_assambler::cmpsb() {
    _cmpsb(context);
}
fuku_instruction fuku_assambler::cmpsw() {
    _cmpsw(context);
}
fuku_instruction fuku_assambler::cmpsd() {
    _cmpsd(context);
}
fuku_instruction fuku_assambler::cmpsq() {
    _cmpsq(context);
}
fuku_instruction fuku_assambler::scasb() {
    _scasb(context);
}
fuku_instruction fuku_assambler::scasw() {
    _scasw(context);
}
fuku_instruction fuku_assambler::scasd() {
    _scasd(context);
}
fuku_instruction fuku_assambler::scasq() {
    _scasq(context);
}
fuku_instruction fuku_assambler::lodsb() {
    _lodsb(context);
}
fuku_instruction fuku_assambler::lodsw() {
    _lodsw(context);
}
fuku_instruction fuku_assambler::lodsd() {
    _lodsd(context);
}
fuku_instruction fuku_assambler::lodsq() {
    _lodsq(context);
}
fuku_instruction fuku_assambler::stosb() {
    _stosb(context);
}
fuku_instruction fuku_assambler::stosw() {
    _stosw(context);
}
fuku_instruction fuku_assambler::stosd() {
    _stosd(context);
}
fuku_instruction fuku_assambler::stosq() {
    _stosq(context);
}

fuku_instruction fuku_assambler::stc() {
    _stc(context);
}
fuku_instruction fuku_assambler::clc() {
    _clc(context);
}
fuku_instruction fuku_assambler::cmc() {
    _cmc(context);
}
fuku_instruction fuku_assambler::cld() {
    _cld(context);
}
fuku_instruction fuku_assambler::std() {
    _std(context);
}
fuku_instruction fuku_assambler::lahf() {
    _lahf(context);
}
fuku_instruction fuku_assambler::sahf() {
    _sahf(context);
}
fuku_instruction fuku_assambler::pusha() {
    _pusha(context);
}
fuku_instruction fuku_assambler::pushad() {
    _pushad(context);
}
fuku_instruction fuku_assambler::popa() {
    _popa(context);
}
fuku_instruction fuku_assambler::popad() {
    _popad(context);
}
fuku_instruction fuku_assambler::pushf() {
    _pushf(context);
}
fuku_instruction fuku_assambler::pushfd() {
    _pushfd(context);
}
fuku_instruction fuku_assambler::pushfq() {
    _pushfq(context);
}
fuku_instruction fuku_assambler::popf() {
    _popf(context);
}
fuku_instruction fuku_assambler::popfd() {
    _popfd(context);
}
fuku_instruction fuku_assambler::popfq() {
    _popfq(context);
}
//Miscellaneous Instructions
fuku_instruction fuku_assambler::lea(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::nop(uint8_t size) {
    _nop(context, size);
}
fuku_instruction fuku_assambler::ud2() {
    _ud2(context);
}
fuku_instruction fuku_assambler::cpuid() {
    _cpuid(context);
}
//Random Number Generator Instructions
fuku_instruction fuku_assambler::rdrand(fuku_type dst) {
    if (dst.get_type() != FUKU_T0_REGISTER) {
        FUKU_DEBUG;
    };

    return fuku_instruction();
}
fuku_instruction fuku_assambler::rdseed(fuku_type dst) {
    return fuku_instruction();
}
//SYSTEM INSTRUCTIONS
fuku_instruction fuku_assambler::hlt() {
    _hlt(context);
}
fuku_instruction fuku_assambler::rdtsc() {
    _rdtsc(context);
}
fuku_instruction fuku_assambler::lfence() {
    _lfence(context);
}