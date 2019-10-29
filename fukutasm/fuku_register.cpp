#include "stdafx.h"
#include "fuku_register.h"


fuku_register::fuku_register()
    :reg(FUKU_REG_NONE), index(FUKU_REG_INDEX_INVALID), size(FUKU_OPERAND_SIZE_0), ext64(0), arch64(0) {}

fuku_register::fuku_register(const fuku_register& reg) {
    operator=(reg);
}
fuku_register::fuku_register(fuku_register_enum reg) {
    this->set_reg(reg);;
}
fuku_register::~fuku_register() {}

fuku_register& fuku_register::operator=(const fuku_register& reg) {
    this->reg = reg.reg;
    this->size = reg.size;
    this->index = reg.index;
    this->ext64 = reg.ext64;
    this->arch64 = reg.arch64;

    return *this;
}

fuku_register& fuku_register::operator=(fuku_register_enum reg) {
    set_reg(reg);

    return *this;
}

void fuku_register::set_reg(fuku_register_enum reg) {
    this->reg = reg;
    this->size = get_register_size(reg);
    this->index = get_index_by_register(reg);
    this->arch64 = is_x64arch_register(reg);

    if (this->arch64) {
        this->ext64 = is_x64arch_ext_register(reg);
    }
    else {
        this->ext64 = false;
    }
}

fuku_register_enum fuku_register::get_reg() const {
    return this->reg;
}
fuku_register_index fuku_register::get_index() const {
    return this->index;
}
fuku_operand_size fuku_register::get_size() const {
    return this->size;
}
bool fuku_register::is_ext64() const {
    return this->ext64;
}
bool fuku_register::is_arch64() const {
    return this->arch64;
}