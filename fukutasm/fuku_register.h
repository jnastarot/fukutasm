#pragma once

enum fuku_register_enum {
    FUKU_REG_NONE,

 /*|_____64bit____|*/
 /*     |___LOW32bit___|*/
 /*          |___LOW16bit___|*/
 /*    |___HIGH8bit___||___LOW8bit___|*/

 //x86-x32 registers
    FUKU_REG_RAX,
        FUKU_REG_EAX,
                FUKU_REG_AX,
            FUKU_REG_AH, FUKU_REG_AL,
  
    FUKU_REG_RCX,
          FUKU_REG_ECX,
                FUKU_REG_CX,
            FUKU_REG_CH, FUKU_REG_CL,

    FUKU_REG_RDX,
          FUKU_REG_EDX,
                FUKU_REG_DX,
            FUKU_REG_DH, FUKU_REG_DL,

    FUKU_REG_RBX,
         FUKU_REG_EBX, 
                FUKU_REG_BX,
            FUKU_REG_BH, FUKU_REG_BL,

    FUKU_REG_RSP,
           FUKU_REG_ESP,
                FUKU_REG_SP,
                         FUKU_REG_SPL,

    FUKU_REG_RBP,
          FUKU_REG_EBP,
                FUKU_REG_BP, 
                         FUKU_REG_BPL,

    FUKU_REG_RSI,
          FUKU_REG_ESI,
                FUKU_REG_SI,
                         FUKU_REG_SIL,

    FUKU_REG_RDI,
          FUKU_REG_EDI,
                FUKU_REG_DI, 
                         FUKU_REG_DIL,


 //x86-x64 registers
    FUKU_REG_R8,
           FUKU_REG_R8D,
                 FUKU_REG_R8W,
                          FUKU_REG_R8B,
    FUKU_REG_R9,
           FUKU_REG_R9D,
                 FUKU_REG_R9W,
                          FUKU_REG_R9B,

    FUKU_REG_R10,
           FUKU_REG_R10D,
                 FUKU_REG_R10W,
                          FUKU_REG_R10B,

    FUKU_REG_R11,
           FUKU_REG_R11D,
                 FUKU_REG_R11W,
                          FUKU_REG_R11B,

    FUKU_REG_R12,
           FUKU_REG_R12D,
                 FUKU_REG_R12W,
                          FUKU_REG_R12B,

    FUKU_REG_R13,
           FUKU_REG_R13D,
                 FUKU_REG_R13W,
                          FUKU_REG_R13B,

    FUKU_REG_R14,
           FUKU_REG_R14D,
                 FUKU_REG_R14W,
                          FUKU_REG_R14B,

    FUKU_REG_R15,
           FUKU_REG_R15D,
                 FUKU_REG_R15W,
                          FUKU_REG_R15B,

    FUKU_REG_MAX,
};

enum fuku_register_index {
    FUKU_REG_INDEX_AX = 0, FUKU_REG_INDEX_R8 = 0,
    FUKU_REG_INDEX_CX = 1, FUKU_REG_INDEX_R9 = 1,
    FUKU_REG_INDEX_DX = 2, FUKU_REG_INDEX_R10 = 2,
    FUKU_REG_INDEX_BX = 3, FUKU_REG_INDEX_R11 = 3,
    FUKU_REG_INDEX_SP = 4, FUKU_REG_INDEX_R12 = 4,
    FUKU_REG_INDEX_BP = 5, FUKU_REG_INDEX_R13 = 5,
    FUKU_REG_INDEX_SI = 6, FUKU_REG_INDEX_R14 = 6,
    FUKU_REG_INDEX_DI = 7, FUKU_REG_INDEX_R15 = 7,

    FUKU_REG_INDEX_INVALID = -1
};

#pragma pack(push, 1)

class fuku_register {
    fuku_register_enum reg;
    fuku_register_index index;
    fuku_operand_size size;

    bool ext64;
public:
    fuku_register();
    fuku_register(const fuku_register& reg);
    fuku_register(fuku_register_enum reg);
    ~fuku_register();

    fuku_register& operator=(const fuku_register& reg);
    fuku_register& operator=(fuku_register_enum reg);
public:
    void set_reg(fuku_register_enum reg);
public:
    fuku_register_enum get_reg() const;
    fuku_register_index get_index() const;
    fuku_operand_size get_size() const;
    bool is_ext64() const;
};

#pragma pack(pop)

inline fuku_register reg_(fuku_register_enum reg) {
    return fuku_register(reg);
}