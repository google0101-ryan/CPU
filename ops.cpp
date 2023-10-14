#include "x86.h"
#include "IOBus.h"
#include "Bus.h"
#include <cstdio>
#include <bitset>
#include <x86intrin.h>

void TigerLake::MakeOpcodeTables()
{
    lookup16[0x0C] = std::bind(&TigerLake::OrAlImm8, this);
    lookup16[0x31] = std::bind(&TigerLake::XorRm16R16, this);
    lookup16[0x85] = std::bind(&TigerLake::TestRm16R16, this);
    lookup16[0x8B] = std::bind(&TigerLake::MovR16Rm16, this);
    lookup16[0x8E] = std::bind(&TigerLake::MovSregRm16, this);
    lookup16[0xE4] = std::bind(&TigerLake::InAlImm8, this);
    lookup16[0xE6] = std::bind(&TigerLake::OutImm8Al, this);
    lookup16[0xE9] = std::bind(&TigerLake::JmpRel16, this);
    lookup16[0xEA] = std::bind(&TigerLake::JmpPtr1616, this);
    lookup16[0xFA] = std::bind(&TigerLake::Cli, this);
    lookup16[0xFC] = std::bind(&TigerLake::Cld, this);

    lookup32[0x29] = std::bind(&TigerLake::SubRm32R32, this);
    lookup32[0x31] = std::bind(&TigerLake::XorRm32R32, this);
    lookup32[0x3C] = std::bind(&TigerLake::CmpAlImm8, this);
    for (int i = 0; i < 8; i++)
    {
        lookup32[0x48+i] = std::bind(&TigerLake::DecR32, this);
    }
    for (int i = 0; i < 8; i++)
    {
        lookup32[0x50+i] = std::bind(&TigerLake::PushR32, this);
    }
    for (int i = 0; i < 8; i++)
    {
        lookup32[0x58+i] = std::bind(&TigerLake::PopR32, this);
    }
    lookup32[0x68] = std::bind(&TigerLake::PushImm32, this);
    lookup32[0x6A] = std::bind(&TigerLake::PushImm8, this);
    lookup32[0x74] = std::bind(&TigerLake::JzRel8, this);
    lookup32[0x75] = std::bind(&TigerLake::JnzRel8, this);
    lookup32[0x76] = std::bind(&TigerLake::JnaRel8, this);
    lookup32[0x77] = std::bind(&TigerLake::JaRel8, this);
    lookup32[0x7E] = std::bind(&TigerLake::JngRel8, this);
    lookup32[0x7F] = std::bind(&TigerLake::JgRel8, this);
    lookup32[0x80] = std::bind(&TigerLake::Code80, this);
    lookup32[0x81] = std::bind(&TigerLake::Code81_32, this);
    lookup32[0x83] = std::bind(&TigerLake::Code83_32, this);
    lookup32[0x84] = std::bind(&TigerLake::TestRm8R8, this);
    lookup32[0x85] = std::bind(&TigerLake::TestRm32R32, this);
    lookup32[0x88] = std::bind(&TigerLake::MovRm8R8, this);
    lookup32[0x89] = std::bind(&TigerLake::MovRm32R32, this);
    lookup32[0x8A] = std::bind(&TigerLake::MovR8Rm8, this);
    lookup32[0x8B] = std::bind(&TigerLake::MovR32Rm32, this);
    lookup32[0x8D] = std::bind(&TigerLake::LeaR32M, this);
    lookup32[0x8E] = std::bind(&TigerLake::MovSregRm16, this);
    lookup32[0xA8] = std::bind(&TigerLake::TestAlImm8, this);
    for (int i = 0; i < 0x8; i++)
    {
        lookup32[0xB0+i] = std::bind(&TigerLake::MovR8Imm8, this);
    }
    for (int i = 0; i < 0x8; i++)
    {
        lookup32[0xB8+i] = std::bind(&TigerLake::MovR32Imm32, this);
    }
    lookup32[0xC1] = std::bind(&TigerLake::CodeC1_32, this);
    lookup32[0xC3] = std::bind(&TigerLake::Ret, this);
    lookup32[0xC6] = std::bind(&TigerLake::MovRm8Imm8, this);
    lookup32[0xE8] = std::bind(&TigerLake::CallRel32, this);
    lookup32[0xE9] = std::bind(&TigerLake::JmpRel32, this);
    lookup32[0xEA] = std::bind(&TigerLake::JmpPtr1632, this);
    lookup32[0xEB] = std::bind(&TigerLake::JmpRel8, this);
    lookup32[0xEE] = std::bind(&TigerLake::OutDxAl, this);
    lookup32[0xEC] = std::bind(&TigerLake::InAlDx, this);
    lookup32[0xFF] = std::bind(&TigerLake::CodeFF_32, this);

    extLookup16[0x01] = std::bind(&TigerLake::Code0f01, this);
    extLookup16[0x20] = std::bind(&TigerLake::MovR64CRn, this);
    extLookup16[0x22] = std::bind(&TigerLake::MovCRnR64, this);
    extLookup16[0x85] = std::bind(&TigerLake::JnzRel16, this);

    extLookup32[0x84] = std::bind(&TigerLake::JzRel32, this);
    extLookup32[0x85] = std::bind(&TigerLake::JnzRel32, this);
    extLookup32[0xB6] = std::bind(&TigerLake::MovzxR32Rm8, this);
    extLookup32[0xBE] = std::bind(&TigerLake::MovsxR32Rm8, this);
    extLookup32[0x94] = std::bind(&TigerLake::SetzRm8, this);
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

    rflags.flag_bits.cf = (result >> 8) != 0;
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

    rflags.flag_bits.cf = (result >> 16) != 0;
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

    rflags.flag_bits.cf = (result >> 32) != 0;
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

    rflags.flag_bits.cf = (result >> 8) != 0;
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

    rflags.flag_bits.cf = (result >> 16) != 0;
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

    rflags.flag_bits.cf = (result >> 32) != 0;
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
void TigerLake::OrAlImm8()
{
    uint8_t imm8 = ReadImm8();

    uint8_t result = regs[RAX].lo | imm8;
    regs[RAX].lo = result;

    UpdateFlagsLogic8(result);

    if constexpr (canDisassemble)
        printf("or al, 0x%02x\n", imm8);
}

void TigerLake::CmpAlImm8()
{
    uint8_t imm8 = ReadImm8(true);

    uint16_t result = (uint16_t)regs[RAX].lo - (uint16_t)imm8;

    UpdateFlagsSub8(regs[RAX].lo, imm8, result);

    if constexpr (canDisassemble)
        printf("cmp al, 0x%02x\n", imm8);
}

void TigerLake::PushImm8()
{
    uint8_t imm8 = ReadImm8();

    if (mode == Mode::ProtectedMode)
        Push32((int32_t)(int8_t)imm8);
    else if (mode == Mode::LongMode)
        Push64((int32_t)(int8_t)imm8);
    else
    {
        printf("TODO: 16-bit push\n");
        exit(1);
    }

    if constexpr (canDisassemble)
        printf("push byte +0x%02x\n", imm8);
}

void TigerLake::JzRel8()
{
    int8_t rel8 = ReadImm8();

    if constexpr (canDisassemble)
        printf("jz 0x%08lx\n", TranslateAddr(CS, rip+rel8));
    
    if (rflags.flag_bits.zf)
        rip += rel8;
}

void TigerLake::JnzRel8()
{
    int8_t rel8 = ReadImm8();

    if constexpr (canDisassemble)
        printf("jnz 0x%08lx\n", TranslateAddr(CS, rip+rel8));
    
    if (!rflags.flag_bits.zf)
        rip += rel8;
}

void TigerLake::JnaRel8()
{
    int8_t rel8 = ReadImm8(true);

    if constexpr (canDisassemble)
        printf("jna 0x%08lx\n", TranslateAddr(CS, rip+rel8));
    
    if (rflags.flag_bits.zf || rflags.flag_bits.cf)
        rip += rel8;
}

void TigerLake::JaRel8()
{
    int8_t rel8 = ReadImm8(true);

    if constexpr (canDisassemble)
        printf("ja 0x%08lx\n", TranslateAddr(CS, rip+rel8));
    
    if (!rflags.flag_bits.zf && !rflags.flag_bits.cf)
        rip += rel8;
}

void TigerLake::JngRel8()
{
    int8_t rel8 = ReadImm8(true);

    if constexpr (canDisassemble)
        printf("jng 0x%08lx\n", TranslateAddr(CS, rip+rel8));
    
    if (rflags.flag_bits.zf || (rflags.flag_bits.sf!=rflags.flag_bits.of))
        rip += rel8;
}

void TigerLake::JgRel8()
{
    int8_t rel8 = ReadImm8(true);

    if constexpr (canDisassemble)
        printf("jg 0x%08lx\n", TranslateAddr(CS, rip+rel8));
    
    if (!rflags.flag_bits.zf && (rflags.flag_bits.sf==rflags.flag_bits.of))
        rip += rel8;
}

void TigerLake::Code80()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x07:
        CmpRm8Imm8();
        break;
    default:
        printf("Error: Unknown opcode 0x80 0x%02x\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::CmpRm8Imm8()
{
    std::string disasm;
    uint8_t rm8 = ReadModrm8(disasm);
    uint8_t imm8 = ReadImm8();

    uint16_t result = (uint16_t)rm8 - (uint16_t)imm8;
    UpdateFlagsSub8(rm8, imm8, result);

    if constexpr (canDisassemble)
        printf("cmp %s, 0x%02x\n", disasm.c_str(), imm8);
}

void TigerLake::TestRm8R8()
{
    FetchModrm();

    std::string disasm;
    uint8_t rm8 = ReadModrm8(disasm);
    uint8_t r8 = ReadReg8((Registers)modrm.reg);

    uint8_t result = rm8 & r8;
    UpdateFlagsLogic8(result);

    if constexpr (canDisassemble)
        printf("test %s, %s\n", disasm.c_str(), Reg8[modrm.reg]);
}

void TigerLake::MovRm8R8()
{
    FetchModrm();

    std::string disasm;
    WriteModrm8(disasm, ReadReg8((Registers)modrm.reg));

    if constexpr (canDisassemble)
        printf("mov %s, %s\n", disasm.c_str(), Reg8[modrm.reg]);
}

void TigerLake::MovR8Rm8()
{
    FetchModrm();

    std::string disasm;
    WriteReg8((Registers)modrm.reg, ReadModrm8(disasm));

    if constexpr (canDisassemble)
        printf("mov %s, %s\n", Reg8[modrm.reg], disasm.c_str());
}

void TigerLake::MovSregRm16()
{
    FetchModrm();

    std::string disasm;
    CacheSegment((Segments)modrm.reg, ReadModrm16(disasm));

    if constexpr (canDisassemble)
        printf("mov %s, %s\n", Segs[modrm.reg], disasm.c_str());
}

void TigerLake::TestAlImm8()
{
    uint8_t imm8 = ReadImm8(true);

    uint8_t result = regs[RAX].lo & imm8;

    UpdateFlagsLogic8(result);

    if constexpr (canDisassemble)
        printf("test al, 0x%02x\n", imm8);
}

void TigerLake::MovR8Imm8()
{
    uint8_t reg = Bus::Read8(TranslateAddr(CS, rip-1)) - 0xB0;
    uint8_t imm8 = ReadImm8(true);

    WriteReg8((Registers)reg, imm8);

    if constexpr (canDisassemble)
        printf("mov %s, 0x%02x\n", Reg8[reg], imm8);
}

void TigerLake::MovRm8Imm8()
{
    FetchModrm();

    uint8_t imm8 = ReadImm8(true);

    std::string disasm;
    WriteModrm8(disasm, imm8);

    if constexpr (canDisassemble)
        printf("mov %s, 0x%02x\n", disasm.c_str(), imm8);
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

void TigerLake::JmpRel8()
{
    int8_t rel8 = ReadImm8(true);

    rip += (int64_t)rel8;

    if constexpr (canDisassemble)
        printf("jmp 0x%08lx\n", TranslateAddr(CS, rip));
}

void TigerLake::InAlDx()
{
    regs[RAX].lo = IOBus::In8(regs[RDX].reg16);

    if constexpr (canDisassemble)
        printf("in al, dx\n");
}

void TigerLake::OutDxAl()
{
    IOBus::Out8(regs[RDX].reg16, regs[RAX].lo);

    if constexpr (canDisassemble)
        printf("out dx, al\n");
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
    FetchModrm();

    std::string disasm;
    uint16_t rm16 = ReadModrm16(disasm);
    uint16_t r16 = regs[modrm.reg].reg16;

    uint16_t result = rm16 ^ r16;
    WriteModrm16(disasm, result);

    UpdateFlagsLogic16(result);

    if constexpr (canDisassemble)
        printf("xor %s, %s\n", disasm.c_str(), Reg16[modrm.reg]);
}

void TigerLake::TestRm16R16()
{
    FetchModrm();

    std::string disasm;
    uint16_t rm16 = ReadModrm16(disasm);
    uint16_t r16 = regs[modrm.reg].reg16;

    uint16_t result = rm16 & r16;

    UpdateFlagsLogic16(result);

    if constexpr (canDisassemble)
        printf("test %s, %s\n", disasm.c_str(), Reg16[modrm.reg]);
}

void TigerLake::MovR16Rm16()
{
    FetchModrm();

    std::string disasm;
    regs[modrm.reg].reg16 = ReadModrm16(disasm);

    if constexpr (canDisassemble)
        printf("mov %s, %s\n", Reg16[modrm.reg], disasm.c_str());
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
        printf("jmp 0x%04x:0x%04lx\n", cs, rip);
}

// 32-bit mode

void TigerLake::SubRm32R32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t r32 = regs[modrm.reg].reg32;

    uint64_t result = (uint64_t)rm32 - (uint64_t)r32;

    UpdateFlagsSub32(rm32, r32, result);

    WriteModrm32(disasm, result);

    if constexpr (canDisassemble)
        printf("sub %s, %s\n", disasm.c_str(), Reg32[modrm.reg]);
}

void TigerLake::XorRm32R32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t r32 = regs[modrm.reg].reg32;

    uint32_t result = rm32 ^ r32;

    UpdateFlagsLogic32(result);

    WriteModrm32(disasm, result);

    if constexpr (canDisassemble)
        printf("xor %s, %s\n", disasm.c_str(), Reg32[modrm.reg]);
}

void TigerLake::DecR32()
{
    uint8_t reg = Bus::Read32(TranslateAddr(CS, rip-1)) - 0x48;

    uint32_t r32 = regs[reg].reg32;
    uint64_t result = (uint64_t)r32 - 1UL;

    UpdateFlagsSub32(r32, 1, result);

    regs[reg].reg32 = result;

    if constexpr (canDisassemble)
        printf("dec %s\n", Reg32[reg]);
}

void TigerLake::PushR32()
{
    uint8_t reg = Bus::Read8(TranslateAddr(CS, rip-1)) - 0x50;

    if (mode == Mode::LongMode)
    {
        Push64(regs[reg].reg64);
        if constexpr (canDisassemble)
            printf("push %s\n", Reg64[reg]);
    }
    else
    {
        Push32(regs[reg].reg32);
        if constexpr (canDisassemble)
            printf("push %s\n", Reg32[reg]);
    }
}

void TigerLake::PopR32()
{
    uint8_t reg = Bus::Read8(TranslateAddr(CS, rip-1)) - 0x58;

    if (mode == Mode::LongMode)
    {
        regs[reg].reg64 = Pop64();
        if constexpr (canDisassemble)
            printf("pop %s\n", Reg64[reg]);
    }
    else
    {
        regs[reg].reg32 = Pop32();
        if constexpr (canDisassemble)
            printf("pop %s\n", Reg32[reg]);
    }
}

void TigerLake::PushImm32()
{
    uint32_t imm32 = ReadImm32();

    if (mode == Mode::LongMode)
        Push32(imm32);
    else
        Push64((int64_t)(int32_t)imm32);
    
    if constexpr (canDisassemble)
        printf("push dword 0x%08x\n", imm32);
}

void TigerLake::Code81_32()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x04:
        AndRm32Imm32();
        break;
    case 0x07:
        CmpRm32Imm32();
        break;
    default:
        printf("Error: Unknown opcode 0x81 0x%02x\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::AndRm32Imm32()
{
    uint32_t imm32 = ReadImm32();
    
    std::string disasm;
    uint32_t result = ReadModrm32(disasm) & imm32;
    WriteModrm32(disasm, result);

    UpdateFlagsLogic32(result);

    if constexpr (canDisassemble)
        printf("and %s, 0x%08x\n", disasm.c_str(), imm32);
}

void TigerLake::CmpRm32Imm32()
{
    uint32_t imm32 = ReadImm32();
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    
    uint64_t result = (uint64_t)rm32 - (uint64_t)imm32;

    UpdateFlagsSub32(rm32, imm32, result);

    if constexpr (canDisassemble)
        printf("cmp %s, 0x%08x\n", disasm.c_str(), imm32);
}

void TigerLake::Code83_32()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x00:
        AddRm32Imm8();
        break;
    case 0x01:
        OrRm32Imm8();
        break;
    case 0x04:
        AndRm32Imm8();
        break;
    case 0x05:
        SubRm32Imm8();
        break;
    case 0x07:
        CmpRm32Imm8();
        break;
    default:
        printf("Error: Unknown opcode 0x83 0x%02x\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::AddRm32Imm8()
{
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint8_t imm8 = ReadImm8(true);

    uint64_t result = (uint64_t)rm32 + (uint64_t)imm8;

    UpdateFlagsAdd32(rm32, (uint32_t)imm8, result);

    WriteModrm32(disasm, result);

    if constexpr (canDisassemble)
        printf("add %s, 0x%02x\n", disasm.c_str(), imm8);
}

void TigerLake::OrRm32Imm8()
{
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint8_t imm8 = ReadImm8(true);

    rm32 |= imm8;
    UpdateFlagsLogic32(rm32);

    WriteModrm32(disasm, rm32);

    if constexpr (canDisassemble)
        printf("or %s, 0x%02x\n", disasm.c_str(), imm8);
}

void TigerLake::AndRm32Imm8()
{
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint8_t imm8 = ReadImm8(true);

    rm32 &= imm8;
    UpdateFlagsLogic32(rm32);

    WriteModrm32(disasm, rm32);

    if constexpr (canDisassemble)
        printf("and %s, 0x%02x\n", disasm.c_str(), imm8);
}

void TigerLake::SubRm32Imm8()
{
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint8_t imm8 = ReadImm8(true);

    uint64_t result = (uint64_t)rm32 - (uint64_t)imm8;

    UpdateFlagsSub32(rm32, (uint32_t)imm8, result);

    WriteModrm32(disasm, result);

    if constexpr (canDisassemble)
        printf("sub %s, 0x%02x\n", disasm.c_str(), imm8);
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

void TigerLake::TestRm32R32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t r32 = regs[modrm.reg].reg32;

    uint32_t result = rm32 & r32;

    UpdateFlagsLogic32(result);

    if constexpr (canDisassemble)
        printf("test %s, %s\n", disasm.c_str(), Reg32[modrm.reg]);
}

void TigerLake::MovRm32R32()
{
    FetchModrm();

    std::string disasm;
    WriteModrm32(disasm, regs[modrm.reg].reg32);

    if constexpr (canDisassemble)
        printf("mov %s, %s\n", disasm.c_str(), Reg32[modrm.reg]);
}

void TigerLake::MovR32Rm32()
{
    FetchModrm();

    std::string disasm;
    regs[modrm.reg].reg32 = ReadModrm32(disasm);

    if constexpr (canDisassemble)
        printf("mov %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void TigerLake::LeaR32M()
{
    FetchModrm();

    std::string disasm;
    regs[modrm.reg].reg32 = DecodeModrmAddr(disasm);

    if constexpr (canDisassemble)
        printf("lea %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void TigerLake::MovR32Imm32()
{
    uint8_t reg = Bus::Read8(TranslateAddr(CS, rip-1)) - 0xB8;
    uint32_t imm32 = ReadImm32(true);

    regs[reg].reg32 = imm32;

    if constexpr (canDisassemble)
        printf("mov %s, 0x%08x\n", Reg32[reg], imm32);
}

void TigerLake::CodeC1_32()
{
    
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x05:
        ShrRm32Imm8();
        break;
    default:
        printf("Error: Unknown opcode 0xC1 0x%02x\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::ShrRm32Imm8()
{
    uint8_t imm8 = ReadImm8(true) & 0x1F;
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);

    rflags.flag_bits.cf = (rm32 >> (imm8-1)) & 1;

    rm32 >>= imm8;

    if (imm8 == 1)
    {
        rflags.flag_bits.of = (rm32 >> 31) & 1;
    }

    rflags.flag_bits.sf = (rm32 >> 31) & 1;
    rflags.flag_bits.zf = (rm32 == 0);
    rflags.flag_bits.pf = CalcPF(rm32 & 0xFF);

    WriteModrm32(disasm, rm32);
    
    if constexpr (canDisassemble)
        printf("shr %s, 0x%02x\n", disasm.c_str(), imm8);
}

void TigerLake::Ret()
{
    if (mode == Mode::LongMode)
        rip = Pop64();
    else
        rip = Pop32();
    if constexpr (canDisassemble)
        printf("ret\n");
}

void TigerLake::CallRel32()
{
    int32_t rel32 = ReadImm32();

    if (mode == Mode::LongMode)
        Push64(rip);
    else
        Push32(rip);

    rip += rel32;

    if constexpr (canDisassemble)
        printf("call 0x%08lx\n", TranslateAddr(CS, rip));
}

void TigerLake::JmpRel32()
{
    int32_t rel32 = ReadImm32();
    rip += (int64_t)rel32;

    if constexpr (canDisassemble)
        printf("jmp 0x%08lx\n", TranslateAddr(CS, rip));
}

void TigerLake::JmpPtr1632()
{
    uint32_t ip = ReadImm32(true);
    uint16_t cs = ReadImm16(true);

    CacheSegment(CS, cs);
    rip = (uint64_t)ip;

    CheckModeChange();

    if constexpr (canDisassemble)
        printf("jmp 0x%04x:0x%08lx\n", cs, rip);
}

void TigerLake::CodeFF_32()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x04:
        JmpRm32();
        break;
    default:
        printf("Unknown opcode 0xFF 0x%02x\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::JmpRm32()
{
    std::string disasm;
    rip = (uint64_t)ReadModrm32(disasm);

    if constexpr (canDisassemble)
        printf("jmp %s\n", disasm.c_str());
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

// All modes, extended
void TigerLake::Code0f01()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x02:
        Lgdt();
        break;
    case 0x03:
        Lidt();
        break;
    default:
        printf("Unknown opcode 0x0f 0x01 0x%02x\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::Lgdt()
{
    std::string disasm;
    uint64_t address = TranslateAddr(prefix, DecodeModrmAddr(disasm));

    uint64_t base;
    uint16_t limit = Bus::Read16(address);

    if (mode != Mode::LongMode)
        base = Bus::Read32(address+2);
    else
        base = Read64(prefix, address+2);
    
    gdtr.limit = limit;
    gdtr.base = base;

    if constexpr (canDisassemble)
        printf("lgdt %s\n", disasm.c_str());
}

void TigerLake::Lidt()
{
    std::string disasm;
    uint64_t address = TranslateAddr(prefix, DecodeModrmAddr(disasm));

    uint64_t base;
    uint16_t limit = Bus::Read16(address);

    if (mode != Mode::LongMode)
        base = Bus::Read32(address+2);
    else
        base = Read64(prefix, address+2);
    
    idtr.limit = limit;
    idtr.base = base;

    if constexpr (canDisassemble)
        printf("lidt %s\n", disasm.c_str());
}

void TigerLake::MovR64CRn()
{
    FetchModrm();

    regs[modrm.rm].reg64 = cr[modrm.reg];

    if constexpr (canDisassemble)
        printf("mov %s, cr%d\n", (mode == Mode::LongMode) ? Reg64[modrm.rm] : Reg32[modrm.rm], modrm.reg);
}

void TigerLake::MovCRnR64()
{
    FetchModrm();

    cr[modrm.reg] = regs[modrm.rm].reg64;

    if constexpr (canDisassemble)
        printf("mov cr%d, %s\n", modrm.reg, (mode == Mode::LongMode) ? Reg64[modrm.rm] : Reg32[modrm.rm]);
}

void TigerLake::SetzRm8()
{
    FetchModrm();

    std::string disasm;

    if (rflags.flag_bits.zf)
        WriteModrm8(disasm, 1);
    else
        WriteModrm8(disasm, 0);
    
    if constexpr (canDisassemble)
        printf("setz %s\n", disasm.c_str());
}

// 32-bit mode, extended

void TigerLake::JzRel32()
{
    int32_t rel32 = ReadImm32(true);

    if constexpr (canDisassemble)
        printf("jz 0x%08lx\n", TranslateAddr(CS, rip+(int64_t)rel32));

    if (rflags.flag_bits.zf)
        rip += (int64_t)rel32;
}

void TigerLake::JnzRel32()
{
    int32_t rel32 = ReadImm32(true);

    if constexpr (canDisassemble)
        printf("jz 0x%08lx\n", TranslateAddr(CS, rip+(int64_t)rel32));

    if (!rflags.flag_bits.zf)
        rip += (int64_t)rel32;
}

void TigerLake::MovzxR32Rm8()
{
    FetchModrm();

    std::string disasm;
    regs[modrm.reg].reg32 = ReadModrm8(disasm);

    if constexpr (canDisassemble)
        printf("movzx %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void TigerLake::MovsxR32Rm8()
{
    FetchModrm();

    std::string disasm;
    regs[modrm.reg].reg32 = (int32_t)(int8_t)ReadModrm8(disasm);

    if constexpr (canDisassemble)
        printf("movsx %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}
