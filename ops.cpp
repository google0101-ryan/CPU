#include "x86.h"
#include "IOBus.h"
#include "Bus.h"
#include <cstdio>
#include <bitset>
#include <x86intrin.h>

void TigerLake::MakeOpcodeTables()
{
    lookup16[0x31] = std::bind(&TigerLake::XorRm16R16, this);
    lookup16[0x8E] = std::bind(&TigerLake::MovSregRm16, this);
    lookup16[0xE4] = std::bind(&TigerLake::InAlImm8, this);
    lookup16[0xE6] = std::bind(&TigerLake::OutImm8Al, this);
    lookup16[0xE9] = std::bind(&TigerLake::JmpRel16, this);
    lookup16[0xEA] = std::bind(&TigerLake::JmpPtr1616, this);
    lookup16[0xFA] = std::bind(&TigerLake::Cli, this);
    lookup16[0xFC] = std::bind(&TigerLake::Cld, this);

    lookup32[0x83] = std::bind(&TigerLake::Code83_32, this);
    lookup32[0x89] = std::bind(&TigerLake::MovRm32R32, this);
    for (int i = 0; i < 0x8; i++)
    {
        lookup32[0xB8+i] = std::bind(&TigerLake::MovR32Imm32, this);
    }

    extLookup16[0x85] = std::bind(&TigerLake::JnzRel16, this);
}

bool CalcPF(uint8_t result)
{
    int pf_cnt = 0;
    for (int i = 0; i < 8; i++)
        if ((result >> i) & 1)
            pf_cnt++;
    return (pf_cnt % 2) == 0;
}

void TigerLake::UpdateFlagsLogic8(uint8_t result)
{
    rflags.flag_bits.zf = (result == 0);
    rflags.flag_bits.sf = (result >> 7) & 1;
    rflags.flag_bits.pf = CalcPF(result);
    rflags.flag_bits.cf = 0;
    rflags.flag_bits.of = 0;
}

void TigerLake::UpdateFlagsLogic16(uint16_t result)
{
    rflags.flag_bits.zf = (result == 0);
    rflags.flag_bits.sf = (result >> 15) & 1;
    rflags.flag_bits.pf = CalcPF(result & 0xff);
    rflags.flag_bits.cf = 0;
    rflags.flag_bits.of = 0;
}

void TigerLake::UpdateFlagsLogic32(uint32_t result)
{
    rflags.flag_bits.zf = (result == 0);
    rflags.flag_bits.sf = (result >> 31) & 1;
    rflags.flag_bits.pf = CalcPF(result & 0xff);
    rflags.flag_bits.cf = 0;
    rflags.flag_bits.of = 0;
}

void TigerLake::UpdateFlagsLogic64(uint64_t result)
{
    rflags.flag_bits.zf = (result == 0);
    rflags.flag_bits.sf = (result >> 63) & 1;
    rflags.flag_bits.pf = CalcPF(result & 0xff);
    rflags.flag_bits.cf = 0;
    rflags.flag_bits.of = 0;
}

void TigerLake::UpdateFlagsSub8(uint8_t value1, uint8_t value2, uint32_t result)
{
    bool sign1 = value1 >> 7;
    bool sign2 = value2 >> 7;
    bool signr = (result >> 7) & 1;

    rflags.flag_bits.cf = (result >> 8);
    rflags.flag_bits.zf = ((result & 0xFFFFFFFF) == 0);
    rflags.flag_bits.sf = signr;
    rflags.flag_bits.of = (sign1 != sign2 && sign1 != signr);
    rflags.flag_bits.pf = CalcPF(result & 0xff);
}

void TigerLake::UpdateFlagsSub16(uint16_t value1, uint16_t value2, uint32_t result)
{
    bool sign1 = value1 >> 15;
    bool sign2 = value2 >> 15;
    bool signr = (result >> 15) & 1;

    rflags.flag_bits.cf = (result >> 16);
    rflags.flag_bits.zf = ((result & 0xFFFFFFFF) == 0);
    rflags.flag_bits.sf = signr;
    rflags.flag_bits.of = (sign1 != sign2 && sign1 != signr);
    rflags.flag_bits.pf = CalcPF(result & 0xff);
}

void TigerLake::UpdateFlagsSub32(uint32_t value1, uint32_t value2, uint64_t result)
{
    bool sign1 = value1 >> 31;
    bool sign2 = value2 >> 31;
    bool signr = (result >> 31) & 1;

    rflags.flag_bits.cf = (result >> 32);
    rflags.flag_bits.zf = ((result & 0xFFFFFFFF) == 0);
    rflags.flag_bits.sf = signr;
    rflags.flag_bits.of = (sign1 != sign2 && sign1 != signr);
    rflags.flag_bits.pf = CalcPF(result & 0xff);
}

void TigerLake::UpdateFlagsSub64(uint64_t value1, uint64_t value2, uint64_t result)
{
    bool sign1 = value1 >> 63;
    bool sign2 = value2 >> 63;
    bool signr = (result >> 63) & 1;

    rflags.flag_bits.cf = value2 > value1;
    rflags.flag_bits.zf = (result == 0);
    rflags.flag_bits.sf = signr;
    rflags.flag_bits.of = (sign1 != sign2 && sign1 != signr);
    rflags.flag_bits.pf = CalcPF(result & 0xff);
}

void TigerLake::UpdateFlagsAdd8(uint8_t value1, uint8_t value2, uint16_t result)
{
    bool sign1 = value1 >> 7;
    bool sign2 = value2 >> 7;
    bool signr = (result >> 7) & 1;

    rflags.flag_bits.cf = (result >> 8);
    rflags.flag_bits.zf = ((result & 0xff) == 0);
    rflags.flag_bits.sf = signr;
    rflags.flag_bits.of = (sign1 != sign2 && sign1 != signr);
    rflags.flag_bits.pf = CalcPF(result & 0xff);
}

void TigerLake::UpdateFlagsAdd16(uint16_t value1, uint16_t value2, uint32_t result)
{
    bool sign1 = value1 >> 15;
    bool sign2 = value2 >> 15;
    bool signr = (result >> 15) & 1;

    rflags.flag_bits.cf = (result >> 16);
    rflags.flag_bits.zf = ((result & 0xffff) == 0);
    rflags.flag_bits.sf = signr;
    rflags.flag_bits.of = (sign1 != sign2 && sign1 != signr);
    rflags.flag_bits.pf = CalcPF(result & 0xff);
}

void TigerLake::UpdateFlagsAdd32(uint32_t value1, uint32_t value2, uint64_t result)
{
    bool sign1 = value1 >> 31;
    bool sign2 = value2 >> 31;
    bool signr = (result >> 31) & 1;

    rflags.flag_bits.cf = (result >> 32);
    rflags.flag_bits.zf = ((result & 0xFFFFFFFF) == 0);
    rflags.flag_bits.sf = signr;
    rflags.flag_bits.of = (sign1 == sign2 && sign1 != signr);
    rflags.flag_bits.pf = CalcPF(result & 0xff);
}

void TigerLake::UpdateFlagsAdd64(uint64_t value1, uint64_t value2, uint64_t result)
{
    bool sign1 = value1 >> 63;
    bool sign2 = value2 >> 63;
    bool signr = (result >> 63) & 1;

    rflags.flag_bits.cf = result < value1;
    rflags.flag_bits.zf = (result == 0);
    rflags.flag_bits.sf = signr;
    rflags.flag_bits.of = (sign1 == sign2 && sign1 != signr);
    rflags.flag_bits.pf = CalcPF(result & 0xff);
}

// All modes
void TigerLake::MovSregRm16()
{
    FetchModrm();

    std::string disasm;
    CacheSegment((Segments)modrm.reg, ReadModrm16(disasm));

    if constexpr (canDisassemble)
        printf("mov %s, %s\n", Segs[modrm.reg], disasm.c_str());
}

void TigerLake::InAlImm8()
{
    uint8_t port = ReadImm8();
    regs[RAX].lo = IOBus::In8(port);

    if constexpr (canDisassemble)
        printf("in al, 0x%02x\n", port);
}

void TigerLake::OutImm8Al()
{
    uint8_t imm8 = ReadImm8();

    IOBus::Out8(imm8, regs[RAX].lo);

    if constexpr (canDisassemble)
        printf("outb 0x%02x, al\n", imm8);
}

void TigerLake::Cld()
{
    rflags.flag_bits.df = 0;

    if constexpr (canDisassemble)
        printf("cld\n");
}

void TigerLake::Cli()
{
    rflags.flag_bits.if_ = 0;

    if constexpr (canDisassemble)
        printf("cli\n");
}

// 16-bit mode

void TigerLake::XorRm16R16()
{
    FetchModrm16();

    std::string disasm;
    uint16_t rm16 = ReadModrm16(disasm);
    uint16_t r16 = regs[modrm.reg].reg16;

    uint16_t result = rm16 ^ r16;
    WriteModrm16(disasm, result);

    UpdateFlagsLogic16(result);

    if constexpr (canDisassemble)
        printf("xor %s, %s\n", disasm.c_str(), Reg16[modrm.reg]);
}

void TigerLake::JmpRel16()
{
    int16_t rel16 = ReadImm16();

    rip += (int64_t)rel16;

    if constexpr (canDisassemble)
        printf("jmp 0x%08lx\n", TranslateAddr(CS, rip));
}

void TigerLake::JmpPtr1616()
{
    uint16_t ip = ReadImm16(true);
    uint16_t cs = ReadImm16(true);

    CacheSegment(CS, cs);
    rip = (uint64_t)ip;

    if constexpr (canDisassemble)
        printf("jmp 0x%04x:0x%04x\n", cs, rip);
}

// 32-bit mode

void TigerLake::Code83_32()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x07:
        CmpRm32Imm8();
        break;
    default:
        printf("Error: Unknown opcode 0x83 0x%02x\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::CmpRm32Imm8()
{
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint8_t imm8 = ReadImm8(true);

    uint64_t result = (uint64_t)rm32 - (uint64_t)imm8;

    UpdateFlagsSub32(rm32, (uint32_t)imm8, result);

    if constexpr (canDisassemble)
        printf("cmp %s, 0x%02x\n", disasm.c_str(), imm8);
}

void TigerLake::MovRm32R32()
{
    FetchModrm();

    std::string disasm;
    WriteModrm32(disasm, regs[modrm.reg].reg32);

    if constexpr (canDisassemble)
        printf("mov %s, %s\n", disasm.c_str(), Reg32[modrm.reg]);
}

void TigerLake::MovR32Imm32()
{
    uint8_t reg = Bus::Read8(TranslateAddr(CS, rip-1)) - 0xB8;
    uint32_t imm32 = ReadImm32(true);

    regs[reg].reg32 = imm32;

    if constexpr (canDisassemble)
        printf("mov %s, 0x%08x\n", Reg32[reg], imm32);
}

// Extended 16-bit
void TigerLake::JnzRel16()
{
    int16_t rel16 = ReadImm16(true);

    if constexpr (canDisassemble)
        printf("jne 0x%08lx\n", TranslateAddr(CS, rip+(int64_t)rel16));

    if (!rflags.flag_bits.zf)
        rip += (int64_t)rel16;
}