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
    this->lntrnl_asm = new fuku_internal_assambler(arch);
}

fuku_assambler::~fuku_assambler() {
    delete this->lntrnl_asm;
}

void fuku_assambler::set_optimization_flags(uint8_t flags) {
    this->lntrnl_asm->set_optimization_flags(flags);
}
void fuku_assambler::set_arch(fuku_assambler_arch arch) {
    this->lntrnl_asm->set_arch(arch);
}
fuku_assambler_arch fuku_assambler::get_arch() {
    return this->lntrnl_asm->get_arch();
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
                return this->lntrnl_asm->_mov_b(dst.get_register(), src.get_register());
            }
            case FUKU_T0_OPERAND: {
                return this->lntrnl_asm->_mov_b(dst.get_register(), src.get_operand());
            }
            case FUKU_T0_IMMEDIATE: {
                return this->lntrnl_asm->_mov_b(dst.get_register(), src.get_immediate());
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
                return this->lntrnl_asm->_mov_b(dst.get_operand(), src.get_register());
            }
            case FUKU_T0_IMMEDIATE: {
                return this->lntrnl_asm->_mov_b(dst.get_operand(), src.get_immediate());
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
                return this->lntrnl_asm->_mov_w(dst.get_register(), src.get_register());
            }
            case FUKU_T0_OPERAND: {
                return this->lntrnl_asm->_mov_w(dst.get_register(), src.get_operand());
            }
            case FUKU_T0_IMMEDIATE: {
                return this->lntrnl_asm->_mov_w(dst.get_register(), src.get_immediate());
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
                return this->lntrnl_asm->_mov_w(dst.get_operand(), src.get_register());
            }
            case FUKU_T0_IMMEDIATE: {
                return this->lntrnl_asm->_mov_w(dst.get_operand(), src.get_immediate());
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
                return this->lntrnl_asm->_mov_dw(dst.get_register(), src.get_register());
            }
            case FUKU_T0_OPERAND: {
                return this->lntrnl_asm->_mov_dw(dst.get_register(), src.get_operand());
            }
            case FUKU_T0_IMMEDIATE: {
                return this->lntrnl_asm->_mov_dw(dst.get_register(), src.get_immediate());
            }
            default: { FUKU_DEBUG; break; }
            }
            break;
        }
        case FUKU_T0_OPERAND: {
            switch (src.get_type()) {
            case FUKU_T0_REGISTER: {
                return this->lntrnl_asm->_mov_dw(dst.get_operand(), src.get_register());
            }
            case FUKU_T0_IMMEDIATE: {
                return this->lntrnl_asm->_mov_dw(dst.get_operand(), src.get_immediate());
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
                return this->lntrnl_asm->_mov_qw(dst.get_register(), src.get_register());
            }
            case FUKU_T0_OPERAND: {
                return this->lntrnl_asm->_mov_qw(dst.get_register(), src.get_operand());
            }
            case FUKU_T0_IMMEDIATE: {
                return this->lntrnl_asm->_mov_qw(dst.get_register(), src.get_immediate());
            }
            default: { FUKU_DEBUG; break; }
            }
            break;
        }
        case FUKU_T0_OPERAND: {
            switch (src.get_type()) {
            case FUKU_T0_REGISTER: {
                return this->lntrnl_asm->_mov_qw(dst.get_operand(), src.get_register());
            }
            case FUKU_T0_IMMEDIATE: {
                return this->lntrnl_asm->_mov_qw(dst.get_operand(), src.get_immediate());
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
    return fuku_instruction();
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
    return fuku_instruction();
}
fuku_instruction fuku_assambler::das() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::aaa() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::aas() {
    return fuku_instruction();
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
    return fuku_instruction();
}
fuku_instruction fuku_assambler::enter(fuku_type size, fuku_type nestinglevel) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::leave_() {
    return fuku_instruction();
}

//String Instructions
fuku_instruction fuku_assambler::outsb() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::outsw() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::outsd() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::movsb() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::movsw() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::movsd() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::movsq() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::cmpsb() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::cmpsw() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::cmpsd() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::cmpsq() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::scasb() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::scasw() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::scasd() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::scasq() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::lodsb() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::lodsw() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::lodsd() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::lodsq() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::stosb() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::stosw() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::stosd() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::stosq() {
    return fuku_instruction();
}

fuku_instruction fuku_assambler::stc() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::clc() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::cmc() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::cld() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::std() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::lahf() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::sahf() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::pusha() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::pushad() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::popa() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::popad() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::pushf() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::pushfd() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::pushfq() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::popf() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::popfd() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::popfq() {
    return fuku_instruction();
}
//Miscellaneous Instructions
fuku_instruction fuku_assambler::lea(fuku_type dst, fuku_type src) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::nop(uint8_t size) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::ud2() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::cpuid() {
    return fuku_instruction();
}
//Random Number Generator Instructions
fuku_instruction fuku_assambler::rdrand(fuku_type dst) {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::rdseed(fuku_type dst) {
    return fuku_instruction();
}
//SYSTEM INSTRUCTIONS
fuku_instruction fuku_assambler::hlt() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::rdtsc() {
    return fuku_instruction();
}
fuku_instruction fuku_assambler::lfence() {
    return fuku_instruction();
}