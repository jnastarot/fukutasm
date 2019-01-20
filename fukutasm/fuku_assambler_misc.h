#pragma once


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

bool is_valid_op_r_i(const fuku_type& dst,const fuku_type& src) {

    switch (dst.get_type()) {

    case FUKU_T0_REGISTER: {
        switch (src.get_type()) {

        case FUKU_T0_REGISTER: {
            return true;
        }
        case FUKU_T0_OPERAND: {
            return true;
        }
        case FUKU_T0_IMMEDIATE: {
            return true;
        }
        }
        break;
    }
    case FUKU_T0_OPERAND: {
        switch (src.get_type()) {

        case FUKU_T0_REGISTER: {
            return true;
        }
        case FUKU_T0_OPERAND: {
            return false;
        }
        case FUKU_T0_IMMEDIATE: {
            return true;
        }
        }
        break;
    }
    case FUKU_T0_IMMEDIATE: {
        return false;
    }
       
    default:
        return false;
    }


    return false;
}

fuku_instruction emit_instruction() {

    FUKU_ASSERT_EQ(is_valid_op_r_i(dst, src), false);

    fuku_operand_size size = get_minimal_op_size(dst, src);

    switch (size) {

    case fuku_operand_size::FUKU_OPERAND_SIZE_8: {

        switch (dst.get_type()) {

        case FUKU_T0_REGISTER: {

            switch (src.get_type()) {

            case FUKU_T0_REGISTER: {
                return this->lntrnl_asm->_mov_b(dst.get_register(), src.get_register());
            }
            case FUKU_T0_OPERAND: {
                return this->lntrnl_asm->_mov_b(dst.get_register(), src.get_operand());
            }
            case FUKU_T0_IMMEDIATE: {
                return this->lntrnl_asm->_mov_b(dst.get_register(), src.get_immediate());
            }
            default: {
                FUKU_DEBUG;
                break;
            }
            }
            break;
        }
        case FUKU_T0_OPERAND: {
            switch (src.get_type()) {

            case FUKU_T0_REGISTER: {
                return this->lntrnl_asm->_mov_b(dst.get_operand(), src.get_register());
            }
            case FUKU_T0_IMMEDIATE: {
                return this->lntrnl_asm->_mov_b(dst.get_operand(), src.get_immediate());
            }
            default: {
                FUKU_DEBUG;
                break;
            }
            }
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

            switch (src.get_type()) {

            case FUKU_T0_REGISTER: {
                return this->lntrnl_asm->_mov_w(dst.get_register(), src.get_register());
            }
            case FUKU_T0_OPERAND: {
                return this->lntrnl_asm->_mov_w(dst.get_register(), src.get_operand());
            }
            case FUKU_T0_IMMEDIATE: {
                return this->lntrnl_asm->_mov_w(dst.get_register(), src.get_immediate());
            }
            default: {
                FUKU_DEBUG;
                break;
            }
            }
            break;
        }
        case FUKU_T0_OPERAND: {
            switch (src.get_type()) {

            case FUKU_T0_REGISTER: {
                return this->lntrnl_asm->_mov_w(dst.get_operand(), src.get_register());
            }
            case FUKU_T0_IMMEDIATE: {
                return this->lntrnl_asm->_mov_w(dst.get_operand(), src.get_immediate());
            }
            default: {
                FUKU_DEBUG;
                break;
            }
            }
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

            switch (src.get_type()) {

            case FUKU_T0_REGISTER: {
                return this->lntrnl_asm->_mov_dw(dst.get_register(), src.get_register());
            }
            case FUKU_T0_OPERAND: {
                return this->lntrnl_asm->_mov_dw(dst.get_register(), src.get_operand());
            }
            case FUKU_T0_IMMEDIATE: {
                return this->lntrnl_asm->_mov_dw(dst.get_register(), src.get_immediate());
            }
            default: {
                FUKU_DEBUG;
                break;
            }
            }
            break;
        }
        case FUKU_T0_OPERAND: {
            switch (src.get_type()) {

            case FUKU_T0_REGISTER: {
                return this->lntrnl_asm->_mov_dw(dst.get_operand(), src.get_register());
            }
            case FUKU_T0_IMMEDIATE: {
                return this->lntrnl_asm->_mov_dw(dst.get_operand(), src.get_immediate());
            }
            default: {
                FUKU_DEBUG;
                break;
            }
            }
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

            switch (src.get_type()) {

            case FUKU_T0_REGISTER: {
                return this->lntrnl_asm->_mov_qw(dst.get_register(), src.get_register());
            }
            case FUKU_T0_OPERAND: {
                return this->lntrnl_asm->_mov_qw(dst.get_register(), src.get_operand());
            }
            case FUKU_T0_IMMEDIATE: {
                return this->lntrnl_asm->_mov_qw(dst.get_register(), src.get_immediate());
            }
            default: {
                FUKU_DEBUG;
                break;
            }
            }
            break;
        }
        case FUKU_T0_OPERAND: {
            switch (src.get_type()) {

            case FUKU_T0_REGISTER: {
                return this->lntrnl_asm->_mov_qw(dst.get_operand(), src.get_register());
            }
            case FUKU_T0_IMMEDIATE: {
                return this->lntrnl_asm->_mov_qw(dst.get_operand(), src.get_immediate());
            }
            default: {
                FUKU_DEBUG;
                break;
            }
            }
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

    return fuku_instruction();
}