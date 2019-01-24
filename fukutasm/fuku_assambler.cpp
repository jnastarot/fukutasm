#include "stdafx.h"
#include "fuku_assambler.h"

#define UNUSUAL_DATASET FUKU_DEBUG

using namespace fukutasm;

fuku_type::fuku_type(const fuku_register& reg)
    :reg(&reg), type(FUKU_T0_REGISTER){}
fuku_type::fuku_type(const fuku_operand& op)
    : op(&op), type(FUKU_T0_OPERAND) {}
fuku_type::fuku_type(const fuku_immediate& imm)
    : imm(&imm), type(FUKU_T0_IMMEDIATE) {}
fuku_type::~fuku_type(){}

const fuku_t0_types fuku_type::get_type() const {
    return this->type;
}
const fuku_register &fuku_type::get_register() const {
    return *this->reg;
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


void fuku_assambler::on_new_chain_item() {
    
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
    on_new_chain_item();
}

void fuku_assambler::cmovcc(fuku_condition cond, const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _cmovcc_w(context, cond, dst.get_register(), src.get_register());,
        _cmovcc_w(context, cond, dst.get_register(), src.get_operand());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _cmovcc_dw(context, cond, dst.get_register(), src.get_register());,
        _cmovcc_dw(context, cond, dst.get_register(), src.get_operand());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _cmovcc_qw(context, cond, dst.get_register(), src.get_register());,
        _cmovcc_qw(context, cond, dst.get_register(), src.get_operand());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
}
void fuku_assambler::xchg(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        _xchg_b(context, dst.get_register(), src.get_register());,
        _xchg_b(context, src.get_operand(), dst.get_register());,
        UNUSUAL_DATASET,
        _xchg_b(context, dst.get_operand(), src.get_register());,
        UNUSUAL_DATASET,

        _xchg_w(context, dst.get_operand(), src.get_register());,
        _xchg_w(context, src.get_operand(), dst.get_register());,
        UNUSUAL_DATASET,
        _xchg_w(context, dst.get_operand(), src.get_register());,
        UNUSUAL_DATASET,

        _xchg_dw(context, dst.get_register(), src.get_register());,
        _xchg_dw(context, src.get_operand(), dst.get_register());,
        UNUSUAL_DATASET,
        _xchg_dw(context, dst.get_operand(), src.get_register());,
        UNUSUAL_DATASET,

        _xchg_qw(context, dst.get_register(), src.get_register());,
        _xchg_qw(context, src.get_operand(), dst.get_register());,
        UNUSUAL_DATASET,
        _xchg_qw(context, dst.get_operand(), src.get_register());,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
}
void fuku_assambler::bswap(const fuku_type& dst) {
    fuku_assambler_command_1op_graph(dst,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _bswap_dw(context, dst.get_register());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _bswap_qw(context, dst.get_register());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
}
void fuku_assambler::xadd(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        _xadd_b(context, dst.get_register(), src.get_register());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _xadd_b(context, dst.get_operand(), src.get_register());,
        UNUSUAL_DATASET,

        _xadd_w(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _xadd_w(context, dst.get_operand(), src.get_register()); ,
        UNUSUAL_DATASET,

        _xadd_dw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _xadd_dw(context, dst.get_operand(), src.get_register()); ,
        UNUSUAL_DATASET,

        _xadd_qw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _xadd_qw(context, dst.get_operand(), src.get_register()); ,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
}
void fuku_assambler::cmpxchg(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        _cmpxchg_b(context, dst.get_register(), src.get_register());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _cmpxchg_b(context, dst.get_operand(), src.get_register());,
        UNUSUAL_DATASET,

        _cmpxchg_w(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _cmpxchg_w(context, dst.get_operand(), src.get_register()); ,
        UNUSUAL_DATASET,

        _cmpxchg_dw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _cmpxchg_dw(context, dst.get_operand(), src.get_register()); ,
        UNUSUAL_DATASET,

        _cmpxchg_qw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _cmpxchg_qw(context, dst.get_operand(), src.get_register()); ,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
}
void fuku_assambler::cmpxchg8b(const fuku_type& dst) {
    fuku_assambler_command_1op_graph(dst,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        
        UNUSUAL_DATASET,
        _cmpxchg8b(context, dst.get_operand()); ,
        UNUSUAL_DATASET,
        
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
}
void fuku_assambler::cmpxchg16b(const fuku_type& dst) {
    fuku_assambler_command_1op_graph(dst,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        _cmpxchg16b(context, dst.get_operand()); ,
        UNUSUAL_DATASET,
        
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
}
void fuku_assambler::push(const fuku_type& src) {
    fuku_assambler_command_1op_graph(src,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _push_w(context,  src.get_register());,
        _push_w(context, src.get_operand());  ,
        _push_w(context, src.get_immediate());,

        _push_dw(context,  src.get_register());,
        _push_dw(context, src.get_operand());  ,
        _push_dw(context, src.get_immediate());,

        _push_qw(context,  src.get_register());,
        _push_qw(context, src.get_operand());  ,
        _push_qw(context, src.get_immediate());
    )
    on_new_chain_item();
}
void fuku_assambler::pop(const fuku_type& dst) {
    fuku_assambler_command_1op_graph(dst,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _pop_w(context, dst.get_register()); ,
        _pop_w(context, dst.get_operand());  ,
        UNUSUAL_DATASET,

        _pop_dw(context, dst.get_register());,
        _pop_dw(context, dst.get_operand()); ,
        UNUSUAL_DATASET,

        _pop_qw(context, dst.get_register());,
        _pop_qw(context, dst.get_operand()); ,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
}
void fuku_assambler::cwd() {
    _cwd(context); on_new_chain_item();
}
void fuku_assambler::cdq() {
    _cdq(context); on_new_chain_item();
}
void fuku_assambler::cqo() {
    _cqo(context); on_new_chain_item();
}
void fuku_assambler::movzx(const fuku_type& dst, const fuku_type& src) {
    
}
void fuku_assambler::movsx(const fuku_type& dst, const fuku_type& src) {
    
}

//Binary Arithmetic Instructions
void fuku_assambler::adcx(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _adcx_dw(context, dst.get_register(), src.get_register()); ,
        _adcx_dw(context, dst.get_register(), src.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _adcx_qw(context, dst.get_register(), src.get_register()); ,
        _adcx_qw(context, dst.get_register(), src.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
}
void fuku_assambler::adox(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET, 
        UNUSUAL_DATASET, 
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _adcx_dw(context, dst.get_register(), src.get_register());,
        _adcx_dw(context, dst.get_register(), src.get_operand());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _adcx_qw(context, dst.get_register(), src.get_register());,
        _adcx_qw(context, dst.get_register(), src.get_operand());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
        )
    on_new_chain_item();
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
    on_new_chain_item();
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
    on_new_chain_item();
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
    on_new_chain_item();
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
    on_new_chain_item();
}
void fuku_assambler::imul(const fuku_type& src) {
    fuku_assambler_command_1op_graph(src,
        _imul_b(context, src.get_register());,
        _imul_b(context, src.get_operand()); ,
        UNUSUAL_DATASET,

        _imul_w(context, src.get_register()); ,
        _imul_w(context, src.get_operand());,
        UNUSUAL_DATASET,

        _imul_dw(context, src.get_register()); ,
        _imul_dw(context, src.get_operand());,
        UNUSUAL_DATASET,

        _imul_qw(context, src.get_register()); ,
        _imul_qw(context, src.get_operand());,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
}
void fuku_assambler::mul(const fuku_type& dst) {
    fuku_assambler_command_1op_graph(dst,
        _mul_b(context, dst.get_register());,
        _mul_b(context, dst.get_operand()); ,
        UNUSUAL_DATASET,

        _mul_w(context, dst.get_register()); ,
        _mul_w(context, dst.get_operand());,
        UNUSUAL_DATASET,

        _mul_dw(context, dst.get_register()); ,
        _mul_dw(context, dst.get_operand());,
        UNUSUAL_DATASET,

        _mul_qw(context, dst.get_register()); ,
        _mul_qw(context, dst.get_operand());,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
}
void fuku_assambler::idiv(const fuku_type& src) { 
    fuku_assambler_command_1op_graph(src,
        _idiv_b(context, src.get_register());,
        _idiv_b(context, src.get_operand()); ,
        UNUSUAL_DATASET,

        _idiv_w(context, src.get_register()); ,
        _idiv_w(context, src.get_operand());,
        UNUSUAL_DATASET,

        _idiv_dw(context, src.get_register()); ,
        _idiv_dw(context, src.get_operand());,
        UNUSUAL_DATASET,

        _idiv_qw(context, src.get_register()); ,
        _idiv_qw(context, src.get_operand());,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
}
void fuku_assambler::div(const fuku_type& dst) {
    fuku_assambler_command_1op_graph(dst,
        _div_b(context, dst.get_register());,
        _div_b(context, dst.get_operand()); ,
        UNUSUAL_DATASET,

        _div_w(context, dst.get_register()); ,
        _div_w(context, dst.get_operand());,
        UNUSUAL_DATASET,

        _div_dw(context, dst.get_register()); ,
        _div_dw(context, dst.get_operand());,
        UNUSUAL_DATASET,

        _div_qw(context, dst.get_register()); ,
        _div_qw(context, dst.get_operand());,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
}
void fuku_assambler::inc(const fuku_type& src) {
    fuku_assambler_command_1op_graph(src,
        _inc_b(context, src.get_register());,
        _inc_b(context, src.get_operand()); ,
        UNUSUAL_DATASET,

        _inc_w(context, src.get_register()); ,
        _inc_w(context, src.get_operand());,
        UNUSUAL_DATASET,

        _inc_dw(context, src.get_register()); ,
        _inc_dw(context, src.get_operand());,
        UNUSUAL_DATASET,

        _inc_qw(context, src.get_register()); ,
        _inc_qw(context, src.get_operand());,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
}
void fuku_assambler::dec(const fuku_type& dst) {
    fuku_assambler_command_1op_graph(dst,
        _dec_b(context, dst.get_register());,
        _dec_b(context, dst.get_operand()); ,
        UNUSUAL_DATASET,

        _dec_w(context, dst.get_register()); ,
        _dec_w(context, dst.get_operand());,
        UNUSUAL_DATASET,

        _dec_dw(context, dst.get_register()); ,
        _dec_dw(context, dst.get_operand());,
        UNUSUAL_DATASET,

        _dec_qw(context, dst.get_register()); ,
        _dec_qw(context, dst.get_operand());,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
}
void fuku_assambler::neg(const fuku_type& src) {
    fuku_assambler_command_1op_graph(src,
        _neg_b(context, src.get_register());,
        _neg_b(context, src.get_operand()); ,
        UNUSUAL_DATASET,

        _neg_w(context, src.get_register()); ,
        _neg_w(context, src.get_operand());,
        UNUSUAL_DATASET,

        _neg_dw(context, src.get_register()); ,
        _neg_dw(context, src.get_operand());,
        UNUSUAL_DATASET,

        _neg_qw(context, src.get_register()); ,
        _neg_qw(context, src.get_operand());,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
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
    on_new_chain_item();
}

//Decimal Arithmetic Instructions
void fuku_assambler::daa() {
    _daa(context); on_new_chain_item();
}
void fuku_assambler::das() {
    _das(context); on_new_chain_item();
}
void fuku_assambler::aaa() {
    _aaa(context); on_new_chain_item();
}
void fuku_assambler::aas() {
    _aas(context); on_new_chain_item();
}
void fuku_assambler::aam(const fuku_type& src) {
    _aam(context, src.get_immediate()); on_new_chain_item();
}
void fuku_assambler::aad(const fuku_type& src) {
    _aad(context, src.get_immediate()); on_new_chain_item();
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
    on_new_chain_item();
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
    on_new_chain_item();
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
    on_new_chain_item();
}
void fuku_assambler::not_(const fuku_type& dst) {   
    fuku_assambler_command_1op_graph(dst,
        _not_b(context, dst.get_register());,
        _not_b(context, dst.get_operand()); ,
        UNUSUAL_DATASET,

        _not_w(context, dst.get_register()); ,
        _not_w(context, dst.get_operand());,
        UNUSUAL_DATASET,

        _not_dw(context, dst.get_register()); ,
        _not_dw(context, dst.get_operand());,
        UNUSUAL_DATASET,

        _not_qw(context, dst.get_register()); ,
        _not_qw(context, dst.get_operand());,
        UNUSUAL_DATASET,
    )
    on_new_chain_item();
}

//Shift and Rotate Instructions
void fuku_assambler::sar(const fuku_type& dst, const fuku_type& src) {
    if (src.get_type() == FUKU_T0_REGISTER && src.get_register().get_index() != FUKU_REG_INDEX_CX) {
        UNUSUAL_DATASET
    }

    fuku_assambler_command_2op_graph(
        _sar_cl_b(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        _sar_b(context, dst.get_register(), src.get_immediate()); ,
        _sar_cl_b(context, dst.get_operand()); ,
        _sar_b(context, dst.get_operand(), src.get_immediate()); ,

        _sar_cl_w(context, dst.get_register());,
        UNUSUAL_DATASET,
        _sar_w(context, dst.get_register(), src.get_immediate());,
        _sar_cl_w(context, dst.get_operand());,
        _sar_w(context, dst.get_operand(), src.get_immediate());,

        _sar_cl_dw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _sar_dw(context, dst.get_register(), src.get_immediate());,
        _sar_cl_dw(context, dst.get_operand());,
        _sar_dw(context, dst.get_operand(), src.get_immediate());,

        _sar_cl_qw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _sar_qw(context, dst.get_register(), src.get_immediate());,
        _sar_cl_qw(context, dst.get_operand());,
        _sar_qw(context, dst.get_operand(), src.get_immediate());
    )
    on_new_chain_item();
}
void fuku_assambler::shr(const fuku_type& dst, const fuku_type& src) {
    if (src.get_type() == FUKU_T0_REGISTER && src.get_register().get_index() != FUKU_REG_INDEX_CX) {
        UNUSUAL_DATASET
    }

    fuku_assambler_command_2op_graph(
        _shr_cl_b(context, dst.get_register());,
        UNUSUAL_DATASET,
        _shr_b(context, dst.get_register(), src.get_immediate());,
        _shr_cl_b(context, dst.get_operand());,
        _shr_b(context, dst.get_operand(), src.get_immediate());,

        _shr_cl_w(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        _shr_w(context, dst.get_register(), src.get_immediate()); ,
        _shr_cl_w(context, dst.get_operand()); ,
        _shr_w(context, dst.get_operand(), src.get_immediate()); ,

        _shr_cl_dw(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        _shr_dw(context, dst.get_register(), src.get_immediate()); ,
        _shr_cl_dw(context, dst.get_operand()); ,
        _shr_dw(context, dst.get_operand(), src.get_immediate()); ,

        _shr_cl_qw(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        _shr_qw(context, dst.get_register(), src.get_immediate()); ,
        _shr_cl_qw(context, dst.get_operand()); ,
        _shr_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    on_new_chain_item();
}
void fuku_assambler::shl(const fuku_type& dst, const fuku_type& src) {
    if (src.get_type() == FUKU_T0_REGISTER && src.get_register().get_index() != FUKU_REG_INDEX_CX) {
        UNUSUAL_DATASET
    }

    fuku_assambler_command_2op_graph(
        _shl_cl_b(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        _shl_b(context, dst.get_register(), src.get_immediate()); ,
        _shl_cl_b(context, dst.get_operand()); ,
        _shl_b(context, dst.get_operand(), src.get_immediate()); ,

        _shl_cl_w(context, dst.get_register());,
        UNUSUAL_DATASET,
        _shl_w(context, dst.get_register(), src.get_immediate());,
        _shl_cl_w(context, dst.get_operand());,
        _shl_w(context, dst.get_operand(), src.get_immediate());,

        _shl_cl_dw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _shl_dw(context, dst.get_register(), src.get_immediate());,
        _shl_cl_dw(context, dst.get_operand());,
        _shl_dw(context, dst.get_operand(), src.get_immediate());,

        _shl_cl_qw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _shl_qw(context, dst.get_register(), src.get_immediate());,
        _shl_cl_qw(context, dst.get_operand());,
        _shl_qw(context, dst.get_operand(), src.get_immediate());
    )
    on_new_chain_item();
}
void fuku_assambler::shrd(const fuku_type& dst, const fuku_type& src, const fuku_type& shift) {
    
}
void fuku_assambler::shld(const fuku_type& dst, const fuku_type& src, const fuku_type& shift) {
    
}
void fuku_assambler::ror(const fuku_type& dst, const fuku_type& src) {
    if (src.get_type() == FUKU_T0_REGISTER && src.get_register().get_index() != FUKU_REG_INDEX_CX) {
        UNUSUAL_DATASET
    }

    fuku_assambler_command_2op_graph(
        _ror_cl_b(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        _ror_b(context, dst.get_register(), src.get_immediate()); ,
        _ror_cl_b(context, dst.get_operand()); ,
        _ror_b(context, dst.get_operand(), src.get_immediate()); ,

        _ror_cl_w(context, dst.get_register());,
        UNUSUAL_DATASET,
        _ror_w(context, dst.get_register(), src.get_immediate());,
        _ror_cl_w(context, dst.get_operand());,
        _ror_w(context, dst.get_operand(), src.get_immediate());,

        _ror_cl_dw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _ror_dw(context, dst.get_register(), src.get_immediate());,
        _ror_cl_dw(context, dst.get_operand());,
        _ror_dw(context, dst.get_operand(), src.get_immediate());,

        _ror_cl_qw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _ror_qw(context, dst.get_register(), src.get_immediate());,
        _ror_cl_qw(context, dst.get_operand());,
        _ror_qw(context, dst.get_operand(), src.get_immediate());
    )
    on_new_chain_item();
}
void fuku_assambler::rol(const fuku_type& dst, const fuku_type& src) {
    if (src.get_type() == FUKU_T0_REGISTER && src.get_register().get_index() != FUKU_REG_INDEX_CX) {
        UNUSUAL_DATASET
    }

    fuku_assambler_command_2op_graph(
        _rol_cl_b(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        _rol_b(context, dst.get_register(), src.get_immediate()); ,
        _rol_cl_b(context, dst.get_operand()); ,
        _rol_b(context, dst.get_operand(), src.get_immediate()); ,

        _rol_cl_w(context, dst.get_register());,
        UNUSUAL_DATASET,
        _rol_w(context, dst.get_register(), src.get_immediate());,
        _rol_cl_w(context, dst.get_operand());,
        _rol_w(context, dst.get_operand(), src.get_immediate());,

        _rol_cl_dw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _rol_dw(context, dst.get_register(), src.get_immediate());,
        _rol_cl_dw(context, dst.get_operand());,
        _rol_dw(context, dst.get_operand(), src.get_immediate());,

        _rol_cl_qw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _rol_qw(context, dst.get_register(), src.get_immediate());,
        _rol_cl_qw(context, dst.get_operand());,
        _rol_qw(context, dst.get_operand(), src.get_immediate());
    )
    on_new_chain_item();
}
void fuku_assambler::rcr(const fuku_type& dst, const fuku_type& src) {
    if (src.get_type() == FUKU_T0_REGISTER && src.get_register().get_index() != FUKU_REG_INDEX_CX) {
        UNUSUAL_DATASET
    }

    fuku_assambler_command_2op_graph(
        _rcr_cl_b(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        _rcr_b(context, dst.get_register(), src.get_immediate()); ,
        _rcr_cl_b(context, dst.get_operand()); ,
        _rcr_b(context, dst.get_operand(), src.get_immediate()); ,

        _rcr_cl_w(context, dst.get_register());,
        UNUSUAL_DATASET,
        _rcr_w(context, dst.get_register(), src.get_immediate());,
        _rcr_cl_w(context, dst.get_operand());,
        _rcr_w(context, dst.get_operand(), src.get_immediate());,

        _rcr_cl_dw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _rcr_dw(context, dst.get_register(), src.get_immediate());,
        _rcr_cl_dw(context, dst.get_operand());,
        _rcr_dw(context, dst.get_operand(), src.get_immediate());,

        _rcr_cl_qw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _rcr_qw(context, dst.get_register(), src.get_immediate());,
        _rcr_cl_qw(context, dst.get_operand());,
        _rcr_qw(context, dst.get_operand(), src.get_immediate());
    )
    on_new_chain_item();
}
void fuku_assambler::rcl(const fuku_type& dst, const fuku_type& src) {
    if (src.get_type() == FUKU_T0_REGISTER && src.get_register().get_index() != FUKU_REG_INDEX_CX) {
        UNUSUAL_DATASET
    }

    fuku_assambler_command_2op_graph(
        _rcl_cl_b(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        _rcl_b(context, dst.get_register(), src.get_immediate()); ,
        _rcl_cl_b(context, dst.get_operand()); ,
        _rcl_b(context, dst.get_operand(), src.get_immediate()); ,

        _rcl_cl_w(context, dst.get_register());,
        UNUSUAL_DATASET,
        _rcl_w(context, dst.get_register(), src.get_immediate());,
        _rcl_cl_w(context, dst.get_operand());,
        _rcl_w(context, dst.get_operand(), src.get_immediate());,

        _rcl_cl_dw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _rcl_dw(context, dst.get_register(), src.get_immediate());,
        _rcl_cl_dw(context, dst.get_operand());,
        _rcl_dw(context, dst.get_operand(), src.get_immediate());,

        _rcl_cl_qw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _rcl_qw(context, dst.get_register(), src.get_immediate());,
        _rcl_cl_qw(context, dst.get_operand());,
        _rcl_qw(context, dst.get_operand(), src.get_immediate());
    )
    on_new_chain_item();
}

//Bit and Byte Instructions
void fuku_assambler::bt(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _bt_w(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _bt_w(context, dst.get_register(), src.get_immediate()); ,
        _bt_w(context, dst.get_operand(), src.get_register()); ,
        _bt_w(context, dst.get_operand(), src.get_immediate()); ,

        _bt_dw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _bt_dw(context, dst.get_register(), src.get_immediate()); ,
        _bt_dw(context, dst.get_operand(), src.get_register()); ,
        _bt_dw(context, dst.get_operand(), src.get_immediate()); ,

        _bt_qw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _bt_qw(context, dst.get_register(), src.get_immediate()); ,
        _bt_qw(context, dst.get_operand(), src.get_register()); ,
        _bt_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    on_new_chain_item();
}
void fuku_assambler::bts(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _bts_w(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _bts_w(context, dst.get_register(), src.get_immediate()); ,
        _bts_w(context, dst.get_operand(), src.get_register()); ,
        _bts_w(context, dst.get_operand(), src.get_immediate()); ,

        _bts_dw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _bts_dw(context, dst.get_register(), src.get_immediate()); ,
        _bts_dw(context, dst.get_operand(), src.get_register()); ,
        _bts_dw(context, dst.get_operand(), src.get_immediate()); ,

        _bts_qw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _bts_qw(context, dst.get_register(), src.get_immediate()); ,
        _bts_qw(context, dst.get_operand(), src.get_register()); ,
        _bts_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    on_new_chain_item();
}
void fuku_assambler::btr(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _btr_w(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _btr_w(context, dst.get_register(), src.get_immediate()); ,
        _btr_w(context, dst.get_operand(), src.get_register()); ,
        _btr_w(context, dst.get_operand(), src.get_immediate()); ,

        _btr_dw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _btr_dw(context, dst.get_register(), src.get_immediate()); ,
        _btr_dw(context, dst.get_operand(), src.get_register()); ,
        _btr_dw(context, dst.get_operand(), src.get_immediate()); ,

        _btr_qw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _btr_qw(context, dst.get_register(), src.get_immediate()); ,
        _btr_qw(context, dst.get_operand(), src.get_register()); ,
        _btr_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    on_new_chain_item();
}
void fuku_assambler::btc(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _btc_w(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _btc_w(context, dst.get_register(), src.get_immediate()); ,
        _btc_w(context, dst.get_operand(), src.get_register()); ,
        _btc_w(context, dst.get_operand(), src.get_immediate()); ,

        _btc_dw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _btc_dw(context, dst.get_register(), src.get_immediate()); ,
        _btc_dw(context, dst.get_operand(), src.get_register()); ,
        _btc_dw(context, dst.get_operand(), src.get_immediate()); ,

        _btc_qw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _btc_qw(context, dst.get_register(), src.get_immediate()); ,
        _btc_qw(context, dst.get_operand(), src.get_register()); ,
        _btc_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    on_new_chain_item();
}
void fuku_assambler::setcc(fuku_condition cond, const fuku_type& dst) {
    
}
void fuku_assambler::test(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        _test_b(context, dst.get_register(), src.get_register());,
        UNUSUAL_DATASET,
        _test_b(context, dst.get_register(), src.get_immediate());,
        _test_b(context, dst.get_operand(), src.get_register());,
        _test_b(context, dst.get_operand(), src.get_immediate());,

        _test_w(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _test_w(context, dst.get_register(), src.get_immediate()); ,
        _test_w(context, dst.get_operand(), src.get_register()); ,
        _test_w(context, dst.get_operand(), src.get_immediate()); ,

        _test_dw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _test_dw(context, dst.get_register(), src.get_immediate()); ,
        _test_dw(context, dst.get_operand(), src.get_register()); ,
        _test_dw(context, dst.get_operand(), src.get_immediate()); ,

        _test_qw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _test_qw(context, dst.get_register(), src.get_immediate()); ,
        _test_qw(context, dst.get_operand(), src.get_register()); ,
        _test_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    on_new_chain_item();
}
void fuku_assambler::popcnt(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _popcnt_w(context, dst.get_register(), src.get_register()); ,
        _popcnt_w(context, dst.get_register(), dst.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _popcnt_dw(context, dst.get_register(), src.get_register()); ,
        _popcnt_dw(context, dst.get_register(), dst.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _popcnt_qw(context, dst.get_register(), src.get_register()); ,
        _popcnt_qw(context, dst.get_register(), dst.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
}

//Control Transfer Instructions
void fuku_assambler::jmp(const fuku_type& src) {
    fuku_assambler_command_1op_graph(src,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _jmp(context, src.get_register()); ,
        _jmp(context, src.get_operand());,
        _jmp(context, src.get_immediate()); ,

        _jmp(context, src.get_register()); ,
        _jmp(context, src.get_operand());,
        _jmp(context, src.get_immediate()); ,
    )
    on_new_chain_item();
}
void fuku_assambler::jcc(fuku_condition cond, const fuku_type& src) {
    fuku_assambler_command_1op_graph(src,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _jcc(context, cond, src.get_immediate()); ,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _jcc(context, cond, src.get_immediate());,
    )
    on_new_chain_item();
}
void fuku_assambler::call(const fuku_type& src) {
    fuku_assambler_command_1op_graph(src,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _call(context, src.get_register()); ,
        _call(context, src.get_operand());  ,
        _call(context, src.get_immediate());,

        _call(context, src.get_register()); ,
        _call(context, src.get_operand());  ,
        _call(context, src.get_immediate());,
    )
    on_new_chain_item();
}
void fuku_assambler::ret(const fuku_type& src) {
    
    fuku_assambler_command_1op_graph(src,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        src.get_immediate().get_immediate16() ? _ret(context, src.get_immediate()) : _ret(context); ,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        src.get_immediate().get_immediate16() ? _ret(context, src.get_immediate()) : _ret(context);,
    )
    on_new_chain_item();
}
void fuku_assambler::int3() {
    _int3(context); on_new_chain_item();
}
void fuku_assambler::enter(const fuku_type& size, const fuku_type& nestinglevel) {
    
}
void fuku_assambler::leave_() {
    _leave_(context); on_new_chain_item();
}

//String Instructions
void fuku_assambler::outsb() {
    _outsb(context); on_new_chain_item();
}
void fuku_assambler::outsw() {
    _outsw(context); on_new_chain_item();
}
void fuku_assambler::outsd() {
    _outsd(context); on_new_chain_item();
}
void fuku_assambler::movsb() {
    _movsb(context); on_new_chain_item();
}
void fuku_assambler::movsw() {
    _movsw(context); on_new_chain_item();
}
void fuku_assambler::movsd() {
    _movsd(context); on_new_chain_item();
}
void fuku_assambler::movsq() {
    _movsq(context); on_new_chain_item();
}
void fuku_assambler::cmpsb() {
    _cmpsb(context); on_new_chain_item();
}
void fuku_assambler::cmpsw() {
    _cmpsw(context); on_new_chain_item();
}
void fuku_assambler::cmpsd() {
    _cmpsd(context); on_new_chain_item();
}
void fuku_assambler::cmpsq() {
    _cmpsq(context); on_new_chain_item();
}
void fuku_assambler::scasb() {
    _scasb(context); on_new_chain_item();
}
void fuku_assambler::scasw() {
    _scasw(context); on_new_chain_item();
}
void fuku_assambler::scasd() {
    _scasd(context); on_new_chain_item();
}
void fuku_assambler::scasq() {
    _scasq(context); on_new_chain_item();
}
void fuku_assambler::lodsb() {
    _lodsb(context); on_new_chain_item();
}
void fuku_assambler::lodsw() {
    _lodsw(context); on_new_chain_item();
}
void fuku_assambler::lodsd() {
    _lodsd(context); on_new_chain_item();
}
void fuku_assambler::lodsq() {
    _lodsq(context); on_new_chain_item();
}
void fuku_assambler::stosb() {
    _stosb(context); on_new_chain_item();
}
void fuku_assambler::stosw() {
    _stosw(context); on_new_chain_item();
}
void fuku_assambler::stosd() {
    _stosd(context); on_new_chain_item();
}
void fuku_assambler::stosq() {
    _stosq(context); on_new_chain_item();
}

void fuku_assambler::stc() {
    _stc(context); on_new_chain_item();
}
void fuku_assambler::clc() {
    _clc(context); on_new_chain_item();
}
void fuku_assambler::cmc() {
    _cmc(context); on_new_chain_item();
}
void fuku_assambler::cld() {
    _cld(context); on_new_chain_item();
}
void fuku_assambler::std() {
    _std(context); on_new_chain_item();
}
void fuku_assambler::lahf() {
    _lahf(context); on_new_chain_item();
}
void fuku_assambler::sahf() {
    _sahf(context); on_new_chain_item();
}
void fuku_assambler::pusha() {
    _pusha(context); on_new_chain_item();
}
void fuku_assambler::pushad() {
    _pushad(context); on_new_chain_item();
}
void fuku_assambler::popa() {
    _popa(context); on_new_chain_item();
}
void fuku_assambler::popad() {
    _popad(context); on_new_chain_item();
}
void fuku_assambler::pushf() {
    _pushf(context); on_new_chain_item();
}
void fuku_assambler::pushfd() {
    _pushfd(context); on_new_chain_item();
}
void fuku_assambler::pushfq() {
    _pushfq(context); on_new_chain_item();
}
void fuku_assambler::popf() {
    _popf(context); on_new_chain_item();
}
void fuku_assambler::popfd() {
    _popfd(context); on_new_chain_item();
}
void fuku_assambler::popfq() {
    _popfq(context); on_new_chain_item();
}
//Miscellaneous Instructions
void fuku_assambler::lea(const fuku_type& dst, const fuku_type& src) {
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        _lea_w(context, dst.get_register(), src.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        _lea_dw(context, dst.get_register(), src.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        _lea_qw(context, dst.get_register(), src.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
}
void fuku_assambler::nop(uint8_t size) {
    _nop(context, size); on_new_chain_item();
}
void fuku_assambler::ud2() {
    _ud2(context); on_new_chain_item();
}
void fuku_assambler::cpuid() {
    _cpuid(context); on_new_chain_item();
}
//Random Number Generator Instructions
void fuku_assambler::rdrand(const fuku_type& dst) {
    fuku_assambler_command_1op_graph(dst,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _rdrand_w(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _rdrand_dw(context, dst.get_register());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _rdrand_qw(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
}
void fuku_assambler::rdseed(const fuku_type& dst) {
    fuku_assambler_command_1op_graph(dst,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _rdseed_w(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _rdseed_dw(context, dst.get_register());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _rdseed_qw(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    on_new_chain_item();
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