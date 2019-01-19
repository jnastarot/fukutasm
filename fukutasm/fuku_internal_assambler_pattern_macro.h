#pragma once

#define gencleanerdata \
     clear_space();

#define gen_func_return(cap_id, cap_eflags)\
     return fuku_instruction().set_op_code(bytecode, length) \
        .set_id(cap_id) \
        .set_eflags(cap_eflags);  \

//emit 1 byte , emit modrm(regrm, idx)
#define gen_pattern32_1em_rm_idx(byte1, rm_reg, idx)\
            emit_optional_rex_32(rm_reg);\
            emit_b(byte1);\
            emit_modrm(rm_reg, idx);
#define gen_pattern64_1em_rm_idx(byte1, rm_reg, idx)\
            emit_rex_64(rm_reg);\
            emit_b(byte1);\
            emit_modrm(rm_reg, idx);

//emit 2 bytes , emit modrm(regrm, idx)
#define gen_pattern32_2em_rm_idx(byte1 ,byte2, rm_reg, idx)\
            emit_optional_rex_32(rm_reg);\
            emit_b(byte1);\
            emit_b(byte2);\
            emit_modrm(rm_reg, idx);
#define gen_pattern64_2em_rm_idx(byte1 ,byte2, rm_reg, idx)\
            emit_rex_64(rm_reg);\
            emit_b(byte1);\
            emit_b(byte2);\
            emit_modrm(rm_reg, idx);

//emit 1 byte , emit modrm(regrm, reg)
#define gen_pattern32_1em_rm_r(byte1, rm_reg, reg)\
            emit_optional_rex_32(rm_reg, reg);\
            emit_b(byte1);\
            emit_modrm(rm_reg, reg);
#define gen_pattern64_1em_rm_r(byte1, rm_reg, reg)\
            emit_rex_64(rm_reg, reg);\
            emit_b(byte1);\
            emit_modrm(rm_reg, reg);

//emit 2 bytes , emit modrm(regrm, reg)
#define gen_pattern32_2em_rm_r(byte1 ,byte2, rm_reg, reg)\
            emit_optional_rex_32(rm_reg, reg);\
            emit_b(byte1);\
            emit_b(byte2);\
            emit_modrm(rm_reg, reg);
#define gen_pattern64_2em_rm_r(byte1 ,byte2, rm_reg, reg)\
            emit_rex_64(rm_reg, reg);\
            emit_b(byte1);\
            emit_b(byte2);\
            emit_modrm(rm_reg, reg);

//emit 1 bytes , emit operand(regrm, idx)
#define gen_pattern32_1em_op_idx(byte1, operand, idx)\
            emit_optional_rex_32(operand);\
            emit_b(byte1);\
            emit_operand(operand, idx);
#define gen_pattern64_1em_op_idx(byte1, operand, idx)\
            emit_rex_64(operand);\
            emit_b(byte1);\
            emit_operand(operand, idx);

//emit 2 bytes , emit operand(regrm, idx)
#define gen_pattern32_2em_op_idx(byte1,byte2, operand, idx)\
            emit_optional_rex_32(operand);\
            emit_b(byte1);\
            emit_b(byte2);\
            emit_operand(operand, idx);
#define gen_pattern64_2em_op_idx(byte1 ,byte2, operand, idx)\
            emit_rex_64(operand);\
            emit_b(byte1);\
            emit_b(byte2);\
            emit_operand(operand, idx);

//emit 1 bytes , emit operand(regrm, reg)
#define gen_pattern32_1em_op_r(byte1, operand, reg)\
            emit_optional_rex_32(operand, reg);\
            emit_b(byte1);\
            emit_operand(operand, reg);
#define gen_pattern64_1em_op_r(byte1, operand, reg)\
            emit_rex_64(operand, reg);\
            emit_b(byte1);\
            emit_operand(operand, reg);

//emit 2 bytes , emit operand(regrm, reg)
#define gen_pattern32_2em_op_r(byte1,byte2, operand, reg)\
            emit_optional_rex_32(operand, reg);\
            emit_b(byte1);\
            emit_b(byte2);\
            emit_operand(operand, reg);
#define gen_pattern64_2em_op_r(byte1 ,byte2, operand, reg)\
            emit_rex_64(operand, reg);\
            emit_b(byte1);\
            emit_b(byte2);\
            emit_operand(operand, reg);



#define gen_pattern32_1em_rm_r_word(byte1, rm_reg, reg)\
        emit_b(FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_1em_rm_r(byte1, rm_reg, reg)
#define gen_pattern64_1em_rm_r_word(byte1, rm_reg, reg)\
        emit_b(FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_1em_rm_r(byte1, rm_reg, reg)
#define gen_pattern32_2em_rm_r_word(byte1 ,byte2, rm_reg, reg)\
        emit_b(FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_2em_rm_r(byte1, byte2, rm_reg, reg)
#define gen_pattern64_2em_rm_r_word(byte1 ,byte2, rm_reg, reg)\
        emit_b(FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_2em_rm_r(byte1, byte2, rm_reg, reg)

#define gen_pattern32_1em_rm_idx_word(byte1, rm_reg, idx)\
        emit_b(FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_1em_rm_idx(byte1, rm_reg, idx)
#define gen_pattern64_1em_rm_idx_word(byte1, rm_reg, idx)\
        emit_b(FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_1em_rm_idx(byte1, rm_reg, idx)
#define gen_pattern32_2em_rm_idx_word(byte1 ,byte2, rm_reg_idx, reg)\
        emit_b(FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_2em_rm_idx(byte1, byte2, rm_reg_idx, reg)
#define gen_pattern64_2em_rm_idx_word(byte1 ,byte2, rm_reg_idx, reg)\
        emit_b(FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_2em_rm_idx(byte1, byte2, rm_reg_idx, reg)


#define gen_pattern32_1em_op_r_word(byte1, rm_reg, reg)\
        emit_b(FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_1em_op_r(byte1, rm_reg, reg)
#define gen_pattern64_1em_op_r_word(byte1, rm_reg, reg)\
        emit_b(FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_1em_op_r(byte1, rm_reg, reg)
#define gen_pattern32_2em_op_r_word(byte1 ,byte2, rm_reg_idx, reg)\
        emit_b(FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_2em_op_r(byte1, byte2, rm_reg_idx, reg)
#define gen_pattern64_2em_op_r_word(byte1 ,byte2, rm_reg_idx, reg)\
        emit_b(FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_2em_op_r(byte1, byte2, rm_reg_idx, reg)

#define gen_pattern32_1em_op_idx_word(byte1, rm_reg, idx)\
        emit_b(FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_1em_op_idx(byte1, rm_reg, idx)
#define gen_pattern64_1em_op_idx_word(byte1, rm_reg, idx)\
        emit_b(FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_1em_op_idx(byte1, rm_reg, idx)
#define gen_pattern32_2em_op_idx_word(byte1 ,byte2, rm_reg_idx, reg)\
        emit_b(FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_2em_op_idx(byte1, byte2, rm_reg_idx, reg)
#define gen_pattern64_2em_op_idx_word(byte1 ,byte2, rm_reg_idx, reg)\
        emit_b(FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_2em_op_idx(byte1, byte2, rm_reg_idx, reg)