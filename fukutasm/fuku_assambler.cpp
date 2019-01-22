#include "stdafx.h"
#include "fuku_assambler.h"

using namespace fukutasm;

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

fuku_assambler_ctx& fuku_assambler::get_context() {
    return this->context;
}

void fuku_assambler::mov(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        _mov_b(context, dst.get_register(), src.get_register());,
        _mov_b(context, dst.get_register(), src.get_operand());,
        _mov_b(context, dst.get_register(), src.get_immediate());,
        _mov_b(context, dst.get_operand(), src.get_register());,
        _mov_b(context, dst.get_operand(), src.get_immediate());,

        _mov_w(context, dst.get_register(), src.get_register()); ,
        _mov_w(context, dst.get_register(), src.get_operand()); ,
        _mov_w(context, dst.get_register(), src.get_immediate()); ,
        _mov_w(context, dst.get_operand(), src.get_register()); ,
        _mov_w(context, dst.get_operand(), src.get_immediate()); ,

        _mov_dw(context, dst.get_register(), src.get_register()); ,
        _mov_dw(context, dst.get_register(), src.get_operand()); ,
        _mov_dw(context, dst.get_register(), src.get_immediate()); ,
        _mov_dw(context, dst.get_operand(), src.get_register()); ,
        _mov_dw(context, dst.get_operand(), src.get_immediate()); ,

        _mov_qw(context, dst.get_register(), src.get_register()); ,
        _mov_qw(context, dst.get_register(), src.get_operand()); ,
        _mov_qw(context, dst.get_register(), src.get_immediate()); ,
        _mov_qw(context, dst.get_operand(), src.get_register()); ,
        _mov_qw(context, dst.get_operand(), src.get_immediate()); 
    )
}

void fuku_assambler::cmovcc(fuku_condition cond, const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,

        _cmovcc_w(context, cond, dst.get_register(), src.get_register());,
        _cmovcc_w(context, cond, dst.get_register(), src.get_operand());,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,

        _cmovcc_dw(context, cond, dst.get_register(), src.get_register());,
        _cmovcc_dw(context, cond, dst.get_register(), src.get_operand());,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,

        _cmovcc_qw(context, cond, dst.get_register(), src.get_register());,
        _cmovcc_qw(context, cond, dst.get_register(), src.get_operand());,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG
    )
}
void fuku_assambler::xchg(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        _xchg_b(context, dst.get_register(), src.get_register());,
        _xchg_b(context, src.get_operand(), dst.get_register());,
        FUKU_DEBUG,
        _xchg_b(context, dst.get_operand(), src.get_register());,
        FUKU_DEBUG,

        _xchg_w(context, dst.get_operand(), src.get_register());,
        _xchg_w(context, src.get_operand(), dst.get_register());,
        FUKU_DEBUG,
        _xchg_w(context, dst.get_operand(), src.get_register());,
        FUKU_DEBUG,

        _xchg_dw(context, dst.get_register(), src.get_register());,
        _xchg_dw(context, src.get_operand(), dst.get_register());,
        FUKU_DEBUG,
        _xchg_dw(context, dst.get_operand(), src.get_register());,
        FUKU_DEBUG,

        _xchg_qw(context, dst.get_register(), src.get_register());,
        _xchg_qw(context, src.get_operand(), dst.get_register());,
        FUKU_DEBUG,
        _xchg_qw(context, dst.get_operand(), src.get_register());,
        FUKU_DEBUG
    )
}
void fuku_assambler::bswap(const fuku_type& dst) {
    
}
void fuku_assambler::xadd(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        _xadd_b(context, dst.get_register(), src.get_register());,
        FUKU_DEBUG,
        FUKU_DEBUG,
        _xadd_b(context, dst.get_operand(), src.get_register());,
        FUKU_DEBUG,

        _xadd_w(context, dst.get_register(), src.get_register()); ,
        FUKU_DEBUG,
        FUKU_DEBUG,
        _xadd_w(context, dst.get_operand(), src.get_register()); ,
        FUKU_DEBUG,

        _xadd_dw(context, dst.get_register(), src.get_register()); ,
        FUKU_DEBUG,
        FUKU_DEBUG,
        _xadd_dw(context, dst.get_operand(), src.get_register()); ,
        FUKU_DEBUG,

        _xadd_qw(context, dst.get_register(), src.get_register()); ,
        FUKU_DEBUG,
        FUKU_DEBUG,
        _xadd_qw(context, dst.get_operand(), src.get_register()); ,
        FUKU_DEBUG
    )
}
void fuku_assambler::cmpxchg(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        _cmpxchg_b(context, dst.get_register(), src.get_register());,
        FUKU_DEBUG,
        FUKU_DEBUG,
        _cmpxchg_b(context, dst.get_operand(), src.get_register());,
        FUKU_DEBUG,

        _cmpxchg_w(context, dst.get_register(), src.get_register()); ,
        FUKU_DEBUG,
        FUKU_DEBUG,
        _cmpxchg_w(context, dst.get_operand(), src.get_register()); ,
        FUKU_DEBUG,

        _cmpxchg_dw(context, dst.get_register(), src.get_register()); ,
        FUKU_DEBUG,
        FUKU_DEBUG,
        _cmpxchg_dw(context, dst.get_operand(), src.get_register()); ,
        FUKU_DEBUG,

        _cmpxchg_qw(context, dst.get_register(), src.get_register()); ,
        FUKU_DEBUG,
        FUKU_DEBUG,
        _cmpxchg_qw(context, dst.get_operand(), src.get_register()); ,
        FUKU_DEBUG
    )
}
void fuku_assambler::cmpxchg8b(const fuku_type& dst) {
    
}
void fuku_assambler::cmpxchg16b(const fuku_type& dst) {
    
}
void fuku_assambler::push(const fuku_type& src) {
    
}
void fuku_assambler::pop(const fuku_type& dst) {
    
}
void fuku_assambler::cwd() {
    _cwd(context);
}
void fuku_assambler::cdq() {
    _cdq(context);
}
void fuku_assambler::cqo() {
    _cqo(context);
}
void fuku_assambler::movzx(const fuku_type& dst, const fuku_type& src) {
    
}
void fuku_assambler::movsx(const fuku_type& dst, const fuku_type& src) {
    
}

//Binary Arithmetic Instructions
void fuku_assambler::adcx(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,

        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,

        _adcx_dw(context, dst.get_register(), src.get_register()); ,
        _adcx_dw(context, dst.get_register(), src.get_operand()); ,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,

        _adcx_qw(context, dst.get_register(), src.get_register()); ,
        _adcx_qw(context, dst.get_register(), src.get_operand()); ,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG
    )
}
void fuku_assambler::adox(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        FUKU_DEBUG, 
        FUKU_DEBUG, 
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,

        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,

        _adcx_dw(context, dst.get_register(), src.get_register());,
        _adcx_dw(context, dst.get_register(), src.get_operand());,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,

        _adcx_qw(context, dst.get_register(), src.get_register());,
        _adcx_qw(context, dst.get_register(), src.get_operand());,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG
        )
}
void fuku_assambler::add(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        _add_b(context, dst.get_register(), src.get_register());,
        _add_b(context, dst.get_register(), src.get_operand());,
        _add_b(context, dst.get_register(), src.get_immediate());,
        _add_b(context, dst.get_operand(), src.get_register());,
        _add_b(context, dst.get_operand(), src.get_immediate());,

        _add_w(context, dst.get_register(), src.get_register()); ,
        _add_w(context, dst.get_register(), src.get_operand()); ,
        _add_w(context, dst.get_register(), src.get_immediate()); ,
        _add_w(context, dst.get_operand(), src.get_register()); ,
        _add_w(context, dst.get_operand(), src.get_immediate()); ,

        _add_dw(context, dst.get_register(), src.get_register()); ,
        _add_dw(context, dst.get_register(), src.get_operand()); ,
        _add_dw(context, dst.get_register(), src.get_immediate()); ,
        _add_dw(context, dst.get_operand(), src.get_register()); ,
        _add_dw(context, dst.get_operand(), src.get_immediate()); ,

        _add_qw(context, dst.get_register(), src.get_register()); ,
        _add_qw(context, dst.get_register(), src.get_operand()); ,
        _add_qw(context, dst.get_register(), src.get_immediate()); ,
        _add_qw(context, dst.get_operand(), src.get_register()); ,
        _add_qw(context, dst.get_operand(), src.get_immediate()); 
    )
}
void fuku_assambler::adc(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        _adc_b(context, dst.get_register(), src.get_register());,
        _adc_b(context, dst.get_register(), src.get_operand());,
        _adc_b(context, dst.get_register(), src.get_immediate());,
        _adc_b(context, dst.get_operand(), src.get_register());,
        _adc_b(context, dst.get_operand(), src.get_immediate());,

        _adc_w(context, dst.get_register(), src.get_register()); ,
        _adc_w(context, dst.get_register(), src.get_operand()); ,
        _adc_w(context, dst.get_register(), src.get_immediate()); ,
        _adc_w(context, dst.get_operand(), src.get_register()); ,
        _adc_w(context, dst.get_operand(), src.get_immediate()); ,

        _adc_dw(context, dst.get_register(), src.get_register()); ,
        _adc_dw(context, dst.get_register(), src.get_operand()); ,
        _adc_dw(context, dst.get_register(), src.get_immediate()); ,
        _adc_dw(context, dst.get_operand(), src.get_register()); ,
        _adc_dw(context, dst.get_operand(), src.get_immediate()); ,

        _adc_qw(context, dst.get_register(), src.get_register()); ,
        _adc_qw(context, dst.get_register(), src.get_operand()); ,
        _adc_qw(context, dst.get_register(), src.get_immediate()); ,
        _adc_qw(context, dst.get_operand(), src.get_register()); ,
        _adc_qw(context, dst.get_operand(), src.get_immediate()); 
    )
}
void fuku_assambler::sub(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        _sub_b(context, dst.get_register(), src.get_register());,
        _sub_b(context, dst.get_register(), src.get_operand());,
        _sub_b(context, dst.get_register(), src.get_immediate());,
        _sub_b(context, dst.get_operand(), src.get_register());,
        _sub_b(context, dst.get_operand(), src.get_immediate());,

        _sub_w(context, dst.get_register(), src.get_register()); ,
        _sub_w(context, dst.get_register(), src.get_operand()); ,
        _sub_w(context, dst.get_register(), src.get_immediate()); ,
        _sub_w(context, dst.get_operand(), src.get_register()); ,
        _sub_w(context, dst.get_operand(), src.get_immediate()); ,

        _sub_dw(context, dst.get_register(), src.get_register()); ,
        _sub_dw(context, dst.get_register(), src.get_operand()); ,
        _sub_dw(context, dst.get_register(), src.get_immediate()); ,
        _sub_dw(context, dst.get_operand(), src.get_register()); ,
        _sub_dw(context, dst.get_operand(), src.get_immediate()); ,

        _sub_qw(context, dst.get_register(), src.get_register()); ,
        _sub_qw(context, dst.get_register(), src.get_operand()); ,
        _sub_qw(context, dst.get_register(), src.get_immediate()); ,
        _sub_qw(context, dst.get_operand(), src.get_register()); ,
        _sub_qw(context, dst.get_operand(), src.get_immediate()); 
    )
}
void fuku_assambler::sbb(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        _sbb_b(context, dst.get_register(), src.get_register());,
        _sbb_b(context, dst.get_register(), src.get_operand());,
        _sbb_b(context, dst.get_register(), src.get_immediate());,
        _sbb_b(context, dst.get_operand(), src.get_register());,
        _sbb_b(context, dst.get_operand(), src.get_immediate());,

        _sbb_w(context, dst.get_register(), src.get_register()); ,
        _sbb_w(context, dst.get_register(), src.get_operand()); ,
        _sbb_w(context, dst.get_register(), src.get_immediate()); ,
        _sbb_w(context, dst.get_operand(), src.get_register()); ,
        _sbb_w(context, dst.get_operand(), src.get_immediate()); ,

        _sbb_dw(context, dst.get_register(), src.get_register()); ,
        _sbb_dw(context, dst.get_register(), src.get_operand()); ,
        _sbb_dw(context, dst.get_register(), src.get_immediate()); ,
        _sbb_dw(context, dst.get_operand(), src.get_register()); ,
        _sbb_dw(context, dst.get_operand(), src.get_immediate()); ,

        _sbb_qw(context, dst.get_register(), src.get_register()); ,
        _sbb_qw(context, dst.get_register(), src.get_operand()); ,
        _sbb_qw(context, dst.get_register(), src.get_immediate()); ,
        _sbb_qw(context, dst.get_operand(), src.get_register()); ,
        _sbb_qw(context, dst.get_operand(), src.get_immediate()); 
    )
}
void fuku_assambler::imul(const fuku_type& src) {
    
}
void fuku_assambler::mul(const fuku_type& dst) {
    
}
void fuku_assambler::idiv(const fuku_type& src) {
    
}
void fuku_assambler::div(const fuku_type& dst) {
    
}
void fuku_assambler::inc(const fuku_type& src) {
    
}
void fuku_assambler::dec(const fuku_type& dst) {
    
}
void fuku_assambler::neg(const fuku_type& src) {
    
}
void fuku_assambler::cmp(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        _cmp_b(context, dst.get_register(), src.get_register());,
        _cmp_b(context, dst.get_register(), src.get_operand());,
        _cmp_b(context, dst.get_register(), src.get_immediate());,
        _cmp_b(context, dst.get_operand(), src.get_register());,
        _cmp_b(context, dst.get_operand(), src.get_immediate());,

        _cmp_w(context, dst.get_register(), src.get_register()); ,
        _cmp_w(context, dst.get_register(), src.get_operand()); ,
        _cmp_w(context, dst.get_register(), src.get_immediate()); ,
        _cmp_w(context, dst.get_operand(), src.get_register()); ,
        _cmp_w(context, dst.get_operand(), src.get_immediate()); ,

        _cmp_dw(context, dst.get_register(), src.get_register()); ,
        _cmp_dw(context, dst.get_register(), src.get_operand()); ,
        _cmp_dw(context, dst.get_register(), src.get_immediate()); ,
        _cmp_dw(context, dst.get_operand(), src.get_register()); ,
        _cmp_dw(context, dst.get_operand(), src.get_immediate()); ,

        _cmp_qw(context, dst.get_register(), src.get_register()); ,
        _cmp_qw(context, dst.get_register(), src.get_operand()); ,
        _cmp_qw(context, dst.get_register(), src.get_immediate()); ,
        _cmp_qw(context, dst.get_operand(), src.get_register()); ,
        _cmp_qw(context, dst.get_operand(), src.get_immediate()); 
    )
}

//Decimal Arithmetic Instructions
void fuku_assambler::daa() {
    _daa(context);
}
void fuku_assambler::das() {
    _das(context);
}
void fuku_assambler::aaa() {
    _aaa(context);
}
void fuku_assambler::aas() {
    _aas(context);
}
void fuku_assambler::aam(const fuku_type& src) {
    _aam(context, src.get_immediate());
}
void fuku_assambler::aad(const fuku_type& src) {
    _aad(context, src.get_immediate());
}

//Logical Instructions Instructions
void fuku_assambler::and_(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        _and_b(context, dst.get_register(), src.get_register());,
        _and_b(context, dst.get_register(), src.get_operand());,
        _and_b(context, dst.get_register(), src.get_immediate());,
        _and_b(context, dst.get_operand(), src.get_register());,
        _and_b(context, dst.get_operand(), src.get_immediate());,

        _and_w(context, dst.get_register(), src.get_register()); ,
        _and_w(context, dst.get_register(), src.get_operand()); ,
        _and_w(context, dst.get_register(), src.get_immediate()); ,
        _and_w(context, dst.get_operand(), src.get_register()); ,
        _and_w(context, dst.get_operand(), src.get_immediate()); ,

        _and_dw(context, dst.get_register(), src.get_register()); ,
        _and_dw(context, dst.get_register(), src.get_operand()); ,
        _and_dw(context, dst.get_register(), src.get_immediate()); ,
        _and_dw(context, dst.get_operand(), src.get_register()); ,
        _and_dw(context, dst.get_operand(), src.get_immediate()); ,

        _and_qw(context, dst.get_register(), src.get_register()); ,
        _and_qw(context, dst.get_register(), src.get_operand()); ,
        _and_qw(context, dst.get_register(), src.get_immediate()); ,
        _and_qw(context, dst.get_operand(), src.get_register()); ,
        _and_qw(context, dst.get_operand(), src.get_immediate()); 
    )
}
void fuku_assambler::or_(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        _or_b(context, dst.get_register(), src.get_register());,
        _or_b(context, dst.get_register(), src.get_operand());,
        _or_b(context, dst.get_register(), src.get_immediate());,
        _or_b(context, dst.get_operand(), src.get_register());,
        _or_b(context, dst.get_operand(), src.get_immediate());,

        _or_w(context, dst.get_register(), src.get_register()); ,
        _or_w(context, dst.get_register(), src.get_operand()); ,
        _or_w(context, dst.get_register(), src.get_immediate()); ,
        _or_w(context, dst.get_operand(), src.get_register()); ,
        _or_w(context, dst.get_operand(), src.get_immediate()); ,

        _or_dw(context, dst.get_register(), src.get_register()); ,
        _or_dw(context, dst.get_register(), src.get_operand()); ,
        _or_dw(context, dst.get_register(), src.get_immediate()); ,
        _or_dw(context, dst.get_operand(), src.get_register()); ,
        _or_dw(context, dst.get_operand(), src.get_immediate()); ,

        _or_qw(context, dst.get_register(), src.get_register()); ,
        _or_qw(context, dst.get_register(), src.get_operand()); ,
        _or_qw(context, dst.get_register(), src.get_immediate()); ,
        _or_qw(context, dst.get_operand(), src.get_register()); ,
        _or_qw(context, dst.get_operand(), src.get_immediate()); 
    )
}
void fuku_assambler::xor_(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        _xor_b(context, dst.get_register(), src.get_register());,
        _xor_b(context, dst.get_register(), src.get_operand());,
        _xor_b(context, dst.get_register(), src.get_immediate());,
        _xor_b(context, dst.get_operand(), src.get_register());,
        _xor_b(context, dst.get_operand(), src.get_immediate());,

        _xor_w(context, dst.get_register(), src.get_register()); ,
        _xor_w(context, dst.get_register(), src.get_operand()); ,
        _xor_w(context, dst.get_register(), src.get_immediate()); ,
        _xor_w(context, dst.get_operand(), src.get_register()); ,
        _xor_w(context, dst.get_operand(), src.get_immediate()); ,

        _xor_dw(context, dst.get_register(), src.get_register()); ,
        _xor_dw(context, dst.get_register(), src.get_operand()); ,
        _xor_dw(context, dst.get_register(), src.get_immediate()); ,
        _xor_dw(context, dst.get_operand(), src.get_register()); ,
        _xor_dw(context, dst.get_operand(), src.get_immediate()); ,

        _xor_qw(context, dst.get_register(), src.get_register()); ,
        _xor_qw(context, dst.get_register(), src.get_operand()); ,
        _xor_qw(context, dst.get_register(), src.get_immediate()); ,
        _xor_qw(context, dst.get_operand(), src.get_register()); ,
        _xor_qw(context, dst.get_operand(), src.get_immediate()); 
    )
}
void fuku_assambler::not_(const fuku_type& dst) {
    
}

//Shift and Rotate Instructions
void fuku_assambler::sar(const fuku_type& dst, const fuku_type& src) {
    
}
void fuku_assambler::shr(const fuku_type& dst, const fuku_type& src) {
    
}
void fuku_assambler::shl(const fuku_type& dst, const fuku_type& src) {
    
}
void fuku_assambler::shrd(const fuku_type& dst, const fuku_type& src, const fuku_type& shift) {
    
}
void fuku_assambler::shld(const fuku_type& dst, const fuku_type& src, const fuku_type& shift) {
    
}
void fuku_assambler::ror(const fuku_type& dst, const fuku_type& src) {
    
}
void fuku_assambler::rol(const fuku_type& dst, const fuku_type& src) {
    
}
void fuku_assambler::rcr(const fuku_type& dst, const fuku_type& src) {
    
}
void fuku_assambler::rcl(const fuku_type& dst, const fuku_type& src) {
    
}

//Bit and Byte Instructions
void fuku_assambler::bt(const fuku_type& dst, const fuku_type& src) {
    
}
void fuku_assambler::bts(const fuku_type& dst, const fuku_type& src) {
    
}
void fuku_assambler::btr(const fuku_type& dst, const fuku_type& src) {
    
}
void fuku_assambler::btc(const fuku_type& dst, const fuku_type& src) {
    
}
void fuku_assambler::setcc(fuku_condition cond, const fuku_type& dst) {
    
}
void fuku_assambler::test(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        _test_b(context, dst.get_register(), src.get_register());,
        FUKU_DEBUG,
        _test_b(context, dst.get_register(), src.get_immediate());,
        _test_b(context, dst.get_operand(), src.get_register());,
        _test_b(context, dst.get_operand(), src.get_immediate());,

        _test_w(context, dst.get_register(), src.get_register()); ,
        FUKU_DEBUG,
        _test_w(context, dst.get_register(), src.get_immediate()); ,
        _test_w(context, dst.get_operand(), src.get_register()); ,
        _test_w(context, dst.get_operand(), src.get_immediate()); ,

        _test_dw(context, dst.get_register(), src.get_register()); ,
        FUKU_DEBUG,
        _test_dw(context, dst.get_register(), src.get_immediate()); ,
        _test_dw(context, dst.get_operand(), src.get_register()); ,
        _test_dw(context, dst.get_operand(), src.get_immediate()); ,

        _test_qw(context, dst.get_register(), src.get_register()); ,
        FUKU_DEBUG,
        _test_qw(context, dst.get_register(), src.get_immediate()); ,
        _test_qw(context, dst.get_operand(), src.get_register()); ,
        _test_qw(context, dst.get_operand(), src.get_immediate()); 
    )
}
void fuku_assambler::popcnt(const fuku_type& dst, const fuku_type& src) {
    
}

//Control Transfer Instructions
void fuku_assambler::jmp(const fuku_type& src) {
    
}
void fuku_assambler::jcc(fuku_condition cond, const fuku_type& src) {
    
}
void fuku_assambler::call(const fuku_type& src) {
    
}
void fuku_assambler::ret(const fuku_type& src) {
    
}
void fuku_assambler::int3() {
    _int3(context);
}
void fuku_assambler::enter(const fuku_type& size, const fuku_type& nestinglevel) {
    
}
void fuku_assambler::leave_() {
    _leave_(context);
}

//String Instructions
void fuku_assambler::outsb() {
    _outsb(context);
}
void fuku_assambler::outsw() {
    _outsw(context);
}
void fuku_assambler::outsd() {
    _outsd(context);
}
void fuku_assambler::movsb() {
    _movsb(context);
}
void fuku_assambler::movsw() {
    _movsw(context);
}
void fuku_assambler::movsd() {
    _movsd(context);
}
void fuku_assambler::movsq() {
    _movsq(context);
}
void fuku_assambler::cmpsb() {
    _cmpsb(context);
}
void fuku_assambler::cmpsw() {
    _cmpsw(context);
}
void fuku_assambler::cmpsd() {
    _cmpsd(context);
}
void fuku_assambler::cmpsq() {
    _cmpsq(context);
}
void fuku_assambler::scasb() {
    _scasb(context);
}
void fuku_assambler::scasw() {
    _scasw(context);
}
void fuku_assambler::scasd() {
    _scasd(context);
}
void fuku_assambler::scasq() {
    _scasq(context);
}
void fuku_assambler::lodsb() {
    _lodsb(context);
}
void fuku_assambler::lodsw() {
    _lodsw(context);
}
void fuku_assambler::lodsd() {
    _lodsd(context);
}
void fuku_assambler::lodsq() {
    _lodsq(context);
}
void fuku_assambler::stosb() {
    _stosb(context);
}
void fuku_assambler::stosw() {
    _stosw(context);
}
void fuku_assambler::stosd() {
    _stosd(context);
}
void fuku_assambler::stosq() {
    _stosq(context);
}

void fuku_assambler::stc() {
    _stc(context);
}
void fuku_assambler::clc() {
    _clc(context);
}
void fuku_assambler::cmc() {
    _cmc(context);
}
void fuku_assambler::cld() {
    _cld(context);
}
void fuku_assambler::std() {
    _std(context);
}
void fuku_assambler::lahf() {
    _lahf(context);
}
void fuku_assambler::sahf() {
    _sahf(context);
}
void fuku_assambler::pusha() {
    _pusha(context);
}
void fuku_assambler::pushad() {
    _pushad(context);
}
void fuku_assambler::popa() {
    _popa(context);
}
void fuku_assambler::popad() {
    _popad(context);
}
void fuku_assambler::pushf() {
    _pushf(context);
}
void fuku_assambler::pushfd() {
    _pushfd(context);
}
void fuku_assambler::pushfq() {
    _pushfq(context);
}
void fuku_assambler::popf() {
    _popf(context);
}
void fuku_assambler::popfd() {
    _popfd(context);
}
void fuku_assambler::popfq() {
    _popfq(context);
}
//Miscellaneous Instructions
void fuku_assambler::lea(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,

        FUKU_DEBUG,
        _lea_w(context, dst.get_register(), src.get_operand()); ,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,

        FUKU_DEBUG,
        _lea_dw(context, dst.get_register(), src.get_operand()); ,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG,

        FUKU_DEBUG,
        _lea_qw(context, dst.get_register(), src.get_operand()); ,
        FUKU_DEBUG,
        FUKU_DEBUG,
        FUKU_DEBUG
    )
}
void fuku_assambler::nop(uint8_t size) {
    _nop(context, size);
}
void fuku_assambler::ud2() {
    _ud2(context);
}
void fuku_assambler::cpuid() {
    _cpuid(context);
}
//Random Number Generator Instructions
void fuku_assambler::rdrand(const fuku_type& dst) {
    if (dst.get_type() != FUKU_T0_REGISTER) {
        FUKU_DEBUG;
    };

    
}
void fuku_assambler::rdseed(const fuku_type& dst) {
    
}
//SYSTEM INSTRUCTIONS
void fuku_assambler::hlt() {
    _hlt(context);
}
void fuku_assambler::rdtsc() {
    _rdtsc(context);
}
void fuku_assambler::lfence() {
    _lfence(context);
}