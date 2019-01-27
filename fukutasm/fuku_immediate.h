#pragma once


class fuku_immediate {
    bool relocate;

    uint64_t immediate_value;
public:
    fuku_immediate();
    fuku_immediate(uint64_t immediate);
    fuku_immediate(uint64_t immediate, bool is_rel);

    fuku_immediate(const fuku_immediate& imm);

    ~fuku_immediate();

    fuku_immediate& operator=(const fuku_immediate& imm);
public:
    fuku_immediate& set_relocate(bool is_rel);
    fuku_immediate& set_immediate(uint64_t immediate);
public:
    bool is_8() const;
    bool is_16() const;
    bool is_32() const;
    bool is_64() const;

    fuku_operand_size get_size() const;

    bool is_relocate() const;

    uint8_t get_immediate8() const;
    uint16_t get_immediate16() const;
    uint32_t get_immediate32() const;
    uint64_t get_immediate64() const;

    int8_t get_signed_value8() const;
    int16_t get_signed_value16() const;
    int32_t get_signed_value32() const;
    int64_t get_signed_value64() const;
};

inline fuku_immediate imm(uint64_t imm) {
    return fuku_immediate(imm);
}

inline fuku_immediate imm(uint64_t imm, bool relocate) {
    return fuku_immediate(imm, relocate);
}