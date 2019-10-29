#include "stdafx.h"
#include "fuku_type.h"


fuku_type::fuku_type()
    :segment(FUKU_PREFIX_NONE), base(FUKU_REG_NONE), index(FUKU_REG_NONE), scale(FUKU_OPERAND_SCALE_1), disp(0), size(FUKU_OPERAND_SIZE_0), type(FUKU_T0_NONE) {}
fuku_type::fuku_type(const fuku_register& reg)
    : segment(FUKU_PREFIX_NONE), base(reg.get_reg()), index(FUKU_REG_NONE), scale(FUKU_OPERAND_SCALE_1), disp(0), size(reg.get_size()), type(FUKU_T0_REGISTER) {}
fuku_type::fuku_type(const fuku_operand& op)
    : segment(op.get_segment()), base(op.get_base()), index(op.get_index()), scale(op.get_scale()), disp(op.get_disp()), size(op.get_size()), type(FUKU_T0_OPERAND) {}
fuku_type::fuku_type(const fuku_immediate& imm)
    : segment(FUKU_PREFIX_NONE), base(FUKU_REG_NONE), index(FUKU_REG_NONE), scale(FUKU_OPERAND_SCALE_1), disp(imm), size(FUKU_OPERAND_SIZE_0), type(FUKU_T0_IMMEDIATE) {}
fuku_type::~fuku_type() { }


fuku_type& fuku_type::operator=(const fuku_register& _reg) {
    this->segment = FUKU_PREFIX_NONE;
    this->base = _reg.get_reg();
    this->index = FUKU_REG_NONE;
    this->scale = FUKU_OPERAND_SCALE_1;
    this->disp = 0;
    this->size = _reg.get_size();
    this->type = FUKU_T0_REGISTER;
    return *this;
}
fuku_type& fuku_type::operator=(const fuku_operand& _op) {
    this->segment = _op.get_segment();
    this->base = _op.get_base();
    this->index = _op.get_index();
    this->scale = _op.get_scale();
    this->disp = _op.get_disp();
    this->size = _op.get_size();
    this->type = FUKU_T0_OPERAND;
    return *this;
}
fuku_type& fuku_type::operator=(const fuku_immediate& _imm) {
    this->segment = FUKU_PREFIX_NONE;
    this->base = FUKU_REG_NONE;
    this->index = FUKU_REG_NONE;
    this->scale = FUKU_OPERAND_SCALE_1;
    this->disp = _imm;
    this->size = FUKU_OPERAND_SIZE_0;
    this->type = FUKU_T0_IMMEDIATE;
    return *this;
}

const fuku_t0_types fuku_type::get_type() const {
    return this->type;
}
fuku_register fuku_type::get_register() const {
    return this->base;
}
fuku_operand fuku_type::get_operand() const {
    return fuku_operand(this->segment, this->base, this->index, this->scale, this->disp, this->size);
}
fuku_immediate fuku_type::get_immediate() const {
    return this->disp;
}

const fuku_register& fuku_type::get_base() const {
    return this->base;
}

const fuku_register& fuku_type::get_index() const {
    return this->index;
}

fuku_operand_scale fuku_type::get_scale() const {
    return this->scale;
}

const fuku_immediate& fuku_type::get_disp() const {
    return this->disp;
}

fuku_operand_size fuku_type::get_size() const {
    return this->size;
}

fuku_prefix fuku_type::get_segment() const {
    return this->segment;
}
