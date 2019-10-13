#include "stdafx.h"
#include "fuku_code_holder.h"


fuku_code_holder::fuku_code_holder()
    :arch(fuku_assambler_arch::FUKU_ASSAMBLER_ARCH_X86){}

fuku_code_holder::fuku_code_holder(fuku_assambler_arch arch)
    :arch(arch) {}

fuku_code_holder::fuku_code_holder(const fuku_code_holder& code_holder) {
    this->operator=(code_holder);
}

fuku_code_holder::~fuku_code_holder() {

}


fuku_code_holder& fuku_code_holder::operator=(const fuku_code_holder& code_holder) {

    this->arch = code_holder.arch;
    this->labels = code_holder.labels;
    this->relocations = code_holder.relocations;
    this->rip_relocations = code_holder.rip_relocations;
    this->available_relocations = code_holder.available_relocations;
    this->available_rip_relocations = code_holder.available_rip_relocations;
    this->insts = code_holder.insts;

    if (labels.size()) {
     
        std::vector<fuku_code_label*> label_map;
        std::map<const fuku_relocation*, fuku_relocation*> reloc_map;
        std::map<const fuku_rip_relocation*, fuku_rip_relocation*> rip_reloc_map;


        create_relation_map(
            code_holder.labels.begin(), this->labels.begin(),
            code_holder.labels.end(), this->labels.end(),

            code_holder.relocations.begin(), this->relocations.begin(),
            code_holder.relocations.end(), this->relocations.end(),

            code_holder.rip_relocations.begin(), this->rip_relocations.begin(),
            code_holder.rip_relocations.end(), this->rip_relocations.end(),

            label_map, 
            reloc_map, 
            rip_reloc_map
        );

        for (size_t idx = 0; idx < this->available_relocations.size(); idx++) {

            this->available_relocations[idx] = reloc_map[this->available_relocations[idx]];
        }

        for (size_t idx = 0; idx < this->available_rip_relocations.size(); idx++) {

            this->available_rip_relocations[idx] = rip_reloc_map[this->available_rip_relocations[idx]];
        }


        for (auto& inst : this->insts) {

            if (inst.get_label()) {
                auto label = label_map[inst.get_label()->label_id];

                inst.set_label(label);
                label->set_inst(&inst);
            }

            if (inst.get_imm_reloc()) {

                auto prev_imm_reloc = inst.get_imm_reloc();

                if (!prev_imm_reloc->label) {
                    continue;
                }

                auto new_imm_reloc = reloc_map[prev_imm_reloc];

                new_imm_reloc->label = label_map[prev_imm_reloc->label->label_id];

                inst.set_imm_reloc(new_imm_reloc);
            }

            if (inst.get_disp_reloc()) {

                if (inst.is_used_disp_reloc()) {

                    auto prev_disp_reloc = inst.get_disp_reloc();

                    if (!prev_disp_reloc->label) {
                        continue;
                    }

                    auto new_disp_reloc  = reloc_map[prev_disp_reloc];

                    new_disp_reloc->label = label_map[prev_disp_reloc->label->label_id];

                    inst.set_disp_reloc(new_disp_reloc);
                }
                else { //else rip reloc

                    auto prev_rip_reloc = inst.get_rip_reloc();

                    if (!prev_rip_reloc->label) {
                        continue;
                    }

                    auto new_rip_reloc = rip_reloc_map[prev_rip_reloc];

                    new_rip_reloc->label = label_map[prev_rip_reloc->label->label_id];

                    inst.set_rip_reloc(new_rip_reloc);
                }
            }
        }
    }


    return *this;
}


void fuku_code_holder::create_relation_map(
    std::list<fuku_code_label>::const_iterator prev_labels, std::list<fuku_code_label>::iterator new_labels,
    std::list<fuku_code_label>::const_iterator prev_labels_end, std::list<fuku_code_label>::iterator new_labels_end,

    std::list<fuku_relocation>::const_iterator prev_relocs, std::list<fuku_relocation>::iterator new_relocs,
    std::list<fuku_relocation>::const_iterator prev_relocs_end, std::list<fuku_relocation>::iterator new_relocs_end,

    std::list<fuku_rip_relocation>::const_iterator prev_rip_relocs, std::list<fuku_rip_relocation>::iterator new_rip_relocs,
    std::list<fuku_rip_relocation>::const_iterator prev_rip_relocs_end, std::list<fuku_rip_relocation>::iterator new_rip_relocs_end,

    std::vector<fuku_code_label*>& label_map,
    std::map<const fuku_relocation*, fuku_relocation*>& reloc_map,
    std::map<const fuku_rip_relocation*, fuku_rip_relocation*>& rip_reloc_map
) const {

    label_map.clear();
    reloc_map.clear();
    rip_reloc_map.clear();


    for (; prev_labels != prev_labels_end && new_labels != new_labels_end;
        prev_labels++, new_labels++) {

        // vec by idx of orig label to current label

        ((fuku_code_label*)&(*prev_labels))->label_id = 
            label_map.size();

        label_map.push_back(&(*new_labels));
    }

    for (; prev_relocs != prev_relocs_end && new_relocs != new_relocs_end;
        prev_relocs++, new_relocs++) {

        reloc_map[&(*prev_relocs)] = &(*new_relocs);
    }

    for (; prev_rip_relocs != prev_rip_relocs_end && new_rip_relocs != new_rip_relocs_end;
        prev_rip_relocs++, new_rip_relocs++) {

        rip_reloc_map[&(*prev_rip_relocs)] = &(*new_rip_relocs);
    }
}

void fuku_code_holder::update_current_address(uint64_t virtual_address) {

    uint64_t _virtual_address = virtual_address;

    for (auto& inst : this->insts) {

        inst.set_current_address(_virtual_address);
        _virtual_address += inst.get_oplength();
    }
}

void   fuku_code_holder::update_source_insts() {

    source_insts.clear();

    for (auto& inst : insts) {     
        if (inst.has_source_address()) {
            source_insts.push_back(&inst);
        }
    }

    std::sort(source_insts.begin(), source_insts.end(), [&, this](const fuku_inst * l_line, const fuku_inst * r_line) {
        return l_line->get_source_address() < r_line->get_source_address();
    });
}


fuku_code_label* fuku_code_holder::create_label(const fuku_code_label& label) {

    if (label.has_linked_instruction &&
        label.inst->get_label()) {

        return label.inst->get_label();
    }

    this->labels.push_back(label);

    return &this->labels.back();
}

fuku_relocation* fuku_code_holder::create_relocation(const fuku_relocation& reloc) {

    if (this->available_relocations.size()) {

        auto aval_reloc = this->available_relocations[this->available_relocations.size() - 1];

        this->available_relocations.erase(
            this->available_relocations.begin() + 
            this->available_relocations.size() - 1
        );

        *aval_reloc = reloc;
        return aval_reloc;
    }
    else {
     
        this->relocations.push_back(reloc);

        return &this->relocations.back();
    }
}

fuku_rip_relocation* fuku_code_holder::create_rip_relocation(const fuku_rip_relocation& rip_reloc) {


    if (this->available_rip_relocations.size()) {

        auto aval_reloc = this->available_rip_relocations[this->available_rip_relocations.size() - 1];

        this->available_rip_relocations.erase(
            this->available_rip_relocations.begin() +
            this->available_rip_relocations.size() - 1
        );

        *aval_reloc = rip_reloc;
        return aval_reloc;
    }
    else {

        this->rip_relocations.push_back(rip_reloc);

        return &this->rip_relocations.back();
    }
}

void  fuku_code_holder::release_relocation(fuku_relocation* reloc) {
   
    reloc->label = 0;
    available_relocations.push_back(reloc);
}

void  fuku_code_holder::release_rip_relocation(fuku_rip_relocation* reloc) {

    reloc->label = 0;
    available_rip_relocations.push_back(reloc);
}

fuku_inst& fuku_code_holder::add_inst() {

    insts.push_back(fuku_inst());

    return insts.back();
}

fuku_code_holder& fuku_code_holder::add_inst(const fuku_inst& inst) {

    insts.push_back(inst);

    return *this;
}

void fuku_code_holder::clear() {
    this->labels.clear();
    this->relocations.clear();
    this->rip_relocations.clear();
    this->source_insts.clear();
    this->insts.clear();
    this->available_relocations.clear();
    this->available_rip_relocations.clear();
}



void fuku_code_holder::set_arch(fuku_assambler_arch arch) {
    this->arch = arch;
}

fuku_assambler_arch fuku_code_holder::get_arch() const {
    return this->arch;
}


size_t fuku_code_holder::get_labels_count() const {
    return labels.size();
}

size_t fuku_code_holder::get_relocations_count() const {
    return this->relocations.size();
}

size_t fuku_code_holder::get_rip_relocations_count() const {
    return this->rip_relocations.size();
}

const std::list<fuku_code_label>& fuku_code_holder::get_labels() const {
    return this->labels;
}

std::list<fuku_code_label>& fuku_code_holder::get_labels() {
    return this->labels;
}

std::list<fuku_relocation>& fuku_code_holder::get_relocations() {
    return this->relocations;
}

const std::list<fuku_relocation>& fuku_code_holder::get_relocations() const {
    return this->relocations;
}

std::list<fuku_rip_relocation>& fuku_code_holder::get_rip_relocations() {
    return this->rip_relocations;
}

const std::list<fuku_rip_relocation>& fuku_code_holder::get_rip_relocations() const {
    return this->rip_relocations;
}

inststorage& fuku_code_holder::get_insts() {
    return this->insts;
}

const inststorage& fuku_code_holder::get_insts() const {
    return this->insts;
}

const std::vector<fuku_inst*>& fuku_code_holder::get_source_insts() const {
    return this->source_insts;
}



fuku_inst* fuku_code_holder::get_source_inst_range(uint64_t virtual_address) {


    if (source_insts.size()) {

        if (source_insts[0]->get_source_address() <= virtual_address &&

            (source_insts[source_insts.size() - 1]->get_source_address() +
                source_insts[source_insts.size() - 1]->get_oplength()) >= virtual_address) {

            size_t left = 0;
            size_t right = source_insts.size();
            size_t mid = 0;

            while (left < right) {
                mid = left + (right - left) / 2;

                if (source_insts[mid]->get_source_address() <= virtual_address &&
                    source_insts[mid]->get_source_address() + source_insts[mid]->get_oplength() > virtual_address) {

                    return source_insts[mid];
                }
                else if (source_insts[mid]->get_source_address() > virtual_address) {
                    right = mid;
                }
                else {
                    left = mid + 1;
                }
            }

        }
    }

    return 0;
}

fuku_inst* fuku_code_holder::get_source_inst_direct(uint64_t virtual_address) {

    if (source_insts.size()) {

        if (source_insts[0]->get_source_address() <= virtual_address &&
            source_insts[source_insts.size() - 1]->get_source_address() >= virtual_address) {

            size_t left = 0;
            size_t right = source_insts.size();
            size_t mid = 0;

            while (left < right) {
                mid = left + (right - left) / 2;

                if (source_insts[mid]->get_source_address() == virtual_address) {
                    return source_insts[mid];
                }
                else if (source_insts[mid]->get_source_address() > virtual_address) {
                    right = mid;
                }
                else {
                    left = mid + 1;
                }
            }

        }
    }

    return 0;
}

bool fuku_code_holder::finalize_code(
    std::map<uint64_t, uint64_t>* associations,
    std::vector<fuku_image_relocation>* relocations) {


    if (associations) { associations->clear(); }
    if (relocations) { relocations->clear(); }


    for (auto &inst : this->insts) {


        if (associations) {

            if (inst.has_source_address()) {
                (*associations)[inst.get_source_address()] = inst.get_current_address();
            }
        }
        

        if (inst.get_disp_reloc()) {


            if (inst.is_used_disp_reloc()) {

                if (this->arch == fuku_assambler_arch::FUKU_ASSAMBLER_ARCH_X86) {

                    if (inst.get_disp_reloc()->label->has_linked_instruction) {

                        *(uint32_t*)(&inst.get_opcode()[inst.get_disp_reloc()->offset]) =
                            (uint32_t)inst.get_disp_reloc()->label->inst->get_current_address();
                    }
                    else {

                        *(uint32_t*)(&inst.get_opcode()[inst.get_disp_reloc()->offset]) =
                            (uint32_t)inst.get_disp_reloc()->label->address;
                    }
                }
                else {

                    if (inst.get_disp_reloc()->label->has_linked_instruction) {

                        *(uint64_t*)(&inst.get_opcode()[inst.get_disp_reloc()->offset]) =
                            inst.get_disp_reloc()->label->inst->get_current_address();
                    }
                    else {

                        *(uint64_t*)(&inst.get_opcode()[inst.get_disp_reloc()->offset]) =
                            inst.get_disp_reloc()->label->address;
                    }
                }


                if (relocations) {

                    relocations->push_back({ 
                        inst.get_disp_reloc()->reloc_id,
                        (inst.get_current_address() + inst.get_disp_reloc()->offset)
                    });
                }
            }
            else {

                if (inst.get_rip_reloc()->label->has_linked_instruction) {

                    *(uint32_t*)(&inst.get_opcode()[inst.get_rip_reloc()->offset]) =
                        uint32_t(inst.get_rip_reloc()->label->inst->get_current_address() - inst.get_current_address() - inst.get_oplength());
                }
                else {

                    *(uint32_t*)(&inst.get_opcode()[inst.get_rip_reloc()->offset]) =
                        uint32_t(inst.get_rip_reloc()->label->address - inst.get_current_address() - inst.get_oplength());
                }
            }
        }

        if (inst.get_imm_reloc()) {

            if (this->arch == fuku_assambler_arch::FUKU_ASSAMBLER_ARCH_X86) {

                if (inst.get_imm_reloc()->label->has_linked_instruction) {

                    *(uint32_t*)(&inst.get_opcode()[inst.get_imm_reloc()->offset]) =
                        (uint32_t)inst.get_imm_reloc()->label->inst->get_current_address();
                }
                else {

                    *(uint32_t*)(&inst.get_opcode()[inst.get_imm_reloc()->offset]) =
                        (uint32_t)inst.get_imm_reloc()->label->address;
                }
            }
            else {

                if (inst.get_imm_reloc()->label->has_linked_instruction) {

                    *(uint64_t*)(&inst.get_opcode()[inst.get_imm_reloc()->offset]) =
                        inst.get_imm_reloc()->label->inst->get_current_address();
                }
                else {

                    *(uint64_t*)(&inst.get_opcode()[inst.get_imm_reloc()->offset]) =
                        inst.get_imm_reloc()->label->address;
                }
            }


            if (relocations) {

                relocations->push_back({ 
                    inst.get_imm_reloc()->reloc_id, 
                    (inst.get_current_address() + inst.get_imm_reloc()->offset) 
                });
            }
        }
    }

    return true;
}

std::vector<uint8_t> fuku_code_holder::dump_code() {

    std::vector<uint8_t> code_raw;
    size_t raw_caret_pos = 0;

    for (auto& inst : this->insts) {
        raw_caret_pos += inst.get_oplength();
    }

    {
        code_raw.resize(raw_caret_pos); raw_caret_pos = 0;

        for (auto& inst : this->insts) {

            memcpy(&code_raw[raw_caret_pos], inst.get_opcode(), inst.get_oplength());
            raw_caret_pos += inst.get_oplength();
        }
    }

    return code_raw;
}

bool fuku_code_holder::resolve_labels() {

    std::vector<std::list<fuku_code_label>::iterator> delete_labels;
    std::map<fuku_code_label*, fuku_code_label*> remap_labels;

    for (auto label_iter = labels.begin(); 
        label_iter != labels.end(); label_iter++) { //re associate external labels

        if (!label_iter->has_linked_instruction) {

            fuku_inst* inst_dst = get_source_inst_direct(label_iter->address);

            if (inst_dst) {

                if (inst_dst->get_label()) {
               
                    delete_labels.push_back(label_iter);
                    remap_labels[&(*label_iter)] = inst_dst->get_label();
                }
                else {

                    label_iter->set_inst(inst_dst);
                    inst_dst->set_label(&(*label_iter));
                }
            }
        }
    }


    if (remap_labels.size()) {

        for (auto& reloc : relocations) {

            if (reloc.label) {

                auto reloc_map_label = remap_labels.find(reloc.label);

                if (reloc_map_label != remap_labels.end()) {
                    reloc.label = reloc_map_label->second;
                }
            }
        }
        for (auto& reloc : rip_relocations) {

            if (reloc.label) {

                auto reloc_map_label = remap_labels.find(reloc.label);

                if (reloc_map_label != remap_labels.end()) {
                    reloc.label = reloc_map_label->second;
                }
            }
        }

        while(delete_labels.size()) {
            this->labels.erase(delete_labels[delete_labels.size() - 1]);
            delete_labels.erase(delete_labels.begin() + (delete_labels.size() - 1));
        }
    }

    return true;
}

bool fuku_code_holder::merge_code(const fuku_code_holder& code_holder) {

    if (!code_holder.insts.size()) { return true; }

    if (this->insts.size()) {

        size_t inst_count = insts.size();
        size_t label_count = labels.size();
        size_t reloc_count = relocations.size();
        size_t rip_reloc_count = rip_relocations.size();
        size_t cache_reloc_count = available_relocations.size();
        size_t cache_rip_reloc_count = available_rip_relocations.size();

        insts.insert(
            insts.end(),
            code_holder.insts.begin(), code_holder.insts.end()
        );

        labels.insert(
            labels.end(),
            code_holder.labels.begin(), code_holder.labels.end()
        );

        relocations.insert(
            relocations.end(),
            code_holder.relocations.begin(), code_holder.relocations.end()
        );

        rip_relocations.insert(
            rip_relocations.end(),
            code_holder.rip_relocations.begin(), code_holder.rip_relocations.end()
        );

        auto inst_iter = insts.begin();
        std::advance(inst_iter, inst_count);

        auto label_iter = labels.begin();
        std::advance(label_iter, label_count);

        auto reloc_iter = relocations.begin();
        std::advance(reloc_iter, reloc_count);

        auto rip_reloc_iter = rip_relocations.begin();
        std::advance(rip_reloc_iter, rip_reloc_count);


        if (code_holder.get_labels_count()) {

            std::vector<fuku_code_label*> label_map;
            std::map<const fuku_relocation*, fuku_relocation*> reloc_map;
            std::map<const fuku_rip_relocation*, fuku_rip_relocation*> rip_reloc_map;

            create_relation_map(
                code_holder.labels.begin(), label_iter,
                code_holder.labels.end(),   this->labels.end(),

                code_holder.relocations.begin(), reloc_iter,
                code_holder.relocations.end(), this->relocations.end(),

                code_holder.rip_relocations.begin(), rip_reloc_iter,
                code_holder.rip_relocations.end(), this->rip_relocations.end(),

                label_map,
                reloc_map,
                rip_reloc_map
            );

            for (; inst_iter != insts.end(); ++inst_iter) { //fix new items label idxs

                auto& inst = *inst_iter;

                if (inst.get_label()) {
                    auto label = label_map[inst.get_label()->label_id];

                    inst.set_label(label);
                    label->set_inst(&inst);
                }

                if (inst.get_imm_reloc()) {

                    auto prev_imm_reloc = inst.get_imm_reloc();

                    if (!prev_imm_reloc->label) {
                        continue;
                    }

                    auto new_imm_reloc = reloc_map[prev_imm_reloc];

                    new_imm_reloc->label = label_map[prev_imm_reloc->label->label_id];

                    inst.set_imm_reloc(new_imm_reloc);
                }

                if (inst.get_disp_reloc()) {

                    if (inst.is_used_disp_reloc()) {

                        auto prev_disp_reloc = inst.get_disp_reloc();

                        if (!prev_disp_reloc->label) {
                            continue;
                        }

                        auto new_disp_reloc = reloc_map[prev_disp_reloc];

                        new_disp_reloc->label = label_map[prev_disp_reloc->label->label_id];

                        inst.set_disp_reloc(new_disp_reloc);
                    }
                    else { //else rip reloc

                        auto prev_rip_reloc = inst.get_rip_reloc();

                        if (!prev_rip_reloc->label) {
                            continue;
                        }

                        auto new_rip_reloc = rip_reloc_map[prev_rip_reloc];

                        new_rip_reloc->label = label_map[prev_rip_reloc->label->label_id];

                        inst.set_rip_reloc(new_rip_reloc);
                    }
                }
            }

            { //cache update

                for (size_t idx = cache_reloc_count; idx < this->available_relocations.size(); idx++) {

                    this->available_relocations[idx] = reloc_map[this->available_relocations[idx]];
                }

                for (size_t idx = cache_rip_reloc_count; idx < this->available_rip_relocations.size(); idx++) {

                    this->available_rip_relocations[idx] = rip_reloc_map[this->available_rip_relocations[idx]];
                }
            }

        }

        update_source_insts();
        resolve_labels();
    }
    else {
        this->operator=(code_holder);
    }

    return true;
}

bool fuku_code_holder::splice_code(fuku_code_holder& code_holder) {

    if (!code_holder.insts.size()) { return true; }

    if (insts.size()) {

        this->labels.splice(this->labels.end(), code_holder.labels);
        this->relocations.splice(this->relocations.end(), code_holder.relocations);
        this->rip_relocations.splice(this->rip_relocations.end(), code_holder.rip_relocations);
        this->insts.splice(this->insts.end(), code_holder.insts);

        this->available_relocations.insert(this->available_relocations.end(), 
            code_holder.available_relocations.begin(), code_holder.available_relocations.end());

        this->available_rip_relocations.insert(this->available_rip_relocations.end(),
            code_holder.available_rip_relocations.begin(), code_holder.available_rip_relocations.end());

   
        update_source_insts();
        resolve_labels();

        code_holder.clear();
    }
    else {
        
    clear();

        this->arch = code_holder.arch;
        this->labels.swap(code_holder.labels);
        this->relocations.swap(code_holder.relocations);
        this->rip_relocations.swap(code_holder.rip_relocations);
        this->available_relocations.swap(code_holder.available_relocations);
        this->available_rip_relocations.swap(code_holder.available_rip_relocations);
        this->source_insts.swap(code_holder.source_insts);
        this->insts.swap(code_holder.insts);
    }

    return true;
}