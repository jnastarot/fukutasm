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
    fuku_operand_size size = get_minimal_op_size(dst, src);\
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
            default: { FUKU_DEBUG; break; }\
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
            default: { FUKU_DEBUG; break; }\
            }\
            break;\
        }\
        default: { FUKU_DEBUG; break; }\
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
            default: { FUKU_DEBUG; break; }\
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
            default: { FUKU_DEBUG; break; }\
            }\
            break;\
        }\
        default: { FUKU_DEBUG; break; }\
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
            default: { FUKU_DEBUG; break; }\
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
            default: { FUKU_DEBUG; break; }\
            }\
            break;\
        }\
        default: { FUKU_DEBUG; break; }\
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
            default: { FUKU_DEBUG; break; }\
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
            default: { FUKU_DEBUG; break; }\
            }\
            break;\
        }\
        default: { FUKU_DEBUG; break; }\
        }\
        break;\
    }\
    default: { FUKU_DEBUG; break; }\
    }

fuku_operand_size get_minimal_op_size(const fuku_type& dst, const fuku_type& src) {

    switch (dst.get_type()) {

    case FUKU_T0_REGISTER: {
        return fuku_get_register_size(dst.get_register());
    }
    case FUKU_T0_OPERAND: {
        switch (src.get_type()) {

        case FUKU_T0_REGISTER: {
            return fuku_get_register_size(src.get_register());
        }
        case FUKU_T0_OPERAND: {
            break;
        }
        case FUKU_T0_IMMEDIATE: {
            return src.get_immediate().get_size();
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

fuku_instruction emit_instruction() {
    /*
    FUKU_ASSERT_EQ(is_valid_op_r_i(dst, src), false);

    fuku_operand_size size = get_minimal_op_size(dst, src);

    switch (size) {
    case fuku_operand_size::FUKU_OPERAND_SIZE_8: {
        switch (dst.get_type()) {
        case FUKU_T0_REGISTER: {
       
            break;
        }
        case FUKU_T0_OPERAND: {
         
            break;
        }
        case FUKU_T0_IMMEDIATE: {

            break;
        }
        default: {
            FUKU_DEBUG;
            break;
        }
        }
        break;
    }

    case fuku_operand_size::FUKU_OPERAND_SIZE_16: {
        switch (dst.get_type()) {

        case FUKU_T0_REGISTER: {

            break;
        }
        case FUKU_T0_OPERAND: {
       
            break;
        }
        case FUKU_T0_IMMEDIATE: {

            break;
        }
        default: {
            FUKU_DEBUG;
            break;
        }
        }
        break;
    }

    case fuku_operand_size::FUKU_OPERAND_SIZE_32: {
        switch (dst.get_type()) {
        case FUKU_T0_REGISTER: {

            break;
        }
        case FUKU_T0_OPERAND: {
     
            break;
        }
        case FUKU_T0_IMMEDIATE: {

            break;
        }
        default: {
            FUKU_DEBUG;
            break;
        }
        }
        break;
    }

    case fuku_operand_size::FUKU_OPERAND_SIZE_64: {

        switch (dst.get_type()) {

        case FUKU_T0_REGISTER: {

            break;
        }
        case FUKU_T0_OPERAND: {
       
            break;
        }
        case FUKU_T0_IMMEDIATE: {

            break;
        }
        default: {
            FUKU_DEBUG;
            break;
        }
        }
        break;
    }

    default:
        break;
    }
        */
    return fuku_instruction();
}