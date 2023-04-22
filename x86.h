#pragma once

#include <cstdint>
#include <types.h>
#include <Cache.h>
#include <unordered_map>
#include <functional>
#include <string>

class TigerLake
{
private:
    Register regs[Registers::NUM_REGS];
    Segment segs[Segments::NUM_SEGS];
    uint32_t cr[8];

    bool canDisassemble = false;

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

    uint64_t ReadModrm64(std::string& disasm);
    uint32_t ReadModrm32(std::string& disasm);
    uint16_t ReadModrm16(std::string& disasm);
    uint8_t ReadModrm8(std::string& disasm);

    void WriteModrm8(std::string& disasm, uint8_t val);
    void WriteModrm16(std::string& disasm, uint16_t val);
    void WriteModrm32(std::string& disasm, uint32_t val);
    void WriteModrm64(std::string& disasm, uint64_t val);

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
    } sse1;

    using InstrFunc =  std::function<void()>;
    std::unordered_map<uint8_t, InstrFunc> lookup16, extLookup16,
                                            lookup32, extLookup32,
                                            lookup64, extLookup64;
    
    void MakeOpcodeTables();

    // RFLAGS convenience functions
    void UpdateFlagsLogic8(uint8_t result);
    void UpdateFlagsLogic32(uint32_t result);
    void UpdateFlagsLogic64(uint64_t result);
    void UpdateFlagsSub8(uint8_t value1, uint8_t value2, uint32_t result);
    void UpdateFlagsSub16(uint16_t value1, uint16_t value2, uint32_t result);
    void UpdateFlagsSub32(uint32_t value1, uint32_t value2, uint64_t result);
    void UpdateFlagsSub64(uint64_t value1, uint64_t value2, uint64_t result);
    void UpdateFlagsAdd32(uint32_t value1, uint32_t value2, uint64_t result);
    void UpdateFlagsAdd64(uint64_t value1, uint64_t value2, uint64_t result);

    // All modes
    void AndAlImm8(); // 0x24
    void CmpAlImm8(); // 0x3C
    void JcRel8(); // 0x72
    void JncRel8(); // 0x73
    void JzRel8(); // 0x74
    void JnzRel8(); // 0x75
    void JnaRel8(); // 0x76
    void JaRel8(); // 0x77
    void JsRel8(); // 0x79
    void JlRel8(); // 0x7C
    void JgeRel8(); // 0x7D
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
    void TestAlImm8(); // 0xA8
    void MovRm8Imm8(); // 0xC6
    void JmpRel8(); // 0xEB
    void InAlDx(); // 0xEC
    void CodeF6(); // 0xF6
    void TestRm8Imm8(); // 0xF6 0x00-0x01
    void NegRm8(); // 0xF6 0x03
    void Cld(); // 0xFC
    void Cli(); // 0xFA

    // 16-bit mode
    void Code81_16(); // 0x81
    void CmpRm16Imm16(); // 0x81 0x07
    void MovR16Rm16(); // 0x8B
    void MovR16Imm16(); // 0xB8+r
    void CodeC1_16(); // 0xC1
    void ShrRm16Imm8(); // 0xC1 0x05
    void MovRm16Imm16(); // 0xC7
    void JmpRel16(); // 0xE9
    void InAxDx(); // 0xED

    // 32-bit mode
    void AddRm32R32(); // 0x01
    void AddR32Rm32(); // 0x03
    void AddEaxImm32(); // 0x05
    void OrRm32R32(); // 0x09
    void AndRm32R32(); // 0x21
    void AndEaxImm32(); // 0x25
    void SubEaxImm32(); // 0x2D
    void XorRm32R32(); // 0x31
    void CmpRm32R32(); // 0x39
    void CmpEaxImm32(); // 0x3D
    void IncR32(); // 0x40+r
    void DecR32(); // 0x48+r
    void PushR32(); // 0x50+r
    void PopR32(); // 0x58+r
    void Outsb(); // 0x6E
    void Code81_32(); // 0x81
    void OrRm32Imm32(); // 0x81 0x01
    void AndRm32Imm32(); // 0x81 0x04
    void CmpRm32Imm32(); // 0x81 0x07
    void Code83_32(); // 0x83
    void AddRm32Imm8(); // 0x83 0x00
    void AndRm32Imm8(); // 0x83 0x04
    void SubRm32Imm8(); // 0x83 0x05
    void CmpRm32Imm8(); // 0x83 0x07
    void TestRm32R32(); // 0x85
    void MovRm32R32(); // 0x89
    void MovR32Rm32(); // 0x8B
    void LeaR32M(); // 0x8D
    void BtRm32R32(); // 0xA3
    void MovR8Imm8(); // 0xB0+r
    void MovR32Imm32(); // 0xB8+r
    void CodeC1_32(); // 0xC1
    void ShlRm32Imm8(); // 0xC1 0x04
    void Ret(); // 0xC3
    void MovRm32Imm32(); // 0xC7
    void CodeD9(); // 0xD9
    void Fldcw(); // 0xD9 0x05
    void CodeDB(); // 0xDB
    void Finit(); // 0xE3 0xDB 0x04
    void LoopEcxRel8(); // 0xE2
    void CallRel32(); // 0xE8
    void JmpRel32(); // 0xE9
    void JmpPtr1632(); // 0xEA
    void InEaxDx(); // 0xED
    void OutDxAl(); // 0xEE
    void OutDxEax(); // 0xEF
    void CodeF7_32(); // 0xF7
    void TestRm32Imm32(); // 0xF7 0x00-0x01
    void CodeFF_32(); // 0xFF
    void JmpRm32(); // 0xFF 0x04

    // 64-bit mode
    void AddRm64R64(); // 0x01
    void AddR64Rm64(); // 0x03
    void OrR64Rm64(); // 0x0B
    void OrRaxImm32(); // 0x0D
    void SbbRm64R64(); // 0x19
    void AndRm64R64(); // 0x21
    void SubRm64R64(); // 0x29
    void SubRaxImm32(); // 0x2D
    void SubR64Rm64(); // 0x3B
    void CmpRm64R64(); // 0x39
    void CmpR64Rm64(); // 0x3B
    void CmpRaxImm32(); // 0x3D
    void MovsxdR64Rm32(); // 0x63
    void Code81_64(); // 0x81
    void AddRm64Imm32(); // 0x81 0x00
    void AndRm64Imm32(); // 0x81 0x04
    void SubRm64Imm32(); // 0x81 0x05
    void CmpRm64Imm32(); // 0x81 0x07
    void Code83_64(); // 0x83
    void AddRm64Imm8(); // 0x83 0x00
    void OrRm64Imm8(); // 0x83 0x01
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

    // All modes, 0x0f
    void Code0f01(); // 0x01
        void Lgdt(); // 0x01 0x02
        void Lidt(); // 0x01 0x03
    void HintNop(); // 0x1E
    void MovR64CRn(); // 0x20
    void MovCRnR64(); // 0x22
    void Wrmsr(); // 0x30
    void Rdmsr(); // 0x32
    void JnzRel32(); // 0x85
    void SetneRm8(); // 0x95
    void Cpuid(); // 0xA2

    // 32-bit, 0x0f
    void JncRel32(); // 0x83
    void JzRel32(); // 0x84
    void JaRel32(); // 0x87
    void JsRel32(); // 0x88
    void Code0FAE(); // 0x0F 0xAE
    void Ldmxcsr(); // 0x0F 0xAE 0x02
    void MovzxR32Rm8(); // 0xB6
    void MovzxR32Rm16(); // 0xB7
    void MovsxR32Rm8(); // 0xB6
    void Code0FBA_32(); // 0xBA
    void BtRm32Imm8(); // 0xBA 0x05
    void BtsRm32Imm8(); // 0xBA 0x05

    // 64-bit, 0x0f
    void CmovcR64Rm64(); // 0x42
    void CmovneR64Rm64(); // 0x45
    void MovzxR64Rm8(); // 0xB6
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
public:
    TigerLake();
    ~TigerLake();

    void Reset();
    void Clock();

    void Dump();
};