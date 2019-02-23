#pragma once

#define gencleanerdata \
     clear_space(ctx);

#define gen_func_return(cap_id, cap_eflags)\
     *ctx.inst = fuku_instruction().set_op_code(ctx.bytecode, ctx.length) \
        .set_id(cap_id) \
        .set_eflags(cap_eflags);  \
    return;

//emit 1 byte , emit modrm(regrm, idx)
#define gen_pattern32_1em_rm_idx(byte1, rm_reg, idx)\
            emit_optional_rex_32(ctx, rm_reg);\
            emit_b(ctx, byte1);\
            emit_modrm(ctx, rm_reg, idx);
#define gen_pattern64_1em_rm_idx(byte1, rm_reg, idx)\
            emit_rex_64(ctx, rm_reg);\
            emit_b(ctx, byte1);\
            emit_modrm(ctx, rm_reg, idx);

//emit 2 bytes , emit modrm(regrm, idx)
#define gen_pattern32_2em_rm_idx(byte1 ,byte2, rm_reg, idx)\
            emit_optional_rex_32(ctx, rm_reg);\
            emit_b(ctx, byte1);\
            emit_b(ctx, byte2);\
            emit_modrm(ctx, rm_reg, idx);
#define gen_pattern64_2em_rm_idx(byte1 ,byte2, rm_reg, idx)\
            emit_rex_64(ctx, rm_reg);\
            emit_b(ctx, byte1);\
            emit_b(ctx, byte2);\
            emit_modrm(ctx, rm_reg, idx);

//emit 1 byte , emit modrm(regrm, reg)
#define gen_pattern32_1em_rm_r(byte1, rm_reg, reg)\
            emit_optional_rex_32(ctx, rm_reg, reg);\
            emit_b(ctx, byte1);\
            emit_modrm(ctx, rm_reg, reg);
#define gen_pattern64_1em_rm_r(byte1, rm_reg, reg)\
            emit_rex_64(ctx, rm_reg, reg);\
            emit_b(ctx, byte1);\
            emit_modrm(ctx, rm_reg, reg);

//emit 2 bytes , emit modrm(regrm, reg)
#define gen_pattern32_2em_rm_r(byte1 ,byte2, rm_reg, reg)\
            emit_optional_rex_32(ctx, rm_reg, reg);\
            emit_b(ctx, byte1);\
            emit_b(ctx, byte2);\
            emit_modrm(ctx, rm_reg, reg);
#define gen_pattern64_2em_rm_r(byte1 ,byte2, rm_reg, reg)\
            emit_rex_64(ctx, rm_reg, reg);\
            emit_b(ctx, byte1);\
            emit_b(ctx, byte2);\
            emit_modrm(ctx, rm_reg, reg);

//emit 1 byte , emit imm b
#define gen_pattern32_1em_immb(byte1, rm_reg, imm)\
            emit_optional_rex_32(ctx, rm_reg);\
            emit_b(ctx, byte1);\
            emit_immediate_b(ctx, imm);
#define gen_pattern64_1em_immb(byte1, rm_reg, imm)\
            emit_rex_64(ctx, rm_reg);\
            emit_b(ctx, byte1);\
            emit_immediate_b(ctx, imm);

//emit 1 byte , emit imm w
#define gen_pattern32_1em_immw(byte1, rm_reg, imm)\
            emit_optional_rex_32(ctx, rm_reg);\
            emit_b(ctx, byte1);\
            emit_immediate_w(ctx, imm);
#define gen_pattern64_1em_immw(byte1, rm_reg, imm)\
            emit_rex_64(ctx, rm_reg);\
            emit_b(ctx, byte1);\
            emit_immediate_w(ctx, imm);

//emit 1 byte , emit imm dw
#define gen_pattern32_1em_immdw(byte1, rm_reg, imm)\
            emit_optional_rex_32(ctx, rm_reg);\
            emit_b(ctx, byte1);\
            emit_immediate_dw(ctx, imm);
#define gen_pattern64_1em_immdw(byte1, rm_reg, imm)\
            emit_rex_64(ctx, rm_reg);\
            emit_b(ctx, byte1);\
            emit_immediate_dw(ctx, imm);

//emit 2 byte , emit imm b
#define gen_pattern32_2em_immb(byte1,byte2 , rm_reg, imm)\
            emit_optional_rex_32(ctx, rm_reg);\
            emit_b(ctx, byte1);\
            emit_b(ctx, byte2);\
            emit_immediate_b(ctx, imm);
#define gen_pattern64_2em_immb(byte1,byte2 , rm_reg, imm)\
            emit_rex_64(ctx, rm_reg);\
            emit_b(ctx, byte1);\
            emit_b(ctx, byte2);\
            emit_immediate_b(ctx, imm);

//emit 2 byte , emit imm w
#define gen_pattern32_2em_immw(byte1,byte2,rm_reg, imm)\
            emit_optional_rex_32(ctx, rm_reg);\
            emit_b(ctx, byte1);\
            emit_b(ctx, byte2);\
            emit_immediate_w(ctx, imm);
#define gen_pattern64_2em_immw(byte1,byte2,rm_reg, imm)\
            emit_rex_64(ctx, rm_reg);\
            emit_b(ctx, byte1);\
            emit_b(ctx, byte2);\
            emit_immediate_w(ctx, imm);

//emit 2 byte , emit imm dw
#define gen_pattern32_2em_immdw(byte1,byte2,rm_reg, imm)\
            emit_optional_rex_32(ctx, rm_reg);\
            emit_b(ctx, byte1);\
            emit_b(ctx, byte2);\
            emit_immediate_dw(ctx, imm);
#define gen_pattern64_2em_immdw(byte1,byte2,rm_reg, imm)\
            emit_rex_64(ctx, rm_reg);\
            emit_b(ctx, byte1);\
            emit_b(ctx, byte2);\
            emit_immediate_dw(ctx, imm);

//emit 1 bytes , emit operand(regrm, idx)
#define gen_pattern32_1em_op_idx(byte1, operand, idx)\
            emit_optional_rex_32(ctx, operand);\
            emit_b(ctx, byte1);\
            emit_operand(ctx, operand, idx);
#define gen_pattern64_1em_op_idx(byte1, operand, idx)\
            emit_rex_64(ctx, operand);\
            emit_b(ctx, byte1);\
            emit_operand(ctx, operand, idx);

//emit 2 bytes , emit operand(regrm, idx)
#define gen_pattern32_2em_op_idx(byte1,byte2, operand, idx)\
            emit_optional_rex_32(ctx, operand);\
            emit_b(ctx, byte1);\
            emit_b(ctx, byte2);\
            emit_operand(ctx, operand, idx);
#define gen_pattern64_2em_op_idx(byte1 ,byte2, operand, idx)\
            emit_rex_64(ctx, operand);\
            emit_b(ctx, byte1);\
            emit_b(ctx, byte2);\
            emit_operand(ctx, operand, idx);

//emit 1 bytes , emit operand(regrm, reg)
#define gen_pattern32_1em_op_r(byte1, operand, reg)\
            emit_optional_rex_32(ctx, operand, reg);\
            emit_b(ctx, byte1);\
            emit_operand(ctx, operand, reg);
#define gen_pattern64_1em_op_r(byte1, operand, reg)\
            emit_rex_64(ctx, operand, reg);\
            emit_b(ctx, byte1);\
            emit_operand(ctx, operand, reg);

//emit 2 bytes , emit operand(regrm, reg)
#define gen_pattern32_2em_op_r(byte1,byte2, operand, reg)\
            emit_optional_rex_32(ctx, operand, reg);\
            emit_b(ctx, byte1);\
            emit_b(ctx, byte2);\
            emit_operand(ctx, operand, reg);
#define gen_pattern64_2em_op_r(byte1 ,byte2, operand, reg)\
            emit_rex_64(ctx, operand, reg);\
            emit_b(ctx, byte1);\
            emit_b(ctx, byte2);\
            emit_operand(ctx, operand, reg);


//emit 1 bytes , emit operand(regrm, idx) imm b
#define gen_pattern32_1em_op_idx_immb(byte1, operand, idx, imm)\
            gen_pattern32_1em_op_idx(byte1, operand, idx)\
            emit_immediate_b(ctx, imm);
#define gen_pattern64_1em_op_idx_immb(byte1, operand, idx, imm)\
            gen_pattern64_1em_op_idx(byte1, operand, idx)\
            emit_immediate_b(ctx, imm);

//emit 1 bytes , emit operand(regrm, idx) imm w
#define gen_pattern32_1em_op_idx_immw(byte1, operand, idx, imm)\
            gen_pattern32_1em_op_idx(byte1, operand, idx)\
            emit_immediate_w(ctx, imm);
#define gen_pattern64_1em_op_idx_immw(byte1, operand, idx, imm)\
            gen_pattern64_1em_op_idx(byte1, operand, idx)\
            emit_immediate_w(ctx, imm);

//emit 1 bytes , emit operand(regrm, idx) imm dw
#define gen_pattern32_1em_op_idx_immdw(byte1, operand, idx, imm)\
            gen_pattern32_1em_op_idx(byte1, operand, idx)\
            emit_immediate_dw(ctx, imm);
#define gen_pattern64_1em_op_idx_immdw(byte1, operand, idx, imm)\
            gen_pattern64_1em_op_idx(byte1, operand, idx)\
            emit_immediate_dw(ctx, imm);

//emit 2 bytes , emit operand(regrm, idx) imm b
#define gen_pattern32_2em_op_idx_immb(byte1,byte2, operand, idx, imm)\
            gen_pattern32_2em_op_idx(byte1,byte2, operand, idx)\
            emit_immediate_b(ctx, imm);
#define gen_pattern64_2em_op_idx_immb(byte1,byte2, operand, idx, imm)\
            gen_pattern64_2em_op_idx(byte1,byte2, operand, idx)\
            emit_immediate_b(ctx, imm);

//emit 2 bytes , emit operand(regrm, idx) imm w
#define gen_pattern32_2em_op_idx_immw(byte1,byte2, operand, idx, imm)\
            gen_pattern32_2em_op_idx(byte1,byte2, operand, idx)\
            emit_immediate_w(ctx, imm);
#define gen_pattern64_2em_op_idx_immw(byte1,byte2, operand, idx, imm)\
            gen_pattern64_2em_op_idx(byte1,byte2, operand, idx)\
            emit_immediate_w(ctx, imm);

//emit 2 bytes , emit operand(regrm, idx) imm dw
#define gen_pattern32_2em_op_idx_immdw(byte1,byte2, operand, idx, imm)\
            gen_pattern32_2em_op_idx(byte1,byte2, operand, idx)\
            emit_immediate_dw(ctx, imm);
#define gen_pattern64_2em_op_idx_immdw(byte1, operand, idx, imm)\
            gen_pattern64_2em_op_idx(byte1, operand, idx)\
            emit_immediate_dw(ctx, imm);


//emit 1 bytes , emit rm(regrm, idx) imm b
#define gen_pattern32_1em_rm_idx_immb(byte1, rm_reg, idx, imm)\
            gen_pattern32_1em_rm_idx(byte1, rm_reg, idx)\
            emit_immediate_b(ctx, imm);
#define gen_pattern64_1em_rm_idx_immb(byte1, rm_reg, idx, imm)\
            gen_pattern64_1em_rm_idx(byte1, rm_reg, idx)\
            emit_immediate_b(ctx, imm);

//emit 1 bytes , emit rm(regrm, idx) imm w
#define gen_pattern32_1em_rm_idx_immw(byte1, rm_reg, idx, imm)\
            gen_pattern32_1em_rm_idx(byte1, rm_reg, idx)\
            emit_immediate_w(ctx, imm);
#define gen_pattern64_1em_rm_idx_immw(byte1, rm_reg, idx, imm)\
            gen_pattern64_1em_rm_idx(byte1, rm_reg, idx)\
            emit_immediate_w(ctx, imm);

//emit 1 bytes , emit rm(regrm, idx) imm dw
#define gen_pattern32_1em_rm_idx_immdw(byte1, rm_reg, idx, imm)\
            gen_pattern32_1em_rm_idx(byte1, rm_reg, idx)\
            emit_immediate_dw(ctx, imm);
#define gen_pattern64_1em_rm_idx_immdw(byte1, rm_reg, idx, imm)\
            gen_pattern64_1em_rm_idx(byte1, rm_reg, idx)\
            emit_immediate_dw(ctx, imm);

//emit 2 bytes , emit rm(regrm, idx) imm b
#define gen_pattern32_2em_rm_idx_immb(byte1,byte2, rm_reg, idx, imm)\
            gen_pattern32_2em_rm_idx(byte1,byte2, rm_reg, idx)\
            emit_immediate_b(ctx, imm);
#define gen_pattern64_2em_rm_idx_immb(byte1,byte2, rm_reg, idx, imm)\
            gen_pattern64_2em_rm_idx(byte1,byte2, rm_reg, idx)\
            emit_immediate_b(ctx, imm);

//emit 2 bytes , emit rm(regrm, idx) imm w
#define gen_pattern32_2em_rm_idx_immw(byte1,byte2, rm_reg, idx, imm)\
            gen_pattern32_2em_rm_idx(byte1,byte2, rm_reg, idx)\
            emit_immediate_w(ctx, imm);
#define gen_pattern64_2em_rm_idx_immw(byte1,byte2, rm_reg, idx, imm)\
            gen_pattern64_2em_rm_idx(byte1,byte2, rm_reg, idx)\
            emit_immediate_w(ctx, imm);

//emit 2 bytes , emit rm(regrm, idx) imm dw
#define gen_pattern32_2em_rm_idx_immdw(byte1,byte2, rm_reg, idx, imm)\
            gen_pattern32_2em_rm_idx(byte1,byte2, rm_reg, idx)\
            emit_immediate_dw(ctx, imm);
#define gen_pattern64_2em_rm_idx_immdw(byte1, rm_reg, idx, imm)\
            gen_pattern64_2em_rm_idx(byte1, rm_reg, idx)\
            emit_immediate_dw(ctx, imm);



#define gen_pattern32_1em_rm_r_word(byte1, rm_reg, reg)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_1em_rm_r(byte1, rm_reg, reg)
#define gen_pattern64_1em_rm_r_word(byte1, rm_reg, reg)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_1em_rm_r(byte1, rm_reg, reg)
#define gen_pattern32_2em_rm_r_word(byte1 ,byte2, rm_reg, reg)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_2em_rm_r(byte1, byte2, rm_reg, reg)
#define gen_pattern64_2em_rm_r_word(byte1 ,byte2, rm_reg, reg)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_2em_rm_r(byte1, byte2, rm_reg, reg)

#define gen_pattern32_1em_rm_idx_word(byte1, rm_reg, idx)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_1em_rm_idx(byte1, rm_reg, idx)
#define gen_pattern64_1em_rm_idx_word(byte1, rm_reg, idx)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_1em_rm_idx(byte1, rm_reg, idx)
#define gen_pattern32_2em_rm_idx_word(byte1 ,byte2, rm_reg_idx, reg)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_2em_rm_idx(byte1, byte2, rm_reg_idx, reg)
#define gen_pattern64_2em_rm_idx_word(byte1 ,byte2, rm_reg_idx, reg)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_2em_rm_idx(byte1, byte2, rm_reg_idx, reg)


#define gen_pattern32_1em_immb_word(byte1, rm_reg, imm)\
            emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
            gen_pattern32_1em_immb(byte1, rm_reg, imm)
#define gen_pattern64_1em_immb_word(byte1, rm_reg, imm)\
            emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
            gen_pattern64_1em_immb(byte1, rm_reg, imm)
#define gen_pattern32_1em_immw_word(byte1, rm_reg, imm)\
            emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
            gen_pattern32_1em_immw(byte1, rm_reg, imm)
#define gen_pattern64_1em_immw_word(byte1,rm_reg,  imm)\
            gen_pattern64_1em_immw(byte1,rm_reg,  imm)
#define gen_pattern32_1em_immdw_word(byte1,rm_reg, imm)\
            emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
            gen_pattern32_1em_immdw(byte1, rm_reg, imm)
#define gen_pattern64_1em_immdw_word(byte1, rm_reg, imm)\
            emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
            gen_pattern64_1em_immdw(byte1, rm_reg, imm)

#define gen_pattern32_2em_immb_word(byte1,byte2, imm)\
            emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
            gen_pattern32_2em_immb(byte1,byte2, imm)
#define gen_pattern64_2em_immb_word(byte1,byte2, imm)\
            emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
            gen_pattern64_2em_immb(byte1,byte2, imm)
#define gen_pattern32_2em_immw_word(byte1,byte2, imm)\
            emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
            gen_pattern32_2em_immw(byte1,byte2, imm)
#define gen_pattern64_2em_immw_word(byte1,byte2, imm)\
            emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
            gen_pattern64_2em_immw(byte1,byte2, imm)
#define gen_pattern32_2em_immdw_word(byte1,byte2, imm)\
            emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
            gen_pattern32_2em_immdw(byte1,byte2, imm)
#define gen_pattern64_2em_immdw_word(byte1,byte2, imm)\
            emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
            gen_pattern64_2em_immdw(byte1,byte2, imm)


#define gen_pattern32_1em_op_r_word(byte1, rm_reg, reg)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_1em_op_r(byte1, rm_reg, reg)
#define gen_pattern64_1em_op_r_word(byte1, rm_reg, reg)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_1em_op_r(byte1, rm_reg, reg)
#define gen_pattern32_2em_op_r_word(byte1 ,byte2, rm_reg_idx, reg)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_2em_op_r(byte1, byte2, rm_reg_idx, reg)
#define gen_pattern64_2em_op_r_word(byte1 ,byte2, rm_reg_idx, reg)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_2em_op_r(byte1, byte2, rm_reg_idx, reg)

#define gen_pattern32_1em_op_idx_word(byte1, rm_reg, idx)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_1em_op_idx(byte1, rm_reg, idx)
#define gen_pattern64_1em_op_idx_word(byte1, rm_reg, idx)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_1em_op_idx(byte1, rm_reg, idx)
#define gen_pattern32_2em_op_idx_word(byte1 ,byte2, rm_reg_idx, reg)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_2em_op_idx(byte1, byte2, rm_reg_idx, reg)
#define gen_pattern64_2em_op_idx_word(byte1 ,byte2, rm_reg_idx, reg)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_2em_op_idx(byte1, byte2, rm_reg_idx, reg)

#define gen_pattern32_1em_op_idx_immb_word(byte1, operand, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_1em_op_idx_immb(byte1, operand, idx, imm)
#define gen_pattern64_1em_op_idx_immb_word(byte1, operand, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_1em_op_idx_immb(byte1, operand, idx, imm)
#define gen_pattern32_2em_op_idx_immb_word(byte1, byte2, operand, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_2em_op_idx_immb(byte1, byte2, operand, idx, imm)
#define gen_pattern64_2em_op_idx_immb_word(byte1, byte2, operand, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_2em_op_idx_immb(byte1, byte2, operand, idx, imm)

#define gen_pattern32_1em_op_idx_immw_word(byte1, operand, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_1em_op_idx_immw(byte1, operand, idx, imm)
#define gen_pattern64_1em_op_idx_immw_word(byte1, operand, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_1em_op_idx_immw(byte1, operand, idx, imm)
#define gen_pattern32_2em_op_idx_immw_word(byte1, byte2, operand, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_2em_op_idx_immw(byte1, byte2, operand, idx, imm)
#define gen_pattern64_2em_op_idx_immw_word(byte1, byte2, operand, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_2em_op_idx_immw(byte1, byte2, operand, idx, imm)

#define gen_pattern32_1em_op_idx_immdw_word(byte1, operand, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_1em_op_idx_immdw(byte1, operand, idx, imm)
#define gen_pattern64_1em_op_idx_immdw_word(byte1, operand, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_1em_op_idx_immdw(byte1, operand, idx, imm)
#define gen_pattern32_2em_op_idx_immdw_word(byte1, byte2, operand, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_2em_op_idx_immdw(byte1, byte2, operand, idx, imm)
#define gen_pattern64_2em_op_idx_immdw_word(byte1, byte2, operand, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_2em_op_idx_immdw(byte1, byte2, operand, idx, imm)

#define gen_pattern32_1em_rm_idx_immb_word(byte1, rm_reg, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_1em_rm_idx_immb(byte1, rm_reg, idx, imm)
#define gen_pattern64_1em_rm_idx_immb_word(byte1, rm_reg, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_1em_rm_idx_immb(byte1, rm_reg, idx, imm)
#define gen_pattern32_2em_rm_idx_immb_word(byte1, byte2, rm_reg, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_2em_rm_idx_immb(byte1, byte2, rm_reg, idx, imm)
#define gen_pattern64_2em_rm_idx_immb_word(byte1, byte2, rm_reg, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_2em_rm_idx_immb(byte1, byte2, rm_reg, idx, imm)

#define gen_pattern32_1em_rm_idx_immw_word(byte1, rm_reg, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_1em_rm_idx_immw(byte1, rm_reg, idx, imm)
#define gen_pattern64_1em_rm_idx_immw_word(byte1, rm_reg, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_1em_rm_idx_immw(byte1, rm_reg, idx, imm)
#define gen_pattern32_2em_rm_idx_immw_word(byte1, byte2, rm_reg, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_2em_rm_idx_immw(byte1, byte2, rm_reg, idx, imm)
#define gen_pattern64_2em_rm_idx_immw_word(byte1, byte2, rm_reg, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_2em_rm_idx_immw(byte1, byte2, rm_reg, idx, imm)

#define gen_pattern32_1em_rm_idx_immdw_word(byte1, rm_reg, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_1em_rm_idx_immdw(byte1, rm_reg, idx, imm)
#define gen_pattern64_1em_rm_idx_immdw_word(byte1, rm_reg, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_1em_rm_idx_immdw(byte1, rm_reg, idx, imm)
#define gen_pattern32_2em_rm_idx_immdw_word(byte1, byte2, rm_reg, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern32_2em_rm_idx_immdw(byte1, byte2, rm_reg, idx, imm)
#define gen_pattern64_2em_rm_idx_immdw_word(byte1, byte2, rm_reg, idx, imm)\
        emit_b(ctx, FUKU_PREFIX_OVERRIDE_DATA);\
        gen_pattern64_2em_rm_idx_immdw(byte1, byte2, rm_reg, idx, imm)