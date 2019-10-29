#pragma once

#include "fuku_register.h"
#include "fuku_immediate.h"
#include "fuku_operand.h"

enum fuku_t0_types {
    FUKU_T0_NONE,
    FUKU_T0_REGISTER,
    FUKU_T0_OPERAND,
    FUKU_T0_IMMEDIATE,
};


class fuku_type {
    fuku_t0_types type;

    fuku_register base;
    fuku_register index;
    fuku_operand_scale scale;
    fuku_immediate disp;
    fuku_operand_size size;
    fuku_prefix segment;

public:
    fuku_type();
    fuku_type(const fuku_register& reg);
    fuku_type(const fuku_operand& op);
    fuku_type(const fuku_immediate& imm);
    ~fuku_type();

    fuku_type& operator=(const fuku_register& _reg);
    fuku_type& operator=(const fuku_operand& _op);
    fuku_type& operator=(const fuku_immediate& _imm);
public:
    const fuku_t0_types get_type() const;
    fuku_register get_register() const;
    fuku_operand  get_operand() const;
    fuku_immediate get_immediate() const;

public:
    const fuku_register& get_base() const;
    const fuku_register& get_index() const;
    fuku_operand_scale get_scale() const;
    const fuku_immediate& get_disp() const;
    fuku_operand_size get_size() const;
    fuku_prefix get_segment() const;
};
