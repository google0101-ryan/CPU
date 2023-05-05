#include "x86.h"
#include "IOBus.h"
#include <cstdio>

void TigerLake::MakeOpcodeTables()
{
    lookup16[0x24] = std::bind(&TigerLake::AndAlImm8, this);
    lookup16[0x3C] = std::bind(&TigerLake::CmpAlImm8, this);
    lookup16[0x3D] = std::bind(&TigerLake::CmpAxImm16, this);
    lookup16[0x72] = std::bind(&TigerLake::JcRel8, this);
    lookup16[0x73] = std::bind(&TigerLake::JncRel8, this);
    lookup16[0x74] = std::bind(&TigerLake::JzRel8, this);
    lookup16[0x75] = std::bind(&TigerLake::JnzRel8, this);
    lookup16[0x76] = std::bind(&TigerLake::JnaRel8, this);
    lookup16[0x77] = std::bind(&TigerLake::JaRel8, this);
    lookup16[0x7C] = std::bind(&TigerLake::JlRel8, this);
    lookup16[0x80] = std::bind(&TigerLake::Code80, this);
    lookup16[0x81] = std::bind(&TigerLake::Code81_16, this);
    lookup16[0x84] = std::bind(&TigerLake::TestRm8R8, this);
    lookup16[0x89] = std::bind(&TigerLake::MovRm16R16, this);
    lookup16[0x8A] = std::bind(&TigerLake::MovR8Rm8, this);
    lookup16[0x8B] = std::bind(&TigerLake::MovR16Rm16, this);
    lookup16[0x8E] = std::bind(&TigerLake::MovSregRm16, this);
    lookup16[0x90] = std::bind(&TigerLake::Nop, this);
    lookup16[0xA8] = std::bind(&TigerLake::TestAlImm8, this);
    for (int i = 0; i < 8; i++)
        lookup16[0xB8 + i] = std::bind(&TigerLake::MovR16Imm16, this);
    lookup16[0xC1] = std::bind(&TigerLake::CodeC1_16, this);
    lookup16[0xC6] = std::bind(&TigerLake::MovRm8Imm8, this);
    lookup16[0xC7] = std::bind(&TigerLake::MovRm16Imm16, this);
    lookup16[0xE9] = std::bind(&TigerLake::JmpRel16, this);
    lookup16[0xEB] = std::bind(&TigerLake::JmpRel8, this);
    lookup16[0xEC] = std::bind(&TigerLake::InAlDx, this);
    lookup16[0xED] = std::bind(&TigerLake::InAxDx, this);
    lookup16[0xF6] = std::bind(&TigerLake::CodeF6, this);
    lookup16[0xFC] = std::bind(&TigerLake::Cld, this);
    lookup16[0xFA] = std::bind(&TigerLake::Cli, this);

    lookup32[0x01] = std::bind(&TigerLake::AddRm32R32, this);
    lookup32[0x03] = std::bind(&TigerLake::AddR32Rm32, this);
    lookup32[0x05] = std::bind(&TigerLake::AddEaxImm32, this);
    lookup32[0x09] = std::bind(&TigerLake::OrRm32R32, this);
    lookup32[0x0B] = std::bind(&TigerLake::OrR32Rm32, this);
    lookup32[0x0D] = std::bind(&TigerLake::OrEaxImm32, this);
    lookup32[0x19] = std::bind(&TigerLake::SbbRm32R32, this);
    lookup32[0x21] = std::bind(&TigerLake::AndRm32R32, this);
    lookup32[0x23] = std::bind(&TigerLake::AndR32Rm32, this);
    lookup32[0x24] = std::bind(&TigerLake::AndAlImm8, this);
    lookup32[0x25] = std::bind(&TigerLake::AndEaxImm32, this);
    lookup32[0x29] = std::bind(&TigerLake::SubRm32R32, this);
    lookup32[0x2B] = std::bind(&TigerLake::SubR32Rm32, this);
    lookup32[0x2D] = std::bind(&TigerLake::SubEaxImm32, this);
    lookup32[0x31] = std::bind(&TigerLake::XorRm32R32, this);
    lookup32[0x33] = std::bind(&TigerLake::XorR32Rm32, this);
    lookup32[0x38] = std::bind(&TigerLake::CmpRm8R8, this);
    lookup32[0x39] = std::bind(&TigerLake::CmpRm32R32, this);
    lookup32[0x3B] = std::bind(&TigerLake::CmpR32Rm32, this);
    lookup32[0x3C] = std::bind(&TigerLake::CmpAlImm8, this);
    lookup32[0x3D] = std::bind(&TigerLake::CmpEaxImm32, this);
    for (int i = 0; i < 8; i++)
        lookup32[0x40+i] = std::bind(&TigerLake::IncR32, this);
    for (int i = 0; i < 8; i++)
        lookup32[0x48+i] = std::bind(&TigerLake::DecR32, this);
    for (int i = 0; i < 8; i++)
        lookup32[0x50+i] = std::bind(&TigerLake::PushR32, this);
    for (int i = 0; i < 8; i++)
        lookup32[0x58+i] = std::bind(&TigerLake::PopR32, this);
    lookup32[0x6A] = std::bind(&TigerLake::PushImm8, this);
    lookup32[0x6B] = std::bind(&TigerLake::ImulR32Rm32Imm8, this);
    lookup32[0x6E] = std::bind(&TigerLake::Outsb, this);
    lookup32[0x72] = std::bind(&TigerLake::JcRel8, this);
    lookup32[0x73] = std::bind(&TigerLake::JncRel8, this);
    lookup32[0x74] = std::bind(&TigerLake::JzRel8, this);
    lookup32[0x75] = std::bind(&TigerLake::JnzRel8, this);
    lookup32[0x76] = std::bind(&TigerLake::JnaRel8, this);
    lookup32[0x77] = std::bind(&TigerLake::JaRel8, this);
    lookup32[0x79] = std::bind(&TigerLake::JnsRel8, this);
    lookup32[0x7C] = std::bind(&TigerLake::JlRel8, this);
    lookup32[0x7D] = std::bind(&TigerLake::JgeRel8, this);
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
    lookup32[0x90] = std::bind(&TigerLake::Nop, this);
    lookup32[0x9C] = std::bind(&TigerLake::Pushfq, this);
    lookup32[0xA8] = std::bind(&TigerLake::TestAlImm8, this);
    lookup32[0xAA] = std::bind(&TigerLake::Stosb, this);
    for (int i = 0; i < 8; i++)
        lookup32[0xB0 + i] = std::bind(&TigerLake::MovR8Imm8, this);
    for (int i = 0; i < 8; i++)
        lookup32[0xB8 + i] = std::bind(&TigerLake::MovR32Imm32, this);
    lookup32[0xC6] = std::bind(&TigerLake::MovRm8Imm8, this);
    lookup32[0xC1] = std::bind(&TigerLake::CodeC1_32, this);
    lookup32[0xC3] = std::bind(&TigerLake::Ret, this);
    lookup32[0xC7] = std::bind(&TigerLake::MovRm32Imm32, this);
    lookup32[0xC9] = std::bind(&TigerLake::Leave, this);
    lookup32[0xD1] = std::bind(&TigerLake::CodeD1_32, this);
    lookup32[0xD3] = std::bind(&TigerLake::CodeD3_32, this);
    lookup32[0xD9] = std::bind(&TigerLake::CodeD9, this);
    lookup32[0xDB] = std::bind(&TigerLake::CodeDB, this);
    lookup32[0xE2] = std::bind(&TigerLake::LoopEcxRel8, this);
    lookup32[0xE3] = std::bind(&TigerLake::Jecxz, this);
    lookup32[0xE8] = std::bind(&TigerLake::CallRel32, this);
    lookup32[0xE9] = std::bind(&TigerLake::JmpRel32, this);
    lookup32[0xEA] = std::bind(&TigerLake::JmpPtr1632, this);
    lookup32[0xEB] = std::bind(&TigerLake::JmpRel8, this);
    lookup32[0xEC] = std::bind(&TigerLake::InAlDx, this);
    lookup32[0xED] = std::bind(&TigerLake::InEaxDx, this);
    lookup32[0xEE] = std::bind(&TigerLake::OutDxAl, this);
    lookup32[0xEF] = std::bind(&TigerLake::OutDxEax, this);
    lookup32[0xF6] = std::bind(&TigerLake::CodeF6, this);
    lookup32[0xF7] = std::bind(&TigerLake::CodeF7_32, this);
    lookup32[0xFA] = std::bind(&TigerLake::Cli, this);
    lookup32[0xFC] = std::bind(&TigerLake::Cld, this);
    lookup32[0xFF] = std::bind(&TigerLake::CodeFF_32, this);
    
    lookup64[0x01] = std::bind(&TigerLake::AddRm64R64, this);
    lookup64[0x03] = std::bind(&TigerLake::AddR64Rm64, this);
    lookup64[0x05] = std::bind(&TigerLake::AddRaxImm32, this);
    lookup64[0x09] = std::bind(&TigerLake::OrRm64R64, this);
    lookup64[0x0B] = std::bind(&TigerLake::OrR64Rm64, this);
    lookup64[0x0D] = std::bind(&TigerLake::OrRaxImm32, this);
    lookup64[0x19] = std::bind(&TigerLake::SbbRm64R64, this);
    lookup64[0x21] = std::bind(&TigerLake::AndRm64R64, this);
    lookup64[0x23] = std::bind(&TigerLake::AndR64Rm64, this);
    lookup64[0x24] = std::bind(&TigerLake::AndAlImm8, this);
    lookup64[0x25] = std::bind(&TigerLake::AndRaxImm32, this);
    lookup64[0x29] = std::bind(&TigerLake::SubRm64R64, this);
    lookup64[0x2B] = std::bind(&TigerLake::SubR64Rm64, this);
    lookup64[0x2D] = std::bind(&TigerLake::SubRaxImm32, this);
    lookup64[0x31] = std::bind(&TigerLake::XorRm64R64, this);
    lookup64[0x39] = std::bind(&TigerLake::CmpRm64R64, this);
    lookup64[0x3B] = std::bind(&TigerLake::CmpR64Rm64, this);
    lookup64[0x3C] = std::bind(&TigerLake::CmpAlImm8, this);
    lookup64[0x3D] = std::bind(&TigerLake::CmpRaxImm32, this);
    lookup64[0x63] = std::bind(&TigerLake::MovsxdR64Rm32, this);
    lookup64[0x6A] = std::bind(&TigerLake::PushImm8, this);
    lookup64[0x6B] = std::bind(&TigerLake::ImulR64Rm64Imm8, this);
    lookup64[0x6E] = std::bind(&TigerLake::Outsb, this);
    lookup64[0x72] = std::bind(&TigerLake::JcRel8, this);
    lookup64[0x73] = std::bind(&TigerLake::JncRel8, this);
    lookup64[0x74] = std::bind(&TigerLake::JzRel8, this);
    lookup64[0x75] = std::bind(&TigerLake::JnzRel8, this);
    lookup64[0x76] = std::bind(&TigerLake::JnaRel8, this);
    lookup64[0x77] = std::bind(&TigerLake::JaRel8, this);
    lookup64[0x7C] = std::bind(&TigerLake::JlRel8, this);
    lookup64[0x80] = std::bind(&TigerLake::Code80, this);
    lookup64[0x81] = std::bind(&TigerLake::Code81_64, this);
    lookup64[0x83] = std::bind(&TigerLake::Code83_64, this);
    lookup64[0x84] = std::bind(&TigerLake::TestRm8R8, this);
    lookup64[0x85] = std::bind(&TigerLake::TestRm64R64, this);
    lookup64[0x87] = std::bind(&TigerLake::XchgR64Rm64, this);
    lookup64[0x89] = std::bind(&TigerLake::MovRm64R64, this);
    lookup64[0x8A] = std::bind(&TigerLake::MovR8Rm8, this);
    lookup64[0x8B] = std::bind(&TigerLake::MovR64Rm64, this);
    lookup64[0x8D] = std::bind(&TigerLake::LeaR64M, this);
    lookup64[0x8E] = std::bind(&TigerLake::MovSregRm16, this);
    lookup64[0x90] = std::bind(&TigerLake::Nop, this);
    lookup64[0x98] = std::bind(&TigerLake::Cdqe, this);
    lookup64[0x9C] = std::bind(&TigerLake::Pushfq, this);
    lookup64[0xA8] = std::bind(&TigerLake::TestAlImm8, this);
    lookup64[0xAB] = std::bind(&TigerLake::Stosq, this);
    for (int i = 0; i < 8; i++)
        lookup64[0xB8 + i] = std::bind(&TigerLake::MovR64Imm64, this);
    lookup64[0xC1] = std::bind(&TigerLake::CodeC1_64, this);
    lookup64[0xC6] = std::bind(&TigerLake::MovRm8Imm8, this);
    lookup64[0xC7] = std::bind(&TigerLake::MovRm64Imm32, this);
    lookup64[0xE8] = std::bind(&TigerLake::CallRel32, this);
    lookup64[0xE9] = std::bind(&TigerLake::JmpRel32, this);
    lookup64[0xEB] = std::bind(&TigerLake::JmpRel8, this);
    lookup64[0xEC] = std::bind(&TigerLake::InAlDx, this);
    lookup64[0xF6] = std::bind(&TigerLake::CodeF6, this);
    lookup64[0xF7] = std::bind(&TigerLake::CodeF7_64, this);
    lookup64[0xFA] = std::bind(&TigerLake::Cli, this);
    lookup64[0xFC] = std::bind(&TigerLake::Cld, this);
    lookup64[0xFF] = std::bind(&TigerLake::CodeFF_64, this);

    extLookup16[0x01] = std::bind(&TigerLake::Code0f01, this);
    extLookup16[0x20] = std::bind(&TigerLake::MovR64CRn, this);
    extLookup16[0x22] = std::bind(&TigerLake::MovCRnR64, this);
    
    extLookup32[0x01] = std::bind(&TigerLake::Code0f01, this);
    extLookup32[0x1E] = std::bind(&TigerLake::HintNop, this);
    extLookup32[0x20] = std::bind(&TigerLake::MovR64CRn, this);
    extLookup32[0x22] = std::bind(&TigerLake::MovCRnR64, this);
    extLookup32[0x30] = std::bind(&TigerLake::Wrmsr, this);
    extLookup32[0x32] = std::bind(&TigerLake::Rdmsr, this);
    extLookup32[0x42] = std::bind(&TigerLake::CmovcR32Rm32, this);
    extLookup32[0x43] = std::bind(&TigerLake::CmovncR32Rm32, this);
    extLookup32[0x44] = std::bind(&TigerLake::CmovzR32Rm32, this);
    extLookup32[0x46] = std::bind(&TigerLake::CmovnaR32Rm32, this);
    extLookup32[0x82] = std::bind(&TigerLake::JcRel32, this);
    extLookup32[0x83] = std::bind(&TigerLake::JncRel32, this);
    extLookup32[0x84] = std::bind(&TigerLake::JzRel32, this);
    extLookup32[0x85] = std::bind(&TigerLake::JnzRel32, this);
    extLookup32[0x86] = std::bind(&TigerLake::JnaRel32, this);
    extLookup32[0x87] = std::bind(&TigerLake::JaRel32, this);
    extLookup32[0x88] = std::bind(&TigerLake::JsRel32, this);
    extLookup32[0x94] = std::bind(&TigerLake::SetzRm8, this);
    extLookup32[0x95] = std::bind(&TigerLake::SetneRm8, this);
    extLookup32[0x97] = std::bind(&TigerLake::SetaRm8, this);
    extLookup32[0xA2] = std::bind(&TigerLake::Cpuid, this);
    extLookup32[0xA3] = std::bind(&TigerLake::BtRm32R32, this);
    extLookup32[0xAE] = std::bind(&TigerLake::Code0FAE, this);
    extLookup32[0xAF] = std::bind(&TigerLake::ImulR32Rm32, this);
    extLookup32[0xB6] = std::bind(&TigerLake::MovzxR32Rm8, this);
    extLookup32[0xB7] = std::bind(&TigerLake::MovzxR32Rm16, this);
    extLookup32[0xBA] = std::bind(&TigerLake::Code0FBA_32, this);
    extLookup32[0xBE] = std::bind(&TigerLake::MovzxR32Rm8, this);
    for (int i = 0; i < 8; i++)
        extLookup32[0xC8+i] = std::bind(&TigerLake::BswapR32, this);
    
    extLookup64[0x01] = std::bind(&TigerLake::Code0f01, this);
    extLookup64[0x1E] = std::bind(&TigerLake::HintNop, this);
    extLookup64[0x42] = std::bind(&TigerLake::CmovcR64Rm64, this);
    extLookup64[0x45] = std::bind(&TigerLake::CmovneR64Rm64, this);
    extLookup64[0x83] = std::bind(&TigerLake::JncRel32, this);
    extLookup64[0x84] = std::bind(&TigerLake::JzRel32, this);
    extLookup64[0x85] = std::bind(&TigerLake::JnzRel32, this);
    extLookup64[0x87] = std::bind(&TigerLake::JaRel32, this);
    extLookup64[0x88] = std::bind(&TigerLake::JsRel32, this);
    extLookup64[0x20] = std::bind(&TigerLake::MovR64CRn, this);
    extLookup64[0x22] = std::bind(&TigerLake::MovCRnR64, this);
    extLookup64[0xB6] = std::bind(&TigerLake::MovzxR64Rm8, this);
    extLookup64[0xBA] = std::bind(&TigerLake::Code0FBA_64, this);
    extLookup64[0xBE] = std::bind(&TigerLake::MovsxR64Rm8, this);
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
    bool sign1 = value1 >> 31;
    bool sign2 = value2 >> 31;
    bool signr = (result >> 31) & 1;

    rflags.flag_bits.cf = value2 > value1;
    rflags.flag_bits.zf = (result == 0);
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

void TigerLake::AndAlImm8()
{
    uint8_t imm8 = ReadImm8(true);
    regs[RAX].lo &= imm8;

    UpdateFlagsLogic8(regs[RAX].lo);

    if (canDisassemble)
        printf("and al, 0x%02x\n", imm8);
}

void TigerLake::CmpRm8R8()
{
    FetchModrm();

    std::string disasm;
    uint8_t rm8 = ReadModrm8(disasm);
    uint8_t r8 = ReadReg8((Registers)modrm.reg);

    uint16_t result = (uint16_t)rm8 - (uint16_t)r8;

    UpdateFlagsSub8(rm8, r8, result);

    if (canDisassemble)
        printf("cmp %s, %s\n", disasm.c_str(), GetReg8((Registers)modrm.reg));
}

void TigerLake::CmpAlImm8()
{
    uint8_t imm8 = ReadImm8(true);
    uint16_t result = (uint16_t)regs[RAX].lo - (uint16_t)imm8;

    UpdateFlagsSub8(regs[RAX].lo, imm8, result);

    if (canDisassemble)
        printf("cmp al, 0x%02x\n", imm8);
}

void TigerLake::PushImm8()
{
    uint64_t imm8 = (int64_t)(int8_t)ReadImm8();

    if (mode == Mode::LongMode)
        Push64(imm8);
    else
        Push32(imm8);

    if (canDisassemble)
        printf("push 0x%02lx\n", imm8);
}

void TigerLake::JcRel8()
{
    int8_t rel8 = ReadImm8(true);

    if (canDisassemble)
        printf("jc 0x%08lx\n", TranslateAddr(CS, rip + (int64_t)rel8));

    if (rflags.flag_bits.cf)
        rip += (int64_t)rel8;
}

void TigerLake::JncRel8()
{
    int8_t rel8 = ReadImm8(true);

    if (canDisassemble)
        printf("jnc 0x%08lx\n", TranslateAddr(CS, rip + (int64_t)rel8));

    if (!rflags.flag_bits.cf)
        rip += (int64_t)rel8;
}

void TigerLake::JzRel8()
{
    int8_t rel8 = ReadImm8(true);

    if (canDisassemble)
        printf("jz 0x%08lx (%s)\n", TranslateAddr(CS, rip+(int64_t)rel8), rflags.flag_bits.zf ? "z" : ".");

    if (rflags.flag_bits.zf)
        rip += (int64_t)rel8;
}

void TigerLake::JnzRel8()
{
    int8_t rel8 = ReadImm8(true);

    if (canDisassemble)
        printf("jnz 0x%08lx\n", TranslateAddr(CS, rip + (int64_t)rel8));

    if (!rflags.flag_bits.zf)
        rip += (int64_t)rel8;
}

void TigerLake::JnaRel8()
{
    int8_t rel8 = ReadImm8(true);

    if (canDisassemble)
        printf("jna 0x%08lx\n", TranslateAddr(CS, rip + (int64_t)rel8));

    if (rflags.flag_bits.zf || rflags.flag_bits.cf)
        rip += (int64_t)rel8;
}

void TigerLake::JaRel8()
{
    int8_t rel8 = ReadImm8(true);

    if (canDisassemble)
        printf("ja 0x%08lx\n", TranslateAddr(CS, rip + (int64_t)rel8));

    if (!rflags.flag_bits.zf && !rflags.flag_bits.cf)
        rip += (int64_t)rel8;
}

void TigerLake::JnsRel8()
{
    int8_t rel8 = ReadImm8(true);

    if (canDisassemble)
        printf("jns 0x%08lx\n", TranslateAddr(CS, rip + (int64_t)rel8));

    if (!rflags.flag_bits.sf)
        rip += (int64_t)rel8;
}

void TigerLake::JlRel8()
{
    int8_t rel8 = ReadImm8(true);

    if (canDisassemble)
        printf("jl 0x%08lx\n", TranslateAddr(CS, rip + (int64_t)rel8));

    if (rflags.flag_bits.sf != rflags.flag_bits.of)
        rip += (int64_t)rel8;
}

void TigerLake::JgeRel8()
{
    int8_t rel8 = ReadImm8(true);

    if (canDisassemble)
        printf("jge 0x%08lx\n", TranslateAddr(CS, rip + (int64_t)rel8));

    if (rflags.flag_bits.sf == rflags.flag_bits.of)
        rip += (int64_t)rel8;
}

void TigerLake::JngRel8()
{
    int8_t rel8 = ReadImm8(true);

    if (canDisassemble)
        printf("jng 0x%08lx\n", TranslateAddr(CS, rip + (int64_t)rel8));

    if (rflags.flag_bits.zf || (rflags.flag_bits.sf != rflags.flag_bits.of))
        rip += (int64_t)rel8;
}

void TigerLake::JgRel8()
{
    int8_t rel8 = ReadImm8(true);

    if (canDisassemble)
        printf("jg 0x%08lx\n", TranslateAddr(CS, rip + (int64_t)rel8));

    if (!rflags.flag_bits.zf && (rflags.flag_bits.sf == rflags.flag_bits.of))
        rip += (int64_t)rel8;
}

void TigerLake::Code80()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x01:
        OrRm8Imm8();
        break;
    case 0x04:
        AndRm8Imm8();
        break;
    case 0x07:
        CmpRm8Imm8();
        break;
    default:
        printf("Unknown opcode 0x80 0x%02x\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::OrRm8Imm8()
{
    std::string disasm;

    uint8_t imm8 = ReadImm8(true);
    uint8_t rm8 = ReadModrm8(disasm);

    uint8_t result = rm8 | imm8;

    WriteModrm8(disasm, result);

    UpdateFlagsLogic8(result);

    if (canDisassemble)
        printf("or %s, 0x%02x\n", disasm.c_str(), imm8);
}

void TigerLake::AndRm8Imm8()
{
    std::string disasm;

    uint8_t imm8 = ReadImm8(true);
    uint8_t rm8 = ReadModrm8(disasm);

    uint8_t result = rm8 & imm8;

    UpdateFlagsLogic8(result);

    WriteModrm8(disasm, result);

    if (canDisassemble)
        printf("and %s, 0x%02x (0x%02x)\n", disasm.c_str(), imm8, rm8);
}

void TigerLake::CmpRm8Imm8()
{
    std::string disasm;
    uint8_t imm8 = ReadImm8(true);
    uint8_t rm8 = ReadModrm8(disasm);

    uint16_t result = (uint16_t)rm8 - (uint16_t)imm8;

    UpdateFlagsSub8(rm8, imm8, result);

    if (canDisassemble)
        printf("cmp %s, 0x%02x\n", disasm.c_str(), imm8);
}

void TigerLake::TestRm8R8()
{
    FetchModrm();

    std::string disasm;
    uint8_t rm8 = ReadModrm8(disasm);
    uint8_t r8 = ReadReg8((Registers)modrm.reg);

    uint8_t result = r8 & rm8;

    UpdateFlagsLogic8(result);

    if (canDisassemble)
        printf("test %s, %s, 0x%02x, 0x%02x\n", disasm.c_str(), GetReg8((Registers)modrm.reg), rm8, r8);
}

void TigerLake::MovRm8R8()
{
    FetchModrm();

    std::string disasm;
    uint8_t r8 = ReadReg8((Registers)modrm.reg);

    WriteModrm8(disasm, r8);

    if (canDisassemble)
        printf("mov %s, %s\n", disasm.c_str(), GetReg8((Registers)modrm.reg));
}

void TigerLake::MovR8Rm8()
{
    FetchModrm();

    std::string disasm;
    uint8_t rm8 = ReadModrm8(disasm);

    WriteReg8((Registers)modrm.reg, rm8);

    if (canDisassemble)
        printf("mov %s, %s\n", GetReg8((Registers)modrm.reg), disasm.c_str());
}

void TigerLake::MovSregRm16()
{
    FetchModrm();

    std::string disasm;
    uint16_t rm16 = ReadModrm16(disasm);

    CacheSegment((Segments)modrm.reg, rm16);

    if (canDisassemble)
        printf("mov %s, %s\n", Segs[modrm.reg], disasm.c_str());
}

void TigerLake::Nop()
{
    if (canDisassemble)
        printf("nop\n");
}

void TigerLake::TestAlImm8()
{
    uint8_t imm8 = ReadImm8(true);
    uint8_t al = regs[RAX].lo;

    uint8_t result = al & imm8;

    UpdateFlagsLogic8(result);

    if (canDisassemble)
        printf("test al, 0x%02x\n", imm8);
}

void TigerLake::Stosb()
{
    if (canDisassemble)
        printf("stosb\n");

    if (regs[RCX].reg64 == 0 && rep)
    {
        return;
    }


start:
    l1->Write8(TranslateAddr(DS, regs[RDI].reg64), regs[RAX].lo);
    
    if (!rflags.flag_bits.df) regs[RDI].reg64 += 8;
    else regs[RDI].reg64 -= 8;

    if (rep)
    {
        regs[RCX].reg64--;
        if (regs[RCX].reg64)
            goto start;
    }
}

void TigerLake::MovRm8Imm8()
{
    FetchModrm();

    uint8_t imm8 = ReadImm8(true);

    std::string disasm;
    WriteModrm8(disasm, imm8);

    if (canDisassemble)
        printf("mov %s, 0x%02x\n", disasm.c_str(), imm8);
}

void TigerLake::JmpRel8()
{
    int8_t rel8 = ReadImm8(true);

    rip += (int64_t)rel8;

    if (canDisassemble)
        printf("jmp 0x%08lx\n", TranslateAddr(CS, rip));
}

void TigerLake::InAlDx()
{
    regs[RAX].lo = IOBus::In8(regs[RDX].reg16);

    if (canDisassemble)
        printf("in al, dx\n");
}

void TigerLake::CodeF6()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x00:
    case 0x01:
        TestRm8Imm8();
        break;
    case 0x03:
        NegRm8();
        break;
    case 0x06:
        DivAlRm8();
        break;
    default:
        printf("Unknown opcode 0xF6 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::TestRm8Imm8()
{
    std::string disasm;
    uint8_t imm8 = ReadImm8(true);
    uint8_t rm8 = ReadModrm8(disasm);

    uint8_t result = rm8 & imm8;

    UpdateFlagsLogic8(result);

    if (canDisassemble)
        printf("test %s, 0x%02x\n", disasm.c_str(), imm8);
}

void TigerLake::DivAlRm8()
{
    std::string disasm;
    uint8_t rm8 = ReadModrm8(disasm);
    uint16_t ax = regs[RAX].reg16;

    uint16_t temp = regs[RAX].reg16 / rm8;
    if (temp > 0xFF)
    {
        printf("#DE: Divide result to big to fit in 8-bit result!\n");
        exit(1);
    }

    regs[RAX].lo = temp;
    regs[RAX].hi = ax % rm8;

    if (canDisassemble)
        printf("div %s\n", disasm.c_str());
}

void TigerLake::NegRm8()
{
    std::string disasm;
    uint8_t rm8 = ReadModrm8(disasm);

    if (!rm8) rflags.flag_bits.cf = 0;
    else rflags.flag_bits.cf = 1;

    uint8_t result = -rm8;

    WriteModrm8(disasm, result);

    if (canDisassemble)
        printf("neg %s\n", disasm.c_str());
}

void TigerLake::Cld()
{
    rflags.flag_bits.df = 0;
    if (canDisassemble)
        printf("cld\n");
}

void TigerLake::Cli()
{
    rflags.flag_bits.if_ = 0;
    if (canDisassemble)
        printf("cli\n");
}

void TigerLake::CmpAxImm16()
{
    uint16_t imm16 = ReadImm16(true);
    uint16_t ax = regs[RAX].reg16;

    uint32_t result = (uint32_t)ax - (uint32_t)imm16;

    UpdateFlagsSub16(ax, imm16, result);

    if (canDisassemble)
        printf("cmp ax, 0x%04x\n", imm16);
}

void TigerLake::Code81_16()
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

void TigerLake::CmpRm16Imm16()
{
    std::string disasm;
    uint16_t imm16 = ReadImm16(true);
    uint16_t rm16 = ReadModrm16(disasm);

    uint32_t result = (uint32_t)rm16 - (uint32_t)imm16;

    UpdateFlagsSub16(rm16, imm16, result);

    if (canDisassemble)
        printf("cmp %s, 0x%04x\n", disasm.c_str(), imm16);
}

void TigerLake::MovRm16R16()
{
    FetchModrm();

    std::string disasm;
    WriteModrm16(disasm, regs[modrm.reg].reg16);

    if (canDisassemble)
        printf("mov %s, %s\n", disasm.c_str(), Reg16[modrm.reg]);
}

void TigerLake::MovR16Rm16()
{
    FetchModrm();

    std::string disasm;
    regs[modrm.reg].reg16 = ReadModrm16(disasm);

    if (canDisassemble)
        printf("mov %s, %s\n", Reg16[modrm.reg], disasm.c_str());
}

void TigerLake::MovR16Imm16()
{
    uint8_t reg = l1->Read8(TranslateAddr(CS, rip-1), false) - 0xB8;
    uint16_t imm16 = ReadImm16(true);

    if (rex.b) reg += 8;

    regs[reg].reg16 = imm16;

    if (canDisassemble)
        printf("mov %s, 0x%04x\n", Reg16[reg], imm16);
}

void TigerLake::CodeC1_16()
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

void TigerLake::ShrRm16Imm8()
{
    std::string disasm;
    uint8_t imm8 = ReadImm8(true) & 0xF;
    uint16_t rm16 = ReadModrm16(disasm);

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

void TigerLake::MovRm16Imm16()
{
    FetchModrm();

    uint16_t imm16 = ReadImm16(true);

    std::string disasm;
    WriteModrm16(disasm, imm16);

    if (canDisassemble)
        printf("mov %s, 0x%04x\n", disasm.c_str(), imm16);
}

void TigerLake::JmpRel16()
{
    int16_t rel16 = ReadImm16(true);

    rip += (int64_t)rel16;

    if (canDisassemble)
        printf("jmp 0x%08lx\n", TranslateAddr(CS, rip));
}

void TigerLake::InAxDx()
{
    regs[RAX].reg16 = IOBus::In16(regs[RDX].reg16);

    if (canDisassemble)
        printf("in ax, dx\n");
}

void TigerLake::AddRm32R32()
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

void TigerLake::AddR32Rm32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t r32 = regs[modrm.reg].reg32;

    uint64_t result = (uint64_t)r32 + (uint64_t)rm32;

    UpdateFlagsAdd32(r32, rm32, result);

    regs[modrm.reg].reg64 = (uint32_t)result;

    if (canDisassemble)
        printf("add %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void TigerLake::AddEaxImm32()
{
    uint32_t imm32 = ReadImm32(true);
    uint32_t eax = regs[RAX].reg32;

    uint64_t result = (uint64_t)eax + (uint64_t)imm32;

    UpdateFlagsAdd32(eax, imm32, result);

    regs[RAX].reg64 = (uint32_t)result;

    if (canDisassemble)
        printf("add eax, 0x%08x\n", imm32);
}

void TigerLake::OrRm32R32()
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

void TigerLake::OrR32Rm32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t r32 = regs[modrm.reg].reg32;

    uint32_t result = rm32 | r32;

    UpdateFlagsLogic32(result);

    regs[modrm.reg].reg64 = result;

    if (canDisassemble)
        printf("or %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void TigerLake::OrEaxImm32()
{
    uint32_t imm32 = ReadImm32(true);
    regs[RAX].reg64 = regs[RAX].reg32 | imm32;

    UpdateFlagsLogic32(regs[RAX].reg32);

    if (canDisassemble)
        printf("or eax, 0x%08x\n", imm32);
}

void TigerLake::SbbRm32R32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t r32 = regs[modrm.reg].reg32 + (uint32_t)rflags.flag_bits.cf;

    uint64_t result = (uint64_t)rm32 - (uint64_t)r32;

    WriteModrm32(disasm, result);

    if (canDisassemble)
        printf("sbb %s, %s\n", disasm.c_str(), Reg32[modrm.reg]);
}

void TigerLake::AndRm32R32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t r32 = regs[modrm.reg].reg32;

    uint32_t result = rm32 & r32;

    WriteModrm32(disasm, result);

    UpdateFlagsLogic32(result);

    if (canDisassemble)
        printf("and %s, %s\n", disasm.c_str(), Reg32[modrm.reg]);
}

void TigerLake::AndR32Rm32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t r32 = regs[modrm.reg].reg32;

    uint32_t result = r32 & rm32;

    regs[modrm.reg].reg64 = result;

    UpdateFlagsLogic32(result);

    if (canDisassemble)
        printf("and %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void TigerLake::AndEaxImm32()
{
    uint32_t imm32 = ReadImm32(true);
    uint32_t eax = regs[RAX].reg32;

    uint32_t result = eax & imm32;

    UpdateFlagsLogic32(result);

    regs[RAX].reg64 = result;

    if (canDisassemble)
        printf("and eax, 0x%08x\n", imm32);
}

void TigerLake::SubRm32R32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t r32 = regs[modrm.reg].reg32;

    uint64_t result = (uint64_t)rm32 - (uint64_t)r32;

    UpdateFlagsSub32(rm32, r32, result);

    WriteModrm32(disasm, result);

    if (canDisassemble)
        printf("sub %s, %s\n", disasm.c_str(), Reg32[modrm.reg]);
}

void TigerLake::SubR32Rm32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t r32 = regs[modrm.reg].reg32;

    uint64_t result = (uint64_t)r32 - (uint64_t)rm32;

    UpdateFlagsSub32(rm32, r32, result);

    regs[modrm.reg].reg64 = (uint32_t)result;

    if (canDisassemble)
        printf("sub %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void TigerLake::SubEaxImm32()
{
    uint32_t imm32 = ReadImm32(true);
    uint32_t eax = regs[RAX].reg32;

    uint64_t result = (uint64_t)eax - (uint64_t)imm32;

    UpdateFlagsSub32(eax, imm32, result);

    regs[RAX].reg64 = (uint32_t)result;

    if (canDisassemble)
        printf("sub eax, 0x%08x\n", imm32);
}

void TigerLake::XorRm32R32()
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

void TigerLake::XorR32Rm32()
{
    FetchModrm();

    std::string disasm;

    uint32_t r32 = regs[modrm.reg].reg32;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t result = rm32 ^ r32;
    regs[modrm.reg].reg64 = result;
    UpdateFlagsLogic32(result);

    if (canDisassemble)
        printf("xor %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void TigerLake::CmpRm32R32()
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

void TigerLake::CmpR32Rm32()
{
    FetchModrm();

    std::string disasm;
    uint32_t r32 = regs[modrm.reg].reg32;
    uint32_t rm32 = ReadModrm32(disasm);

    uint64_t result = (uint64_t)r32 - (uint64_t)rm32;

    UpdateFlagsSub32(r32, rm32, result);

    if (canDisassemble)
        printf("cmp %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void TigerLake::CmpEaxImm32()
{
    uint32_t imm32 = ReadImm32(true);
    uint32_t eax = regs[RAX].reg32;

    uint64_t result = (uint64_t)eax - (uint64_t)imm32;

    UpdateFlagsSub32(eax, imm32, result);

    if (canDisassemble)
        printf("cmp eax, 0x%08x\n", imm32);
}

void TigerLake::IncR32()
{
    uint8_t reg = l1->Read8(TranslateAddr(CS, rip-1), false) - 0x40;
    if (rex.b) reg += 8;
    
    uint64_t result = (uint64_t)regs[reg].reg32 + 1UL;

    UpdateFlagsAdd32(regs[reg].reg32, 1, result);

    regs[reg].reg64 = (uint64_t)result;

    if (canDisassemble)
        printf("inc %s\n", Reg32[reg]);
}

void TigerLake::DecR32()
{
    uint8_t reg = l1->Read8(TranslateAddr(CS, rip-1), false) - 0x48;
    if (rex.b) reg += 8;
    
    uint64_t result = (uint64_t)regs[reg].reg32 - 1UL;

    UpdateFlagsSub32(regs[reg].reg32, 1, result);

    regs[reg].reg64 = (uint32_t)result;

    if (canDisassemble)
        printf("dec %s\n", Reg32[reg]);
}

void TigerLake::PushR32()
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

void TigerLake::PopR32()
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
        regs[reg].reg64 = Pop32();

        if (canDisassemble)
            printf("pop %s\n", Reg32[reg]);
    }
}

void TigerLake::ImulR32Rm32Imm8()
{
    FetchModrm();

    std::string disasm;
    int8_t imm8 = ReadImm8(true);
    int32_t rm32 = ReadModrm32(disasm);

    int64_t x = (int64_t)rm32;
    int64_t y = (int64_t)imm8;

    int64_t z = (int128_t)x * y;
    int o = z != (int32_t)z;
    rflags.flag_bits.cf = o;
    rflags.flag_bits.of = o;
    
    regs[modrm.reg].reg64 = (uint32_t)z;

    if (canDisassemble)
        printf("imul %s, %s, 0x%02x\n", Reg32[modrm.reg], disasm.c_str(), imm8);
}

void TigerLake::Outsb()
{
start:
    uint8_t rm8 = l1->Read8(TranslateAddr(prefix, regs[RSI].reg64), false);
    IOBus::Out8(regs[RDX].reg16, rm8);

    if (rflags.flag_bits.df) regs[RSI].reg64 -= 1;
    else regs[RSI].reg64 += 1;

    if (rep)
    {
        regs[RCX].reg64--;
        if (regs[RCX].reg64)
            goto start;
    }

    if (canDisassemble)
        printf("outsb\n");
}

void TigerLake::Code81_32()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x00:
        AddRm32Imm32();
        break;
    case 0x01:
        OrRm32Imm32();
        break;
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

void TigerLake::AddRm32Imm32()
{
    std::string disasm;
    uint32_t imm32 = ReadImm32(true);
    uint32_t rm32 = ReadModrm32(disasm);

    uint64_t result = (uint64_t)rm32 + (uint64_t)imm32;

    UpdateFlagsAdd32(rm32, imm32, result);

    WriteModrm32(disasm, result);

    if (canDisassemble)
        printf("add %s, 0x%08x\n", disasm.c_str(), imm32);
}

void TigerLake::OrRm32Imm32()
{
    std::string disasm;
    uint32_t imm32 = ReadImm32(true);
    uint32_t rm32 = ReadModrm32(disasm);

    uint32_t result = rm32 | imm32;

    UpdateFlagsLogic32(result);

    WriteModrm32(disasm, result);

    if (canDisassemble)
        printf("or %s, 0x%08x\n", disasm.c_str(), imm32);
}

void TigerLake::AndRm32Imm32()
{
    std::string disasm;
    uint32_t imm32 = ReadImm32(true);
    uint32_t rm32 = ReadModrm32(disasm);

    uint32_t result = rm32 & imm32;

    UpdateFlagsLogic32(result);

    WriteModrm32(disasm, result);

    if (canDisassemble)
        printf("and %s, 0x%08x\n", disasm.c_str(), imm32);
}

void TigerLake::CmpRm32Imm32()
{
    std::string disasm;
    uint32_t imm32 = ReadImm32(true);
    uint32_t rm32 = ReadModrm32(disasm);

    uint64_t result = (uint64_t)rm32 - (uint64_t)imm32;

    UpdateFlagsSub32(rm32, imm32, result);

    if (canDisassemble)
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
        printf("Unknown opcode 0x83 0x%02x\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::OrRm32Imm8()
{
    std::string disasm;
    uint32_t imm32 = (int32_t)(int8_t)ReadImm8(true);
    uint32_t rm32 = ReadModrm32(disasm);

    uint32_t result = rm32 | imm32;

    UpdateFlagsLogic32(result);

    WriteModrm32(disasm, result);

    if (canDisassemble)
        printf("or %s, 0x%02x\n", disasm.c_str(), imm32);
}

void TigerLake::AddRm32Imm8()
{
    std::string disasm;
    uint32_t imm32 = (int32_t)(int8_t)ReadImm8(true);
    uint32_t rm32 = ReadModrm32(disasm);

    uint64_t result = (uint64_t)rm32 + (uint64_t)imm32;

    UpdateFlagsAdd32(rm32, imm32, result);

    WriteModrm32(disasm, result);

    if (canDisassemble)
        printf("add %s, 0x%02x\n", disasm.c_str(), imm32);
}

void TigerLake::AndRm32Imm8()
{
    std::string disasm;
    uint32_t imm32 = (int32_t)(int8_t)ReadImm8(true);
    uint32_t rm32 = ReadModrm32(disasm);

    uint32_t result = rm32 & imm32;

    UpdateFlagsLogic32(result);

    WriteModrm32(disasm, result);

    if (canDisassemble)
        printf("and %s, 0x%02x\n", disasm.c_str(), imm32);
}

void TigerLake::SubRm32Imm8()
{
    std::string disasm;
    uint32_t imm32 = (int32_t)(int8_t)ReadImm8(true);
    uint32_t rm32 = ReadModrm32(disasm);

    uint64_t result = (uint64_t)rm32 - (uint64_t)imm32;

    UpdateFlagsSub32(rm32, imm32, result);

    WriteModrm32(disasm, result);

    if (canDisassemble)
        printf("cmp %s, 0x%02x\n", disasm.c_str(), imm32);
}

void TigerLake::CmpRm32Imm8()
{
    std::string disasm;
    uint32_t imm32 = (int32_t)(int8_t)ReadImm8(true);
    uint32_t rm32 = ReadModrm32(disasm);

    uint64_t result = (uint64_t)rm32 - (uint64_t)imm32;

    UpdateFlagsSub32(rm32, imm32, result);

    if (canDisassemble)
        printf("cmp %s, 0x%02x\n", disasm.c_str(), imm32);
}

void TigerLake::TestRm32R32()
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

void TigerLake::MovRm32R32()
{
    std::string disasm;
    FetchModrm();
    
    WriteModrm32(disasm, regs[modrm.reg].reg32);

    if (canDisassemble)
        printf("mov %s, %s\n", disasm.c_str(), Reg32[modrm.reg]);
}

void TigerLake::MovR32Rm32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);

    regs[modrm.reg].reg64 = rm32;

    if (canDisassemble)
        printf("mov %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void TigerLake::LeaR32M()
{
    FetchModrm();

    std::string disasm;
    regs[modrm.reg].reg64 = (DecodeModrmAddr(disasm) & 0xFFFFFFFF);

    if (canDisassemble)
        printf("lea %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void TigerLake::BtRm32R32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint32_t r32 = regs[modrm.reg].reg32;

    rflags.flag_bits.cf = rm32 & (1 << r32);

    if (canDisassemble)
        printf("bt %s, %s\n", disasm.c_str(), Reg32[modrm.reg]);
}

void TigerLake::MovR8Imm8()
{
    uint8_t reg = l1->Read8(TranslateAddr(CS, rip-1), false) - 0xB0;
    if (rex.b) reg += 8;

    uint8_t imm8 = ReadImm8(true);

    WriteReg8((Registers)reg, imm8);

    if (canDisassemble)
        printf("mov %s, 0x%02x\n", GetReg8((Registers)reg), imm8);
}

void TigerLake::MovR32Imm32()
{
    uint8_t reg = l1->Read8(TranslateAddr(CS, rip-1), false) - 0xB8;
    if (rex.b) reg += 8;

    uint32_t imm32 = ReadImm32(true);

    regs[reg].reg64 = imm32;

    if (canDisassemble)
        printf("mov %s, 0x%08x\n", Reg32[reg], imm32);
}

void TigerLake::CodeC1_32()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x04:
        ShlRm32Imm8();
        break;
    case 0x05:
        ShrRm32Imm8();
        break;
    default:
        printf("Unknown 32-bit opcode 0xC1 0x%02x\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::ShlRm32Imm8()
{
    std::string disasm;
    uint8_t imm8 = ReadImm8(true) & 0x1F;
    uint32_t rm32 = ReadModrm32(disasm);

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

void TigerLake::ShrRm32Imm8()
{
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint8_t imm8 = ReadImm8(true) & 0x1F;

    uint64_t result = rm32 >> imm8;
    WriteModrm32(disasm, result);

    rflags.flag_bits.cf = ((rm32 << (imm8-1)) & 1);
    rflags.flag_bits.pf = CalcPF(result);
    rflags.flag_bits.zf = !(result);
    rflags.flag_bits.sf = (result >> 31) & 1;

    if (imm8 == 1)
        rflags.flag_bits.of = ((rm32 >> 31) & 1) ^ ((rm32 >> 30) & 1);
    
    if (canDisassemble)
        printf("shr %s, 0x%02x\n", disasm.c_str(), imm8);
}

void TigerLake::Ret()
{
    rip = (mode == Mode::LongMode) ? Pop64() : Pop32();

    if (canDisassemble)
        printf("ret (0x%08lx)\n", TranslateAddr(CS, rip));
}

void TigerLake::MovRm32Imm32()
{
    FetchModrm();
    uint32_t imm32 = ReadImm32(true);

    std::string disasm;
    WriteModrm32(disasm, imm32);

    if (canDisassemble)
        printf("mov %s, 0x%08x\n", disasm.c_str(), imm32);
}

void TigerLake::Leave()
{
    regs[RSP].reg64 = regs[RBP].reg64;

    if (mode == Mode::LongMode)
        regs[RBP].reg64 = Pop64();
    else
        regs[RBP].reg64 = Pop32();
    
    if (canDisassemble)
        printf("leave\n");
}

void TigerLake::CodeD1_32()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x04:
        ShlRm321();
        break;
    case 0x05:
        ShrRm321();
        break;
    default:
        printf("Unknown opcode 0xD1 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::ShlRm321()
{   
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);

    uint64_t result = rm32 << 1;
    WriteModrm32(disasm, result);

    rflags.flag_bits.cf = ((rm32 >> (31)) & 1);
    rflags.flag_bits.pf = CalcPF(result);
    rflags.flag_bits.zf = !(result);
    rflags.flag_bits.sf = (result >> 31) & 1;

    rflags.flag_bits.of = ((rm32 >> 31) & 1) ^ ((rm32 >> 30) & 1);
    
    if (canDisassemble)
        printf("shl %s, 1\n", disasm.c_str());
}

void TigerLake::ShrRm321()
{
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);

    uint64_t result = rm32 >> 1;
    WriteModrm32(disasm, result);

    rflags.flag_bits.cf = (rm32 & 1);
    rflags.flag_bits.pf = CalcPF(result);
    rflags.flag_bits.zf = !(result);
    rflags.flag_bits.sf = (result >> 31) & 1;

    rflags.flag_bits.of = ((rm32 >> 31) & 1) ^ ((rm32 >> 30) & 1);
    
    if (canDisassemble)
        printf("shr %s, 1\n", disasm.c_str());
}

void TigerLake::CodeD3_32()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x04:
        ShlRm32Cl();
        break;
    case 0x05:
        ShrRm32Cl();
        break;
    default:
        printf("Unknown opcode 0xD3 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::ShlRm32Cl()
{
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);
    uint8_t cl = regs[RCX].lo & 0x1F;

    uint64_t result = rm32 << cl;
    WriteModrm32(disasm, result);

    rflags.flag_bits.cf = ((rm32 >> (32-cl)) & 1);
    rflags.flag_bits.pf = CalcPF(result);
    rflags.flag_bits.zf = !(result);
    rflags.flag_bits.sf = (result >> 31) & 1;

    if (cl == 1)
        rflags.flag_bits.of = ((rm32 >> 31) & 1) ^ ((rm32 >> 30) & 1);
    
    if (canDisassemble)
        printf("shl %s, cl\n", disasm.c_str());
}

void TigerLake::ShrRm32Cl()
{
    std::string disasm;
    uint8_t cl = regs[RCX].lo & 0x1F;
    uint32_t rm32 = ReadModrm32(disasm);

    uint64_t result = rm32 >> cl;
    WriteModrm32(disasm, result);

    rflags.flag_bits.cf = ((rm32 << (cl-1)) & 1);
    rflags.flag_bits.pf = CalcPF(result);
    rflags.flag_bits.zf = !(result);
    rflags.flag_bits.sf = (result >> 31) & 1;

    if (cl == 1)
        rflags.flag_bits.of = ((rm32 >> 31) & 1) ^ ((rm32 >> 30) & 1);
    
    if (canDisassemble)
        printf("shr %s, cl\n", disasm.c_str());
}

void TigerLake::CodeD9()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x05:
        Fldcw();
        break;
    default:
        printf("Unknown opcode 0xD9 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::Fldcw()
{
    std::string disasm;
    uint16_t rm16 = ReadModrm16(disasm);

    fpu.control = rm16;

    if (canDisassemble)
        printf("fldcw %s\n", disasm.c_str());
}

void TigerLake::CodeDB()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x04:
        Finit();
        break;
    default:
        printf("Unknown opcode %s0xDB 0x%02X\n", fwait ? "0x9B " : "", modrm.reg);
        exit(1);
    }
}

void TigerLake::Finit()
{
    fpu.control = 0x37F;
    fpu.status = 0;
    fpu.tagWord = 0;
    fpu.dataPointer = 0;
    fpu.fpuIp = 0;
    fpu.lastInstr = 0;

    if (canDisassemble)
        printf("finit\n");
}

void TigerLake::LoopEcxRel8()
{
    int8_t rel8 = ReadImm8(true);

    regs[RCX].reg32--;

    if (canDisassemble)
        printf("loop 0x%08lx\n", TranslateAddr(CS, rip+(uint64_t)rel8));

    if (regs[RCX].reg32)
        rip += (int64_t)rel8;
}

void TigerLake::Jecxz()
{
    int8_t imm8 = ReadImm8(true);

    if (canDisassemble)
        printf("jecxz 0x%08lx\n", TranslateAddr(CS, rip+(int64_t)imm8));

    if (!regs[RCX].reg32)
        rip += (int64_t)imm8;
}

void TigerLake::CallRel32()
{
    int32_t rel32 = ReadImm32(true);

    (mode == Mode::LongMode) ? Push64(rip) : Push32(rip);

    rip += (int64_t)rel32;

    if (rip == 0xfffcc390)
        exit(1);

    if (canDisassemble)
        printf("call 0x%08lx\n", TranslateAddr(CS, rip));
}

void TigerLake::JmpRel32()
{
    int32_t rel32 = ReadImm32(true);

    rip += (int64_t)rel32;

    if (canDisassemble)
        printf("jmp 0x%08lx\n", TranslateAddr(CS, rip));
}

void TigerLake::JmpPtr1632()
{
    uint32_t newRip = ReadImm32(true);
    uint16_t selector = ReadImm16(true);
    
    CheckModeChange();

    CacheSegment(CS, selector);
    rip = newRip;

    if (canDisassemble)
        printf("jmp 0x%02x:0x%08x\n", selector, newRip);
}

void TigerLake::InEaxDx()
{
    regs[RAX].reg32 = IOBus::In32(regs[RDX].reg16);

    if (canDisassemble)
        printf("in eax, dx\n");
}

void TigerLake::OutDxAl()
{
    IOBus::Out8(regs[RDX].reg16, regs[RAX].lo);

    if (canDisassemble)
        printf("out dx, al\n");
}

void TigerLake::OutDxEax()
{
    IOBus::Out32(regs[RDX].reg16, regs[RAX].reg32);

    if (canDisassemble)
        printf("out dx, eax\n");
}

void TigerLake::CodeF7_32()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x00:
    case 0x01:
        TestRm32Imm32();
        break;
    case 0x02:
        NotRm32();
        break;
    default:
        printf("Unknown 32-bit opcode 0xF7 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::TestRm32Imm32()
{
    std::string disasm;
    uint32_t imm32 = ReadImm32(true);
    uint32_t rm32 = ReadModrm32(disasm);

    uint32_t result = rm32 & imm32;

    UpdateFlagsLogic32(result);

    if (canDisassemble)
        printf("test %s, 0x%08x\n", disasm.c_str(), imm32);
}

void TigerLake::NotRm32()
{
    std::string disasm;

    WriteModrm32(disasm, ~ReadModrm32(disasm));

    if (canDisassemble)
        printf("not %s\n", disasm.c_str());
}

void TigerLake::CodeFF_32()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x00:
        IncRm32();
        break;
    case 0x01:
        DecRm32();
        break;
    case 0x02:
        CallRm32();
        break;
    case 0x04:
        JmpRm32();
        break;
    default:
        printf("Unknown opcode 0xFF 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::IncRm32()
{
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);

    uint64_t result = (uint64_t)rm32 + 1UL;

    UpdateFlagsAdd32(rm32, 1, result);

    WriteModrm32(disasm, result);

    if (canDisassemble)
        printf("inc %s\n", disasm.c_str());
}

void TigerLake::DecRm32()
{
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);

    uint64_t result = (uint64_t)rm32 - 1UL;

    UpdateFlagsSub32(rm32, 1, result);

    WriteModrm32(disasm, result);

    if (canDisassemble)
        printf("dec %s\n", disasm.c_str());
}

void TigerLake::CallRm32()
{
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);

    if (mode == Mode::LongMode) Push64(rip);
    else Push32(rip);

    rip = rm32;

    if (canDisassemble)
        printf("call %s\n", disasm.c_str());
}

void TigerLake::JmpRm32()
{
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);

    rip = rm32;

    if (canDisassemble)
        printf("jmp %s\n", disasm.c_str());
}

void TigerLake::AddRm64R64()
{
    FetchModrm();

    std::string disasm;
    uint64_t rm64 = ReadModrm64(disasm);
    uint64_t r64 = regs[modrm.reg].reg64;

    uint64_t result = rm64 + r64;

    UpdateFlagsAdd64(rm64, r64, result);

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("add %s, %s\n", disasm.c_str(), Reg64[modrm.reg]);
}

void TigerLake::AddR64Rm64()
{
    FetchModrm();

    std::string disasm;
    uint64_t rm64 = ReadModrm64(disasm);
    uint64_t r64 = regs[modrm.reg].reg64;

    uint64_t result = r64 + rm64;

    UpdateFlagsAdd64(r64, rm64, result);

    regs[modrm.reg].reg64 = result;

    if (canDisassemble)
        printf("add %s, %s\n", Reg64[modrm.reg], disasm.c_str());
}

void TigerLake::AddRaxImm32()
{
    uint64_t imm64 = (int64_t)(int32_t)ReadImm32(true);
    uint64_t rax = regs[RAX].reg64;

    uint64_t result = rax + imm64;

    UpdateFlagsAdd64(rax, imm64, result);

    regs[RAX].reg64 = result;

    if (canDisassemble)
        printf("add rax, 0x%08lx\n", imm64);
}

void TigerLake::OrR64Rm64()
{
    FetchModrm();

    std::string disasm;

    uint64_t r64 = regs[modrm.reg].reg64;
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = r64 | rm64;

    UpdateFlagsLogic64(result);

    regs[modrm.reg].reg64 = result;

    if (canDisassemble)
        printf("or %s, %s\n", Reg64[modrm.reg], disasm.c_str());
}

void TigerLake::OrRm64R64()
{
    FetchModrm();

    std::string disasm;

    uint64_t r64 = regs[modrm.reg].reg64;
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = r64 | rm64;

    UpdateFlagsLogic64(result);

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("or %s, %s\n", disasm.c_str(), Reg64[modrm.reg]);
}

void TigerLake::OrRaxImm32()
{
    uint64_t imm32 = (int64_t)(int32_t)ReadImm32(true);

    regs[RAX].reg64 |= imm32;

    UpdateFlagsLogic64(regs[RAX].reg64);

    if (canDisassemble)
        printf("or rax, 0x%08lx\n", imm32);
}

void TigerLake::SbbRm64R64()
{
    FetchModrm();

    std::string disasm;
    uint64_t rm64 = ReadModrm64(disasm);
    uint64_t r64 = regs[modrm.reg].reg64 + (uint64_t)rflags.flag_bits.cf;

    uint64_t result = rm64 - r64;

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("sbb %s, %s\n", disasm.c_str(), Reg64[modrm.reg]);
}

void TigerLake::AndRm64R64()
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

void TigerLake::AndR64Rm64()
{
    FetchModrm();

    std::string disasm;
    uint64_t rm64 = ReadModrm64(disasm);
    uint64_t r64 = regs[modrm.reg].reg64;

    uint64_t result = rm64 & r64;

    UpdateFlagsLogic64(result);

    regs[modrm.reg].reg64 = result;

    if (canDisassemble)
        printf("and %s, %s\n", Reg64[modrm.reg], disasm.c_str());
}

void TigerLake::SubRm64R64()
{
    FetchModrm();

    std::string disasm;

    uint64_t r64 = regs[modrm.reg].reg64;
    uint64_t rm64 = ReadModrm64(disasm);

    UpdateFlagsSub64(rm64, r64, rm64 - r64);

    WriteModrm64(disasm, rm64 - r64);

    if (canDisassemble)
        printf("sub %s, %s\n", disasm.c_str(), Reg64[modrm.reg]);
}

void TigerLake::SubRaxImm32()
{
    uint64_t rax = regs[RAX].reg64;
    uint64_t imm32 = (int64_t)(int32_t)ReadImm32();

    uint64_t result = rax - imm32;

    UpdateFlagsSub64(rax, imm32, result);

    regs[RAX].reg64 = result;

    if (canDisassemble)
        printf("sub rax, 0x%08lx\n", imm32);
}

void TigerLake::XorRm64R64()
{
    FetchModrm();

    std::string disasm;
    uint64_t rm64 = ReadModrm64(disasm);
    uint64_t r64 = regs[modrm.reg].reg64;

    uint64_t result = rm64 ^ r64;

    UpdateFlagsLogic64(result);

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("xor %s, %s\n", disasm.c_str(), Reg64[modrm.reg]);
}

void TigerLake::SubR64Rm64()
{
    FetchModrm();

    std::string disasm;

    uint64_t r64 = regs[modrm.reg].reg64;
    uint64_t rm64 = ReadModrm64(disasm);

    UpdateFlagsSub64(r64, rm64, r64 - rm64);

    regs[modrm.reg].reg64 = r64 - rm64;

    if (canDisassemble)
        printf("sub %s, %s\n", Reg64[modrm.reg], disasm.c_str());
}

void TigerLake::CmpRm64R64()
{
    FetchModrm();

    std::string disasm;

    uint64_t r64 = regs[modrm.reg].reg64;
    uint64_t rm64 = ReadModrm64(disasm);

    UpdateFlagsSub64(rm64, r64, rm64 - r64);

    if (canDisassemble)
        printf("cmp %s, %s (0x%08lx, 0x%08lx, 0x%08lx)\n", disasm.c_str(), Reg64[modrm.reg], rm64 - r64, rm64, r64);
}

void TigerLake::CmpR64Rm64()
{
    FetchModrm();

    std::string disasm;

    uint64_t r64 = regs[modrm.reg].reg64;
    uint64_t rm64 = ReadModrm64(disasm);

    UpdateFlagsSub64(r64, rm64, r64 - rm64);

    if (canDisassemble)
        printf("cmp %s, %s\n", Reg64[modrm.reg], disasm.c_str());
}

void TigerLake::CmpRaxImm32()
{
    uint64_t imm32 = (int64_t)(int32_t)ReadImm32(true);

    uint64_t result = regs[RAX].reg64 - imm32;

    UpdateFlagsSub64(regs[RAX].reg64, imm32, result);

    if (canDisassemble)
        printf("cmp rax, 0x%08lx\n", imm32);
}

void TigerLake::AndRaxImm32()
{
    uint64_t imm32 = (int64_t)(int32_t)ReadImm32(true);
    regs[RAX].reg64 &= imm32;

    UpdateFlagsLogic64(regs[RAX].reg64);

    if (canDisassemble)
        printf("and rax, 0x%08lx\n", imm32);
}

void TigerLake::MovsxdR64Rm32()
{
    FetchModrm();

    std::string disasm;
    uint64_t rm32 = (int64_t)(int32_t)ReadModrm32(disasm);

    regs[modrm.reg].reg64 = rm32;

    if (canDisassemble)
        printf("movsxd %s, %s (0x%08lx)\n", Reg64[modrm.reg], disasm.c_str(), rm32);
}

void TigerLake::ImulR64Rm64Imm8()
{
    FetchModrm();

    std::string disasm;
    uint8_t imm8 = ReadImm8(true);
    uint64_t rm64 = ReadModrm64(disasm);

    int64_t x = (int64_t)rm64;
    int64_t y = (int64_t)imm8;

    int128_t z = (int128_t)x * y;
    int o = z != (int64_t)z;
    rflags.flag_bits.cf = o;
    rflags.flag_bits.of = o;
    
    regs[modrm.reg].reg64 = (uint64_t)z;

    if (canDisassemble)
        printf("imul %s, %s, 0x%02x\n", Reg64[modrm.reg], disasm.c_str(), imm8);
}

void TigerLake::Code81_64()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x00:
        AddRm64Imm32();
        break;
    case 0x04:
        AndRm64Imm32();
        break;
    case 0x05:
        SubRm64Imm32();
        break;
    case 0x07:
        CmpRm64Imm32();
        break;
    default:
        printf("Unknown 64-bit opcode 0x81 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::AddRm64Imm32()
{
    std::string disasm;
    uint64_t imm32 = (int64_t)(int32_t)ReadImm32(true);
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 + imm32;
    UpdateFlagsAdd64(rm64, imm32, result);

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("add %s, 0x%08lx\n", disasm.c_str(), imm32);
}

void TigerLake::AndRm64Imm32()
{
    std::string disasm;
    uint64_t imm32 = (int64_t)(int32_t)ReadImm32(true);
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 & imm32;

    UpdateFlagsLogic64(result);

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("and %s, 0x%08lx\n", disasm.c_str(), imm32);
}

void TigerLake::SubRm64Imm32()
{
    std::string disasm;
    uint64_t imm32 = (int64_t)(int32_t)ReadImm32(true);
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 - (uint64_t)imm32;
    UpdateFlagsSub64(rm64, (uint64_t)imm32, result);

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("sub %s, 0x%08lx\n", disasm.c_str(), imm32);
}

void TigerLake::CmpRm64Imm32()
{
    std::string disasm;
    uint64_t imm32 = (int64_t)(int32_t)ReadImm32(true);
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 - (uint64_t)imm32;
    UpdateFlagsSub64(rm64, (uint64_t)imm32, result);

    if (canDisassemble)
        printf("cmp %s, 0x%08lx\n", disasm.c_str(), imm32);
}

void TigerLake::Code83_64()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x00:
        AddRm64Imm8();
        break;
    case 0x01:
        OrRm64Imm8();
        break;
    case 0x04:
        AndRm64Imm8();
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

void TigerLake::AddRm64Imm8()
{
    std::string disasm;
    uint64_t imm8 = (int64_t)(int8_t)ReadImm8(true);
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 + (uint64_t)imm8;
    UpdateFlagsAdd64(rm64, (uint64_t)imm8, result);

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("add %s, 0x%02lx\n", disasm.c_str(), imm8);
}

void TigerLake::OrRm64Imm8()
{
    std::string disasm;
    uint64_t imm8 = (int64_t)(int8_t)ReadImm8(true);
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 | imm8;

    UpdateFlagsLogic64(result);

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("or %s, 0x%02lx\n", disasm.c_str(), imm8);
}

void TigerLake::AndRm64Imm8()
{
    std::string disasm;
    uint64_t imm8 = (int64_t)(int8_t)ReadImm8(true);
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 & imm8;

    UpdateFlagsLogic64(result);

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("and %s, 0x%02lx\n", disasm.c_str(), imm8);
}

void TigerLake::SubRm64Imm8()
{
    std::string disasm;
    uint64_t imm8 = (int64_t)(int8_t)ReadImm8(true);
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 - (uint64_t)imm8;
    UpdateFlagsSub64(rm64, (uint64_t)imm8, result);

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("sub %s, 0x%02lx\n", disasm.c_str(), imm8);
}

void TigerLake::CmpRm64Imm8()
{
    std::string disasm;
    uint64_t imm8 = (int64_t)(int8_t)ReadImm8(true);
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 - (uint64_t)imm8;
    UpdateFlagsSub64(rm64, (uint64_t)imm8, result);

    if (canDisassemble)
        printf("cmp %s, 0x%02lx\n", disasm.c_str(), imm8);
}

void TigerLake::TestRm64R64()
{
    FetchModrm();

    std::string disasm;
    uint64_t r64 = regs[modrm.reg].reg64;
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 & r64;

    UpdateFlagsLogic64(result);

    if (canDisassemble)
        printf("test %s, %s\n", disasm.c_str(), Reg64[modrm.reg]);
}

void TigerLake::XchgR64Rm64()
{
    FetchModrm();

    std::string disasm;
    uint64_t r64 = regs[modrm.reg].reg64;
    uint64_t rm64 = ReadModrm64(disasm);

    WriteModrm64(disasm, r64);
    regs[modrm.reg].reg64 = rm64;

    if (canDisassemble)
        printf("xchg %s, %s\n", Reg64[modrm.reg], disasm.c_str());
}

void TigerLake::MovRm64R64()
{
    FetchModrm();

    std::string disasm;
    uint64_t r64 = regs[modrm.reg].reg64;

    WriteModrm64(disasm, r64);

    if (canDisassemble)
        printf("mov %s, %s\n", disasm.c_str(), Reg64[modrm.reg]);
}

void TigerLake::MovR64Rm64()
{
    FetchModrm();

    std::string disasm;
    uint64_t rm64 = ReadModrm64(disasm);
    regs[modrm.reg].reg64 = rm64;

    if (canDisassemble)
        printf("mov %s, %s (0x%08lx)\n", Reg64[modrm.reg], disasm.c_str(), rm64);
}

void TigerLake::LeaR64M()
{
    FetchModrm();

    std::string disasm;
    regs[modrm.reg].reg64 = DecodeModrmAddr(disasm);

    if (canDisassemble)
        printf("lea %s, %s\n", Reg64[modrm.reg], disasm.c_str());
}

void TigerLake::Cdqe()
{
    regs[RAX].reg64 = (int64_t)(int32_t)regs[RAX].reg32;

    if (canDisassemble)
        printf("cdqe\n");
}

void TigerLake::Pushfq()
{
    if (mode == Mode::LongMode)
        Push64(rflags.value);
    else
        Push32(rflags.value);

    if (canDisassemble)
        printf("pushf\n");
}

void TigerLake::Stosq()
{
    if (canDisassemble)
        printf("stosq\n");
    
    if (regs[RCX].reg64 == 0 && rep)
        return;

start:
    l1->Write64(TranslateAddr(DS, regs[RDI].reg64), regs[RAX].reg64);
    
    if (!rflags.flag_bits.df) regs[RDI].reg64 += 8;
    else regs[RDI].reg64 -= 8;

    if (rep)
    {
        regs[RCX].reg64--;
        if (regs[RCX].reg64)
            goto start;
    }
}

void TigerLake::MovR64Imm64()
{
    uint8_t reg = l1->Read8(TranslateAddr(CS, rip-1), false) - 0xB8;
    uint64_t imm64 = ReadImm64();

    if (rex.b) reg += 8;

    regs[reg].reg64 = imm64;

    if (canDisassemble)
        printf("mov %s, 0x%08lx\n", Reg64[reg], imm64);
}

void TigerLake::CodeC1_64()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x04:
        ShlRm64Imm8();
        break;
    case 0x05:
        ShrRm64Imm8();
        break;
    default:
        printf("Unknown 64-bit opcode 0xC1 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::ShlRm64Imm8()
{
    std::string disasm;
    uint8_t imm8 = ReadImm8(true);
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 << imm8;
    WriteModrm64(disasm, result);

    rflags.flag_bits.cf = ((rm64 >> (64-imm8)) & 1);
    rflags.flag_bits.pf = CalcPF(result);
    rflags.flag_bits.zf = !(result);
    rflags.flag_bits.sf = (result >> 63) & 1;

    if (imm8 == 1)
        rflags.flag_bits.of = ((rm64 >> 63) & 1) ^ ((rm64 >> 62) & 1);
    
    if (canDisassemble)
        printf("shl %s, 0x%02x\n", disasm.c_str(), imm8);
}

void TigerLake::ShrRm64Imm8()
{
    std::string disasm;
    uint8_t imm8 = ReadImm8(true);
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 >> imm8;
    WriteModrm64(disasm, result);

    rflags.flag_bits.cf = ((rm64 << (imm8-1)) & 1);
    rflags.flag_bits.pf = CalcPF(result);
    rflags.flag_bits.zf = !(result);
    rflags.flag_bits.sf = (result >> 63) & 1;

    if (imm8 == 1)
        rflags.flag_bits.of = ((rm64 >> 63) & 1) ^ ((rm64 >> 62) & 1);
    
    if (canDisassemble)
        printf("shr %s, 0x%02x\n", disasm.c_str(), imm8);
}

void TigerLake::MovRm64Imm32()
{
    FetchModrm();

    uint64_t imm32 = (int64_t)(int32_t)ReadImm32(true);

    std::string disasm;
    WriteModrm64(disasm, imm32);

    if (canDisassemble)
        printf("mov %s, 0x%08lx\n", disasm.c_str(), imm32);
}

void TigerLake::CodeF7_64()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x03:
        NegRm64();
        break;
    case 0x06:
        DivRm64();
        break;
    default:
        printf("Unknown 64-bit opcode 0xF7 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::NegRm64()
{
    std::string disasm;
    uint64_t rm64 = ReadModrm64(disasm);

    if (!rm64) rflags.flag_bits.cf = 0;
    else rflags.flag_bits.cf = 1;

    uint64_t result = -rm64;

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("neg %s\n", disasm.c_str());
}

void TigerLake::DivRm64()
{
    std::string disasm;
    uint64_t rm64 = ReadModrm64(disasm);

    __uint128_t d = ((__uint128_t)regs[RDX].reg64 << 64) | (__uint128_t)regs[RAX].reg64;

    __uint128_t temp = d / rm64;

    if (temp > UINT64_MAX)
    {
        printf("#DE: Divide result > UINT64_MAX\n");
        exit(1);
    }

    regs[RAX].reg64 = (uint64_t)temp;
    regs[RDX].reg64 = d % rm64;

    if (canDisassemble)
        printf("div %s\n", disasm.c_str());
}

void TigerLake::CodeFF_64()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x00:
        IncRm64();
        break;
    case 0x01:
        DecRm64();
        break;
    case 0x02:
        CallRm64();
        break;
    default:
        printf("Unknown 64-bit opcode 0xFF 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::IncRm64()
{
    std::string disasm;
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 + 1UL;
    UpdateFlagsAdd64(rm64, 1UL, result);

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("inc %s\n", disasm.c_str());
}

void TigerLake::DecRm64()
{
    std::string disasm;
    uint64_t rm64 = ReadModrm64(disasm);

    uint64_t result = rm64 - 1UL;
    UpdateFlagsSub64(rm64, 1UL, result);

    WriteModrm64(disasm, result);

    if (canDisassemble)
        printf("dec %s\n", disasm.c_str());
}

void TigerLake::CallRm64()
{
    std::string disasm;
    uint64_t rm64 = ReadModrm64(disasm);

    Push64(rip);

    rip = rm64;

    if (canDisassemble)
        printf("call %s\n", disasm.c_str());
}

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
        printf("Unknown code 0x0f 0x01 0x%x\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::Lgdt()
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

void TigerLake::Lidt()
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

void TigerLake::HintNop()
{
    FetchModrm();

    if (canDisassemble)
        printf("nop\n");
}

void TigerLake::MovR64CRn()
{
    uint8_t val = ReadImm8(true);
    modrm.mod = (val >> 6) & 0x3;
    modrm.reg = (val >> 3) & 0x7;
    modrm.rm = val & 0x7;

    regs[modrm.rm].reg64 = cr[modrm.reg];

    if (canDisassemble)
        printf("mov %s, cr%d\n", Reg32[modrm.rm], modrm.reg);
}

void TigerLake::MovCRnR64()
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

void TigerLake::Wrmsr()
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

void TigerLake::Rdmsr()
{
    uint64_t val;
    switch (regs[RCX].reg32)
    {
    case 0x0000001B:
        printf("Reading from IA32_APIC_BASE\n");
        val = ia32_apic_base;
        break;
    case 0x0000080F:
        val = lapic->ReadSpuriousVec();
        break;
    case 0xC0000080:
        val = efer.val;
        break;
    default:
        printf("Unknown msr 0x%08x\n", regs[RCX].reg32);
        exit(1);
    }

    regs[RAX].reg64 = val & 0xffffffff;
    regs[RDX].reg64 = (val >> 32);

    if (canDisassemble)
        printf("rdmsr\n");
}

void TigerLake::JnzRel32()
{
    int32_t rel32 = ReadImm32(true);

    if (canDisassemble)
        printf("jnz 0x%08lx\n", TranslateAddr(CS, rip + (int64_t)rel32));

    if (!rflags.flag_bits.zf)
        rip += (int64_t)rel32;
}

void TigerLake::SetzRm8()
{
    FetchModrm();

    std::string disasm;
    
    if (rflags.flag_bits.zf)
        WriteModrm8(disasm, 1);
    else
        WriteModrm8(disasm, 0);
    
    if (canDisassemble)
        printf("setz %s\n", disasm.c_str());
}

void TigerLake::SetneRm8()
{
    FetchModrm();

    std::string disasm;
    
    if (!rflags.flag_bits.zf)
        WriteModrm8(disasm, 1);
    else
        WriteModrm8(disasm, 0);
    
    if (canDisassemble)
        printf("setne %s\n", disasm.c_str());
}

void TigerLake::SetaRm8()
{
    FetchModrm();

    std::string disasm;
    
    if (!rflags.flag_bits.zf && !rflags.flag_bits.cf)
        WriteModrm8(disasm, 1);
    else
        WriteModrm8(disasm, 0);
    
    if (canDisassemble)
        printf("seta %s\n", disasm.c_str());
}

void TigerLake::Cpuid()
{
    uint32_t func = regs[RAX].reg32;

    switch (func)
    {
    case 0x00000000:
        regs[RAX].reg32 = 0x1b;
        regs[RBX].reg32 = 0x756e6547;
        regs[RCX].reg32 = 0x6c65746e;
        regs[RDX].reg32 = 0x49656e69;
        break;
    case 0x00000001:
        regs[RAX].reg32 = (id << 24) | 0x000806c1;
        regs[RBX].reg32 = 0x00080800;
        regs[RCX].reg32 = 0xfffab223;
        regs[RDX].reg32 = 0x1f8bfbff;
        break;
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

void TigerLake::CmovcR32Rm32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);

    if (rflags.flag_bits.cf)
        regs[modrm.reg].reg64 = rm32;
    
    if (canDisassemble)
        printf("cmovc %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void TigerLake::CmovncR32Rm32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);

    if (!rflags.flag_bits.cf)
        regs[modrm.reg].reg64 = rm32;
    
    if (canDisassemble)
        printf("cmovnc %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void TigerLake::CmovzR32Rm32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);

    if (rflags.flag_bits.zf)
        regs[modrm.reg].reg64 = rm32;
    
    if (canDisassemble)
        printf("cmovz %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void TigerLake::CmovnaR32Rm32()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);

    if (rflags.flag_bits.cf || rflags.flag_bits.zf)
        regs[modrm.reg].reg64 = rm32;
    
    if (canDisassemble)
        printf("cmovna %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void TigerLake::JcRel32()
{
    int32_t rel32 = ReadImm32(true);

    if (canDisassemble)
        printf("jc 0x%08lx\n", TranslateAddr(CS, rip+(int64_t)rel32));

    if (rflags.flag_bits.cf)
        rip += (int64_t)rel32;
}

void TigerLake::JncRel32()
{
    int32_t rel32 = ReadImm32(true);

    if (canDisassemble)
        printf("jnc 0x%08lx\n", TranslateAddr(CS, rip+(int64_t)rel32));

    if (!rflags.flag_bits.cf)
        rip += (int64_t)rel32;
}

void TigerLake::JzRel32()
{
    int32_t rel32 = ReadImm32(true);

    if (canDisassemble)
        printf("jz 0x%08lx (%s)\n", TranslateAddr(CS, rip+(int64_t)rel32), rflags.flag_bits.zf ? "z" : ".");

    if (rflags.flag_bits.zf)
        rip += (int64_t)rel32;
}

void TigerLake::JnaRel32()
{
    int32_t rel32 = ReadImm32(true);

    if (canDisassemble)
        printf("jna 0x%08lx\n", TranslateAddr(CS, rip+(int64_t)rel32));

    if (rflags.flag_bits.zf || rflags.flag_bits.cf)
        rip += (int64_t)rel32;
}

void TigerLake::JaRel32()
{
    int32_t rel32 = ReadImm32(true);

    if (canDisassemble)
        printf("ja 0x%08lx\n", TranslateAddr(CS, rip+(int64_t)rel32));

    if (!rflags.flag_bits.zf && !rflags.flag_bits.cf)
        rip += (int64_t)rel32;
}

void TigerLake::JsRel32()
{
    int32_t rel32 = ReadImm32(true);

    if (canDisassemble)
        printf("js 0x%08lx\n", TranslateAddr(CS, rip+(int64_t)rel32));

    if (rflags.flag_bits.sf)
        rip += (int64_t)rel32;
}

void TigerLake::Code0FAE()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x02:
        Ldmxcsr();
        break;
    default:
        printf("Unknown opcode 0x0F 0xAE 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::Ldmxcsr()
{
    std::string disasm;
    uint32_t rm32 = ReadModrm32(disasm);

    sse1.mxcsr = rm32;

    if (canDisassemble)
        printf("ldmxcsr %s\n", disasm.c_str());
}

void TigerLake::ImulR32Rm32()
{
    FetchModrm();

    std::string disasm;
    int32_t r32 = (int32_t)regs[modrm.reg].reg32;
    int32_t rm32 = (int32_t)ReadModrm32(disasm);

    int64_t result = (int64_t)r32 * (int64_t)rm32;

    regs[modrm.reg].reg32 = (uint32_t)result;

    if (((int64_t)(int32_t)regs[modrm.reg].reg32) != result)
    {
        rflags.flag_bits.cf = 1;
        rflags.flag_bits.of = 1;
    }
    else
    {
        rflags.flag_bits.cf = 0;
        rflags.flag_bits.of = 0;
    }

    if (canDisassemble)
        printf("imul %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void TigerLake::MovzxR32Rm8()
{
    FetchModrm();

    std::string disasm;
    uint8_t rm8 = ReadModrm8(disasm);

    regs[modrm.reg].reg64 = (uint32_t)rm8;

    if (canDisassemble)
        printf("movzx %s, %s (0x%08x)\n", Reg32[modrm.reg], disasm.c_str(), rm8);
}

void TigerLake::MovzxR32Rm16()
{
    FetchModrm();

    std::string disasm;
    uint16_t rm16 = ReadModrm16(disasm);

    regs[modrm.reg].reg64 = (uint32_t)rm16;

    if (canDisassemble)
        printf("movzx %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void TigerLake::MovsxR32Rm8()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm8 = (int32_t)(int8_t)ReadModrm8(disasm);

    regs[modrm.reg].reg64 = (uint32_t)rm8;

    if (canDisassemble)
        printf("movsx %s, %s\n", Reg32[modrm.reg], disasm.c_str());
}

void TigerLake::Code0FBA_32()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x04:
        BtRm32Imm8();
        break;
    case 0x05:
        BtsRm32Imm8();
        break;
    default:
        printf("Unknown 32-bit opcode 0x0F 0xBA 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::BtRm32Imm8()
{
    std::string disasm;
    uint8_t imm8 = ReadImm8(true);
    uint32_t rm32 = ReadModrm32(disasm);

    rflags.flag_bits.cf = rm32 & (1 << imm8);

    if (canDisassemble)
        printf("bt %s, 0x%02x\n", disasm.c_str(), imm8);
}

void TigerLake::BtsRm32Imm8()
{
    std::string disasm;
    uint8_t imm8 = ReadImm8(true);
    uint32_t rm32 = ReadModrm32(disasm);

    rflags.flag_bits.cf = rm32 & (1 << imm8);
    rm32 |= (1 << imm8);
    WriteModrm32(disasm, rm32);

    if (canDisassemble)
        printf("bts %s, 0x%02x\n", disasm.c_str(), imm8);
}

void TigerLake::BswapR32()
{
    uint8_t reg = l1->Read8(TranslateAddr(CS, rip-1), false) - 0xC8;
    if (rex.b) reg += 8;
    
    regs[reg].reg64 = __builtin_bswap32(regs[reg].reg32);

    if (canDisassemble)
        printf("bswap %s\n", Reg32[reg]);
}

void TigerLake::CmovcR64Rm64()
{
    FetchModrm();

    std::string disasm;
    uint64_t rm64 = ReadModrm64(disasm);

    if (rflags.flag_bits.cf)
        regs[modrm.reg].reg64 = rm64;
    
    if (canDisassemble)
        printf("cmovc %s, %s\n", Reg64[modrm.reg], disasm.c_str());
}

void TigerLake::CmovneR64Rm64()
{
    FetchModrm();

    std::string disasm;
    uint64_t rm64 = ReadModrm64(disasm);

    if (!rflags.flag_bits.zf)
        regs[modrm.reg].reg64 = rm64;
    
    if (canDisassemble)
        printf("cmovne %s, %s\n", Reg64[modrm.reg], disasm.c_str());
}

void TigerLake::MovzxR64Rm8()
{
    FetchModrm();

    std::string disasm;
    uint8_t rm8 = ReadModrm8(disasm);

    regs[modrm.reg].reg64 = (uint64_t)rm8;

    if (canDisassemble)
        printf("movzx %s, %s\n", Reg64[modrm.reg], disasm.c_str());
}

void TigerLake::Code0FBA_64()
{
    FetchModrm();

    switch (modrm.reg)
    {
    case 0x06:
        BtrRm64Imm8();
        break;
    default:
        printf("Unknown 64-bit opcode 0x0F 0xBA 0x%02X\n", modrm.reg);
        exit(1);
    }
}

void TigerLake::BtrRm64Imm8()
{
    std::string disasm;
    uint8_t imm8 = ReadImm8(true);
    uint64_t rm64 = ReadModrm64(disasm);

    rflags.flag_bits.cf = rm64 & (1 << imm8);

    rm64 &= ~(1 << imm8);

    WriteModrm64(disasm, rm64);

    if (canDisassemble)
        printf("btr %s, 0x%02x\n", disasm.c_str(), imm8);
}

void TigerLake::MovsxR64Rm8()
{
    FetchModrm();

    std::string disasm;
    uint32_t rm8 = (int32_t)(int8_t)ReadModrm8(disasm);

    regs[modrm.reg].reg64 = (uint64_t)rm8;

    if (canDisassemble)
        printf("movsx %s, %s (0x%08x)\n", Reg64[modrm.reg], disasm.c_str(), rm8);
}
