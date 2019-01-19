#pragma once

bool is_valid_op_r_i(fuku_type& dst, fuku_type& src) {

    switch (dst.get_type()) {

    case FUKU_T0_REGISTER: {
        switch (src.get_type()) {

        case FUKU_T0_REGISTER: {
            
        }
        case FUKU_T0_OPERAND: {

            break;
        }
        case FUKU_T0_IMMEDIATE: {

            break;
        }
        break;
    }
    case FUKU_T0_OPERAND: {

        break;
    }
    case FUKU_T0_IMMEDIATE: {

        break;
    }


    default:
        return false;
    }

    if (dst.get_type() ) {

    }

    if (dst.get_type() == src.get_type()) {

    }


}