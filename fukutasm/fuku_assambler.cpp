#include "stdafx.h"
#include "fuku_assambler.h"
#include "fuku_assambler_misc.h"

fuku_type::fuku_type(fuku_register reg)
    :reg(reg), type(FUKU_T0_REGISTER){}
fuku_type::fuku_type(const fuku_operand& op)
    : op(&op), type(FUKU_T0_OPERAND) {}
fuku_type::fuku_type(const fuku_immediate& imm)
    : imm(&imm), type(FUKU_T0_IMMEDIATE) {}
fuku_type::~fuku_type(){}

fuku_t0_types fuku_type::get_type() {
    return this->type;
}
fuku_register fuku_type::get_register() {
    return this->reg;
}
const fuku_operand  &fuku_type::get_operand() {
    return *this->op;
}
const fuku_immediate &fuku_type::get_immediate() {
    return *this->imm;
}


fuku_assambler::fuku_assambler()
{
}


fuku_assambler::~fuku_assambler()
{
}

fuku_instruction fuku_assambler::mov(fuku_type dst, fuku_type src) {

    
}