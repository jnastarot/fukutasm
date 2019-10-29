#include "stdafx.h"
#include "fuku_assambler.h"

#define UNUSUAL_DATASET FUKU_DEBUG

using namespace fukutasm;

#include "fuku_assambler_misc.h"

fuku_assambler::fuku_assambler() 
    :code_holder(0), first_emit(true), hold_type(ASSAMBLER_HOLD_TYPE_NOOVERWRITE), has_label_to_set(false), label(0) {
    
    memset(&context, 0, sizeof(context));
    context.arch = FUKU_ASSAMBLER_ARCH_X86;
    context.short_cfg = 0xFF;
    context.inst = &inst;
}

fuku_assambler::fuku_assambler(fuku_assambler_arch arch)
    :code_holder(0), first_emit(true), hold_type(ASSAMBLER_HOLD_TYPE_NOOVERWRITE), has_label_to_set(false), label(0) {
    
    memset(&context, 0, sizeof(context));
    context.arch = arch;
    context.short_cfg = 0xFF;
    context.inst = &inst;
}


fuku_assambler::~fuku_assambler() {

}

fuku_assambler_ctx& fuku_assambler::get_context() {
    return this->context;
}

fuku_assambler& fuku_assambler::set_holder(fuku_code_holder * code_holder, fuku_assambler_hold_type hold_type) {
    this->code_holder = code_holder;
    this->hold_type = hold_type;
    this->position = this->code_holder->get_insts().begin();

    return *this;
}

fuku_assambler& fuku_assambler::set_position(inststorage::iterator& position) {
    this->position = position;

    return *this;
}

fuku_assambler& fuku_assambler::set_first_emit(bool first_emit) {
    this->first_emit = first_emit;

    return *this;
}

fuku_assambler& fuku_assambler::add_pref(fuku_prefix prefix) {
    prefixes.push_back(prefix);

    return *this;
}

fuku_assambler& fuku_assambler::clear_prefixes() {
    prefixes.clear();

    return *this;
}

void fuku_assambler::set_label(fuku_code_label* label) {
    this->has_label_to_set = true;
    this->label = label;
}

void fuku_assambler::unset_label() {
    this->has_label_to_set = false;
    this->label = 0;
}

void fuku_assambler::on_emit() {
    if (code_holder) {
        
        switch (hold_type) {
        case ASSAMBLER_HOLD_TYPE_FIRST_OVERWRITE: {
            if (first_emit) {
                if (position == code_holder->get_insts().end()) {
                    auto _position = code_holder->get_insts().end();
                    code_holder->get_insts().insert(_position, fuku_inst());
                    this->context.inst = &(*(--_position));
                }
                else {
                    this->context.inst = &(*position);
                    ++position;
                }

                first_emit = false;
                break;
            }
        }
        case ASSAMBLER_HOLD_TYPE_NOOVERWRITE: {
            auto _position = position;
            code_holder->get_insts().insert(_position, fuku_inst());
            this->context.inst = &(*(--_position));
            break;
        }
        case ASSAMBLER_HOLD_TYPE_FULL_OVERWRITE: {
            if (position == code_holder->get_insts().end()) {
                auto _position = code_holder->get_insts().end();
                code_holder->get_insts().insert(_position, fuku_inst());
                this->context.inst = &(*(--_position));
            }
            else {
                this->context.inst = &(*position);
                ++position;
            }
            break;
        }
        }
    }
}


void fuku_assambler::on_emit(const fuku_type& dst, const fuku_type& src) {

    if (dst.get_type() == FUKU_T0_OPERAND) {
        if (dst.get_segment() != FUKU_PREFIX_NONE) {
            add_pref(dst.get_segment());
        }
    }
    else if (src.get_type() == FUKU_T0_OPERAND) {
        if (src.get_segment() != FUKU_PREFIX_NONE) {
            add_pref(src.get_segment());
        }
    }

    on_emit();
}

void fuku_assambler::on_emit(const fuku_type& src) {

    if (src.get_type() == FUKU_T0_OPERAND) {
        if (src.get_segment() != FUKU_PREFIX_NONE) {
            add_pref(src.get_segment());
        }
    }

    on_emit();
}

fuku_assambler_ctx& fuku_assambler::on_new_chain_item() {
    
    if (prefixes.size()) {

        uint8_t inst_[16];

        memcpy(&inst_[prefixes.size()], context.inst->get_opcode(), context.inst->get_oplength());
        memcpy(inst_, prefixes.data(), prefixes.size());

        if (context.displacment_offset) {
            context.displacment_offset += uint32_t(prefixes.size());
        }

        if (context.immediate_offset) {
            context.immediate_offset += uint32_t(prefixes.size());
        }

        context.inst->set_opcode(inst_, uint8_t(prefixes.size() + context.inst->get_oplength()) );

        prefixes.clear();
    }

    if (has_label_to_set) {
        has_label_to_set = false;
        context.inst->set_label(label);
        if (code_holder) {

            label->has_linked_instruction = true;
            label->inst = context.inst;
        }
        label = 0;
    }

    return context;
}

fuku_assambler_ctx& fuku_assambler::mov(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        _mov_b(context, dst.get_register(), src.get_register());,
        _mov_b(context, dst.get_register(), src.get_operand());,
        _mov_b(context, dst.get_register(), src.get_immediate());,
        _mov_b(context, dst.get_operand(), src.get_register());,
        _mov_b(context, dst.get_operand(), src.get_immediate());,

        _mov_w(context, dst.get_register(), src.get_register()); ,
        _mov_w(context, dst.get_register(), src.get_operand()); ,
        _mov_w(context, dst.get_register(), src.get_immediate()); ,
        _mov_w(context, dst.get_operand(), src.get_register()); ,
        _mov_w(context, dst.get_operand(), src.get_immediate()); ,

        _mov_dw(context, dst.get_register(), src.get_register()); ,
        _mov_dw(context, dst.get_register(), src.get_operand()); ,
        _mov_dw(context, dst.get_register(), src.get_immediate()); ,
        _mov_dw(context, dst.get_operand(), src.get_register()); ,
        _mov_dw(context, dst.get_operand(), src.get_immediate()); ,

        _mov_qw(context, dst.get_register(), src.get_register()); ,
        _mov_qw(context, dst.get_register(), src.get_operand()); ,
        _mov_qw(context, dst.get_register(), src.get_immediate()); ,
        _mov_qw(context, dst.get_operand(), src.get_register()); ,
        _mov_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    return on_new_chain_item();
}

fuku_assambler_ctx& fuku_assambler::cmovcc(fuku_condition cond, const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _cmovcc_w(context, cond, dst.get_register(), src.get_register());,
        _cmovcc_w(context, cond, dst.get_register(), src.get_operand());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _cmovcc_dw(context, cond, dst.get_register(), src.get_register());,
        _cmovcc_dw(context, cond, dst.get_register(), src.get_operand());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _cmovcc_qw(context, cond, dst.get_register(), src.get_register());,
        _cmovcc_qw(context, cond, dst.get_register(), src.get_operand());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::xchg(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        _xchg_b(context, dst.get_register(), src.get_register());,
        _xchg_b(context, src.get_operand(), dst.get_register());,
        UNUSUAL_DATASET,
        _xchg_b(context, dst.get_operand(), src.get_register());,
        UNUSUAL_DATASET,

        _xchg_w(context, dst.get_register(), src.get_register());,
        _xchg_w(context, src.get_operand(), dst.get_register());,
        UNUSUAL_DATASET,
        _xchg_w(context, dst.get_operand(), src.get_register());,
        UNUSUAL_DATASET,

        _xchg_dw(context, dst.get_register(), src.get_register());,
        _xchg_dw(context, src.get_operand(), dst.get_register());,
        UNUSUAL_DATASET,
        _xchg_dw(context, dst.get_operand(), src.get_register());,
        UNUSUAL_DATASET,

        _xchg_qw(context, dst.get_register(), src.get_register());,
        _xchg_qw(context, src.get_operand(), dst.get_register());,
        UNUSUAL_DATASET,
        _xchg_qw(context, dst.get_operand(), src.get_register());,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::bswap(const fuku_type& dst) {
    
    fuku_assambler_command_1op_graph(dst,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _bswap_w(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _bswap_dw(context, dst.get_register());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _bswap_qw(context, dst.get_register());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::xadd(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        _xadd_b(context, dst.get_register(), src.get_register());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _xadd_b(context, dst.get_operand(), src.get_register());,
        UNUSUAL_DATASET,

        _xadd_w(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _xadd_w(context, dst.get_operand(), src.get_register()); ,
        UNUSUAL_DATASET,

        _xadd_dw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _xadd_dw(context, dst.get_operand(), src.get_register()); ,
        UNUSUAL_DATASET,

        _xadd_qw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _xadd_qw(context, dst.get_operand(), src.get_register()); ,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::cmpxchg(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        _cmpxchg_b(context, dst.get_register(), src.get_register());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _cmpxchg_b(context, dst.get_operand(), src.get_register());,
        UNUSUAL_DATASET,

        _cmpxchg_w(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _cmpxchg_w(context, dst.get_operand(), src.get_register()); ,
        UNUSUAL_DATASET,

        _cmpxchg_dw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _cmpxchg_dw(context, dst.get_operand(), src.get_register()); ,
        UNUSUAL_DATASET,

        _cmpxchg_qw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _cmpxchg_qw(context, dst.get_operand(), src.get_register()); ,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::cmpxchg8b(const fuku_type& dst) {
    
    fuku_assambler_command_1op_graph(dst,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        
        UNUSUAL_DATASET,
        _cmpxchg8b(context, dst.get_operand()); ,
        UNUSUAL_DATASET,
        
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::cmpxchg16b(const fuku_type& dst) {
    
    fuku_assambler_command_1op_graph(dst,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        _cmpxchg16b(context, dst.get_operand()); ,
        UNUSUAL_DATASET,
        
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::push(const fuku_type& src) {
    
    fuku_assambler_command_1op_graph(src,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _push_w(context,  src.get_register());,
        _push_w(context, src.get_operand());  ,
        _push_w(context, src.get_immediate());,

        _push_dw(context,  src.get_register());,
        _push_dw(context, src.get_operand());  ,
        _push_dw(context, src.get_immediate());,

        _push_qw(context,  src.get_register());,
        _push_qw(context, src.get_operand());  ,
        _push_qw(context, src.get_immediate());
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::pop(const fuku_type& dst) {
    
    fuku_assambler_command_1op_graph(dst,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _pop_w(context, dst.get_register()); ,
        _pop_w(context, dst.get_operand());  ,
        UNUSUAL_DATASET,

        _pop_dw(context, dst.get_register());,
        _pop_dw(context, dst.get_operand()); ,
        UNUSUAL_DATASET,

        _pop_qw(context, dst.get_register());,
        _pop_qw(context, dst.get_operand()); ,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::cwd() {
    on_emit();  _cwd(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::cdq() {
    on_emit(); _cdq(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::cqo() {
    on_emit(); _cqo(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::cbw() {
    on_emit(); _cbw(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::cwde() {
    on_emit(); _cwde(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::cdqe() {
    on_emit(); _cdqe(context); return on_new_chain_item();
}

fuku_assambler_ctx& fuku_assambler::movzx(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _movzx_byte_w(context, dst.get_register(), src.get_register()); ,
        _movzx_byte_w(context, dst.get_register(), src.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        (src.get_register().get_size() == 1 ? 
            _movzx_byte_dw(context, dst.get_register(), src.get_register()) : 
            _movzx_word_dw(context, dst.get_register(), src.get_register())); ,
        (src.get_operand().get_size() == 1 ?
            _movzx_byte_dw(context, dst.get_register(), src.get_operand()) :
            _movzx_word_dw(context, dst.get_register(), src.get_operand()));,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        (src.get_register().get_size() == 1 ? 
            _movzx_byte_qw(context, dst.get_register(), src.get_register()) : 
            _movzx_word_qw(context, dst.get_register(), src.get_register())); ,
        (src.get_operand().get_size() == 1 ?
            _movzx_byte_qw(context, dst.get_register(), src.get_operand()) :
            _movzx_word_qw(context, dst.get_register(), src.get_operand()));,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::movsx(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _movsx_byte_w(context, dst.get_register(), src.get_register());,
        _movsx_byte_w(context, dst.get_register(), src.get_operand());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        (src.get_register().get_size() == 1 ?
            _movsx_byte_dw(context, dst.get_register(), src.get_register()) :
            _movsx_word_dw(context, dst.get_register(), src.get_register()));,
        (src.get_operand().get_size() == 1 ?
            _movsx_byte_dw(context, dst.get_register(), src.get_operand()) :
            _movsx_word_dw(context, dst.get_register(), src.get_operand())); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        (src.get_register().get_size() == 1 ?
            _movsx_byte_qw(context, dst.get_register(), src.get_register()) :
            _movsx_word_qw(context, dst.get_register(), src.get_register()));,
        (src.get_operand().get_size() == 1 ?
            _movsx_byte_qw(context, dst.get_register(), src.get_operand()) :
            _movsx_word_qw(context, dst.get_register(), src.get_operand())); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}

fuku_assambler_ctx& fuku_assambler::movsxd(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _movsxd_word_w(context, dst.get_register(), src.get_register());,
        _movsxd_word_w(context, dst.get_register(), src.get_operand());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _movsxd_dword_dw(context, dst.get_register(), src.get_register()); ,
        _movsxd_dword_dw(context, dst.get_register(), src.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _movsxd_dword_qw(context, dst.get_register(), src.get_register()); ,
        _movsxd_dword_qw(context, dst.get_register(), src.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}

//Binary Arithmetic Instructions
fuku_assambler_ctx& fuku_assambler::adcx(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _adcx_dw(context, dst.get_register(), src.get_register()); ,
        _adcx_dw(context, dst.get_register(), src.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _adcx_qw(context, dst.get_register(), src.get_register()); ,
        _adcx_qw(context, dst.get_register(), src.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::adox(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET, 
        UNUSUAL_DATASET, 
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _adcx_dw(context, dst.get_register(), src.get_register());,
        _adcx_dw(context, dst.get_register(), src.get_operand());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _adcx_qw(context, dst.get_register(), src.get_register());,
        _adcx_qw(context, dst.get_register(), src.get_operand());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
        )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::add(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        _add_b(context, dst.get_register(), src.get_register());,
        _add_b(context, dst.get_register(), src.get_operand());,
        _add_b(context, dst.get_register(), src.get_immediate());,
        _add_b(context, dst.get_operand(), src.get_register());,
        _add_b(context, dst.get_operand(), src.get_immediate());,

        _add_w(context, dst.get_register(), src.get_register()); ,
        _add_w(context, dst.get_register(), src.get_operand()); ,
        _add_w(context, dst.get_register(), src.get_immediate()); ,
        _add_w(context, dst.get_operand(), src.get_register()); ,
        _add_w(context, dst.get_operand(), src.get_immediate()); ,

        _add_dw(context, dst.get_register(), src.get_register()); ,
        _add_dw(context, dst.get_register(), src.get_operand()); ,
        _add_dw(context, dst.get_register(), src.get_immediate()); ,
        _add_dw(context, dst.get_operand(), src.get_register()); ,
        _add_dw(context, dst.get_operand(), src.get_immediate()); ,

        _add_qw(context, dst.get_register(), src.get_register()); ,
        _add_qw(context, dst.get_register(), src.get_operand()); ,
        _add_qw(context, dst.get_register(), src.get_immediate()); ,
        _add_qw(context, dst.get_operand(), src.get_register()); ,
        _add_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::adc(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        _adc_b(context, dst.get_register(), src.get_register());,
        _adc_b(context, dst.get_register(), src.get_operand());,
        _adc_b(context, dst.get_register(), src.get_immediate());,
        _adc_b(context, dst.get_operand(), src.get_register());,
        _adc_b(context, dst.get_operand(), src.get_immediate());,

        _adc_w(context, dst.get_register(), src.get_register()); ,
        _adc_w(context, dst.get_register(), src.get_operand()); ,
        _adc_w(context, dst.get_register(), src.get_immediate()); ,
        _adc_w(context, dst.get_operand(), src.get_register()); ,
        _adc_w(context, dst.get_operand(), src.get_immediate()); ,

        _adc_dw(context, dst.get_register(), src.get_register()); ,
        _adc_dw(context, dst.get_register(), src.get_operand()); ,
        _adc_dw(context, dst.get_register(), src.get_immediate()); ,
        _adc_dw(context, dst.get_operand(), src.get_register()); ,
        _adc_dw(context, dst.get_operand(), src.get_immediate()); ,

        _adc_qw(context, dst.get_register(), src.get_register()); ,
        _adc_qw(context, dst.get_register(), src.get_operand()); ,
        _adc_qw(context, dst.get_register(), src.get_immediate()); ,
        _adc_qw(context, dst.get_operand(), src.get_register()); ,
        _adc_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::sub(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        _sub_b(context, dst.get_register(), src.get_register());,
        _sub_b(context, dst.get_register(), src.get_operand());,
        _sub_b(context, dst.get_register(), src.get_immediate());,
        _sub_b(context, dst.get_operand(), src.get_register());,
        _sub_b(context, dst.get_operand(), src.get_immediate());,

        _sub_w(context, dst.get_register(), src.get_register()); ,
        _sub_w(context, dst.get_register(), src.get_operand()); ,
        _sub_w(context, dst.get_register(), src.get_immediate()); ,
        _sub_w(context, dst.get_operand(), src.get_register()); ,
        _sub_w(context, dst.get_operand(), src.get_immediate()); ,

        _sub_dw(context, dst.get_register(), src.get_register()); ,
        _sub_dw(context, dst.get_register(), src.get_operand()); ,
        _sub_dw(context, dst.get_register(), src.get_immediate()); ,
        _sub_dw(context, dst.get_operand(), src.get_register()); ,
        _sub_dw(context, dst.get_operand(), src.get_immediate()); ,

        _sub_qw(context, dst.get_register(), src.get_register()); ,
        _sub_qw(context, dst.get_register(), src.get_operand()); ,
        _sub_qw(context, dst.get_register(), src.get_immediate()); ,
        _sub_qw(context, dst.get_operand(), src.get_register()); ,
        _sub_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::sbb(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        _sbb_b(context, dst.get_register(), src.get_register());,
        _sbb_b(context, dst.get_register(), src.get_operand());,
        _sbb_b(context, dst.get_register(), src.get_immediate());,
        _sbb_b(context, dst.get_operand(), src.get_register());,
        _sbb_b(context, dst.get_operand(), src.get_immediate());,

        _sbb_w(context, dst.get_register(), src.get_register()); ,
        _sbb_w(context, dst.get_register(), src.get_operand()); ,
        _sbb_w(context, dst.get_register(), src.get_immediate()); ,
        _sbb_w(context, dst.get_operand(), src.get_register()); ,
        _sbb_w(context, dst.get_operand(), src.get_immediate()); ,

        _sbb_dw(context, dst.get_register(), src.get_register()); ,
        _sbb_dw(context, dst.get_register(), src.get_operand()); ,
        _sbb_dw(context, dst.get_register(), src.get_immediate()); ,
        _sbb_dw(context, dst.get_operand(), src.get_register()); ,
        _sbb_dw(context, dst.get_operand(), src.get_immediate()); ,

        _sbb_qw(context, dst.get_register(), src.get_register()); ,
        _sbb_qw(context, dst.get_register(), src.get_operand()); ,
        _sbb_qw(context, dst.get_register(), src.get_immediate()); ,
        _sbb_qw(context, dst.get_operand(), src.get_register()); ,
        _sbb_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::imul(const fuku_type& src) {
    
    fuku_assambler_command_1op_graph(src,
        _imul_b(context, src.get_register());,
        _imul_b(context, src.get_operand()); ,
        UNUSUAL_DATASET,

        _imul_w(context, src.get_register()); ,
        _imul_w(context, src.get_operand());,
        UNUSUAL_DATASET,

        _imul_dw(context, src.get_register()); ,
        _imul_dw(context, src.get_operand());,
        UNUSUAL_DATASET,

        _imul_qw(context, src.get_register()); ,
        _imul_qw(context, src.get_operand());,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::mul(const fuku_type& dst) {
    
    fuku_assambler_command_1op_graph(dst,
        _mul_b(context, dst.get_register());,
        _mul_b(context, dst.get_operand()); ,
        UNUSUAL_DATASET,

        _mul_w(context, dst.get_register()); ,
        _mul_w(context, dst.get_operand());,
        UNUSUAL_DATASET,

        _mul_dw(context, dst.get_register()); ,
        _mul_dw(context, dst.get_operand());,
        UNUSUAL_DATASET,

        _mul_qw(context, dst.get_register()); ,
        _mul_qw(context, dst.get_operand());,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::idiv(const fuku_type& src) {
    
    fuku_assambler_command_1op_graph(src,
        _idiv_b(context, src.get_register());,
        _idiv_b(context, src.get_operand()); ,
        UNUSUAL_DATASET,

        _idiv_w(context, src.get_register()); ,
        _idiv_w(context, src.get_operand());,
        UNUSUAL_DATASET,

        _idiv_dw(context, src.get_register()); ,
        _idiv_dw(context, src.get_operand());,
        UNUSUAL_DATASET,

        _idiv_qw(context, src.get_register()); ,
        _idiv_qw(context, src.get_operand());,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::div(const fuku_type& dst) {
    
    fuku_assambler_command_1op_graph(dst,
        _div_b(context, dst.get_register());,
        _div_b(context, dst.get_operand()); ,
        UNUSUAL_DATASET,

        _div_w(context, dst.get_register()); ,
        _div_w(context, dst.get_operand());,
        UNUSUAL_DATASET,

        _div_dw(context, dst.get_register()); ,
        _div_dw(context, dst.get_operand());,
        UNUSUAL_DATASET,

        _div_qw(context, dst.get_register()); ,
        _div_qw(context, dst.get_operand());,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::inc(const fuku_type& src) {
    
    fuku_assambler_command_1op_graph(src,
        _inc_b(context, src.get_register());,
        _inc_b(context, src.get_operand()); ,
        UNUSUAL_DATASET,

        _inc_w(context, src.get_register()); ,
        _inc_w(context, src.get_operand());,
        UNUSUAL_DATASET,

        _inc_dw(context, src.get_register()); ,
        _inc_dw(context, src.get_operand());,
        UNUSUAL_DATASET,

        _inc_qw(context, src.get_register()); ,
        _inc_qw(context, src.get_operand());,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::dec(const fuku_type& dst) {
    
    fuku_assambler_command_1op_graph(dst,
        _dec_b(context, dst.get_register());,
        _dec_b(context, dst.get_operand()); ,
        UNUSUAL_DATASET,

        _dec_w(context, dst.get_register()); ,
        _dec_w(context, dst.get_operand());,
        UNUSUAL_DATASET,

        _dec_dw(context, dst.get_register()); ,
        _dec_dw(context, dst.get_operand());,
        UNUSUAL_DATASET,

        _dec_qw(context, dst.get_register()); ,
        _dec_qw(context, dst.get_operand());,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::neg(const fuku_type& src) {
    
    fuku_assambler_command_1op_graph(src,
        _neg_b(context, src.get_register());,
        _neg_b(context, src.get_operand()); ,
        UNUSUAL_DATASET,

        _neg_w(context, src.get_register()); ,
        _neg_w(context, src.get_operand());,
        UNUSUAL_DATASET,

        _neg_dw(context, src.get_register()); ,
        _neg_dw(context, src.get_operand());,
        UNUSUAL_DATASET,

        _neg_qw(context, src.get_register()); ,
        _neg_qw(context, src.get_operand());,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::cmp(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        _cmp_b(context, dst.get_register(), src.get_register());,
        _cmp_b(context, dst.get_register(), src.get_operand());,
        _cmp_b(context, dst.get_register(), src.get_immediate());,
        _cmp_b(context, dst.get_operand(), src.get_register());,
        _cmp_b(context, dst.get_operand(), src.get_immediate());,

        _cmp_w(context, dst.get_register(), src.get_register()); ,
        _cmp_w(context, dst.get_register(), src.get_operand()); ,
        _cmp_w(context, dst.get_register(), src.get_immediate()); ,
        _cmp_w(context, dst.get_operand(), src.get_register()); ,
        _cmp_w(context, dst.get_operand(), src.get_immediate()); ,

        _cmp_dw(context, dst.get_register(), src.get_register()); ,
        _cmp_dw(context, dst.get_register(), src.get_operand()); ,
        _cmp_dw(context, dst.get_register(), src.get_immediate()); ,
        _cmp_dw(context, dst.get_operand(), src.get_register()); ,
        _cmp_dw(context, dst.get_operand(), src.get_immediate()); ,

        _cmp_qw(context, dst.get_register(), src.get_register()); ,
        _cmp_qw(context, dst.get_register(), src.get_operand()); ,
        _cmp_qw(context, dst.get_register(), src.get_immediate()); ,
        _cmp_qw(context, dst.get_operand(), src.get_register()); ,
        _cmp_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    return on_new_chain_item();
}

//Decimal Arithmetic Instructions
fuku_assambler_ctx& fuku_assambler::daa() {
    on_emit(); _daa(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::das() {
    on_emit(); _das(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::aaa() {
    on_emit(); _aaa(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::aas() {
    on_emit(); _aas(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::aam(const fuku_type& src) {
    on_emit(); _aam(context, src.get_immediate()); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::aad(const fuku_type& src) {
    on_emit(); _aad(context, src.get_immediate()); return on_new_chain_item();
}

//Logical Instructions Instructions
fuku_assambler_ctx& fuku_assambler::and_(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        _and_b(context, dst.get_register(), src.get_register());,
        _and_b(context, dst.get_register(), src.get_operand());,
        _and_b(context, dst.get_register(), src.get_immediate());,
        _and_b(context, dst.get_operand(), src.get_register());,
        _and_b(context, dst.get_operand(), src.get_immediate());,

        _and_w(context, dst.get_register(), src.get_register()); ,
        _and_w(context, dst.get_register(), src.get_operand()); ,
        _and_w(context, dst.get_register(), src.get_immediate()); ,
        _and_w(context, dst.get_operand(), src.get_register()); ,
        _and_w(context, dst.get_operand(), src.get_immediate()); ,

        _and_dw(context, dst.get_register(), src.get_register()); ,
        _and_dw(context, dst.get_register(), src.get_operand()); ,
        _and_dw(context, dst.get_register(), src.get_immediate()); ,
        _and_dw(context, dst.get_operand(), src.get_register()); ,
        _and_dw(context, dst.get_operand(), src.get_immediate()); ,

        _and_qw(context, dst.get_register(), src.get_register()); ,
        _and_qw(context, dst.get_register(), src.get_operand()); ,
        _and_qw(context, dst.get_register(), src.get_immediate()); ,
        _and_qw(context, dst.get_operand(), src.get_register()); ,
        _and_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::or_(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        _or_b(context, dst.get_register(), src.get_register());,
        _or_b(context, dst.get_register(), src.get_operand());,
        _or_b(context, dst.get_register(), src.get_immediate());,
        _or_b(context, dst.get_operand(), src.get_register());,
        _or_b(context, dst.get_operand(), src.get_immediate());,

        _or_w(context, dst.get_register(), src.get_register()); ,
        _or_w(context, dst.get_register(), src.get_operand()); ,
        _or_w(context, dst.get_register(), src.get_immediate()); ,
        _or_w(context, dst.get_operand(), src.get_register()); ,
        _or_w(context, dst.get_operand(), src.get_immediate()); ,

        _or_dw(context, dst.get_register(), src.get_register()); ,
        _or_dw(context, dst.get_register(), src.get_operand()); ,
        _or_dw(context, dst.get_register(), src.get_immediate()); ,
        _or_dw(context, dst.get_operand(), src.get_register()); ,
        _or_dw(context, dst.get_operand(), src.get_immediate()); ,

        _or_qw(context, dst.get_register(), src.get_register()); ,
        _or_qw(context, dst.get_register(), src.get_operand()); ,
        _or_qw(context, dst.get_register(), src.get_immediate()); ,
        _or_qw(context, dst.get_operand(), src.get_register()); ,
        _or_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::xor_(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        _xor_b(context, dst.get_register(), src.get_register());,
        _xor_b(context, dst.get_register(), src.get_operand());,
        _xor_b(context, dst.get_register(), src.get_immediate());,
        _xor_b(context, dst.get_operand(), src.get_register());,
        _xor_b(context, dst.get_operand(), src.get_immediate());,

        _xor_w(context, dst.get_register(), src.get_register()); ,
        _xor_w(context, dst.get_register(), src.get_operand()); ,
        _xor_w(context, dst.get_register(), src.get_immediate()); ,
        _xor_w(context, dst.get_operand(), src.get_register()); ,
        _xor_w(context, dst.get_operand(), src.get_immediate()); ,

        _xor_dw(context, dst.get_register(), src.get_register()); ,
        _xor_dw(context, dst.get_register(), src.get_operand()); ,
        _xor_dw(context, dst.get_register(), src.get_immediate()); ,
        _xor_dw(context, dst.get_operand(), src.get_register()); ,
        _xor_dw(context, dst.get_operand(), src.get_immediate()); ,

        _xor_qw(context, dst.get_register(), src.get_register()); ,
        _xor_qw(context, dst.get_register(), src.get_operand()); ,
        _xor_qw(context, dst.get_register(), src.get_immediate()); ,
        _xor_qw(context, dst.get_operand(), src.get_register()); ,
        _xor_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::not_(const fuku_type& dst) {
    
    fuku_assambler_command_1op_graph(dst,
        _not_b(context, dst.get_register());,
        _not_b(context, dst.get_operand()); ,
        UNUSUAL_DATASET,

        _not_w(context, dst.get_register()); ,
        _not_w(context, dst.get_operand());,
        UNUSUAL_DATASET,

        _not_dw(context, dst.get_register()); ,
        _not_dw(context, dst.get_operand());,
        UNUSUAL_DATASET,

        _not_qw(context, dst.get_register()); ,
        _not_qw(context, dst.get_operand());,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}

//Shift and Rotate Instructions
fuku_assambler_ctx& fuku_assambler::sar(const fuku_type& dst, const fuku_type& src) {
    if (src.get_type() == FUKU_T0_REGISTER && src.get_register().get_index() != FUKU_REG_INDEX_CX) {
        UNUSUAL_DATASET
    }
    
    
    fuku_assambler_command_2op_graph(
        _sar_cl_b(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        _sar_b(context, dst.get_register(), src.get_immediate()); ,
        _sar_cl_b(context, dst.get_operand()); ,
        _sar_b(context, dst.get_operand(), src.get_immediate()); ,

        _sar_cl_w(context, dst.get_register());,
        UNUSUAL_DATASET,
        _sar_w(context, dst.get_register(), src.get_immediate());,
        _sar_cl_w(context, dst.get_operand());,
        _sar_w(context, dst.get_operand(), src.get_immediate());,

        _sar_cl_dw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _sar_dw(context, dst.get_register(), src.get_immediate());,
        _sar_cl_dw(context, dst.get_operand());,
        _sar_dw(context, dst.get_operand(), src.get_immediate());,

        _sar_cl_qw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _sar_qw(context, dst.get_register(), src.get_immediate());,
        _sar_cl_qw(context, dst.get_operand());,
        _sar_qw(context, dst.get_operand(), src.get_immediate());
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::shr(const fuku_type& dst, const fuku_type& src) {
    if (src.get_type() == FUKU_T0_REGISTER && src.get_register().get_index() != FUKU_REG_INDEX_CX) {
        UNUSUAL_DATASET
    }
    
    fuku_assambler_command_2op_graph(
        _shr_cl_b(context, dst.get_register());,
        UNUSUAL_DATASET,
        _shr_b(context, dst.get_register(), src.get_immediate());,
        _shr_cl_b(context, dst.get_operand());,
        _shr_b(context, dst.get_operand(), src.get_immediate());,

        _shr_cl_w(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        _shr_w(context, dst.get_register(), src.get_immediate()); ,
        _shr_cl_w(context, dst.get_operand()); ,
        _shr_w(context, dst.get_operand(), src.get_immediate()); ,

        _shr_cl_dw(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        _shr_dw(context, dst.get_register(), src.get_immediate()); ,
        _shr_cl_dw(context, dst.get_operand()); ,
        _shr_dw(context, dst.get_operand(), src.get_immediate()); ,

        _shr_cl_qw(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        _shr_qw(context, dst.get_register(), src.get_immediate()); ,
        _shr_cl_qw(context, dst.get_operand()); ,
        _shr_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::shl(const fuku_type& dst, const fuku_type& src) {
    if (src.get_type() == FUKU_T0_REGISTER && src.get_register().get_index() != FUKU_REG_INDEX_CX) {
        UNUSUAL_DATASET
    }
    
    fuku_assambler_command_2op_graph(
        _shl_cl_b(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        _shl_b(context, dst.get_register(), src.get_immediate()); ,
        _shl_cl_b(context, dst.get_operand()); ,
        _shl_b(context, dst.get_operand(), src.get_immediate()); ,

        _shl_cl_w(context, dst.get_register());,
        UNUSUAL_DATASET,
        _shl_w(context, dst.get_register(), src.get_immediate());,
        _shl_cl_w(context, dst.get_operand());,
        _shl_w(context, dst.get_operand(), src.get_immediate());,

        _shl_cl_dw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _shl_dw(context, dst.get_register(), src.get_immediate());,
        _shl_cl_dw(context, dst.get_operand());,
        _shl_dw(context, dst.get_operand(), src.get_immediate());,

        _shl_cl_qw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _shl_qw(context, dst.get_register(), src.get_immediate());,
        _shl_cl_qw(context, dst.get_operand());,
        _shl_qw(context, dst.get_operand(), src.get_immediate());
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::shrd(const fuku_type& dst, const fuku_type& src, const fuku_type& shift) {
    if ((shift.get_type() == FUKU_T0_REGISTER && shift.get_register().get_index() != FUKU_REG_INDEX_CX) || 
        !(shift.get_type() == FUKU_T0_REGISTER || shift.get_type() == FUKU_T0_IMMEDIATE)) {

        UNUSUAL_DATASET
    }

    bool shift_reg = shift.get_type() == FUKU_T0_REGISTER;

    
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        (shift_reg ? _shrd_cl_w(context, dst.get_register(), src.get_register()) :
            _shrd_w(context, dst.get_register(), src.get_register(), shift.get_immediate()));,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        (shift_reg ? _shrd_cl_w(context, dst.get_operand(), src.get_register()) :
            _shrd_w(context, dst.get_operand(), src.get_register(), shift.get_immediate())); ,
        UNUSUAL_DATASET,

        (shift_reg ? _shrd_cl_dw(context, dst.get_register(), src.get_register()) :
            _shrd_dw(context, dst.get_register(), src.get_register(), shift.get_immediate())); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        (shift_reg ? _shrd_cl_dw(context, dst.get_operand(), src.get_register()) :
            _shrd_dw(context, dst.get_operand(), src.get_register(), shift.get_immediate()));,
        UNUSUAL_DATASET,

        (shift_reg ? _shrd_cl_qw(context, dst.get_register(), src.get_register()) :
            _shrd_qw(context, dst.get_register(), src.get_register(), shift.get_immediate())); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        (shift_reg ? _shrd_cl_qw(context, dst.get_operand(), src.get_register()) :
            _shrd_qw(context, dst.get_operand(), src.get_register(), shift.get_immediate()));,
        UNUSUAL_DATASET
    )
        return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::shld(const fuku_type& dst, const fuku_type& src, const fuku_type& shift) {
    if ((shift.get_type() == FUKU_T0_REGISTER && shift.get_register().get_index() != FUKU_REG_INDEX_CX) ||
        !(shift.get_type() == FUKU_T0_REGISTER || shift.get_type() == FUKU_T0_IMMEDIATE)) {

        UNUSUAL_DATASET
    }

    bool shift_reg = shift.get_type() == FUKU_T0_REGISTER;

    
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        (shift_reg ? _shld_cl_w(context, dst.get_register(), src.get_register()) :
            _shld_w(context, dst.get_register(), src.get_register(), shift.get_immediate())); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        (shift_reg ? _shld_cl_w(context, dst.get_operand(), src.get_register()) :
            _shld_w(context, dst.get_operand(), src.get_register(), shift.get_immediate()));,
        UNUSUAL_DATASET,

        (shift_reg ? _shld_cl_dw(context, dst.get_register(), src.get_register()) :
            _shld_dw(context, dst.get_register(), src.get_register(), shift.get_immediate()));,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        (shift_reg ? _shld_cl_dw(context, dst.get_operand(), src.get_register()) :
            _shld_dw(context, dst.get_operand(), src.get_register(), shift.get_immediate())); ,
        UNUSUAL_DATASET,

        (shift_reg ? _shld_cl_qw(context, dst.get_register(), src.get_register()) :
            _shld_qw(context, dst.get_register(), src.get_register(), shift.get_immediate()));,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        (shift_reg ? _shld_cl_qw(context, dst.get_operand(), src.get_register()) :
            _shld_qw(context, dst.get_operand(), src.get_register(), shift.get_immediate())); ,
        UNUSUAL_DATASET
        )
        return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::ror(const fuku_type& dst, const fuku_type& src) {
    if (src.get_type() == FUKU_T0_REGISTER && src.get_register().get_index() != FUKU_REG_INDEX_CX) {
        UNUSUAL_DATASET
    }
    
    fuku_assambler_command_2op_graph(
        _ror_cl_b(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        _ror_b(context, dst.get_register(), src.get_immediate()); ,
        _ror_cl_b(context, dst.get_operand()); ,
        _ror_b(context, dst.get_operand(), src.get_immediate()); ,

        _ror_cl_w(context, dst.get_register());,
        UNUSUAL_DATASET,
        _ror_w(context, dst.get_register(), src.get_immediate());,
        _ror_cl_w(context, dst.get_operand());,
        _ror_w(context, dst.get_operand(), src.get_immediate());,

        _ror_cl_dw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _ror_dw(context, dst.get_register(), src.get_immediate());,
        _ror_cl_dw(context, dst.get_operand());,
        _ror_dw(context, dst.get_operand(), src.get_immediate());,

        _ror_cl_qw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _ror_qw(context, dst.get_register(), src.get_immediate());,
        _ror_cl_qw(context, dst.get_operand());,
        _ror_qw(context, dst.get_operand(), src.get_immediate());
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::rol(const fuku_type& dst, const fuku_type& src) {
    if (src.get_type() == FUKU_T0_REGISTER && src.get_register().get_index() != FUKU_REG_INDEX_CX) {
        UNUSUAL_DATASET
    }
    
    fuku_assambler_command_2op_graph(
        _rol_cl_b(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        _rol_b(context, dst.get_register(), src.get_immediate()); ,
        _rol_cl_b(context, dst.get_operand()); ,
        _rol_b(context, dst.get_operand(), src.get_immediate()); ,

        _rol_cl_w(context, dst.get_register());,
        UNUSUAL_DATASET,
        _rol_w(context, dst.get_register(), src.get_immediate());,
        _rol_cl_w(context, dst.get_operand());,
        _rol_w(context, dst.get_operand(), src.get_immediate());,

        _rol_cl_dw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _rol_dw(context, dst.get_register(), src.get_immediate());,
        _rol_cl_dw(context, dst.get_operand());,
        _rol_dw(context, dst.get_operand(), src.get_immediate());,

        _rol_cl_qw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _rol_qw(context, dst.get_register(), src.get_immediate());,
        _rol_cl_qw(context, dst.get_operand());,
        _rol_qw(context, dst.get_operand(), src.get_immediate());
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::rcr(const fuku_type& dst, const fuku_type& src) {
    if (src.get_type() == FUKU_T0_REGISTER && src.get_register().get_index() != FUKU_REG_INDEX_CX) {
        UNUSUAL_DATASET
    }
    
    fuku_assambler_command_2op_graph(
        _rcr_cl_b(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        _rcr_b(context, dst.get_register(), src.get_immediate()); ,
        _rcr_cl_b(context, dst.get_operand()); ,
        _rcr_b(context, dst.get_operand(), src.get_immediate()); ,

        _rcr_cl_w(context, dst.get_register());,
        UNUSUAL_DATASET,
        _rcr_w(context, dst.get_register(), src.get_immediate());,
        _rcr_cl_w(context, dst.get_operand());,
        _rcr_w(context, dst.get_operand(), src.get_immediate());,

        _rcr_cl_dw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _rcr_dw(context, dst.get_register(), src.get_immediate());,
        _rcr_cl_dw(context, dst.get_operand());,
        _rcr_dw(context, dst.get_operand(), src.get_immediate());,

        _rcr_cl_qw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _rcr_qw(context, dst.get_register(), src.get_immediate());,
        _rcr_cl_qw(context, dst.get_operand());,
        _rcr_qw(context, dst.get_operand(), src.get_immediate());
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::rcl(const fuku_type& dst, const fuku_type& src) {
    if (src.get_type() == FUKU_T0_REGISTER && src.get_register().get_index() != FUKU_REG_INDEX_CX) {
        UNUSUAL_DATASET
    }
    
    fuku_assambler_command_2op_graph(
        _rcl_cl_b(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        _rcl_b(context, dst.get_register(), src.get_immediate()); ,
        _rcl_cl_b(context, dst.get_operand()); ,
        _rcl_b(context, dst.get_operand(), src.get_immediate()); ,

        _rcl_cl_w(context, dst.get_register());,
        UNUSUAL_DATASET,
        _rcl_w(context, dst.get_register(), src.get_immediate());,
        _rcl_cl_w(context, dst.get_operand());,
        _rcl_w(context, dst.get_operand(), src.get_immediate());,

        _rcl_cl_dw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _rcl_dw(context, dst.get_register(), src.get_immediate());,
        _rcl_cl_dw(context, dst.get_operand());,
        _rcl_dw(context, dst.get_operand(), src.get_immediate());,

        _rcl_cl_qw(context, dst.get_register());,
        UNUSUAL_DATASET,
        _rcl_qw(context, dst.get_register(), src.get_immediate());,
        _rcl_cl_qw(context, dst.get_operand());,
        _rcl_qw(context, dst.get_operand(), src.get_immediate());
    )
    return on_new_chain_item();
}

//Bit and Byte Instructions
fuku_assambler_ctx& fuku_assambler::bt(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _bt_w(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _bt_w(context, dst.get_register(), src.get_immediate()); ,
        _bt_w(context, dst.get_operand(), src.get_register()); ,
        _bt_w(context, dst.get_operand(), src.get_immediate()); ,

        _bt_dw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _bt_dw(context, dst.get_register(), src.get_immediate()); ,
        _bt_dw(context, dst.get_operand(), src.get_register()); ,
        _bt_dw(context, dst.get_operand(), src.get_immediate()); ,

        _bt_qw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _bt_qw(context, dst.get_register(), src.get_immediate()); ,
        _bt_qw(context, dst.get_operand(), src.get_register()); ,
        _bt_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::bts(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _bts_w(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _bts_w(context, dst.get_register(), src.get_immediate()); ,
        _bts_w(context, dst.get_operand(), src.get_register()); ,
        _bts_w(context, dst.get_operand(), src.get_immediate()); ,

        _bts_dw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _bts_dw(context, dst.get_register(), src.get_immediate()); ,
        _bts_dw(context, dst.get_operand(), src.get_register()); ,
        _bts_dw(context, dst.get_operand(), src.get_immediate()); ,

        _bts_qw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _bts_qw(context, dst.get_register(), src.get_immediate()); ,
        _bts_qw(context, dst.get_operand(), src.get_register()); ,
        _bts_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::btr(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _btr_w(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _btr_w(context, dst.get_register(), src.get_immediate()); ,
        _btr_w(context, dst.get_operand(), src.get_register()); ,
        _btr_w(context, dst.get_operand(), src.get_immediate()); ,

        _btr_dw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _btr_dw(context, dst.get_register(), src.get_immediate()); ,
        _btr_dw(context, dst.get_operand(), src.get_register()); ,
        _btr_dw(context, dst.get_operand(), src.get_immediate()); ,

        _btr_qw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _btr_qw(context, dst.get_register(), src.get_immediate()); ,
        _btr_qw(context, dst.get_operand(), src.get_register()); ,
        _btr_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::btc(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _btc_w(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _btc_w(context, dst.get_register(), src.get_immediate()); ,
        _btc_w(context, dst.get_operand(), src.get_register()); ,
        _btc_w(context, dst.get_operand(), src.get_immediate()); ,

        _btc_dw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _btc_dw(context, dst.get_register(), src.get_immediate()); ,
        _btc_dw(context, dst.get_operand(), src.get_register()); ,
        _btc_dw(context, dst.get_operand(), src.get_immediate()); ,

        _btc_qw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _btc_qw(context, dst.get_register(), src.get_immediate()); ,
        _btc_qw(context, dst.get_operand(), src.get_register()); ,
        _btc_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::setcc(fuku_condition cond, const fuku_type& dst) {
    
    fuku_assambler_command_1op_graph(dst,
        _setcc(context, cond, dst.get_register());,
        _setcc(context, cond, dst.get_operand()); ,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::test(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        _test_b(context, dst.get_register(), src.get_register());,
        UNUSUAL_DATASET,
        _test_b(context, dst.get_register(), src.get_immediate());,
        _test_b(context, dst.get_operand(), src.get_register());,
        _test_b(context, dst.get_operand(), src.get_immediate());,

        _test_w(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _test_w(context, dst.get_register(), src.get_immediate()); ,
        _test_w(context, dst.get_operand(), src.get_register()); ,
        _test_w(context, dst.get_operand(), src.get_immediate()); ,

        _test_dw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _test_dw(context, dst.get_register(), src.get_immediate()); ,
        _test_dw(context, dst.get_operand(), src.get_register()); ,
        _test_dw(context, dst.get_operand(), src.get_immediate()); ,

        _test_qw(context, dst.get_register(), src.get_register()); ,
        UNUSUAL_DATASET,
        _test_qw(context, dst.get_register(), src.get_immediate()); ,
        _test_qw(context, dst.get_operand(), src.get_register()); ,
        _test_qw(context, dst.get_operand(), src.get_immediate()); 
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::popcnt(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _popcnt_w(context, dst.get_register(), src.get_register()); ,
        _popcnt_w(context, dst.get_register(), dst.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _popcnt_dw(context, dst.get_register(), src.get_register()); ,
        _popcnt_dw(context, dst.get_register(), dst.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _popcnt_qw(context, dst.get_register(), src.get_register()); ,
        _popcnt_qw(context, dst.get_register(), dst.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}

//Control Transfer Instructions
fuku_assambler_ctx& fuku_assambler::jmp(const fuku_type& src) {
    
    fuku_assambler_command_1op_graph(src,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _jmp(context, src.get_register()); ,
        _jmp(context, src.get_operand());,
        _jmp(context, src.get_immediate()); ,

        _jmp(context, src.get_register()); ,
        _jmp(context, src.get_operand());,
        _jmp(context, src.get_immediate()); 
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::jcc(fuku_condition cond, const fuku_type& src) {
    
    fuku_assambler_command_1op_graph(src,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _jcc(context, cond, src.get_immediate()); ,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        _jcc(context, cond, src.get_immediate());
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::call(const fuku_type& src) {
    
    fuku_assambler_command_1op_graph(src,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _call(context, src.get_register()); ,
        _call(context, src.get_operand());  ,
        _call(context, src.get_immediate());,

        _call(context, src.get_register()); ,
        _call(context, src.get_operand());  ,
        _call(context, src.get_immediate());
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::ret(const fuku_type& src) {
    
    fuku_assambler_command_1op_graph(src,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        src.get_immediate().get_immediate16() ? _ret(context, src.get_immediate()) : _ret(context); ,

        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        src.get_immediate().get_immediate16() ? _ret(context, src.get_immediate()) : _ret(context);
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::int3() {
     _int3(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::enter(const fuku_type& size, const fuku_type& nestinglevel) {
    
    if (!(size.get_type() == FUKU_T0_IMMEDIATE) || !(nestinglevel.get_type() == FUKU_T0_IMMEDIATE)) {
        FUKU_DEBUG; 
        return on_new_chain_item();
    }

    on_emit();

    _enter(context,
        size.get_immediate(),
        nestinglevel.get_immediate().get_immediate8());

    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::leave_() {
     _leave_(context); return on_new_chain_item();
}

//String Instructions
fuku_assambler_ctx& fuku_assambler::outsb() {
    on_emit(); _outsb(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::outsw() {
    on_emit(); _outsw(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::outsd() {
    on_emit(); _outsd(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::movsb() {
    on_emit(); _movsb(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::movsw() {
    on_emit(); _movsw(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::movsd() {
    on_emit(); _movsd(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::movsq() {
    on_emit(); _movsq(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::cmpsb() {
    on_emit(); _cmpsb(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::cmpsw() {
    on_emit(); _cmpsw(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::cmpsd() {
    on_emit(); _cmpsd(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::cmpsq() {
    on_emit(); _cmpsq(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::scasb() {
    on_emit(); _scasb(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::scasw() {
    on_emit(); _scasw(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::scasd() {
    on_emit(); _scasd(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::scasq() {
    on_emit(); _scasq(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::lodsb() {
    on_emit(); _lodsb(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::lodsw() {
    on_emit(); _lodsw(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::lodsd() {
    on_emit(); _lodsd(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::lodsq() {
    on_emit(); _lodsq(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::stosb() {
    on_emit(); _stosb(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::stosw() {
    on_emit(); _stosw(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::stosd() {
    on_emit(); _stosd(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::stosq() {
    on_emit(); _stosq(context); return on_new_chain_item();
}

fuku_assambler_ctx& fuku_assambler::stc() {
    on_emit(); _stc(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::clc() {
    on_emit(); _clc(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::cmc() {
    on_emit(); _cmc(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::cld() {
    on_emit(); _cld(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::std() {
    on_emit(); _std(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::lahf() {
    on_emit(); _lahf(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::sahf() {
    on_emit(); _sahf(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::pusha() {
    on_emit(); _pusha(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::pushad() {
    on_emit(); _pushad(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::popa() {
    on_emit(); _popa(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::popad() {
    on_emit(); _popad(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::pushf() {
    on_emit(); _pushf(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::pushfd() {
    on_emit(); _pushfd(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::pushfq() {
    on_emit(); _pushfq(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::popf() {
    on_emit(); _popf(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::popfd() {
    on_emit(); _popfd(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::popfq() {
    on_emit(); _popfq(context); return on_new_chain_item();
}
//Miscellaneous Instructions
fuku_assambler_ctx& fuku_assambler::lea(const fuku_type& dst, const fuku_type& src) {
    
    fuku_assambler_command_2op_graph(
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        _lea_w(context, dst.get_register(), src.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        _lea_dw(context, dst.get_register(), src.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        UNUSUAL_DATASET,
        _lea_qw(context, dst.get_register(), src.get_operand()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::nop(uint8_t size) {
    on_emit(); _nop(context, size); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::ud2() {
    on_emit(); _ud2(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::cpuid() {
    on_emit(); _cpuid(context); return on_new_chain_item();
}
//Random Number Generator Instructions
fuku_assambler_ctx& fuku_assambler::rdrand(const fuku_type& dst) {
    
    fuku_assambler_command_1op_graph(dst,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _rdrand_w(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _rdrand_dw(context, dst.get_register());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _rdrand_qw(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::rdseed(const fuku_type& dst) {
    
    fuku_assambler_command_1op_graph(dst,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _rdseed_w(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _rdseed_dw(context, dst.get_register());,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET,

        _rdseed_qw(context, dst.get_register()); ,
        UNUSUAL_DATASET,
        UNUSUAL_DATASET
    )
    return on_new_chain_item();
}
//SYSTEM INSTRUCTIONS
fuku_assambler_ctx& fuku_assambler::hlt() {
    on_emit(); _hlt(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::rdtsc() {
    on_emit(); _rdtsc(context); return on_new_chain_item();
}
fuku_assambler_ctx& fuku_assambler::lfence() {
    on_emit(); _lfence(context); return on_new_chain_item();
}