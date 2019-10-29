#include "stdafx.h"
#include "fuku_operand.h"


fuku_operand::fuku_operand(const fuku_register& base, fuku_operand_size size)
    : segment(FUKU_PREFIX_NONE), base(base), index(FUKU_REG_NONE), scale(FUKU_OPERAND_SCALE_1), disp(0), size(size) {}
fuku_operand::fuku_operand(fuku_register_enum base, fuku_operand_size size)
    : segment(FUKU_PREFIX_NONE), base(base), index(FUKU_REG_NONE), scale(FUKU_OPERAND_SCALE_1), disp(0), size(size) {}

fuku_operand::fuku_operand(uint32_t disp, fuku_operand_size size)
    : segment(FUKU_PREFIX_NONE), base(FUKU_REG_NONE), index(FUKU_REG_NONE), scale(FUKU_OPERAND_SCALE_1), disp(disp), size(size) {}

fuku_operand::fuku_operand(const fuku_immediate& disp, fuku_operand_size size)
    : segment(FUKU_PREFIX_NONE), base(FUKU_REG_NONE), index(FUKU_REG_NONE), scale(FUKU_OPERAND_SCALE_1), disp(disp), size(size) {}

fuku_operand::fuku_operand(const fuku_register& base, const fuku_immediate& disp, fuku_operand_size size)
    : segment(FUKU_PREFIX_NONE), base(base), index(FUKU_REG_NONE), scale(FUKU_OPERAND_SCALE_1), disp(disp), size(size) {}
fuku_operand::fuku_operand(fuku_register_enum base, const fuku_immediate& disp, fuku_operand_size size)
    : segment(FUKU_PREFIX_NONE), base(base), index(FUKU_REG_NONE), scale(FUKU_OPERAND_SCALE_1), disp(disp), size(size) {}

fuku_operand::fuku_operand(const fuku_register& base, const fuku_register& index, fuku_operand_scale scale, const fuku_immediate& disp, fuku_operand_size size)
    : segment(FUKU_PREFIX_NONE), base(base), index(index), scale(scale), disp(disp), size(size) {}
fuku_operand::fuku_operand(fuku_register_enum base, fuku_register_enum index, fuku_operand_scale scale, const fuku_immediate& disp, fuku_operand_size size)
    : segment(FUKU_PREFIX_NONE), base(base), index(index), scale(scale), disp(disp), size(size) {}

fuku_operand::fuku_operand(const fuku_register& index, fuku_operand_scale scale, const fuku_immediate& disp, fuku_operand_size size)
    : segment(FUKU_PREFIX_NONE), base(FUKU_REG_NONE), index(index), scale(scale), disp(disp), size(size) {}
fuku_operand::fuku_operand(fuku_register_enum index, fuku_operand_scale scale, const fuku_immediate& disp, fuku_operand_size size)
    : segment(FUKU_PREFIX_NONE), base(FUKU_REG_NONE), index(index), scale(scale), disp(disp), size(size) {}

fuku_operand::fuku_operand(fuku_prefix segment, const fuku_register& base, fuku_operand_size size)
    : segment(segment), base(base), index(FUKU_REG_NONE), scale(FUKU_OPERAND_SCALE_1), disp(0), size(size) {}
fuku_operand::fuku_operand(fuku_prefix segment, fuku_register_enum base, fuku_operand_size size)
    : segment(segment), base(base), index(FUKU_REG_NONE), scale(FUKU_OPERAND_SCALE_1), disp(0), size(size) {}

fuku_operand::fuku_operand(fuku_prefix segment, uint32_t disp, fuku_operand_size size)
    : segment(segment), base(FUKU_REG_NONE), index(FUKU_REG_NONE), scale(FUKU_OPERAND_SCALE_1), disp(disp), size(size) {}

fuku_operand::fuku_operand(fuku_prefix segment, const fuku_immediate& disp, fuku_operand_size size)
    : segment(segment), base(FUKU_REG_NONE), index(FUKU_REG_NONE), scale(FUKU_OPERAND_SCALE_1), disp(disp), size(size) {}

fuku_operand::fuku_operand(fuku_prefix segment, const fuku_register& base, const fuku_immediate& disp, fuku_operand_size size)
    : segment(segment), base(base), index(FUKU_REG_NONE), scale(FUKU_OPERAND_SCALE_1), disp(disp), size(size) {}
fuku_operand::fuku_operand(fuku_prefix segment, fuku_register_enum base, const fuku_immediate& disp, fuku_operand_size size)
    : segment(segment), base(base), index(FUKU_REG_NONE), scale(FUKU_OPERAND_SCALE_1), disp(disp), size(size) {}

fuku_operand::fuku_operand(fuku_prefix segment, const fuku_register& base, const fuku_register& index, fuku_operand_scale scale, const fuku_immediate& disp, fuku_operand_size size)
    : segment(segment), base(base), index(index), scale(scale), disp(disp), size(size) {}
fuku_operand::fuku_operand(fuku_prefix segment, fuku_register_enum base, fuku_register_enum index, fuku_operand_scale scale, const fuku_immediate& disp, fuku_operand_size size)
    : segment(segment), base(base), index(index), scale(scale), disp(disp), size(size) {}

fuku_operand::fuku_operand(fuku_prefix segment, const fuku_register& index, fuku_operand_scale scale, const fuku_immediate& disp, fuku_operand_size size)
    : segment(segment), base(FUKU_REG_NONE), index(index), scale(scale), disp(disp), size(size) {}
fuku_operand::fuku_operand(fuku_prefix segment, fuku_register_enum index, fuku_operand_scale scale, const fuku_immediate& disp, fuku_operand_size size)
    : segment(segment), base(FUKU_REG_NONE), index(index), scale(scale), disp(disp), size(size) {}

fuku_operand::fuku_operand(const fuku_operand& op) {
    this->operator=(op);
}

fuku_operand::~fuku_operand() {}

fuku_operand& fuku_operand::operator=(const fuku_operand& op) {
    this->segment = op.segment;
    this->base = op.base;
    this->index = op.index;
    this->scale = op.scale;
    this->disp = op.disp;
    this->size = op.size;

    return *this;
}

void fuku_operand::set_base(const fuku_register& base) {
    this->base = base;
}

void fuku_operand::set_index(const fuku_register& index) {
    this->index = index;
}

void fuku_operand::set_scale(fuku_operand_scale scale) {
    this->scale = scale;
}

void fuku_operand::set_disp(const fuku_immediate& disp) {
    this->disp = disp;
}

void fuku_operand::set_segment(fuku_prefix segment) {
    this->segment = segment;
}

void fuku_operand::set_size(fuku_operand_size size) {
    this->size = size;
}

const fuku_register& fuku_operand::get_base() const {
    return this->base;
}

const fuku_register& fuku_operand::get_index() const {
    return this->index;
}

fuku_operand_scale fuku_operand::get_scale() const {
    return this->scale;
}

fuku_prefix fuku_operand::get_segment() const {
    return this->segment;
}

const fuku_immediate& fuku_operand::get_disp() const {
    return this->disp;
}

fuku_operand_size fuku_operand::get_size() const {
    return this->size;
}

fuku_mem_opernad_type fuku_operand::get_type() const {

    if (base.get_reg() != FUKU_REG_NONE) {
        if (index.get_reg() != FUKU_REG_NONE) {
            if (disp.get_immediate64()) {
                return fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_INDEX_DISP;
            }
            else {
                return fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_INDEX;
            }
        }
        else {
            if (disp.get_immediate64()) {
                return fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_DISP;
            }
            else {
                return fuku_mem_opernad_type::FUKU_MEM_OPERAND_BASE_ONLY;
            }
        }
    }
    else {
        if (index.get_reg() != FUKU_REG_NONE) {
            return fuku_mem_opernad_type::FUKU_MEM_OPERAND_INDEX_DISP;
        }
        else {
            return fuku_mem_opernad_type::FUKU_MEM_OPERAND_DISP_ONLY;
        }
    }
}

uint8_t fuku_operand::get_low_rex() const {
    return ((index.is_ext64() ? 1 : 0) << 1) | (base.is_ext64() ? 1 : 0);
}