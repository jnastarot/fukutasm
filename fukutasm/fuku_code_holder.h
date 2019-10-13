#pragma once


struct fuku_image_relocation {
    uint32_t relocation_id;
    uint64_t virtual_address;
};



class fuku_code_holder {
    fuku_assambler_arch arch;

    std::list<fuku_code_label>     labels;
    std::list<fuku_relocation>     relocations;
    std::list<fuku_rip_relocation> rip_relocations;

    std::vector<fuku_relocation*> available_relocations;
    std::vector<fuku_rip_relocation*> available_rip_relocations;

    std::vector<fuku_inst*> source_insts;

    inststorage insts;

    void create_relation_map(
        std::list<fuku_code_label>::const_iterator prev_labels, std::list<fuku_code_label>::iterator new_labels,
        std::list<fuku_code_label>::const_iterator prev_labels_end, std::list<fuku_code_label>::iterator new_labels_end,

        std::list<fuku_relocation>::const_iterator prev_relocs, std::list<fuku_relocation>::iterator new_relocs,
        std::list<fuku_relocation>::const_iterator prev_relocs_end, std::list<fuku_relocation>::iterator new_relocs_end,

        std::list<fuku_rip_relocation>::const_iterator prev_rip_relocs, std::list<fuku_rip_relocation>::iterator new_rip_relocs,
        std::list<fuku_rip_relocation>::const_iterator prev_rip_relocs_end, std::list<fuku_rip_relocation>::iterator new_rip_relocs_end,

        std::vector<fuku_code_label*>& label_map,
        std::map<const fuku_relocation*, fuku_relocation*>& reloc_map,
        std::map<const fuku_rip_relocation*, fuku_rip_relocation*>& rip_reloc_map
    ) const;

public:

    fuku_code_holder();
    fuku_code_holder(fuku_assambler_arch arch);
    fuku_code_holder(const fuku_code_holder& code_holder);
    ~fuku_code_holder();


    fuku_code_holder& operator=(const fuku_code_holder& code_holder);
public:

    void set_arch(fuku_assambler_arch arch);

    fuku_code_label* create_label(const fuku_code_label& label);
    fuku_relocation* create_relocation(const fuku_relocation& reloc);
    fuku_rip_relocation* create_rip_relocation(const fuku_rip_relocation& rip_reloc);

    void release_relocation(fuku_relocation* reloc);
    void release_rip_relocation(fuku_rip_relocation* reloc);


    fuku_inst& add_inst();

    fuku_code_holder& add_inst(const fuku_inst& inst);

    void clear();
public:

    bool finalize_code(std::map<uint64_t, uint64_t>* associations,
        std::vector<fuku_image_relocation>* relocations);

    std::vector<uint8_t> dump_code();

public:

    bool merge_code(const fuku_code_holder& code_holder);
    bool splice_code(fuku_code_holder& code_holder);

    bool resolve_labels();

public:

    fuku_inst* get_source_inst_direct(uint64_t virtual_address);
    fuku_inst* get_source_inst_range(uint64_t virtual_address);

    void update_current_address(uint64_t virtual_address);
    void update_source_insts();
public:

    fuku_assambler_arch get_arch() const;

    size_t get_labels_count() const;
    size_t get_relocations_count() const;
    size_t get_rip_relocations_count() const;

    std::list<fuku_code_label>& get_labels();
    const std::list<fuku_code_label>& get_labels() const;
    std::list<fuku_relocation>& get_relocations();
    const std::list<fuku_relocation>& get_relocations() const;
    std::list<fuku_rip_relocation>& get_rip_relocations();
    const std::list<fuku_rip_relocation>& get_rip_relocations() const;

    inststorage&  get_insts();
    const inststorage& get_insts() const;

    const std::vector<fuku_inst*>& get_source_insts() const;

};

