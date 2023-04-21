#pragma once

#include <cstdint>
#include <types.h>
#include <Cache.h>
#include <unordered_map>
#include <functional>
#include <string>

class IvyBridge
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

    Segments prefix = Segments::DS;

    uint64_t Read64(Segments seg, uint64_t addr);
    uint32_t Read32(Segments seg, uint64_t addr);
    uint16_t Read16(Segments seg, uint64_t addr);
    uint8_t Read8(Segments seg, uint64_t addr);

    void Push32(uint32_t val);

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
    void WriteModrm32(std::string& disasm, uint32_t val);
    void WriteModrm64(std::string& disasm, uint64_t val);

    void WriteReg8(Registers regs, uint8_t val);
    uint8_t ReadReg8(Registers regs);

    Cache* l1; // We don't emulate multi-level yet

    uint64_t rip;
    RFLAGS rflags;

    int id = 0;
    bool halted = false;

    union
    {
        uint8_t value;
        struct
        {
            uint8_t rm : 3;
            uint8_t reg : 3;
            uint8_t mod : 2;
        };
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
    void UpdateFlagsAdd32(uint32_t value1, uint32_t value2, uint64_t result);

    // All modes
    void AndAlImm8(); // 0x24
    void JcRel8(); // 0x72
    void JncRel8(); // 0x73
    void JzRel8(); // 0x74
    void JnzRel8(); // 0x75
    void JlRel8(); // 0x7C
    void Code80(); // 0x80
    void CmpRm8Imm8(); // 0x80 0x07
    void MovR8Rm8(); // 0x8A
    void MovSregRm16(); // 0x8E
    void TestAlImm8(); // 0xA8
    void MovRm8Imm8(); // 0xC6
    void JmpRel8(); // 0xEB
    void TestRm8Imm8(); // 0xF6
    void Cld(); // 0xFC
    void Cli(); // 0xFA

    // 16-bit mode
    void Code81_16(); // 0x81
    void CmpRm16Imm16(); // 0x81 0x07
    void MovR16Rm16(); // 0x8B
    void MovR16Imm16(); // 0xB8+r
    void JmpRel16(); // 0xE9

    // 32-bit mode
    void AddRm32R32(); // 0x01
    void AddR32Rm32(); // 0x03
    void AddEaxImm32(); // 0x05
    void OrRm32R32(); // 0x09
    void SubEaxImm32(); // 0x2D
    void XorRm32R32(); // 0x31
    void CmpRm32R32(); // 0x39
    void CmpEaxImm32(); // 0x3D
    void IncR32(); // 0x40+r
    void DecR32(); // 0x48+r
    void PushR32(); // 0x50+r
    void PopR32(); // 0x58+r
    void Code81_32(); // 0x81
    void AndRm32Imm32(); // 0x81 0x04
    void CmpRm32Imm32(); // 0x81 0x07
    void Code83_32(); // 0x83
    void AddRm32Imm8(); // 0x83 0x00
    void CmpRm32Imm8(); // 0x83 0x07
    void TestRm32R32(); // 0x85
    void MovRm32R32(); // 0x89
    void MovR32Rm32(); // 0x8B
    void BtRm32R32(); // 0xA3
    void MovR32Imm32(); // 0xB8+r
    void CodeC1_32(); // 0xC1
    void ShlRm32Imm8(); // 0xC1 0x04
    void MovRm32Imm32(); // 0xC7
    void LoopEcxRel8(); // 0xE2
    void JmpRel32(); // 0xE9
    void JmpPtr1632(); // 0xEA
    void CodeFF_32(); // 0xFF
    void JmpRm32(); // 0xFF 0x04

    // 64-bit mode
    void AndRm64R64(); // 0x21
    void MovRm64R64(); // 0x89
    void MovR64Imm64(); // 0xB8+r
    void CodeFF_64(); // 0xFF

    // All modes, 0x0f
    void Code0f01(); // 0x01
        void Lgdt(); // 0x01 0x02
        void Lidt(); // 0x01 0x03
    void MovR64CRn(); // 0x20
    void MovCRnR64(); // 0x22
    void Wrmsr(); // 0x30
    void Rdmsr(); // 0x32
    void Cpuid(); // 0xA2

    // 32-bit, 0x0f
    void JzRel32(); // 0x84
    void MovzxR32Rm16(); // 0xB7
    void Code0FBA_32(); // 0xBA
    void BtsRm32Imm8(); // 0xBA 0x05

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
    IvyBridge();
    ~IvyBridge();

    void Reset();
    void Clock();

    void Dump();
};