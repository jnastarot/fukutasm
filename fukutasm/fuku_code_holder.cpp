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
    this->lines = code_holder.lines;

    if (labels.size()) {
     
        std::vector<fuku_instruction* > labels_cache;
        labels_cache.resize(labels.size());

        for (auto& line : lines) {

            if (line.get_label_idx() != -1) {
                labels_cache[line.get_label_idx()] = &line;
            }
        }



        for (size_t label_idx = 0; label_idx < labels.size(); label_idx++) {

            if (labels[label_idx].has_linked_instruction) {
                labels[label_idx].instruction = labels_cache[label_idx];
            }
        }

    }


    return *this;
}

void   fuku_code_holder::update_virtual_address(uint64_t destination_virtual_address) {

    uint64_t _virtual_address = destination_virtual_address;

    for (auto& line : lines) {

        line.set_virtual_address(_virtual_address);
        _virtual_address += line.get_op_length();
    }
}

void   fuku_code_holder::update_origin_idxs() {

    original_lines.clear();

    for (auto& line : lines) {     
        if (line.get_source_virtual_address() != -1) {
            original_lines.push_back(&line);
        }
    }

    std::sort(original_lines.begin(), original_lines.end(), [&, this](const fuku_instruction * l_line, const fuku_instruction * r_line) {
        return l_line->get_source_virtual_address() < r_line->get_source_virtual_address();
    });
}

size_t fuku_code_holder::create_label(fuku_instruction* line) {

    if (line->get_label_idx() == -1) {

        line->set_label_idx(labels.size());
        
        fuku_code_label label;
        label.has_linked_instruction = 1;
        label.instruction = line;
        label.refs_count = 0;

        labels.push_back(label);
    }

    return line->get_label_idx();
}

size_t fuku_code_holder::create_label(uint64_t dst_address) {

    fuku_code_label label;
    label.has_linked_instruction = 0;
    label.dst_address = dst_address;
    label.refs_count = 0;

    labels.push_back(label);

    return labels.size() - 1;
}

size_t fuku_code_holder::create_relocation(uint8_t offset, uint64_t dst_address, uint32_t relocation_id) {

    if (available_relocations.size()) {
        size_t idx = available_relocations[0];
        available_relocations.erase(available_relocations.begin());

        relocations[idx] = { relocation_id , offset , create_label(dst_address) };

        labels[relocations[idx].label_idx].refs_count++;
        return idx;
    }
    else {
        relocations.push_back({ relocation_id , offset , create_label(dst_address) });

        labels[relocations[relocations.size()-1].label_idx].refs_count++;
        return relocations.size() - 1;
    }
}

size_t fuku_code_holder::create_relocation(uint8_t offset, fuku_instruction* line, uint32_t relocation_id) {

    if (available_relocations.size()) {
        size_t idx = available_relocations[0];
        available_relocations.erase(available_relocations.begin());

        relocations[idx] = { relocation_id , offset , create_label(line) };

        labels[relocations[idx].label_idx].refs_count++;
        return idx;
    }
    else {
        relocations.push_back({ relocation_id , offset , create_label(line) });

        labels[relocations[relocations.size() - 1].label_idx].refs_count++;
        return relocations.size() - 1;
    }
}

size_t fuku_code_holder::create_relocation_lb(uint8_t offset, size_t label_idx, uint32_t relocation_id) {

    if (available_relocations.size()) {
        size_t idx = available_relocations[0];
        available_relocations.erase(available_relocations.begin());

        relocations[idx] = { relocation_id , offset , label_idx };

        labels[label_idx].refs_count++;
        return idx;
    }
    else {
        relocations.push_back({ relocation_id , offset , label_idx });

        labels[label_idx].refs_count++;
        return relocations.size() - 1;
    }
}

size_t fuku_code_holder::create_relocation(const fuku_code_relocation& reloc) {

    if (available_relocations.size()) {
        size_t idx = available_relocations[0];
        available_relocations.erase(available_relocations.begin());

        relocations[idx] = reloc;

        labels[relocations[idx].label_idx].refs_count++;
        return idx;
    }
    else {
        relocations.push_back(reloc);

        labels[relocations[relocations.size() - 1].label_idx].refs_count++;
        return relocations.size() - 1;
    }
}

size_t fuku_code_holder::create_rip_relocation(uint8_t offset, uint64_t dst_address) {

    if (available_rip_relocations.size()) {
        size_t idx = available_rip_relocations[0];
        available_rip_relocations.erase(available_rip_relocations.begin());

        rip_relocations[idx] = { offset , create_label(dst_address) };

        labels[rip_relocations[idx].label_idx].refs_count++;
        return idx;
    }
    else {
        rip_relocations.push_back({ offset , create_label(dst_address) });

        labels[rip_relocations[rip_relocations.size() - 1].label_idx].refs_count++;
        return rip_relocations.size() - 1;
    }
}

size_t fuku_code_holder::create_rip_relocation(uint8_t offset, fuku_instruction* line) {

    if (available_rip_relocations.size()) {
        size_t idx = available_rip_relocations[0];
        available_rip_relocations.erase(available_rip_relocations.begin());

        rip_relocations[idx] = { offset , create_label(line) };

        labels[rip_relocations[idx].label_idx].refs_count++;
        return idx;
    }
    else {
        rip_relocations.push_back({ offset , create_label(line) });

        labels[rip_relocations[rip_relocations.size() - 1].label_idx].refs_count++;
        return rip_relocations.size() - 1;
    }
}

size_t fuku_code_holder::create_rip_relocation_lb(uint8_t offset, size_t label_idx) {

    if (available_rip_relocations.size()) {
        size_t idx = available_rip_relocations[0];
        available_rip_relocations.erase(available_rip_relocations.begin());

        rip_relocations[idx] = { offset , label_idx };

        labels[label_idx].refs_count++;
        return idx;
    }
    else {
        rip_relocations.push_back({ offset , label_idx });

        labels[label_idx].refs_count++;
        return rip_relocations.size() - 1;
    }
}

size_t fuku_code_holder::create_rip_relocation(const fuku_code_rip_relocation& rip_reloc) {

    if (available_rip_relocations.size()) {
        size_t idx = available_rip_relocations[0];
        available_rip_relocations.erase(available_rip_relocations.begin());

        rip_relocations[idx] = rip_reloc;

        labels[rip_relocations[idx].label_idx].refs_count++;
        return idx;
    }
    else {
        rip_relocations.push_back(rip_reloc);

        labels[rip_relocations[rip_relocations.size() - 1].label_idx].refs_count++;
        return rip_relocations.size() - 1;
    }
}

void  fuku_code_holder::delete_relocation(size_t idx) {
    
    if (idx < relocations.size() ) {

        if (relocations.size() - 1 == idx) {
            labels[relocations[idx].label_idx].refs_count--;
            relocations.erase(relocations.begin() + idx);
        }
        else {
            labels[relocations[idx].label_idx].refs_count--;

            memset(&relocations[idx], 0 , sizeof(fuku_code_relocation));
            available_relocations.push_back(idx);
        }
    }
}

void  fuku_code_holder::delete_rip_relocation(size_t idx) {

    if (idx < rip_relocations.size()) {

        if (rip_relocations.size() - 1 == idx) {
            labels[rip_relocations[idx].label_idx].refs_count--;
            rip_relocations.erase(rip_relocations.begin() + idx);
        }
        else {
            labels[rip_relocations[idx].label_idx].refs_count--;

            memset(&rip_relocations[idx], 0, sizeof(fuku_code_rip_relocation));
            available_rip_relocations.push_back(idx);
        }
    }
}

fuku_instruction& fuku_code_holder::add_line() {
    lines.push_back(fuku_instruction());

    return lines.back();
}


void fuku_code_holder::clear() {
    this->labels.clear();
    this->relocations.clear();
    this->rip_relocations.clear();
    this->original_lines.clear();
    this->lines.clear();
}

fuku_instruction * fuku_code_holder::get_range_line_by_source_va(uint64_t virtual_address) {


    if (original_lines.size()) {

        if (original_lines[0]->get_source_virtual_address() <= virtual_address &&

           (original_lines[original_lines.size() - 1]->get_source_virtual_address() + 
               original_lines[original_lines.size() - 1]->get_op_length()) >= virtual_address) {

            size_t left = 0;
            size_t right = original_lines.size();
            size_t mid = 0;

            while (left < right) {
                mid = left + (right - left) / 2;

                if (original_lines[mid]->get_source_virtual_address() <= virtual_address &&
                    original_lines[mid]->get_source_virtual_address() + original_lines[mid]->get_op_length() > virtual_address) {

                    return original_lines[mid];
                }
                else if (original_lines[mid]->get_source_virtual_address() > virtual_address) {
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

fuku_instruction * fuku_code_holder::get_direct_line_by_source_va(uint64_t virtual_address) {

    if (original_lines.size()) {

        if (original_lines[0]->get_source_virtual_address() <= virtual_address &&
            original_lines[original_lines.size() - 1]->get_source_virtual_address() >= virtual_address) {

            size_t left = 0;
            size_t right = original_lines.size();
            size_t mid = 0;

            while (left < right) {
                mid = left + (right - left) / 2;

                if (original_lines[mid]->get_source_virtual_address() == virtual_address) {
                    return original_lines[mid];
                }
                else if (original_lines[mid]->get_source_virtual_address() > virtual_address) {
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


void fuku_code_holder::set_arch(fuku_assambler_arch arch) {
    this->arch = arch;
}


void fuku_code_holder::set_labels(const std::vector<fuku_code_label>& labels) {
    this->labels = labels;
}

void fuku_code_holder::set_relocations(const std::vector<fuku_code_relocation>& relocs) {
    this->relocations = relocs;
}

void fuku_code_holder::set_rip_relocations(const std::vector<fuku_code_rip_relocation>& rip_relocs) {
    this->rip_relocations = rip_relocs;
}

void fuku_code_holder::set_available_relocations(const std::vector<size_t>& relocs) {
    this->available_relocations = relocs;
}

void fuku_code_holder::set_available_rip_relocations(const std::vector<size_t>& rip_relocs) {
    this->available_rip_relocations = rip_relocs;
}

void fuku_code_holder::set_original_lines_idxs(const std::vector<fuku_instruction *>& original_lines) {
    this->original_lines = original_lines;
}

void fuku_code_holder::set_lines(const linestorage& lines) {
    this->lines = lines;
}

std::vector<fuku_code_label>& fuku_code_holder::get_labels() {
    return this->labels;
}

std::vector<fuku_code_relocation>& fuku_code_holder::get_relocations() {
    return this->relocations;
}

std::vector<fuku_code_rip_relocation>& fuku_code_holder::get_rip_relocations() {
    return this->rip_relocations;
}

std::vector<size_t>& fuku_code_holder::get_available_relocations() {
    return this->available_relocations;
}

std::vector<size_t>& fuku_code_holder::get_available_rip_relocations() {
    return this->available_rip_relocations;
}

std::vector<fuku_instruction *>& fuku_code_holder::get_original_lines() {
    return this->original_lines;
}

linestorage&  fuku_code_holder::get_lines() {
    return this->lines;
}


fuku_assambler_arch fuku_code_holder::get_arch() const {
    return this->arch;
}

size_t fuku_code_holder::get_labels_count() const {
    return labels.size();
}

const std::vector<fuku_code_label>& fuku_code_holder::get_labels() const {
    return this->labels;
}

const std::vector<fuku_code_relocation>& fuku_code_holder::get_relocations() const {
    return this->relocations;
}

const std::vector<fuku_code_rip_relocation>& fuku_code_holder::get_rip_relocations() const {
    return this->rip_relocations;
}

const std::vector<size_t>& fuku_code_holder::get_available_relocations() const {
    return this->available_relocations;
}

const std::vector<size_t>& fuku_code_holder::get_available_rip_relocations() const {
    return this->available_rip_relocations;
}

const std::vector<fuku_instruction *>& fuku_code_holder::get_original_lines() const {
    return this->original_lines;
}

const linestorage&  fuku_code_holder::get_lines() const {
    return this->lines;
}

std::vector<uint8_t> finalize_code(fuku_code_holder&  code_holder,
    std::vector<fuku_code_association>* associations,
    std::vector<fuku_image_relocation>* relocations) {


    if (associations) { associations->clear(); }
    if (relocations) { relocations->clear(); }

    fuku_assambler_arch arch = code_holder.get_arch();

    auto& labels = code_holder.get_labels();
    auto& relocs = code_holder.get_relocations();
    auto& rip_relocs = code_holder.get_rip_relocations();

    std::vector<uint8_t> lines_raw;
    size_t raw_caret_pos = 0;

    for (auto &line : code_holder.get_lines()) {


        if (associations) {
            if (line.get_source_virtual_address() != -1) {
                associations->push_back({ line.get_source_virtual_address(), line.get_virtual_address() });
            }
        }

        if (line.get_relocation_disp_idx() != -1) {

            auto& reloc = relocs[line.get_relocation_disp_idx()];
            auto& reloc_label = labels[reloc.label_idx];

            if (arch == fuku_assambler_arch::FUKU_ASSAMBLER_ARCH_X86) {

                if (reloc_label.has_linked_instruction) {
                    *(uint32_t*)(&line.get_op_code()[reloc.offset]) = (uint32_t)reloc_label.instruction->get_virtual_address();
                }
                else {
                    *(uint32_t*)(&line.get_op_code()[reloc.offset]) = (uint32_t)reloc_label.dst_address;
                }
            }
            else {

                if (reloc_label.has_linked_instruction) {
                    *(uint64_t*)(&line.get_op_code()[reloc.offset]) = reloc_label.instruction->get_virtual_address();
                }
                else {
                    *(uint64_t*)(&line.get_op_code()[reloc.offset]) = reloc_label.dst_address;
                }
            }


            if (relocations) {
                relocations->push_back({ reloc.relocation_id, (line.get_virtual_address() + reloc.offset) });
            }
        }

        if (line.get_relocation_imm_idx() != -1) {

            auto& reloc = relocs[line.get_relocation_imm_idx()];
            auto& reloc_label = labels[reloc.label_idx];

            if (arch == fuku_assambler_arch::FUKU_ASSAMBLER_ARCH_X86) {

                if (reloc_label.has_linked_instruction) {
                    *(uint32_t*)(&line.get_op_code()[reloc.offset]) = (uint32_t)reloc_label.instruction->get_virtual_address();
                }
                else {
                    *(uint32_t*)(&line.get_op_code()[reloc.offset]) = (uint32_t)reloc_label.dst_address;
                }
            }
            else {

                if (reloc_label.has_linked_instruction) {
                    *(uint64_t*)(&line.get_op_code()[reloc.offset]) = reloc_label.instruction->get_virtual_address();
                }
                else {
                    *(uint64_t*)(&line.get_op_code()[reloc.offset]) = reloc_label.dst_address;
                }
            }


            if (relocations) {
                relocations->push_back({ reloc.relocation_id, (line.get_virtual_address() + reloc.offset) });
            }
        }

        if (line.get_rip_relocation_idx() != -1) {

            auto& reloc = rip_relocs[line.get_rip_relocation_idx()];
            auto& reloc_label = labels[reloc.label_idx];
            

            if (reloc_label.has_linked_instruction) {
                *(uint32_t*)(&line.get_op_code()[reloc.offset]) =
                    uint32_t(reloc_label.instruction->get_virtual_address() - line.get_virtual_address() - line.get_op_length());
            }
            else {
                *(uint32_t*)(&line.get_op_code()[reloc.offset]) =
                    uint32_t(reloc_label.dst_address - line.get_virtual_address() - line.get_op_length());
            }
        }

        raw_caret_pos += line.get_op_length();
    }

    {
        lines_raw.resize(raw_caret_pos); raw_caret_pos = 0;

        for (auto &line : code_holder.get_lines()) {

            memcpy(&lines_raw.data()[raw_caret_pos], line.get_op_code(), line.get_op_length());
            raw_caret_pos += line.get_op_length();
        }
    }
    
    if (associations) {
        std::sort(associations->begin(), associations->end(), [](const fuku_code_association& l_assoc, const fuku_code_association& r_assoc) {
            return l_assoc.original_virtual_address < r_assoc.original_virtual_address;
        });
    }

    return lines_raw;
}

bool fuku_code_holder::merge_labels() {

    struct label_item {
        size_t new_label_idx;
        fuku_code_label label;
    };

    std::vector<label_item> new_labels_chain;
    new_labels_chain.resize(labels.size());

    bool has_incorrect = false;

    for (size_t label_idx = 0; label_idx < labels.size(); label_idx++) { //associate labels

        auto& label = labels[label_idx];

        if (label.has_linked_instruction) {
            new_labels_chain[label_idx] = { label_idx, label.has_linked_instruction, label.refs_count, label.dst_address };
        }
        else {

            fuku_instruction * line = get_direct_line_by_source_va(label.dst_address);

            if (line) {
                has_incorrect = true;

                if (line->get_label_idx() == -1) {
                    line->set_label_idx(label_idx);
                }

                new_labels_chain[label_idx] = { line->get_label_idx(), 1, label.refs_count, (uint64_t)line };
            }
            else {

                new_labels_chain[label_idx] = { label_idx, label.has_linked_instruction, label.refs_count, label.dst_address };
            }
        }
    }


    {
        std::vector<fuku_code_label> new_labels;
        std::vector<size_t> label_new_map;

        new_labels.reserve(new_labels_chain.size());
        label_new_map.resize(labels.size());


        for (size_t label_idx = 0; label_idx < new_labels_chain.size(); label_idx++) {

            auto& label_chain = new_labels_chain[label_idx];

            if (label_chain.new_label_idx == label_idx) {

                label_new_map[label_idx] = new_labels.size();
                new_labels.push_back(label_chain.label);
            }
            else {
                has_incorrect = true;
            }
        }

        if (has_incorrect) {

            {   //associate old labels map with new labels map

                for (size_t label_idx = 0; label_idx < new_labels_chain.size(); label_idx++) {

                    auto& label_chain = new_labels_chain[label_idx];

                    if (label_chain.new_label_idx != label_idx) {
                        label_new_map[label_idx] = label_new_map[label_chain.new_label_idx];
                        new_labels[label_new_map[label_idx]].refs_count++;
                    }
                }
            }

            {   //reset labels idx to new vector idxs

                for (auto& line : lines) {

                    if (line.get_label_idx() != -1) {
                        line.set_label_idx(label_new_map[line.get_label_idx()]);
                    }
                }

                for (auto& reloc : relocations) {
                    reloc.label_idx = label_new_map[reloc.label_idx];
                }
                for (auto& rip_reloc : rip_relocations) {
                    rip_reloc.label_idx = label_new_map[rip_reloc.label_idx];
                }
            }
            
            this->labels = std::move(new_labels);
        }
    }

    return true;
}

bool fuku_code_holder::merge_code(const fuku_code_holder& code_holder) {

    if (!code_holder.get_lines().size()) { return true; }

    if (lines.size()) {

        linestorage& src_lines = lines;
        size_t src_size = src_lines.size();


        src_lines.insert(
            src_lines.end(),
            code_holder.get_lines().begin(), code_holder.get_lines().end()
        );



        auto src_iter = src_lines.begin();
        std::advance(src_iter, src_size);

        size_t label_count = labels.size();
        size_t reloc_count = relocations.size();
        size_t rip_reloc_count = rip_relocations.size();


        if (code_holder.get_labels_count()) {

            std::vector<fuku_instruction* > labels_cache;
            labels_cache.resize(code_holder.get_labels_count());

            for (; src_iter != src_lines.end(); ++src_iter) { //fix new items label idxs

                if (src_iter->get_label_idx() != -1) {

                    labels_cache[src_iter->get_label_idx()] = (&(*src_iter));

                    if (label_count) {
                        src_iter->set_label_idx(label_count + src_iter->get_label_idx());
                    }
                }

                if (label_count) {

                    if (src_iter->get_relocation_disp_idx() != -1) {
                        src_iter->set_relocation_disp_idx(reloc_count + src_iter->get_relocation_disp_idx());
                    }

                    if (src_iter->get_relocation_imm_idx() != -1) {
                        src_iter->set_relocation_imm_idx(reloc_count + src_iter->get_relocation_imm_idx());
                    }

                    if (src_iter->get_rip_relocation_idx() != -1) {
                        src_iter->set_rip_relocation_idx(rip_reloc_count + src_iter->get_rip_relocation_idx());
                    }
                }
            }

            if (label_count || code_holder.get_labels().size()) { //fix new items label idxs

                auto& src_relocs = code_holder.get_relocations();
                auto& src_rip_relocs = code_holder.get_rip_relocations();

                if (src_relocs.size()) {
                    size_t current_idx = relocations.size();
                    relocations.insert(relocations.end(), src_relocs.begin(), src_relocs.end());

                    for (; current_idx < relocations.size(); current_idx++) {
                        relocations[current_idx].label_idx += label_count;
                    }
                }

                if (src_rip_relocs.size()) {
                    size_t current_idx = rip_relocations.size();

                    rip_relocations.insert(rip_relocations.end(), src_rip_relocs.begin(), src_rip_relocs.end());

                    for (; current_idx < rip_relocations.size(); current_idx++) {
                        rip_relocations[current_idx].label_idx += label_count;
                    }
                }
            }

            { //cache update

                auto& src_cache_relocs = code_holder.get_available_relocations();
                auto& src_cache_rip_relocs = code_holder.get_available_rip_relocations();

                if (src_cache_relocs.size()) {
                    size_t delta_idx = available_relocations.size();

                    available_relocations.insert(available_relocations.end(), src_cache_relocs.begin(), src_cache_relocs.end());

                    for (size_t current_idx = delta_idx; current_idx < available_relocations.size(); current_idx++) {
                        available_relocations[current_idx] += delta_idx;
                    }
                }

                if (src_cache_rip_relocs.size()) {
                    size_t delta_idx = available_rip_relocations.size();

                    available_rip_relocations.insert(available_rip_relocations.end(), src_cache_rip_relocs.begin(), src_cache_rip_relocs.end());

                    for (size_t current_idx = delta_idx; current_idx < available_rip_relocations.size(); current_idx++) {
                        available_rip_relocations[current_idx] += delta_idx;
                    }
                }
            }

            auto& src_labels = code_holder.get_labels();

            labels.insert(labels.end(), src_labels.begin(), src_labels.end());

            for (size_t label_idx = label_count; label_idx < labels.size(); label_idx++) {

                if (labels[label_idx].has_linked_instruction) {
                    labels[label_idx].instruction = labels_cache[label_idx - label_count];
                }
            }
        }

        update_origin_idxs();
        merge_labels();
    }
    else {
        this->operator=(code_holder);

        update_origin_idxs();
        merge_labels();
    }

    return true;
}

bool fuku_code_holder::splice_code(fuku_code_holder& code_holder) {

    if (!code_holder.get_lines().size()) { return true; }

    if (lines.size()) {

        linestorage& src_lines = lines;
        size_t src_size = src_lines.size();

        src_lines.splice( src_lines.end(), code_holder.get_lines() );

        auto src_iter = src_lines.begin();
        std::advance(src_iter, src_size);

        size_t label_count = labels.size();
        size_t reloc_count = relocations.size();
        size_t rip_reloc_count = rip_relocations.size();


        if (code_holder.get_labels_count()) {

            std::vector<fuku_instruction* > labels_cache;
            labels_cache.resize(code_holder.get_labels_count());

            for (; src_iter != src_lines.end(); ++src_iter) { //fix new items label idxs

                if (src_iter->get_label_idx() != -1) {

                    labels_cache[src_iter->get_label_idx()] = (&(*src_iter));

                    if (label_count) {
                        src_iter->set_label_idx(label_count + src_iter->get_label_idx());
                    }
                }

                if (label_count) {

                    if (src_iter->get_relocation_disp_idx() != -1) {
                        src_iter->set_relocation_disp_idx(reloc_count + src_iter->get_relocation_disp_idx());
                    }

                    if (src_iter->get_relocation_imm_idx() != -1) {
                        src_iter->set_relocation_imm_idx(reloc_count + src_iter->get_relocation_imm_idx());
                    }

                    if (src_iter->get_rip_relocation_idx() != -1) {
                        src_iter->set_rip_relocation_idx(rip_reloc_count + src_iter->get_rip_relocation_idx());
                    }
                }
            }

            if (label_count || code_holder.get_labels().size()) { //fix new items label idxs

                auto& src_relocs = code_holder.get_relocations();
                auto& src_rip_relocs = code_holder.get_rip_relocations();

                if (src_relocs.size()) {
                    size_t current_idx = relocations.size();
                    relocations.insert(relocations.end(), src_relocs.begin(), src_relocs.end());

                    for (; current_idx < relocations.size(); current_idx++) {
                        relocations[current_idx].label_idx += label_count;
                    }
                }

                if (src_rip_relocs.size()) {
                    size_t current_idx = rip_relocations.size();

                    rip_relocations.insert(rip_relocations.end(), src_rip_relocs.begin(), src_rip_relocs.end());

                    for (; current_idx < rip_relocations.size(); current_idx++) {
                        rip_relocations[current_idx].label_idx += label_count;
                    }
                }
            }

            { //cache update

                auto& src_cache_relocs = code_holder.get_available_relocations();
                auto& src_cache_rip_relocs = code_holder.get_available_rip_relocations();

                if (src_cache_relocs.size()) {
                    size_t delta_idx = available_relocations.size();

                    available_relocations.insert(available_relocations.end(), src_cache_relocs.begin(), src_cache_relocs.end());

                    for (size_t current_idx = delta_idx; current_idx < available_relocations.size(); current_idx++) {
                        available_relocations[current_idx] += delta_idx;
                    }
                }

                if (src_cache_rip_relocs.size()) {
                    size_t delta_idx = available_rip_relocations.size();

                    available_rip_relocations.insert(available_rip_relocations.end(), src_cache_rip_relocs.begin(), src_cache_rip_relocs.end());

                    for (size_t current_idx = delta_idx; current_idx < available_rip_relocations.size(); current_idx++) {
                        available_rip_relocations[current_idx] += delta_idx;
                    }
                }
            }

            auto& src_labels = code_holder.get_labels();

            labels.insert(labels.end(), src_labels.begin(), src_labels.end());

            for (size_t label_idx = label_count; label_idx < labels.size(); label_idx++) {

                if (labels[label_idx].has_linked_instruction) {
                    labels[label_idx].instruction = labels_cache[label_idx - label_count];
                }
            }
        }

        update_origin_idxs();
        merge_labels();

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
        this->original_lines.swap(code_holder.original_lines);
        this->lines.swap(code_holder.lines);
    }

    return true;
}