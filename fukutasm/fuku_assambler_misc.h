#pragma once

#define fuku_assambler_command_2op_graph( \
    _op_b_r_r, \
    _op_b_r_op, \
    _op_b_r_imm, \
    _op_b_op_r, \
    _op_b_op_imm, \
\
    _op_w_r_r, \
    _op_w_r_op, \
    _op_w_r_imm, \
    _op_w_op_r, \
    _op_w_op_imm, \
\
    _op_dw_r_r, \
    _op_dw_r_op, \
    _op_dw_r_imm, \
    _op_dw_op_r, \
    _op_dw_op_imm, \
\
    _op_qw_r_r, \
    _op_qw_r_op, \
    _op_qw_r_imm, \
    _op_qw_op_r, \
    _op_qw_op_imm \
) \
    on_emit(dst, src);\
    fuku_operand_size size = get_minimal_op_size(context, dst, src);\
    switch (size) {\
    case fuku_operand_size::FUKU_OPERAND_SIZE_8: {\
        switch (dst.get_type()) {\
        case FUKU_T0_REGISTER: {\
            switch (src.get_type()) {\
            case FUKU_T0_REGISTER: {\
                _op_b_r_r break;\
            }\
            case FUKU_T0_OPERAND: {\
                _op_b_r_op break;\
            }\
            case FUKU_T0_IMMEDIATE: {\
                _op_b_r_imm break;\
            }\
            default: { UNUSUAL_DATASET; break; }\
            }\
            break;\
        }\
        case FUKU_T0_OPERAND: {\
            switch (src.get_type()) {\
            case FUKU_T0_REGISTER: {\
                _op_b_op_r break;\
            }\
            case FUKU_T0_IMMEDIATE: {\
                _op_b_op_imm break;\
            }\
            default: { UNUSUAL_DATASET; break; }\
            }\
            break;\
        }\
        default: { UNUSUAL_DATASET; break; }\
        }\
        break;\
    }\
    case fuku_operand_size::FUKU_OPERAND_SIZE_16: {\
        switch (dst.get_type()) {\
        case FUKU_T0_REGISTER: {\
            switch (src.get_type()) {\
            case FUKU_T0_REGISTER: {\
                _op_w_r_r break;\
            }\
            case FUKU_T0_OPERAND: {\
                _op_w_r_op break;\
            }\
            case FUKU_T0_IMMEDIATE: {\
                _op_w_r_imm break;\
            }\
            default: { UNUSUAL_DATASET; break; }\
            }\
            break;\
        }\
        case FUKU_T0_OPERAND: {\
            switch (src.get_type()) {\
            case FUKU_T0_REGISTER: {\
                _op_w_op_r break;\
            }\
            case FUKU_T0_IMMEDIATE: {\
                _op_w_op_imm break;\
            }\
            default: { UNUSUAL_DATASET; break; }\
            }\
            break;\
        }\
        default: { UNUSUAL_DATASET; break; }\
        }\
        break;\
    }\
    case fuku_operand_size::FUKU_OPERAND_SIZE_32: {\
        switch (dst.get_type()) {\
        case FUKU_T0_REGISTER: {\
            switch (src.get_type()) {\
            case FUKU_T0_REGISTER: {\
                _op_dw_r_r break;\
            }\
            case FUKU_T0_OPERAND: {\
                _op_dw_r_op break;\
            }\
            case FUKU_T0_IMMEDIATE: {\
                _op_dw_r_imm break;\
            }\
            default: { UNUSUAL_DATASET; break; }\
            }\
            break;\
        }\
        case FUKU_T0_OPERAND: {\
            switch (src.get_type()) {\
            case FUKU_T0_REGISTER: {\
                _op_dw_op_r break;\
            }\
            case FUKU_T0_IMMEDIATE: {\
                _op_dw_op_imm break;\
            }\
            default: { UNUSUAL_DATASET; break; }\
            }\
            break;\
        }\
        default: { UNUSUAL_DATASET; break; }\
        }\
        break;\
    }\
    case fuku_operand_size::FUKU_OPERAND_SIZE_64: {\
        switch (dst.get_type()) {\
        case FUKU_T0_REGISTER: {\
            switch (src.get_type()) {\
            case FUKU_T0_REGISTER: {\
                _op_qw_r_r break;\
            }\
            case FUKU_T0_OPERAND: {\
                _op_qw_r_op break;\
            }\
            case FUKU_T0_IMMEDIATE: {\
                _op_qw_r_imm break;\
            }\
            default: { UNUSUAL_DATASET; break; }\
            }\
            break;\
        }\
        case FUKU_T0_OPERAND: {\
            switch (src.get_type()) {\
            case FUKU_T0_REGISTER: {\
                _op_qw_op_r break;\
            }\
            case FUKU_T0_IMMEDIATE: {\
                _op_qw_op_imm break;\
            }\
            default: { UNUSUAL_DATASET; break; }\
            }\
            break;\
        }\
        default: { UNUSUAL_DATASET; break; }\
        }\
        break;\
    }\
    default: { UNUSUAL_DATASET; break; }\
    }


#define fuku_assambler_command_1op_graph(opname,\
    _op_b_r, \
    _op_b_op, \
    _op_b_imm, \
\
    _op_w_r, \
    _op_w_op, \
    _op_w_imm, \
\
    _op_dw_r, \
    _op_dw_op, \
    _op_dw_imm, \
\
    _op_qw_r, \
    _op_qw_op, \
    _op_qw_imm \
)\
    on_emit(opname);\
    fuku_operand_size size = get_minimal_op_size(context, opname);\
    switch (size) {\
    case fuku_operand_size::FUKU_OPERAND_SIZE_8: {\
        switch (opname.get_type()) {\
        case FUKU_T0_REGISTER: {\
         _op_b_r break;\
        }\
        case FUKU_T0_OPERAND: {\
         _op_b_op break;\
        }\
        case FUKU_T0_IMMEDIATE: {\
        _op_b_imm break;\
        }\
        default: { UNUSUAL_DATASET; break; }\
        }\
        break;\
    }\
    case fuku_operand_size::FUKU_OPERAND_SIZE_16: {\
        switch (opname.get_type()) {\
        case FUKU_T0_REGISTER: {\
        _op_w_r break;\
        }\
        case FUKU_T0_OPERAND: {\
        _op_w_op break;\
        }\
        case FUKU_T0_IMMEDIATE: {\
        _op_w_imm break;\
        }\
        default: { UNUSUAL_DATASET; break; }\
        }\
        break;\
    }\
    case fuku_operand_size::FUKU_OPERAND_SIZE_32: {\
        switch (opname.get_type()) {\
        case FUKU_T0_REGISTER: {\
         _op_dw_r break;\
        }\
        case FUKU_T0_OPERAND: {\
         _op_dw_op break;\
        }\
        case FUKU_T0_IMMEDIATE: {\
         _op_dw_imm break;\
        }\
        default: { UNUSUAL_DATASET; break; }\
        }\
        break;\
    }\
    case fuku_operand_size::FUKU_OPERAND_SIZE_64: {\
        switch (opname.get_type()) {\
        case FUKU_T0_REGISTER: {\
         _op_qw_r break;\
        }\
        case FUKU_T0_OPERAND: {\
         _op_qw_op  break;\
        }\
        case FUKU_T0_IMMEDIATE: {\
         _op_qw_imm  break;\
        }\
        default: { UNUSUAL_DATASET; break; }\
        }\
        break;\
    }\
    default: { UNUSUAL_DATASET; break; }\
    }\


fuku_operand_size get_minimal_op_size(fuku_assambler_ctx& ctx, const fuku_type& dst, const fuku_type& src) {

    switch (dst.get_type()) {
    case FUKU_T0_REGISTER: {
        switch (src.get_type()) {
        case FUKU_T0_REGISTER: {
            return dst.get_size();
        }
        case FUKU_T0_OPERAND: {
            return src.get_size();
        }
        case FUKU_T0_IMMEDIATE: {
            return dst.get_size();
        }
        }
        break;
    }
    case FUKU_T0_OPERAND: {
        switch (src.get_type()) {

        case FUKU_T0_REGISTER: {
            return dst.get_size();
        }
        case FUKU_T0_OPERAND: {
            break;
        }
        case FUKU_T0_IMMEDIATE: {
            return dst.get_size();
        }
        }
        break;
    }
    case FUKU_T0_IMMEDIATE: {
        break;
    }

    default: {
        break;
    }
    }

    return fuku_operand_size::FUKU_OPERAND_SIZE_0;
}

fuku_operand_size get_minimal_op_size(fuku_assambler_ctx& ctx, const fuku_type& dst) {

    switch (dst.get_type()) {
    case FUKU_T0_REGISTER: {
        return dst.get_size();
    }
    case FUKU_T0_OPERAND: {
        return dst.get_size();
    }
    case FUKU_T0_IMMEDIATE: {
        return ctx.arch == FUKU_ASSAMBLER_ARCH_X86 ? FUKU_OPERAND_SIZE_32 : FUKU_OPERAND_SIZE_64;
    }

    default: {
        break;
    }
    }

    return fuku_operand_size::FUKU_OPERAND_SIZE_0;
}
