#pragma once

#include <cstdint>
#include <sstream>
#include <string>

typedef __int128_t int128_t;
typedef __uint128_t uint128_t;

inline std::string convert_int(int n)
{
   std::stringstream ss;

   ss << "0x" << std::hex << n;
   return ss.str();
}

union [[gnu::packed]] Register
{
    uint64_t reg64;
    uint32_t reg32;
    uint16_t reg16;
    struct
    {
        uint8_t lo;
        uint8_t hi;
    };
    // Included to make my life easier when implementing bswap
    uint8_t bytes[8];
};

enum Registers
{
    RAX,
    RCX,
    RDX,
    RBX,
    RSP,
    RBP,
    RSI,
    RDI,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15,
    NUM_REGS
};

inline const char* Reg8[] =
{
    "al",
    "cl",
    "dl",
    "bl",
    "ah",
    "ch",
    "dh",
    "bh",
    "r8b",
    "r9b",
    "r10b",
    "r11b",
    "r12b",
    "r13b",
    "r14b",
    "r15b",
};

inline const char* Reg8REX[] =
{
    "al",
    "cl",
    "dl",
    "bl",
    "spl",
    "bpl",
    "sil",
    "dil",
    "r8b",
    "r9b",
    "r10b",
    "r11b",
    "r12b",
    "r13b",
    "r14b",
    "r15b",
};

inline const char* Reg16[] =
{
    "ax",
    "cx",
    "dx",
    "bx",
    "sp",
    "bp",
    "si",
    "di",
    "r8w",
    "r9w",
    "r10w",
    "r11w",
    "r12w",
    "r13w",
    "r14w",
    "r15w",
};

inline const char* Reg32[] =
{
    "eax",
    "ecx",
    "edx",
    "ebx",
    "esp",
    "ebp",
    "esi",
    "edi",
    "r8d",
    "r9d",
    "r10d",
    "r11d",
    "r12d",
    "r13d",
    "r14d",
    "r15d",
};

inline const char* Reg64[] =
{
    "rax",
    "rcx",
    "rdx",
    "rbx",
    "rsp",
    "rbp",
    "rsi",
    "rdi",
    "r8",
    "r9",
    "r10",
    "r11",
    "r12",
    "r13",
    "r14",
    "r15",
};

struct Segment
{
    uint16_t selector;
    uint32_t base;
    uint32_t limit;
    uint16_t access_rights;
};

enum Segments
{
    ES,
    CS,
    SS,
    DS,
    FS,
    GS,
    NUM_SEGS
};

inline const char* Segs[] =
{
    "es",
    "cs",
    "ss",
    "ds",
    "fs",
    "gs",
    "undefined",
    "undefined",
    "undefined",
    "undefined",
    "undefined",
    "undefined",
    "undefined",
    "undefined",
    "undefined",
    "undefined",
};

union RFLAGS
{
    uint64_t value;
    struct
    {
        uint64_t cf : 1,
        : 1,
        pf : 1,
        : 1,
        af : 1,
        : 1,
        zf : 1,
        sf : 1,
        tf : 1,
        if_ : 1,
        df : 1,
        of : 1,
        iopl : 2,
        nt : 1,
        md : 1,
        rf : 1,
        vm : 1,
        ac : 1,
        vif : 1,
        vip : 1,
        id : 1,
        : 10,
        : 32;
    } flag_bits;
};

struct DTR
{
    uint64_t base;
    uint16_t limit;
};