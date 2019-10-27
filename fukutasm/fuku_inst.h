#pragma once

class fuku_inst;

struct fuku_code_label;
struct fuku_relocation;
struct fuku_rip_relocation;


#pragma pack(push, 1)

struct fuku_code_label {

    uint32_t label_tag; //user related id
    size_t   label_id;  //internaly used id for cache

    union { //destination of label
        uint64_t   address;
        fuku_inst* inst;
    };

    //type of destination
    uint8_t has_linked_instruction;

    fuku_code_label();
    fuku_code_label& set_inst(fuku_inst* inst);
    fuku_code_label& set_address(uint64_t address);
};

struct fuku_relocation {

    uint32_t reloc_id; //user related id must not be a zero 

    uint8_t offset;

    fuku_code_label* label;

    fuku_relocation();
    fuku_relocation& set_label(fuku_code_label* label);
    fuku_relocation& set_offset(uint8_t  offset);
    fuku_relocation& set_id(uint32_t reloc_id);
};

struct fuku_rip_relocation {

    uint8_t offset;

    fuku_code_label* label;

    fuku_rip_relocation();
    fuku_rip_relocation& set_label(fuku_code_label* label);
    fuku_rip_relocation& set_offset(uint8_t  offset);
};

class fuku_inst {

    uint16_t id;

    uint8_t oplength;
    uint8_t opcode[16];

    uint64_t source_address;
    uint64_t current_address;

    fuku_code_label* label;

    fuku_relocation* imm_reloc;

    union {
        fuku_relocation* disp_reloc;
        fuku_rip_relocation* rip_reloc;
    };

    uint64_t cpu_flags;
    uint64_t cpu_registers;

    struct flag_union {
        uint16_t inst_used_disp   : 1; //disp or rip
        uint16_t inst_has_address : 1; //source_address is used
        uint16_t inst_flags       : 14;
   
    } flags;

public:
    fuku_inst();
    fuku_inst(const fuku_inst& line);
    ~fuku_inst();

    fuku_inst& operator=(const fuku_inst& line);
public:
    fuku_inst& set_id(uint16_t id);
    fuku_inst& set_opcode(const uint8_t* opcode, uint8_t lenght);

    fuku_inst& set_source_address(uint64_t address);
    fuku_inst& set_current_address(uint64_t address);

    fuku_inst& set_label(fuku_code_label* label);
    fuku_inst& set_disp_reloc(fuku_relocation* reloc);
    fuku_inst& set_imm_reloc(fuku_relocation* reloc);
    fuku_inst& set_rip_reloc(fuku_rip_relocation* reloc);

    fuku_inst& set_inst_flags(uint32_t inst_flags);

    fuku_inst& set_cpu_flags(uint64_t flags);
    fuku_inst& set_cpu_registers(uint64_t regs);

    fuku_inst& invalidate_source_address();

public:
    uint16_t get_id() const;

    const uint8_t* get_opcode() const;
    uint8_t  get_oplength() const;

    uint64_t get_source_address() const;
    uint64_t get_current_address() const;

    fuku_code_label* get_label();
    const fuku_code_label* get_label() const;
    fuku_relocation* get_imm_reloc();
    const fuku_relocation* get_imm_reloc() const;
    fuku_relocation* get_disp_reloc();
    const fuku_relocation* get_disp_reloc() const;
    fuku_rip_relocation* get_rip_reloc();
    const fuku_rip_relocation* get_rip_reloc() const;

    uint16_t get_inst_flags() const;
    uint64_t get_cpu_flags() const;
    uint64_t get_cpu_registers() const;

    bool has_source_address() const;
    bool is_used_disp_reloc() const;

};

#pragma pack(pop)

typedef std::list<fuku_inst> inststorage;

uint8_t get_inst_prefixe_count(const fuku_inst& inst);