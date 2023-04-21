#include "x86.h"
#include "IOBus.h"
#include <cstdio>

void IvyBridge::MakeOpcodeTables()
{
    lookup16[0x24] = std::bind(&IvyBridge::AndAlImm8, this);
    lookup16[0x72] = std::bind(&IvyBridge::JcRel8, this);
    lookup16[0x73] = std::bind(&IvyBridge::JncRel8, this);
    lookup16[0x74] = std::bind(&IvyBridge::JzRel8, this);
    lookup16[0x75] = std::bind(&IvyBridge::JnzRel8, this);
    lookup16[0x7C] = std::bind(&IvyBridge::JlRel8, this);
    lookup16[0x80] = std::bind(&IvyBridge::Code80, this);
    lookup16[0x81] = std::bind(&IvyBridge::Code81_16, this);
    lookup16[0x84] = std::bind(&IvyBridge::TestRm8R8, this);
    lookup16[0x8A] = std::bind(&IvyBridge::MovR8Rm8, this);
    lookup16[0x8B] = std::bind(&IvyBridge::MovR16Rm16, this);
    lookup16[0x8E] = std::bind(&IvyBridge::MovSregRm16, this);
    lookup16[0x90] = std::bind(&IvyBridge::Nop, this);
    lookup16[0xA8] = std::bind(&IvyBridge::TestAlImm8, this);
    for (int i = 0; i < 8; i++)
        lookup16[0xB8 + i] = std::bind(&IvyBridge::MovR16Imm16, this);
    lookup16[0xC1] = std::bind(&IvyBridge::CodeC1_16, this);
    lookup16[0xC6] = std::bind(&IvyBridge::MovRm8Imm8, this);
    lookup16[0xC7] = std::bind(&IvyBridge::MovRm16Imm16, this);
    lookup16[0xE9] = std::bind(&IvyBridge::JmpRel16, this);
    lookup16[0xEB] = std::bind(&IvyBridge::JmpRel8, this);
    lookup16[0xED] = std::bind(&IvyBridge::InAxDx, this);
    lookup16[0xF6] = std::bind(&IvyBridge::TestRm8Imm8, this);
    lookup16[0xFC] = std::bind(&IvyBridge::Cld, this);
    lookup16[0xFA] = std::bind(&IvyBridge::Cli, this);

    lookup32[0x01] = std::bind(&IvyBridge::AddRm32R32, this);
    lookup32[0x03] = std::bind(&IvyBridge::AddR32Rm32, this);
    lookup32[0x05] = std::bind(&IvyBridge::AddEaxImm32, this);
    lookup32[0x09] = std::bind(&IvyBridge::OrRm32R32, this);
    lookup32[0x24] = std::bind(&IvyBridge::AndAlImm8, this);
    lookup32[0x2D] = std::bind(&IvyBridge::SubEaxImm32, this);
    lookup32[0x31] = std::bind(&IvyBridge::XorRm32R32, this);
    lookup32[0x39] = std::bind(&IvyBridge::CmpRm32R32, this);
    lookup32[0x3D] = std::bind(&IvyBridge::CmpEaxImm32, this);
    for (int i = 0; i < 8; i++)
        lookup32[0x40+i] = std::bind(&IvyBridge::IncR32, this);
    for (int i = 0; i < 8; i++)
        lookup32[0x48+i] = std::bind(&IvyBridge::DecR32, this);
    for (int i = 0; i < 8; i++)
        lookup32[0x50+i] = std::bind(&IvyBridge::PushR32, this);
    for (int i = 0; i < 8; i++)
        lookup32[0x58+i] = std::bind(&IvyBridge::PopR32, this);
    lookup32[0x72] = std::bind(&IvyBridge::JcRel8, this);
    lookup32[0x73] = std::bind(&IvyBridge::JncRel8, this);
    lookup32[0x74] = std::bind(&IvyBridge::JzRel8, this);
    lookup32[0x75] = std::bind(&IvyBridge::JnzRel8, this);
    lookup32[0x7C] = std::bind(&IvyBridge::JlRel8, this);
    lookup32[0x80] = std::bind(&IvyBridge::Code80, this);
    lookup32[0x81] = std::bind(&IvyBridge::Code81_32, this);
    lookup32[0x83] = std::bind(&IvyBridge::Code83_32, this);
    lookup32[0x84] = std::bind(&IvyBridge::TestRm8R8, this);
    lookup32[0x85] = std::bind(&IvyBridge::TestRm32R32, this);
    lookup32[0x88] = std::bind(&IvyBridge::MovRm8R8, this);
    lookup32[0x89] = std::bind(&IvyBridge::MovRm32R32, this);
    lookup32[0x8A] = std::bind(&IvyBridge::MovR8Rm8, this);
    lookup32[0x8B] = std::bind(&IvyBridge::MovR32Rm32, this);
    lookup32[0x8E] = std::bind(&IvyBridge::MovSregRm16, this);
    lookup32[0x90] = std::bind(&IvyBridge::Nop, this);
    lookup32[0x9C] = std::bind(&IvyBridge::Pushfq, this);
    lookup32[0xA8] = std::bind(&IvyBridge::TestAlImm8, this);
    for (int i = 0; i < 8; i++)
        lookup32[0xB0 + i] = std::bind(&IvyBridge::MovR8Imm8, this);
    for (int i = 0; i < 8; i++)
        lookup32[0xB8 + i] = std::bind(&IvyBridge::MovR32Imm32, this);
    lookup32[0xC6] = std::bind(&IvyBridge::MovRm8Imm8, this);
    lookup32[0xC1] = std::bind(&IvyBridge::CodeC1_32, this);
    lookup32[0xC3] = std::bind(&IvyBridge::Ret, this);
    lookup32[0xC7] = std::bind(&IvyBridge::MovRm32Imm32, this);
    lookup32[0xE2] = std::bind(&IvyBridge::LoopEcxRel8, this);
    lookup32[0xE8] = std::bind(&IvyBridge::CallRel32, this);
    lookup32[0xE9] = std::bind(&IvyBridge::JmpRel32, this);
    lookup32[0xEA] = std::bind(&IvyBridge::JmpPtr1632, this);
    lookup32[0xEB] = std::bind(&IvyBridge::JmpRel8, this);
    lookup32[0xED] = std::bind(&IvyBridge::InEaxDx, this);
    lookup32[0xEF] = std::bind(&IvyBridge::OutEaxDx, this);
    lookup32[0xF6] = std::bind(&IvyBridge::TestRm8Imm8, this);
    lookup32[0xFA] = std::bind(&IvyBridge::Cli, this);
    lookup32[0xFC] = std::bind(&IvyBridge::Cld, this);
    lookup32[0xFF] = std::bind(&IvyBridge::CodeFF_32, this);
    
    lookup64[0x21] = std::bind(&IvyBridge::AndRm64R64, this);
    lookup64[0x24] = std::bind(&IvyBridge::AndAlImm8, this);
    lookup64[0x39] = std::bind(&IvyBridge::CmpRm64R64, this);
    lookup64[0x3D] = std::bind(&IvyBridge::CmpRaxImm32, this);
    lookup64[0x72] = std::bind(&IvyBridge::JcRel8, this);
    lookup64[0x73] = std::bind(&IvyBridge::JncRel8, this);
    lookup64[0x74] = std::bind(&IvyBridge::JzRel8, this);
    lookup64[0x75] = std::bind(&IvyBridge::JnzRel8, this);
    lookup64[0x7C] = std::bind(&IvyBridge::JlRel8, this);
    lookup64[0x80] = std::bind(&IvyBridge::Code80, this);
    lookup64[0x81] = std::bind(&IvyBridge::Code81_64, this);
    lookup64[0x83] = std::bind(&IvyBridge::Code83_64, this);
    lookup64[0x84] = std::bind(&IvyBridge::TestRm8R8, this);
    lookup64[0x89] = std::bind(&IvyBridge::MovRm64R64, this);
    lookup64[0x8A] = std::bind(&IvyBridge::MovR8Rm8, this);
    lookup64[0x8D] = std::bind(&IvyBridge::LeaR64M, this);
    lookup64[0x8E] = std::bind(&IvyBridge::MovSregRm16, this);
    lookup64[0x90] = std::bind(&IvyBridge::Nop, this);
    lookup64[0x9C] = std::bind(&IvyBridge::Pushfq, this);
    lookup64[0xA8] = std::bind(&IvyBridge::TestAlImm8, this);
    lookup64[0xAB] = std::bind(&IvyBridge::Stosq, this);
    for (int i = 0; i < 8; i++)
        lookup64[0xB8 + i] = std::bind(&IvyBridge::MovR64Imm64, this);
    lookup64[0xC6] = std::bind(&IvyBridge::MovRm8Imm8, this);
    lookup64[0xC7] = std::bind(&IvyBridge::MovRm64Imm32, this);
    lookup64[0xE8] = std::bind(&IvyBridge::CallRel32, this);
    lookup64[0xE9] = std::bind(&IvyBridge::JmpRel32, this);
    lookup64[0xEB] = std::bind(&IvyBridge::JmpRel8, this);
    lookup64[0xF6] = std::bind(&IvyBridge::TestRm8Imm8, this);
    lookup64[0xFA] = std::bind(&IvyBridge::Cli, this);
    lookup64[0xFC] = std::bind(&IvyBridge::Cld, this);
    lookup64[0xFF] = std::bind(&IvyBridge::CodeFF_64, this);

    extLookup16[0x01] = std::bind(&IvyBridge::Code0f01, this);
    extLookup16[0x20] = std::bind(&IvyBridge::MovR64CRn, this);
    extLookup16[0x22] = std::bind(&IvyBridge::MovCRnR64, this);
    
    extLookup32[0x01] = std::bind(&IvyBridge::Code0f01, this);
    extLookup32[0x20] = std::bind(&IvyBridge::MovR64CRn, this);
    extLookup32[0x22] = std::bind(&IvyBridge::MovCRnR64, this);
    extLookup32[0x30] = std::bind(&IvyBridge::Wrmsr, this);
    extLookup32[0x32] = std::bind(&IvyBridge::Rdmsr, this);
    extLookup32[0x84] = std::bind(&IvyBridge::JzRel32, this);
    extLookup32[0xA2] = std::bind(&IvyBridge::Cpuid, this);
    extLookup32[0xA3] = std::bind(&IvyBridge::BtRm32R32, this);
    extLookup32[0xB7] = std::bind(&IvyBridge::MovzxR32Rm16, this);
    extLookup32[0xBA] = std::bind(&IvyBridge::Code0FBA_32, this);
    
    extLookup64[0x01] = std::bind(&IvyBridge::Code0f01, this);
    extLookup64[0x1E] = std::bind(&IvyBridge::HintNop, this);
    extLookup64[0x84] = std::bind(&IvyBridge::JzRel32, this);
    extLookup64[0x85] = std::bind(&IvyBridge::JnzRel32, this);
    extLookup64[0x20] = std::bind(&IvyBridge::MovR64CRn, this);
    extLookup64[0x22] = std::bind(&IvyBridge::MovCRnR64, this);
    extLookup64[0xB6] = std::bind(&IvyBridge::MovzxR64Rm8, this);
}

bool CalcPF(uint8_t result)
{
    int pf_cnt = 0;
    for (int i = 0; i < 8; i++)
        if ((result >> i) & 1)
            pf_cnt++;
    return (pf_cnt % 2) == 0;
}

void IvyBridge::UpdateFlagsLogic8(uint8_t result)
{
    rflags.flag_bits.zf = (result == 0);
    rflags.flag_bits.sf = (result >> 7) & 1;
    rflags.flag_bits.pf = CalcPF(result);
    rflags.flag_bits.cf = 0;
    rflags.flag_bits.of = 0;
}

void IvyBridge::UpdateFlagsLogic32(uint32_t result)
{
    rflags.flag_bits.zf = (result == 0);
    rflags.flag_bits.sf = (result >> 31) & 1;
    rflags.flag_bits.pf = CalcPF(result & 0xff);
    rflags.flag_bits.cf = 0;
    rflags.flag_bits.of = 0;
}

void IvyBridge::UpdateFlagsLogic64(uint64_t result)
{
    rflags.flag_bits.zf = (result == 0);
    rflags.flag_bits.sf = (result >> 63) & 1;
    rflags.flag_bits.pf = CalcPF(result & 0xff);
    rflags.flag_bits.cf = 0;
    rflags.flag_bits.of = 0;
}

void IvyBridge::UpdateFlagsSub8(uint8_t value1, uint8_t value2, uint32_t result)
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

void IvyBridge::UpdateFlagsSub16(uint16_t value1, uint16_t value2, uint32_t result)
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

void IvyBridge::UpdateFlagsSub32(uint32_t value1, uint32_t value2, uint64_t result)
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

void IvyBridge::UpdateFlagsSub64(uint64_t value1, uint64_t value2, uint64_t result)
{
    bool sign1 = value1 >> 31;
    bool sign2 = value2 >> 31;
    bool signr = (result >> 31) & 1;

    rflags.flag_bits.cf = value2 > value1;
    rflags.flag_bits.zf = (result == 0);
    rflags.flag_bits.sf = signr;
    rflags.flag_bits.of = (sign1 != sign2 && sign1 != signr);
    rflags.flag_bits.pf = CalcPF(result & 0xff);
}

void IvyBridge::UpdateFlagsAdd32(uint32_t value1, uint32_t value2, uint64_t result)
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

void IvyBridge::UpdateFlagsAdd64(uint64_t value1, uint64_t value2, uint64_t result)
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

void IvyBridge::AndAlImm8()
{
    uint8_t imm8 = ReadImm8(true);
    regs[RAX].lo &= imm8;

    UpdateFlagsLogic8(regs[RAX].lo);

    if (canDisassemble)
        printf("and al, 0x%02x\n", imm8);
}

void IvyBridge::JcRel8()
{
    int8_t rel8 = ReadImm8(true);

    if (canDisassemble)
        printf("jc 0x%08lx\n", TranslateAddr(CS, rip + (int64_t)rel8));

    if (rflags.flag_bits.cf)
        rip += (int64_t)rel8;
}

void IvyBridge::JncRel8()
{
    int8_t rel8 = ReadImm8(true);

    if (canDisassemble)
        printf("jnc 0x%08lx\n", TranslateAddr(CS, rip + (int64_t)rel8));

    if (!rflags.flag_bits.cf)
        rip += (int64_t)rel8;
}

void IvyBridge::JzRel8()
{
    int8_t rel8 = ReadImm8(true);

    if (canDisassemble)
        printf("jz 0x%08lx\n", TranslateAddr(CS, rip + (int64_t)rel8));

    if (rflags.flag_bits.zf)
        rip += (int64_t)rel8;
}

void IvyBridge::JnzRel8()
{
    int8_t rel8 = ReadImm8(true);

    if (canDisassemble)
        printf("jnz 0x%08lx\n", TranslateAddr(CS, rip + (int64_t)rel8));

    if (!rflags.flag_bits.zf)
        rip += (int64_t)rel8;
}

void IvyBridge::JlRel8()
{
    int8_t rel8 = ReadImm8(true);

    if (canDisassemble)
        printf("jl 0x%08lx\n", TranslateAddr(CS, rip + (int64_t)rel8));

    if (rflags.flag_bits.sf != rflags.flag_bits.of)
        rip += (int64_t)rel8;
}

void IvyBridge::Code80()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x07:
        CmpRm8Imm8();
        break;
    default:
        printf("Unknown opcode 0x80 0x%02x\n", modrm.reg);
        exit(1);
    }
}

void IvyBridge::CmpRm8Imm8()
{
    std::string disasm;
    uint8_t rm8 = ReadModrm8(disasm);
    uint8_t imm8 = ReadImm8(true);

    uint16_t result = (uint16_t)rm8 - (uint16_t)imm8;

    UpdateFlagsSub8(rm8, imm8, result);

    if (canDisassemble)
        printf("cmp %s, 0x%02x\n", disasm.c_str(), imm8);
}

void IvyBridge::TestRm8R8()
{
    FetchModrm();

    std::string disasm;
    uint8_t rm8 = ReadModrm8(disasm);
    uint8_t r8 = ReadReg8((Registers)modrm.reg);

    uint8_t result = r8 & rm8;

    UpdateFlagsLogic8(result);

    if (canDisassemble)
        printf("test %s, %s\n", disasm.c_str(), Reg8[modrm.reg]);
}

void IvyBridge::MovRm8R8()
{
    FetchModrm();

    std::string disasm;
    uint8_t r8 = ReadReg8((Registers)modrm.reg);

    WriteModrm8(disasm, r8);

    if (canDisassemble)
        printf("mov %s, %s\n", disasm.c_str(), Reg8[modrm.reg]);
}

void IvyBridge::MovR8Rm8()
{
    FetchModrm();

    std::string disasm;
    uint8_t rm8 = ReadModrm8(disasm);

    WriteReg8((Registers)modrm.reg, rm8);

    if (canDisassemble)
        printf("mov %s, %s\n", Reg8[modrm.reg], disasm.c_str());
}

void IvyBridge::MovSregRm16()
{
    FetchModrm();

    std::string disasm;
    uint16_t rm16 = ReadModrm16(disasm);

    CacheSegment((Segments)modrm.reg, rm16);

    if (canDisassemble)
        printf("mov %s, %s\n", Segs[modrm.reg], disasm.c_str());
}

void IvyBridge::Nop()
{
    if (canDisassemble)
        printf("nop\n");
}

void IvyBridge::TestAlImm8()
{
    uint8_t imm8 = ReadImm8(true);
    uint8_t al = regs[RAX].lo;

    uint8_t result = al & imm8;

    UpdateFlagsLogic8(result);

    if (canDisassemble)
        printf("test al, 0x%02x\n", imm8);
}

void IvyBridge::MovRm8Imm8()
{
    FetchModrm();

    uint8_t imm8 = ReadImm8(true);

    std::string disasm;
    WriteModrm8(disasm, imm8);

    if (canDisassemble)
        printf("mov %s, 0x%02x\n", disasm.c_str(), imm8);
}

void IvyBridge::JmpRel8()
{
    int8_t rel8 = ReadImm8(true);

    rip += (int64_t)rel8;

    printf("jmp 0x%08lx\n", TranslateAddr(CS, rip));
}

void IvyBridge::TestRm8Imm8()
{
    FetchModrm();

    std::string disasm;
    uint8_t rm8 = ReadModrm8(disasm);
    uint8_t imm8 = ReadImm8(true);

    uint8_t result = rm8 & imm8;

    UpdateFlagsLogic8(result);

    if (canDisassemble)
        printf("test %s, 0x%02x\n", disasm.c_str(), imm8);
}

void IvyBridge::Cld()
{
    rflags.flag_bits.df = 0;
    if (canDisassemble)
        printf("cld\n");
}

void IvyBridge::Cli()
{
    rflags.flag_bits.if_ = 0;
    if (canDisassemble)
        printf("cli\n");
}

void IvyBridge::Code81_16()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x07:
        CmpRm16Imm16();
        break;
    default:
        printf("Unknown (16) opcode 0x81 0x%02x\n", modrm.reg);
        exit(1);
    }
}

void IvyBridge::CmpRm16Imm16()
{
    std::string disasm;
    uint16_t rm16 = ReadModrm16(disasm);
    uint16_t imm16 = ReadImm16(true);

    uint32_t result = (uint32_t)rm16 - (uint32_t)imm16;

    UpdateFlagsSub16(rm16, imm16, result);

    if (canDisassemble)
        printf("cmp %s, 0x%04x\n", disasm.c_str(), imm16);
}

void IvyBridge::MovR16Rm16()
{
    FetchModrm();

    std::string disasm;
    regs[modrm.reg].reg16 = ReadModrm16(disasm);

    if (canDisassemble)
        printf("mov %s, %s\n", Reg16[modrm.reg], disasm.c_str());
}

void IvyBridge::MovR16Imm16()
{
    uint8_t reg = l1->Read8(TranslateAddr(CS, rip-1), false) - 0xB8;
    uint16_t imm16 = ReadImm16(true);

    regs[reg].reg16 = imm16;

    if (canDisassemble)
        printf("mov %s, 0x%04x\n", Reg16[reg], imm16);
}

void IvyBridge::CodeC1_16()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x05:
        ShrRm16Imm8();
        break;
    default:
        printf("Unknown opcode 0xC1 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void IvyBridge::ShrRm16Imm8()
{
    std::string disasm;
    uint16_t rm16 = ReadModrm16(disasm);
    uint8_t imm8 = ReadImm8(true) & 0x1F;

    uint32_t result = rm16 >> imm8;
    WriteModrm16(disasm, result);

    rflags.flag_bits.cf = ((rm16 << (imm8-1)) & 1);
    rflags.flag_bits.pf = CalcPF(result);
    rflags.flag_bits.zf = !(result);
    rflags.flag_bits.sf = (result >> 15) & 1;

    if (imm8 == 1)
        rflags.flag_bits.of = ((rm16 >> 15) & 1) ^ ((rm16 >> 14) & 1);
    
    if (canDisassemble)
        printf("shr %s, 0x%02x\n", disasm.c_str(), imm8);
}

void IvyBridge::MovRm16Imm16()
{
    FetchModrm();

    uint16_t imm16 = ReadImm16(true);

    std::string disasm;
    WriteModrm16(disasm, imm16);

    if (canDisassemble)
        printf("mov %s, 0x%04x\n", disasm.c_str(), imm16);
}

void IvyBridge::JmpRel16()
{
    int16_t rel16 = ReadImm16(true);

    rip += (int64_t)rel16;

    printf("jmp 0x%08lx\n", TranslateAddr(CS, rip));
}

void IvyBridge::InAxDx()
{
    regs[RAX].reg16 = IOBus::In16(regs[RDX].reg16);

    if (canDisassemble)
        printf("in ax, dx\n");
}

void IvyBridge::AddRm32R32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t r32 = regs[modrm.reg].reg32;

    uint64_t result = (uint64_t)rm32 + (uint64_t)r32;

    UpdateFlagsAdd32(rm32, r32, result);

    WriteModrm32(disasm, result);

    if (canDisassemble)
        printf("add %s, %s\n", disasm.c_str(), Reg32[modrm.reg]);
}

void IvyBridge::AddR32Rm32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t r32 = regs[modrm.reg].reg32;

    uint64_t result = (uint64_t)r32 + (uint64_t)rm32;

    UpdateFlagsAdd32(r32, rm32, result);

    regs[modrm.reg].reg32 = (uint64_t)result;

    if (canDisassemble)
        printf("add %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void IvyBridge::AddEaxImm32()
{
    uint32_t imm32 = ReadImm32(true);
    uint32_t eax = regs[RAX].reg32;

    uint64_t result = (uint64_t)eax + (uint64_t)imm32;

    UpdateFlagsAdd32(eax, imm32, result);

    regs[RAX].reg32 = (uint32_t)result;

    if (canDisassemble)
        printf("add eax, 0x%08x\n", imm32);
}

void IvyBridge::OrRm32R32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t r32 = regs[modrm.reg].reg32;

    uint32_t result = rm32 | r32;

    UpdateFlagsLogic32(result);

    WriteModrm32(disasm, result);

    if (canDisassemble)
        printf("or %s, %s\n", disasm.c_str(), Reg32[modrm.reg]);
}

void IvyBridge::SubEaxImm32()
{
    uint32_t imm32 = ReadImm32(true);
    uint32_t eax = regs[RAX].reg32;

    uint64_t result = (uint64_t)eax - (uint64_t)imm32;

    UpdateFlagsSub32(eax, imm32, result);

    regs[RAX].reg32 = (uint32_t)result;

    if (canDisassemble)
        printf("sub eax, 0x%08x\n", imm32);
}

void IvyBridge::XorRm32R32()
{
    FetchModrm();

    std::string disasm;

    uint32_t r32 = regs[modrm.reg].reg32;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t result = rm32 ^ r32;
    WriteModrm32(disasm, result);
    UpdateFlagsLogic32(result);

    if (canDisassemble)
        printf("xor %s, %s\n", disasm.c_str(), Reg32[modrm.reg]);
}

void IvyBridge::CmpRm32R32()
{
    FetchModrm();

    std::string disasm;

    uint32_t r32 = regs[modrm.reg].reg32;
    uint32_t rm32 = ReadModrm32(disasm);
    uint64_t result = (uint64_t)rm32 - (uint64_t)r32;
    
    UpdateFlagsSub32(rm32, r32, result);

    if (canDisassemble)
        printf("cmp %s, %s\n", disasm.c_str(), Reg32[modrm.reg]);
}

void IvyBridge::CmpEaxImm32()
{
    uint32_t imm32 = ReadImm32(true);
    uint32_t eax = regs[RAX].reg32;

    uint64_t result = (uint64_t)eax - (uint64_t)imm32;

    UpdateFlagsSub32(eax, imm32, result);

    if (canDisassemble)
        printf("cmp eax, 0x%08x\n", imm32);
}

void IvyBridge::IncR32()
{
    uint8_t reg = l1->Read8(TranslateAddr(CS, rip-1), false) - 0x40;
    
    uint64_t result = (uint64_t)regs[reg].reg32 + 1UL;

    UpdateFlagsAdd32(regs[reg].reg32, 1, result);

    regs[reg].reg32 = result;

    if (canDisassemble)
        printf("inc %s\n", Reg32[reg]);
}

void IvyBridge::DecR32()
{
    uint8_t reg = l1->Read8(TranslateAddr(CS, rip-1), false) - 0x48;
    
    uint64_t result = (uint64_t)regs[reg].reg32 - 1UL;

    UpdateFlagsSub32(regs[reg].reg32, 1, result);

    regs[reg].reg32 = result;

    if (canDisassemble)
        printf("dec %s\n", Reg32[reg]);
}

void IvyBridge::PushR32()
{
    uint8_t reg = l1->Read8(TranslateAddr(CS, rip-1), false) - 0x50;
    if (rex.b) reg += 8;

    if (mode != Mode::LongMode)
    {
        Push32(regs[reg].reg32);

        if (canDisassemble)
            printf("push %s\n", Reg32[reg]);
    }
    else
    {
        Push64(regs[reg].reg64);

        if (canDisassemble)
            printf("push %s\n", Reg64[reg]);
    }
}

void IvyBridge::PopR32()
{
    uint8_t reg = l1->Read8(TranslateAddr(CS, rip-1), false) - 0x58;
    if (rex.b) reg += 8;
    
    if (mode == Mode::LongMode)
    {
        regs[reg].reg64 = Pop64();

        if (canDisassemble)
            printf("pop %s\n", Reg64[reg]);
    }
    else
    {
        regs[reg].reg32 = Pop32();

        if (canDisassemble)
            printf("pop %s\n", Reg32[reg]);
    }
}

void IvyBridge::Code81_32()
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
        printf("Unknown opcode 0x81 0x%02x\n", modrm.reg);
        exit(1);
    }
}

void IvyBridge::AndRm32Imm32()
{
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t imm32 = ReadImm32(true);

    uint32_t result = rm32 & imm32;

    UpdateFlagsLogic32(result);

    WriteModrm32(disasm, result);

    if (canDisassemble)
        printf("and %s, 0x%08x\n", disasm.c_str(), imm32);
}

void IvyBridge::CmpRm32Imm32()
{
    std::string disasm;
    uint32_t imm32 = ReadImm32(true);
    uint32_t rm32 = ReadModrm32(disasm);

    uint64_t result = (uint64_t)rm32 - (uint64_t)imm32;

    UpdateFlagsSub32(rm32, imm32, result);

    if (canDisassemble)
        printf("cmp %s, 0x%08x\n", disasm.c_str(), imm32);
}

void IvyBridge::Code83_32()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x00:
        AddRm32Imm8();
        break;
    case 0x04:
        AndRm32Imm8();
        break;
    case 0x07:
        CmpRm32Imm8();
        break;
    default:
        printf("Unknown opcode 0x83 0x%02x\n", modrm.reg);
        exit(1);
    }
}

void IvyBridge::AddRm32Imm8()
{
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t imm32 = (uint32_t)ReadImm8(true);

    uint64_t result = (uint64_t)rm32 + (uint64_t)imm32;

    UpdateFlagsAdd32(rm32, imm32, result);

    WriteModrm32(disasm, result);

    if (canDisassemble)
        printf("add %s, 0x%02x\n", disasm.c_str(), imm32);
}

void IvyBridge::AndRm32Imm8()
{
    std::string disasm;
    uint32_t imm32 = (uint32_t)ReadImm8(true);
    uint32_t rm32 = ReadModrm32(disasm);

    uint32_t result = rm32 & imm32;

    UpdateFlagsLogic32(result);

    WriteModrm32(disasm, result);

    if (canDisassemble)
        printf("and %s, 0x%02x\n", disasm.c_str(), imm32);
}

void IvyBridge::CmpRm32Imm8()
{
    std::string disasm;
    uint32_t imm32 = (uint32_t)ReadImm8(true);
    uint32_t rm32 = ReadModrm32(disasm);

    uint64_t result = (uint64_t)rm32 - (uint64_t)imm32;

    UpdateFlagsSub32(rm32, imm32, result);

    if (canDisassemble)
        printf("cmp %s, 0x%02x\n", disasm.c_str(), imm32);
}

void IvyBridge::TestRm32R32()
{
    std::string disasm;
    FetchModrm();
    
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t r32 = regs[modrm.reg].reg32;

    uint32_t result = rm32 & r32;

    UpdateFlagsLogic32(result);

    if (canDisassemble)
        printf("test %s, %s\n", disasm.c_str(), Reg32[modrm.reg]);
}

void IvyBridge::MovRm32R32()
{
    std::string disasm;
    FetchModrm();
    
    WriteModrm32(disasm, regs[modrm.reg].reg32);

    if (canDisassemble)
        printf("mov %s, %s\n", disasm.c_str(), Reg32[modrm.reg]);
}

void IvyBridge::MovR32Rm32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);

    regs[modrm.reg].reg32 = rm32;

    if (canDisassemble)
        printf("mov %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void IvyBridge::BtRm32R32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t r32 = regs[modrm.reg].reg32;

    rflags.flag_bits.cf = rm32 & (1 << r32);

    if (canDisassemble)
        printf("bt %s, %s\n", disasm.c_str(), Reg32[modrm.reg]);
}

void IvyBridge::MovR8Imm8()
{
    uint8_t reg = l1->Read8(TranslateAddr(CS, rip-1), false) - 0xB0;
    if (rex.b) reg += 8;

    uint8_t imm8 = ReadImm8(true);

    WriteReg8(reg, imm8);

    if (canDisassemble)
        printf("mov %s, 0x%02x\n", Reg8[reg], imm8);
}

void IvyBridge::MovR32Imm32()
{
    uint8_t reg = l1->Read8(TranslateAddr(CS, rip-1), false) - 0xB8;
    if (rex.b) reg += 8;

    uint32_t imm32 = ReadImm32(true);

    regs[reg].reg64 = imm32;

    if (canDisassemble)
        printf("mov %s, 0x%08x\n", Reg32[reg], imm32);
}

void IvyBridge::CodeC1_32()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x04:
        ShlRm32Imm8();
        break;
    default:
        printf("Unknown 32-bit opcode 0xC1 0x%02x\n", modrm.reg);
        exit(1);
    }
}

void IvyBridge::ShlRm32Imm8()
{
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint8_t imm8 = ReadImm8(true) & 0x1F;

    uint32_t result = rm32 << imm8;
    WriteModrm32(disasm, result);

    rflags.flag_bits.cf = ((rm32 >> (32-imm8)) & 1);
    rflags.flag_bits.pf = CalcPF(result);
    rflags.flag_bits.zf = !(result);
    rflags.flag_bits.sf = (result >> 31) & 1;

    if (imm8 == 1)
        rflags.flag_bits.of = ((rm32 >> 31) & 1) ^ ((rm32 >> 30) & 1);
    
    if (canDisassemble)
        printf("shl %s, 0x%02x\n", disasm.c_str(), imm8);
}

void IvyBridge::Ret()
{
    rip = (mode == Mode::LongMode) ? Pop64() : Pop32();

    if (canDisassemble)
        printf("ret (0x%08lx)\n", TranslateAddr(CS, rip));
}

void IvyBridge::MovRm32Imm32()
{
    FetchModrm();
    uint32_t imm32 = ReadImm32(true);

    std::string disasm;
    WriteModrm32(disasm, imm32);

    if (canDisassemble)
        printf("mov %s, 0x%08x\n", disasm.c_str(), imm32);
}

void IvyBridge::LoopEcxRel8()
{
    int8_t rel8 = ReadImm8(true);

    regs[RCX].reg32--;

    if (canDisassemble)
        printf("loop 0x%08lx\n", TranslateAddr(CS, rip+(uint64_t)rel8));

    if (regs[RCX].reg32)
        rip += (int64_t)rel8;
}

void IvyBridge::CallRel32()
{
    int32_t rel32 = ReadImm32(true);

    (mode == Mode::LongMode) ? Push64(rip) : Push32(rip);

    rip += (int64_t)rel32;

    if (canDisassemble)
        printf("call 0x%08lx\n", TranslateAddr(CS, rip));
}

void IvyBridge::JmpRel32()
{
    int32_t rel32 = ReadImm32(true);

    rip += (int64_t)rel32;

    if (canDisassemble)
        printf("jmp 0x%08lx\n", TranslateAddr(CS, rip));
}

void IvyBridge::JmpPtr1632()
{
    uint32_t newRip = ReadImm32(true);
    uint16_t selector = ReadImm16(true);
    
    CheckModeChange();

    CacheSegment(CS, selector);
    rip = newRip;

    if (canDisassemble)
        printf("jmp 0x%02x:0x%08x\n", selector, newRip);
}

void IvyBridge::InEaxDx()
{
    regs[RAX].reg32 = IOBus::In32(regs[RDX].reg16);

    if (canDisassemble)
        printf("in eax, dx\n");
}

void IvyBridge::OutEaxDx()
{
    IOBus::Out32(regs[RDX].reg16, regs[RAX].reg32);

    if (canDisassemble)
        printf("out dx, eax\n");
}

void IvyBridge::CodeFF_32()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x04:
        JmpRm32();
        break;
    default:
        printf("Unknown opcode 0xFF 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void IvyBridge::JmpRm32()
{
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);

    rip = rm32;

    if (canDisassemble)
        printf("jmp %s\n", disasm.c_str());
}

void IvyBridge::AndRm64R64()
{
    FetchModrm();

    std::string disasm;
    uint64_t rm64 = ReadModrm64(disasm);
    uint64_t r64 = regs[modrm.reg].reg64;

    uint64_t result = rm64 & r64;

    UpdateFlagsLogic64(result);

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("and %s, %s\n", disasm.c_str(), Reg64[modrm.reg]);
}

void IvyBridge::CmpRm64R64()
{
    FetchModrm();

    std::string disasm;

    uint64_t r64 = regs[modrm.reg].reg64;
    uint64_t rm64 = ReadModrm64(disasm);

    UpdateFlagsSub64(rm64, r64, rm64 - r64);

    if (canDisassemble)
        printf("cmp %s, %s\n", disasm.c_str(), Reg64[modrm.reg]);
}

void IvyBridge::CmpRaxImm32()
{
    uint32_t imm32 = ReadImm32(true);

    uint64_t result = regs[RAX].reg64 - imm32;

    UpdateFlagsSub64(regs[RAX].reg64, imm32, result);

    if (canDisassemble)
        printf("cmp rax, 0x%08x\n", imm32);
}

void IvyBridge::Code81_64()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x05:
        SubRm64Imm32();
        break;
    default:
        printf("Unknown 64-bit opcode 0x81 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void IvyBridge::SubRm64Imm32()
{
    std::string disasm;
    uint32_t imm32 = ReadImm32(true);
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 - (uint64_t)imm32;
    UpdateFlagsSub64(rm64, (uint64_t)imm32, result);

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("sub %s, 0x%08x\n", disasm.c_str(), imm32);
}

void IvyBridge::Code83_64()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x00:
        AddRm64Imm8();
        break;
    case 0x05:
        SubRm64Imm8();
        break;
    case 0x07:
        CmpRm64Imm8();
        break;
    default:
        printf("Unknown 64-bit opcode 0x83 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void IvyBridge::AddRm64Imm8()
{
    std::string disasm;
    uint8_t imm8 = ReadImm8(true);
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 + (uint64_t)imm8;
    UpdateFlagsAdd64(rm64, (uint64_t)imm8, result);

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("add %s, 0x%02x\n", disasm.c_str(), imm8);
}

void IvyBridge::SubRm64Imm8()
{
    std::string disasm;
    uint8_t imm8 = ReadImm8(true);
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 - (uint64_t)imm8;
    UpdateFlagsSub64(rm64, (uint64_t)imm8, result);

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("sub %s, 0x%02x\n", disasm.c_str(), imm8);
}

void IvyBridge::CmpRm64Imm8()
{
    std::string disasm;
    uint8_t imm8 = ReadImm8(true);
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 - (uint64_t)imm8;
    UpdateFlagsSub64(rm64, (uint64_t)imm8, result);

    if (canDisassemble)
        printf("cmp %s, 0x%02x\n", disasm.c_str(), imm8);
}

void IvyBridge::MovRm64R64()
{
    FetchModrm();

    std::string disasm;
    uint64_t r64 = regs[modrm.reg].reg64;

    WriteModrm64(disasm, r64);

    if (canDisassemble)
        printf("mov %s, %s\n", disasm.c_str(), Reg64[modrm.reg]);
}

void IvyBridge::LeaR64M()
{
    FetchModrm();

    std::string disasm;
    regs[modrm.reg].reg64 = DecodeModrmAddr(disasm);

    if (canDisassemble)
        printf("lea %s, %s\n", Reg64[modrm.reg], disasm.c_str());
}

void IvyBridge::Pushfq()
{
    if (mode == Mode::LongMode)
        Push64(rflags.value);
    else
        Push32(rflags.value);

    if (canDisassemble)
        printf("pushf\n");
}

void IvyBridge::Stosq()
{
start:
    l1->Write64(TranslateAddr(DS, regs[RDI].reg64), regs[RAX].reg64);
    regs[RDI].reg64 += 8;

    if (rep)
    {
        regs[RCX].reg64--;
        if (regs[RCX].reg64)
            goto start;
    }

    if (canDisassemble)
        printf("stosq\n");
}

void IvyBridge::MovR64Imm64()
{
    uint8_t reg = l1->Read8(TranslateAddr(CS, rip-1), false) - 0xB8;
    uint64_t imm64 = ReadImm64();

    regs[reg].reg64 = imm64;

    if (canDisassemble)
        printf("mov %s, 0x%08lx\n", Reg64[reg], imm64);
}

void IvyBridge::MovRm64Imm32()
{
    FetchModrm64();

    uint64_t imm32 = (int64_t)(int32_t)ReadImm32(true);

    std::string disasm;
    WriteModrm64(disasm, imm32);

    if (canDisassemble)
        printf("mov %s, 0x%08lx\n", disasm.c_str(), imm32);
}

void IvyBridge::CodeFF_64()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x00:
        IncRm64();
        break;
    default:
        printf("Unknown 64-bit opcode 0xFF 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void IvyBridge::IncRm64()
{
    std::string disasm;
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 + 1UL;
    UpdateFlagsAdd64(rm64, 1UL, result);

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("inc %s\n", disasm.c_str());
}

void IvyBridge::Code0f01()
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
        printf("Unknown code 0x0f 0x01 0x%x\n", modrm.reg);
        exit(1);
    }
}

void IvyBridge::Lgdt()
{
    std::string disasm;
    uint64_t addr = DecodeModrmAddr(disasm);

    uint64_t base;
    uint16_t limit = Read16(prefix, addr);

    if (mode != Mode::LongMode)
        base = Read32(prefix, addr+2);
    else
        base = Read64(prefix, addr+2);
    
    gdtr.base = base;
    gdtr.limit = limit;

    if (canDisassemble && mode == Mode::RealMode)
        printf("o32 ");

    if (canDisassemble)
        printf("lgdt %s (0x%08lx, 0x%04x)\n", disasm.c_str(), base, limit);
}

void IvyBridge::Lidt()
{
    std::string disasm;
    uint64_t addr = DecodeModrmAddr(disasm);

    uint64_t base;
    uint16_t limit = Read16(prefix, addr);

    if (mode != Mode::LongMode)
        base = Read32(prefix, addr+2);
    else
        base = Read64(prefix, addr+2);
    
    idtr.base = base;
    idtr.limit = limit;

    if (canDisassemble && mode == Mode::RealMode)
        printf("o32 ");

    if (canDisassemble)
        printf("lidt %s (0x%08lx, 0x%04x)\n", disasm.c_str(), base, limit);
}

void IvyBridge::HintNop()
{
    FetchModrm();

    if (canDisassemble)
        printf("nop\n");
}

void IvyBridge::MovR64CRn()
{
    uint8_t val = ReadImm8(true);
    modrm.mod = (val >> 6) & 0x3;
    modrm.reg = (val >> 3) & 0x7;
    modrm.rm = val & 0x7;

    regs[modrm.reg].reg64 = cr[modrm.rm];

    if (canDisassemble)
        printf("mov %s, cr%d\n", Reg32[modrm.reg], modrm.rm);
}

void IvyBridge::MovCRnR64()
{
    uint8_t val = ReadImm8(true);
    modrm.mod = (val >> 6) & 0x3;
    modrm.reg = (val >> 3) & 0x7;
    modrm.rm = val & 0x7;

    cr[modrm.reg] = regs[modrm.rm].reg32;

    if (modrm.reg == 0 && efer.lme && (cr[0] & (1 << 31)))
    {
        efer.lma = 1;
        mode = Mode::LongMode;
    }

    if (canDisassemble)
        printf("mov cr%d, %s\n", modrm.reg, Reg32[modrm.rm]);
}

void IvyBridge::Wrmsr()
{
    uint64_t val = ((uint64_t)regs[RDX].reg32 << 32) | regs[RAX].reg32;
    switch (regs[RCX].reg32)
    {
    case 0xC0000080:
        efer.val = val;
        break;
    default:
        printf("Write to unknown msr 0x%08x\n", regs[RCX].reg32);
        exit(1);
    }

    if (canDisassemble)
        printf("wrmsr\n");
}

void IvyBridge::Rdmsr()
{
    uint64_t val;
    switch (regs[RCX].reg32)
    {
    case 0xC0000080:
        val = efer.val;
        break;
    default:
        printf("Unknown msr 0x%08x\n", regs[RCX].reg32);
        exit(1);
    }

    regs[RAX].reg32 = val & 0xffffffff;
    regs[RDX].reg32 = (val >> 32);

    if (canDisassemble)
        printf("rdmsr\n");
}

void IvyBridge::JnzRel32()
{
    int32_t rel32 = ReadImm32(true);

    if (canDisassemble)
        printf("jnz 0x%08lx\n", TranslateAddr(CS, rip + (int64_t)rel32));

    if (!rflags.flag_bits.zf)
        rip += (int64_t)rel32;
}

void IvyBridge::Cpuid()
{
    uint32_t func = regs[RAX].reg32;

    switch (func)
    {
    case 0x80000000:
        regs[RAX].reg32 = 0x80000008;
        regs[RBX].reg32 = 0;
        regs[RCX].reg32 = 0;
        regs[RDX].reg32 = 0;
        break;
    default:
        printf("Unknown cpuid func eax=0x%08x\n", func);
        exit(1);
    }

    if (canDisassemble)
        printf("cpuid\n");
}

void IvyBridge::JzRel32()
{
    int32_t rel32 = ReadImm32(true);

    if (canDisassemble)
        printf("jz 0x%08lx\n", TranslateAddr(CS, rip+(int64_t)rel32));

    if (rflags.flag_bits.zf)
        rip += (int64_t)rel32;
}

void IvyBridge::MovzxR32Rm16()
{
    FetchModrm();

    std::string disasm;
    uint16_t rm16 = ReadModrm16(disasm);

    regs[modrm.reg].reg32 = (uint32_t)rm16;

    if (canDisassemble)
        printf("movzx %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void IvyBridge::Code0FBA_32()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x05:
        BtsRm32Imm8();
        break;
    default:
        printf("Unknown 32-bit opcode 0x0F 0xBA 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void IvyBridge::BtsRm32Imm8()
{
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint8_t imm8 = ReadImm8(true);

    rflags.flag_bits.cf = rm32 & (1 << imm8);
    rm32 |= (1 << imm8);
    WriteModrm32(disasm, rm32);

    if (canDisassemble)
        printf("bts %s, 0x%02x\n", disasm.c_str(), imm8);
}



void IvyBridge::MovzxR64Rm8()
{
    FetchModrm();

    std::string disasm;
    uint8_t rm8 = ReadModrm8(disasm);

    regs[modrm.reg].reg64 = (uint64_t)rm8;

    if (canDisassemble)
        printf("movzx %s, %s\n", Reg64[modrm.reg], disasm.c_str());
}