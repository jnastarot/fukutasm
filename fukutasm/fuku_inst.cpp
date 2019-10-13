#include "stdafx.h"
#include "fuku_inst.h"

fuku_code_label::fuku_code_label() 
    : has_linked_instruction(0), inst(0), label_tag(0), label_id(0) {

}

fuku_code_label& fuku_code_label::set_inst(fuku_inst* inst) {

    this->has_linked_instruction = true;
    this->inst = inst;
    
    return *this;
}

fuku_code_label& fuku_code_label::set_address(uint64_t address) {

    this->has_linked_instruction = false;
    this->address = address;

    return *this;
}

fuku_relocation::fuku_relocation() 
    : reloc_id(0), offset(0), label(0) {}

fuku_relocation& fuku_relocation::set_label(fuku_code_label* label) {

    this->label = label;

    return *this;
}

fuku_relocation& fuku_relocation::set_offset(uint8_t  offset) {

    this->offset = offset;

    return *this;
}

fuku_relocation& fuku_relocation::set_id(uint32_t reloc_id) {

    this->reloc_id = reloc_id;

    return *this;
}


fuku_inst::fuku_inst()
    :id(-1), oplength(0),
    source_address(0), current_address(0),
    label(0),
    imm_reloc(0), disp_reloc(0),
    cpu_flags(0), cpu_registers(0),
    inst_flags(0) {

    inst_used_disp = 0;
    inst_has_address = 0;

    __stosb(opcode, 0, sizeof(opcode));
}

fuku_inst::fuku_inst(const fuku_inst& line) {
    this->operator=(line);
}


fuku_inst::~fuku_inst(){

}

fuku_inst& fuku_inst::operator=(const fuku_inst& line) {

    memcpy(this->opcode, line.opcode, line.oplength);
    this->oplength = line.oplength;

    this->id = line.id;

    this->source_address = line.source_address;
    this->current_address = line.current_address;
    this->label = line.label;
    this->imm_reloc = line.imm_reloc;
    this->disp_reloc = line.disp_reloc;
   // this->rip_reloc = line.rip_reloc; union
    this->cpu_flags = line.cpu_flags;
    this->cpu_registers = line.cpu_registers;
    this->inst_flags = line.inst_flags;
    this->inst_used_disp = line.inst_used_disp;
    this->inst_has_address = line.inst_has_address;

    return *this;
}

fuku_inst & fuku_inst::set_id(uint16_t id) {

    this->id = id;

    return *this;
}

fuku_inst&  fuku_inst::set_opcode(const uint8_t* _opcode, uint8_t _lenght) {

    memcpy(this->opcode, _opcode, _lenght);
    this->oplength = _lenght;

    return *this;
}

fuku_inst&  fuku_inst::set_source_address(uint64_t address) {

    this->source_address = address;
    this->inst_has_address = true;

    return *this;
}

fuku_inst&  fuku_inst::set_current_address(uint64_t address) {

    this->current_address = address;

    return *this;
}

fuku_inst& fuku_inst::set_label(fuku_code_label* label) {

    this->label = label;
    this->label->set_inst(this);

    return *this;
}

fuku_inst& fuku_inst::set_disp_reloc(fuku_relocation* reloc) {

    this->disp_reloc = reloc;
    this->inst_used_disp = true;

    return *this;
}

fuku_inst& fuku_inst::set_imm_reloc(fuku_relocation* reloc) {

    this->imm_reloc = reloc;

    return *this;
}

fuku_inst& fuku_inst::set_rip_reloc(fuku_rip_relocation* reloc) {

    this->rip_reloc = reloc;
    this->inst_used_disp = false;

    return *this;
}

fuku_inst& fuku_inst::set_inst_flags(uint32_t inst_flags) {

    this->inst_flags = inst_flags;

    return *this;
}

fuku_inst& fuku_inst::set_cpu_flags(uint64_t flags) {

    this->cpu_flags = flags;

    return *this;
}

fuku_inst& fuku_inst::set_cpu_registers(uint64_t regs) {

    this->cpu_registers = regs;

    return *this;
}

fuku_inst& fuku_inst::invalidate_source_address() {

    this->inst_has_address = false;

    return *this;
}

uint16_t fuku_inst::get_id() const {
    return this->id;
}

const uint8_t* fuku_inst::get_opcode() const {
    return this->opcode;
}

uint8_t  fuku_inst::get_oplength() const {
    return this->oplength;
}

uint64_t fuku_inst::get_source_address() const {
    return this->source_address;
}

uint64_t fuku_inst::get_current_address() const {
    return this->current_address;
}

fuku_code_label* fuku_inst::get_label() {
    return this->label;
}

const fuku_code_label* fuku_inst::get_label() const {
    return this->label;
}

fuku_relocation* fuku_inst::get_imm_reloc() {
    return this->imm_reloc;
}

const fuku_relocation* fuku_inst::get_imm_reloc() const {
    return this->imm_reloc;
}

fuku_relocation* fuku_inst::get_disp_reloc() {
    return this->disp_reloc;
}

const fuku_relocation* fuku_inst::get_disp_reloc() const {
    return this->disp_reloc;
}

fuku_rip_relocation* fuku_inst::get_rip_reloc() {
    return this->rip_reloc;
}

const fuku_rip_relocation* fuku_inst::get_rip_reloc() const {
    return this->rip_reloc;
}

uint16_t fuku_inst::get_inst_flags() const {
    return this->inst_flags;
}

uint64_t fuku_inst::get_cpu_flags() const {
    return this->cpu_flags;
}

uint64_t fuku_inst::get_cpu_registers() const {
    return this->cpu_registers;
}

bool fuku_inst::has_source_address() const {
    return this->inst_has_address;
}

bool fuku_inst::is_used_disp_reloc() const {
    return this->inst_used_disp;
}

uint8_t get_inst_prefixe_count(const fuku_inst& inst) {

    uint32_t i = 0;
    
    for (i = 0;
        i < inst.get_oplength() &&
        (inst.get_opcode()[i] == 0xF0 ||  //lock
            inst.get_opcode()[i] == 0xF3 || //repe
            inst.get_opcode()[i] == 0xF2 || //repne
            inst.get_opcode()[i] == 0x2E || //cs
            inst.get_opcode()[i] == 0x36 || //ss
            inst.get_opcode()[i] == 0x3E || //ds
            inst.get_opcode()[i] == 0x26 || //es
            inst.get_opcode()[i] == 0x64 || //fs
            inst.get_opcode()[i] == 0x65) //gs
        ;
    i++) {
    }

    return i;
}
