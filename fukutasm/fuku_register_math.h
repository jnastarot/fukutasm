#pragma once

#include "fuku_register_math_metadata.h"

/*
    EAX = 0 ECX = 1 r_EDX = 2
    R8 = 0 R9 = 1 R10 = 2
    and them lowest parts etc
*/
fuku_register_index get_index_by_register(fuku_register_enum reg);
fuku_register_enum  get_register_by_index(fuku_register_index idx, bool x64ext, fuku_operand_size size);
fuku_operand_size   get_register_size(fuku_register_enum reg);

//  returns true for RAX - RDI,SPL - DIL, R8 - R15
bool is_x64arch_register(fuku_register_enum reg);
//  returns true for EAX - EDI
bool is_x32arch_register(fuku_register_enum reg);
//  returns true for R8 - R15 , return 0 - 1
uint8_t is_x64arch_ext_register(fuku_register_enum reg);


//uint8_t fuku_to_capstone_reg(const fuku_register& reg);
fuku_register_enum capstone_to_fuku_reg(x86_reg reg);
fuku_operand capstone_to_fuku_op(cs_x86& x86, uint8_t op_idx);

x86_insn fuku_to_capstone_cc(fuku_condition cond, fuku_to_cap_convert_type type);
fuku_condition capstone_to_fuku_cond(x86_insn cond);

uint8_t get_random_bit_by_mask(uint64_t mask, uint8_t min_index, uint8_t max_index);

uint8_t get_flag_register_size(uint64_t reg);
uint8_t get_flag_register_index(uint64_t reg);
uint8_t get_flag_register_ext64(uint64_t reg);

uint64_t fuku_to_flag_register(fuku_register_enum reg);
fuku_register_enum flag_to_fuku_register(uint64_t reg);

fuku_register_enum set_fuku_register_grade(fuku_register_enum reg, uint8_t needed_size);

bool has_flag_free_register(uint64_t regs_flags, uint64_t reg);
bool has_inst_free_register(const fuku_inst& inst, x86_reg reg);
bool has_inst_free_eflags(uint64_t inst_eflags, uint64_t flags); //uint64_t flags used only with MODIFY prefix

uint64_t get_flag_complex_by_fuku_register(const fuku_register& reg, uint8_t size = 0);
uint64_t get_flag_complex_register(uint64_t flag_reg);
uint64_t get_flag_complex_register_by_size(uint64_t flag_reg);

fuku_register_enum get_random_register(uint32_t reg_size, bool x86_only, uint64_t exclude_regs = 0);
fuku_register_enum get_random_free_register(uint64_t reg_flags, uint32_t reg_size, bool x86_only, uint64_t exclude_regs = FUKU_REG_NONE);
fuku_register_enum get_random_free_register(const fuku_inst& inst, uint32_t reg_size, bool x86_only, uint64_t exclude_regs = FUKU_REG_NONE);
fuku_register_enum get_random_free_register_x64(uint64_t reg_flags, uint8_t reg_size, uint64_t exclude_regs = FUKU_REG_NONE);

uint64_t get_operand_mask_register(const fuku_type& op);
uint64_t get_operand_mask_register(const fuku_type& op1, const fuku_type& op2);

fuku_immediate get_immediate_x86(uint8_t size);
bool get_operand_src_x86(fuku_type& op, uint8_t allow_inst, uint8_t size, uint64_t disallow_regs);
bool get_operand_dst_x86(fuku_type& op, uint8_t allow_inst, uint8_t size, uint64_t allow_regs, uint64_t disallow_regs);

fuku_immediate get_immediate_x64(uint8_t size);
bool get_operand_src_x64(fuku_type& op, uint8_t allow_inst, uint8_t size, uint64_t disallow_regs);
bool get_operand_dst_x64(fuku_type& op, uint8_t allow_inst, uint8_t size, uint64_t allow_regs, uint64_t disallow_regs);