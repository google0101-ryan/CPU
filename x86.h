#pragma once

#include <cstdint>
#include <types.h>
#include <Cache.h>
#include <unordered_map>
#include <functional>
#include <string>
#include <Interrupts/LocalAPIC.h>

class TigerLake
{
private:
    Register regs[Registers::NUM_REGS];
    Segment segs[Segments::NUM_SEGS];
    uint32_t cr[8];

    static constexpr bool canDisassemble = true;

    enum Mode
    {
        RealMode,
        ProtectedMode,
        LongMode
    } mode = Mode::RealMode;

    bool is32 = false, is64 = false;
    bool a32 = false, a64 = false;

    bool rep = false;

    Segments prefix = Segments::DS;

    uint64_t Read64(Segments seg, uint64_t addr);
    uint32_t Read32(Segments seg, uint64_t addr);
    uint16_t Read16(Segments seg, uint64_t addr);
    uint8_t Read8(Segments seg, uint64_t addr);

    void Write32(Segments seg, uint64_t addr, uint32_t val);
    void Write16(Segments seg, uint64_t addr, uint16_t val);
    void Write8(Segments seg, uint64_t addr, uint8_t val);

    void Push8(uint8_t val);
    void Push32(uint32_t val);
    void Push64(uint64_t val);

    uint64_t Pop64();
    uint32_t Pop32();

    uint64_t TranslateAddr(Segments seg, uint64_t addr);

    uint8_t ReadImm8(bool is_instr = false);
    uint16_t ReadImm16(bool is_instr = false);
    uint32_t ReadImm32(bool is_instr = false);
    uint64_t ReadImm64(bool is_instr = false);

    void FetchModrm();
    void FetchModrm16();
    void FetchModrm32();
    void FetchModrm64();

    void FetchSib32();

    uint64_t DecodeModrmAddr(std::string& disasm);
    uint64_t DecodeModrmAddr16(std::string& disasm);
    uint64_t DecodeModrmAddr32(std::string& disasm);
    uint64_t DecodeModrmAddr64(std::string& disasm);

    uint64_t DecodeSIBAddr(std::string& disasm);

    uint64_t ReadModrmMMX(std::string& disasm);
    uint64_t ReadModrm64(std::string& disasm);
    uint32_t ReadModrm32(std::string& disasm);
    uint16_t ReadModrm16(std::string& disasm);
    uint8_t ReadModrm8(std::string& disasm);

    void WriteModrm8(std::string& disasm, uint8_t val);
    void WriteModrm16(std::string& disasm, uint16_t val);
    void WriteModrm32(std::string& disasm, uint32_t val);
    void WriteModrm64(std::string& disasm, uint64_t val);
    void WriteModrmMMX(std::string& disasm, uint64_t val);

    void WriteReg8(Registers regs, uint8_t val);
    uint8_t ReadReg8(Registers regs);

    Cache* l1; // We don't emulate multi-level yet

    uint64_t rip;
    RFLAGS rflags;

    int id = 0;
    bool halted = false;
    bool fwait = false;

    struct
    {
        bool rex = false;
        bool b = false;
        bool x = false;
        bool r = false;
    } rex;

    struct
    {
        uint32_t mod;
        uint32_t rm;
        uint32_t reg;
    } modrm;

    union
    {
        uint8_t value;
        struct
        {
            uint8_t base : 3;
            uint8_t index : 3;
            uint8_t scale : 2;
        };
    } sib_;

    struct
    {
        uint32_t base;
        uint32_t index;
        uint32_t scale;
    } sib;
    

    DTR idtr, gdtr;

    int8_t disp8, sib_disp8;
    int16_t disp16;
    int32_t disp32, sib_disp32;

    union
    {
        uint64_t val;
        struct
        {
            uint64_t sce : 1;
            uint64_t : 7;
            uint64_t lme : 1;
            uint64_t lma : 1;
            uint64_t nxe : 1;
            uint64_t svme : 1;
            uint64_t lmsle : 1;
            uint64_t ffxsr : 1;
            uint64_t tce : 1;
            uint64_t : 48;
        };
    } efer;

    uint64_t ia32_apic_base;

    LocalAPIC* lapic;

    struct
    {
        uint16_t control;
        uint16_t status;
        uint16_t tagWord;
        uint16_t dataPointer;
        uint16_t fpuIp;
        uint16_t lastInstr;
    } fpu;

    struct
    {
        uint32_t mxcsr;
        SseRegister xmm[16];
    } sse1;

    union MMXRegs
    {
        uint64_t reg64;
        uint32_t reg32[2];
        uint16_t reg16[4];
        uint8_t reg8[8];
    } mmx[8];

    uint64_t tsc;

    bool is_sse = false;

    using InstrFunc =  std::function<void()>;
    std::unordered_map<uint8_t, InstrFunc> lookup16, extLookup16,
                                            lookup32, extLookup32,
                                            lookup64, extLookup64;
    
    void MakeOpcodeTables();

    // RFLAGS convenience functions
    void UpdateFlagsLogic8(uint8_t result);
    void UpdateFlagsLogic16(uint16_t result);
    void UpdateFlagsLogic32(uint32_t result);
    void UpdateFlagsLogic64(uint64_t result);
    void UpdateFlagsSub8(uint8_t value1, uint8_t value2, uint32_t result);
    void UpdateFlagsSub16(uint16_t value1, uint16_t value2, uint32_t result);
    void UpdateFlagsSub32(uint32_t value1, uint32_t value2, uint64_t result);
    void UpdateFlagsSub64(uint64_t value1, uint64_t value2, uint64_t result);
    void UpdateFlagsAdd8(uint8_t value1, uint8_t value2, uint16_t result);
    void UpdateFlagsAdd16(uint16_t value1, uint16_t value2, uint32_t result);
    void UpdateFlagsAdd32(uint32_t value1, uint32_t value2, uint64_t result);
    void UpdateFlagsAdd64(uint64_t value1, uint64_t value2, uint64_t result);

    // All modes
    void AddAlImm8(); // 0x04
    void OrR8Rm8(); // 0x0A
    void OrAlImm8(); // 0x0C
    void AndAlImm8(); // 0x24
    void CmpRm8R8(); // 0x38
    void CmpR8Rm8(); // 0x3A
    void CmpAlImm8(); // 0x3C
    void PushImm8(); // 0x6A
    void JoRel8(); // 0x70
    void JnoRel8(); // 0x71
    void JcRel8(); // 0x72
    void JncRel8(); // 0x73
    void JzRel8(); // 0x74
    void JnzRel8(); // 0x75
    void JnaRel8(); // 0x76
    void JaRel8(); // 0x77
    void JsRel8(); // 0x78
    void JnsRel8(); // 0x79
    void JpRel8(); // 0x7A
    void JnpRel8(); // 0x7B
    void JlRel8(); // 0x7C
    void JgeRel8(); // 0x7D
    void JngRel8(); // 0x7E
    void JgRel8(); // 0x7F
    void Code80(); // 0x80
        void OrRm8Imm8(); // 0x80 0x01
        void AndRm8Imm8(); // 0x80 0x04
        void CmpRm8Imm8(); // 0x80 0x07
    void TestRm8R8(); // 0x84
    void MovRm8R8(); // 0x88
    void MovR8Rm8(); // 0x8A
    void MovSregRm16(); // 0x8E
    void Nop(); // 0x90
    void Sahf(); // 0x9E
    void Movsb(); // 0xA4
    void TestAlImm8(); // 0xA8
    void Stosb(); // 0xAA
    void MovR8Imm8(); // 0xB0+r
    void MovRm8Imm8(); // 0xC6
    void CodeD0(); // 0xD0
        void ShlRm81(); // 0xD0 0x04
    void InAlImm8(); // 0xE4
    void OutImm8Al(); // 0xE6
    void JmpRel8(); // 0xEB
    void InAlDx(); // 0xEC
    void CodeF6(); // 0xF6
        void TestRm8Imm8(); // 0xF6 0x00-0x01
        void DivAlRm8(); // 0xF6 0x06
        void NegRm8(); // 0xF6 0x03
    void Cld(); // 0xFC
    void Cli(); // 0xFA
    void CodeFE(); // 0xFE
    void DecRm8(); // 0xFE 0x01

    // 16-bit mode
    void OrRm16R16(); // 0x09
    void OrR16Rm16(); // 0x0B
    void AndR16Rm16(); // 0x23
    void XorRm16R16(); // 0x31
    void CmpR16Rm16(); // 0x3B
    void CmpAxImm16(); // 0x3D
    void IncR16(); // 0x40+r
    void Code81_16(); // 0x81
        void AddRm16Imm16(); // 0x81 0x00
        void CmpRm16Imm16(); // 0x81 0x07
    void Code83_16(); // 0x81
    void CmpRm16Imm8(); // 0x83 0x07
    void TestRm16R16(); // 0x85
    void MovRm16R16(); // 0x89
    void MovR16Rm16(); // 0x8B
    void MovMoffs16Ax(); // 0xA3
    void MovR16Imm16(); // 0xB8+r
    void CodeC1_16(); // 0xC1
    void ShrRm16Imm8(); // 0xC1 0x05
    void MovRm16Imm16(); // 0xC7
    void LoopCxRel8(); // 0xE2
    void JcxzRel8(); // 0xE3
    void JmpRel16(); // 0xE9
    void JmpPtr1616(); // 0xEA
    void InAxDx(); // 0xED
    void OutDxAx(); // 0xEF
    void CodeF7_16(); // 0xF7
    void TestRm16Imm16(); // 0xF7 0x00

    // 32-bit mode
    void AddRm32R32(); // 0x01
    void AddR32Rm32(); // 0x03
    void AddEaxImm32(); // 0x05
    void OrRm32R32(); // 0x09
    void OrR32Rm32(); // 0x0B
    void OrEaxImm32(); // 0x0D
    void AdcR32Rm32(); // 0x13
    void SbbRm32R32(); // 0x19
    void SbbR32Rm32(); // 0x1B
    void AndRm32R32(); // 0x21
    void AndR32Rm32(); // 0x23
    void AndEaxImm32(); // 0x25
    void SubRm32R32(); // 0x29
    void SubR32Rm32(); // 0x2B
    void SubAlImm8(); // 0x2C
    void SubEaxImm32(); // 0x2D
    void XorRm32R32(); // 0x31
    void XorR32Rm32(); // 0x33
    void CmpRm32R32(); // 0x39
    void CmpR32Rm32(); // 0x3B
    void CmpEaxImm32(); // 0x3D
    void IncR32(); // 0x40+r
    void DecR32(); // 0x48+r
    void PushR32(); // 0x50+r
    void PopR32(); // 0x58+r
    void Pusha(); // 0x60
    void Popa(); // 0x61
    void PushImm32(); // 0x68
    void ImulR32Rm32Imm8(); // 0x6B
    void Outsb(); // 0x6E
    void Pshufw(); // 0x70
    void Code81_32(); // 0x81
        void AddRm32Imm32(); // 0x81 0x00
        void OrRm32Imm32(); // 0x81 0x01
        void AndRm32Imm32(); // 0x81 0x04
        void SubRm32Imm32(); // 0x81 0x05
        void XorRm32Imm32(); // 0x81 0x06
        void CmpRm32Imm32(); // 0x81 0x07
    void Code83_32(); // 0x83
        void AddRm32Imm8(); // 0x83 0x00
        void OrRm32Imm8(); // 0x83 0x01
        void AndRm32Imm8(); // 0x83 0x04
        void SubRm32Imm8(); // 0x83 0x05
        void CmpRm32Imm8(); // 0x83 0x07
    void TestRm32R32(); // 0x85
    void MovRm32R32(); // 0x89
    void MovR32Rm32(); // 0x8B
    void LeaR32M(); // 0x8D
    void Popf(); // 0x9D
    void MovEaxMoffs32(); // 0xA1
    void MovMoffs32Eax(); // 0xA3
    void Movsd(); // 0xA5
    void BtRm32R32(); // 0xA3
    void Stosd(); // 0xAB
    void MovR32Imm32(); // 0xB8+r
    void CodeC1_32(); // 0xC1
        void RolRm32Imm8(); // 0xC1 0x00
        void RorRm32Imm8(); // 0xC1 0x01
        void ShlRm32Imm8(); // 0xC1 0x04
        void ShrRm32Imm8(); // 0xC1 0x05
        void SarRm32Imm8(); // 0xC1 0x07
    void Ret(); // 0xC3
    void MovRm32Imm32(); // 0xC7
    void Leave(); // 0xC9
    void CodeD1_32(); // 0xD1
        void ShlRm321(); // 0xD1 0x04
        void ShrRm321(); // 0xD1 0x05
    void CodeD3_32(); // 0xD3
        void ShlRm32Cl(); // 0xD3 0x04
        void ShrRm32Cl(); // 0xD3 0x05
    void CodeD9(); // 0xD9
    void Fldcw(); // 0xD9 0x05
    void CodeDB(); // 0xDB
    void Finit(); // 0xE3 0xDB 0x04
    void LoopEcxRel8(); // 0xE2
    void Jecxz(); // 0xE3
    void CallRel32(); // 0xE8
    void JmpRel32(); // 0xE9
    void JmpPtr1632(); // 0xEA
    void InEaxDx(); // 0xED
    void OutDxAl(); // 0xEE
    void OutDxEax(); // 0xEF
    void CodeF7_32(); // 0xF7
        void TestRm32Imm32(); // 0xF7 0x00-0x01
        void NotRm32(); // 0xF7 0x02
        void NegRm32(); // 0xF7 0x03
        void MulEdxEaxRm32(); // 0xF7 0x04
        void DivEdxEaxRm32(); // 0xF7 0x06
    void CodeFF_32(); // 0xFF
        void IncRm32(); // 0xFF 0x00
        void DecRm32(); // 0xFF 0x01
        void CallRm32(); // 0xFF 0x02
        void JmpRm32(); // 0xFF 0x04
        void JmpfRm32(); // 0xFF 0x05
        void PushRm32(); // 0xFF 0x06

    // 64-bit mode
    void AddRm64R64(); // 0x01
    void AddR64Rm64(); // 0x03
    void AddRaxImm32(); // 0x05
    void OrRm64R64(); // 0x09
    void OrR64Rm64(); // 0x0B
    void OrRaxImm32(); // 0x0D
    void SbbRm64R64(); // 0x19
    void AndRm64R64(); // 0x21
    void AndR64Rm64(); // 0x23
    void AndRaxImm32(); // 0x25
    void SubRm64R64(); // 0x29
    void SubRaxImm32(); // 0x2D
    void XorRm64R64(); // 0x31
    void SubR64Rm64(); // 0x3B
    void CmpRm64R64(); // 0x39
    void CmpR64Rm64(); // 0x3B
    void CmpRaxImm32(); // 0x3D
    void MovsxdR64Rm32(); // 0x63
    void ImulR64Rm64Imm8(); // 0x6B
    void Code81_64(); // 0x81
        void AddRm64Imm32(); // 0x81 0x00
        void AndRm64Imm32(); // 0x81 0x04
        void SubRm64Imm32(); // 0x81 0x05
        void CmpRm64Imm32(); // 0x81 0x07
    void Code83_64(); // 0x83
        void AddRm64Imm8(); // 0x83 0x00
        void OrRm64Imm8(); // 0x83 0x01
        void AndRm64Imm8(); // 0x83 0x04
        void SubRm64Imm8(); // 0x83 0x05
        void CmpRm64Imm8(); // 0x83 0x07
    void TestRm64R64(); // 0x85
    void XchgR64Rm64(); // 0x87
    void MovRm64R64(); // 0x89
    void MovR64Rm64(); // 0x8B
    void LeaR64M(); // 0x8D
    void Cdqe(); // 0x98
    void Pushfq(); // 0x9C
    void Stosq(); // 0xAB
    void MovR64Imm64(); // 0xB8+r
    void CodeC1_64(); // 0xC1
        void ShlRm64Imm8(); // 0xC1 0x04
        void ShrRm64Imm8(); // 0xC1 0x05
    void MovRm64Imm32(); // 0xC7
    void CodeF7_64(); // 0xF7
        void NegRm64(); // 0xF7 0x03
        void DivRm64(); // 0xF7 0x06
    void CodeFF_64(); // 0xFF
        void IncRm64(); // 0xFF 0x00
        void DecRm64(); // 0xFF 0x01
        void CallRm64(); // 0xFF 0x02

    // All modes, 0x0f
    void Code0f01(); // 0x01
        void Lgdt(); // 0x01 0x02
        void Lidt(); // 0x01 0x03
    void HintNop(); // 0x1E
    void MovR64CRn(); // 0x20
    void MovCRnR64(); // 0x22
    void Wrmsr(); // 0x30
    void Rdtsc(); // 0x31
    void Rdmsr(); // 0x32
    void MovdMmRm32(); // 0x6E
    void MovdRm32Mm(); // 0x7E
    void JnzRel32(); // 0x85
    void SetzRm8(); // 0x94
    void SetneRm8(); // 0x95
    void SetaRm8(); // 0x97
    void Cpuid(); // 0xA2

    // 16-bit 0x0f
    void JncRel16(); // 0x83
    void JnzRel16(); // 0x85

    // 32-bit, 0x0f
    void CmovcR32Rm32(); // 0x42
    void CmovncR32Rm32(); // 0x43
    void CmovzR32Rm32(); // 0x44
    void CmovnaR32Rm32(); // 0x46
    void MovdMMRm32(); // 0x6E
    void JcRel32(); // 0x82
    void JncRel32(); // 0x83
    void JzRel32(); // 0x84
    void JnaRel32(); // 0x86
    void JaRel32(); // 0x87
    void JsRel32(); // 0x88
    void JnlRel32(); // 0x8D
    void BtsRm32R32(); // 0xAB
    void Code0FAE(); // 0xAE
    void Ldmxcsr(); // 0xAE 0x02
    void ShrdRm32R32Cl(); // 0xAD
    void ImulR32Rm32(); // 0xAF
    void MovzxR32Rm8(); // 0xB6
    void MovzxR32Rm16(); // 0xB7
    void MovsxR32Rm8(); // 0xB6
    void Code0FBA_32(); // 0xBA
    void BtRm32Imm8(); // 0xBA 0x05
    void BtsRm32Imm8(); // 0xBA 0x05
    void BsfR32Rm32(); // 0xBC
    void SetnlRm8(); // 0x9D
    void Pinsrw(); // 0xC4
    void BswapR32(); // 0xC8 + r

    // 64-bit, 0x0f
    void CmovcR64Rm64(); // 0x42
    void CmovneR64Rm64(); // 0x45
    void MovzxR64Rm8(); // 0xB6
    void Code0FBA_64(); // 0xBA
    void BtrRm64Imm8(); // 0xBA 0x06
    void MovsxR64Rm8(); // 0xBE

    void CacheSegment(Segments seg, uint16_t sel, uint32_t base, uint32_t lim, uint16_t access);
    void CacheSegment(Segments seg, uint16_t sel);

    bool efer_lme = false;
    void CheckModeChange();

    // Ok, so weird hack with the A20 line
    // Because UEFI is located at 0xFFFFFFF0, and that's more than 20 bits (A20 is off by default (sometimes))
    // The MMU of x86 doesn't apply a20 until the first long jump
    // So as long as a seg:addr jump doesn't take place, a20 isn't applied
    bool a20 = true;
    bool longJumpDone = false;

    const char* GetReg8(Registers reg);
public:
    TigerLake();
    ~TigerLake();

    void Reset();
    void Clock();

    void Dump();
};