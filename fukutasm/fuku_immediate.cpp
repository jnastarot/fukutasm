#include "stdafx.h"
#include "fuku_immediate.h"


fuku_immediate::fuku_immediate()
    :immediate_value(0), relocate(false) {}

fuku_immediate::fuku_immediate(uint64_t immediate)
    : immediate_value(immediate), relocate(false) {}

fuku_immediate::fuku_immediate(uint64_t immediate, bool is_rel)
    : immediate_value(immediate), relocate(is_rel) {}

fuku_immediate::fuku_immediate(const fuku_immediate& imm) {
    this->operator=(imm);
}

fuku_immediate::~fuku_immediate() {};

fuku_immediate& fuku_immediate::operator=(const fuku_immediate& imm) {
    this->relocate = imm.relocate;
    this->immediate_value = imm.immediate_value;
    return *this;
}

fuku_immediate& fuku_immediate::set_relocate(bool is_rel) {
    this->relocate = is_rel;
    return *this;
}

fuku_immediate& fuku_immediate::set_immediate(uint64_t immediate) {
    this->immediate_value = immediate;
    return *this;
}

bool fuku_immediate::is_8() const {
    return (
        immediate_value <= 0x7F ||
        immediate_value >= 0xFFFFFFFFFFFFFF80
        );
}
bool fuku_immediate::is_16() const {
    return (
        immediate_value <= 0x7FFF ||
        immediate_value >= 0xFFFFFFFFFFFF8000
        );
}
bool fuku_immediate::is_32() const {
    return (
        immediate_value <= 0x7FFFFFFF ||
        immediate_value >= 0xFFFFFFFF80000000
        );
}
bool fuku_immediate::is_64() const {
    return (immediate_value & 0xFFFFFFFF00000000) != 0;
}

fuku_operand_size fuku_immediate::get_size() const {
    if (is_8()) {
        return fuku_operand_size::FUKU_OPERAND_SIZE_8;
    }
    if (is_16()) {
        return fuku_operand_size::FUKU_OPERAND_SIZE_16;
    }
    if (is_32()) {
        return fuku_operand_size::FUKU_OPERAND_SIZE_32;
    }
    if (is_64()) {
        return fuku_operand_size::FUKU_OPERAND_SIZE_64;
    }

    return fuku_operand_size::FUKU_OPERAND_SIZE_0;
}

bool fuku_immediate::is_relocate() const {
    return this->relocate;
}

uint8_t fuku_immediate::get_immediate8() const {
    return this->immediate_value & 0xFF;
}

uint16_t fuku_immediate::get_immediate16() const {
    return this->immediate_value & 0xFFFF;
}

uint32_t fuku_immediate::get_immediate32() const {
    return this->immediate_value & 0xFFFFFFFF;
}

uint64_t fuku_immediate::get_immediate64() const {
    return this->immediate_value;
}

int8_t fuku_immediate::get_signed_value8() const {
    return (int8_t)this->immediate_value;
}

int16_t fuku_immediate::get_signed_value16() const {
    return (int16_t)this->immediate_value;
}

int32_t fuku_immediate::get_signed_value32() const {
    return (int32_t)this->immediate_value;
}

int64_t fuku_immediate::get_signed_value64() const {
    return (int64_t)this->immediate_value;
}