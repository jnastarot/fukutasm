#pragma once

class fuku_code_profiler {
    csh cap_handle;
    fuku_assambler_arch arch;

    uint64_t registers_table[X86_REG_ENDING];
    bool dirty_registers_table;

    bool get_instruction_operands_access(cs_insn* instruction, uint8_t& reg_idx, void* _op_access);
    uint64_t profile_graph_registers(fuku_code_holder& code, inststorage::iterator lines_iter);
    uint64_t profile_graph_eflags(fuku_code_holder& code, inststorage::iterator lines_iter);
public:
    fuku_code_profiler(fuku_assambler_arch arch);
    ~fuku_code_profiler();

public:
    
    bool profile_code(fuku_code_holder& code);

public:
    //for debugging
    void print_reg(uint64_t reg);
    void print_full_reg(uint64_t reg);
    void print_code(fuku_code_holder& code);

};