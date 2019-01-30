#pragma once

enum fuku_mem_opernad_type {
    FUKU_MEM_OPERAND_BASE_ONLY,
    FUKU_MEM_OPERAND_DISP_ONLY,
    FUKU_MEM_OPERAND_BASE_DISP,
    FUKU_MEM_OPERAND_INDEX_DISP,
    FUKU_MEM_OPERAND_BASE_INDEX,
    FUKU_MEM_OPERAND_BASE_INDEX_DISP,
};

enum fuku_operand_scale {
    FUKU_OPERAND_SCALE_1 = 0,  //[index * 1]
    FUKU_OPERAND_SCALE_2 = 1,  //[index * 2]
    FUKU_OPERAND_SCALE_4 = 2,  //[index * 4]
    FUKU_OPERAND_SCALE_8 = 3,  //[index * 8]
};

class fuku_operand {
    fuku_register base;
    fuku_register index;
    fuku_operand_scale scale;
    fuku_immediate disp;
    fuku_operand_size size;
public:
    explicit fuku_operand(const fuku_register& base, fuku_operand_size size);   // [base]
    explicit fuku_operand(fuku_register_enum base, fuku_operand_size size);   // [base]
    explicit fuku_operand(const fuku_immediate& disp, fuku_operand_size size);    // [disp/r]
    explicit fuku_operand(uint32_t disp, fuku_operand_size size);                 // [disp/r]
    explicit fuku_operand(const fuku_register& base, const fuku_immediate& disp, fuku_operand_size size);  // [base + disp/r]
    explicit fuku_operand(fuku_register_enum base, const fuku_immediate& disp, fuku_operand_size size);  // [base + disp/r]
    explicit fuku_operand(const fuku_register& base, const fuku_register& index, fuku_operand_scale scale, const fuku_immediate& disp, fuku_operand_size size);// [base + index*scale + disp/r]
    explicit fuku_operand(fuku_register_enum base, fuku_register_enum index, fuku_operand_scale scale, const fuku_immediate& disp, fuku_operand_size size);// [base + index*scale + disp/r]
    explicit fuku_operand(const fuku_register& index, fuku_operand_scale scale, const fuku_immediate& disp, fuku_operand_size size);// [index*scale + disp/r]
    explicit fuku_operand(fuku_register_enum index, fuku_operand_scale scale, const fuku_immediate& disp, fuku_operand_size size);// [index*scale + disp/r]

    fuku_operand(const fuku_operand& op);

    ~fuku_operand();

    fuku_operand& operator=(const fuku_operand& op);
public:
    void set_base(const fuku_register& base);
    void set_index(const fuku_register& index);
    void set_scale(fuku_operand_scale scale);
    void set_disp(const fuku_immediate& disp);
    void set_size(fuku_operand_size size);
public:
    const fuku_register& get_base() const;
    const fuku_register& get_index() const;
    fuku_operand_scale get_scale() const;
    const fuku_immediate& get_disp() const;
    fuku_operand_size get_size() const;

    fuku_mem_opernad_type get_type() const;
    uint8_t get_low_rex() const;
};


#define OPERAND_PTR_DEF(name, size) \
    namespace fukutasm {\
    inline fuku_operand name(const fuku_register& base) {\
        return fuku_operand(base, size);\
    }\
    inline fuku_operand name(fuku_register_enum base) {\
        return fuku_operand(base, size);\
    }\
    inline fuku_operand name(const fuku_immediate& disp) {\
        return fuku_operand(disp, size);\
    }\
    inline fuku_operand name(uint32_t disp) {\
        return fuku_operand(disp, size);\
    }\
    inline fuku_operand name(const fuku_register& base, const fuku_immediate& disp) {\
        return fuku_operand(base, disp, size);\
    }\
    inline fuku_operand name(fuku_register_enum base, const fuku_immediate& disp) {\
        return fuku_operand(base, disp, size);\
    }\
    inline fuku_operand name(const fuku_register& base, const fuku_register& index, fuku_operand_scale scale, const fuku_immediate& disp) {\
        return fuku_operand(base, index, scale, disp, size);\
    }\
    inline fuku_operand name(fuku_register_enum base, fuku_register_enum index, fuku_operand_scale scale, const fuku_immediate& disp) {\
        return fuku_operand(base, index, scale, disp, size);\
    }\
    inline fuku_operand name(const fuku_register& index, fuku_operand_scale scale, const fuku_immediate& disp) {\
        return fuku_operand(index, scale, size);\
    }\
    inline fuku_operand name(fuku_register_enum index, fuku_operand_scale scale, const fuku_immediate& disp) {\
        return fuku_operand(index, scale, size);\
    }\
    }
    

OPERAND_PTR_DEF(byte_ptr,  FUKU_OPERAND_SIZE_8)
OPERAND_PTR_DEF(word_ptr,  FUKU_OPERAND_SIZE_16)
OPERAND_PTR_DEF(dword_ptr, FUKU_OPERAND_SIZE_32)
OPERAND_PTR_DEF(qword_ptr, FUKU_OPERAND_SIZE_64)

    