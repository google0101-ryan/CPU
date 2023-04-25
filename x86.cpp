#include "x86.h"

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "Bus.h"

static int coreId = 0;

uint64_t TigerLake::Read64(Segments seg, uint64_t addr)
{
    uint64_t a1 = l1->Read8(TranslateAddr(seg, addr++), false);
    uint64_t a2 = l1->Read8(TranslateAddr(seg, addr++), false);
    uint64_t a3 = l1->Read8(TranslateAddr(seg, addr++), false);
    uint64_t a4 = l1->Read8(TranslateAddr(seg, addr++), false);
    uint64_t a5 = l1->Read8(TranslateAddr(seg, addr++), false);
    uint64_t a6 = l1->Read8(TranslateAddr(seg, addr++), false);
    uint64_t a7 = l1->Read8(TranslateAddr(seg, addr++), false);
    uint64_t a8 = l1->Read8(TranslateAddr(seg, addr++), false);
    return a1 | (a2 << 8) | (a3 << 16) | (a4 << 24) | (a5 << 32) | (a6 << 40) | (a7 << 48) | (a8 << 56);
}

uint32_t TigerLake::Read32(Segments seg, uint64_t addr)
{
    if (addr >= 0xfee00000 && addr <= 0xfee003f0)
    {
        switch (addr-0xfee00000)
        {
        case 0xf0:
            return lapic->ReadSpuriousVec();
        case 0x320:
            return lapic->ReadTimerLVT();
        default:
            printf("Read32 from unknown LAPIC register 0x%08lx\n", addr-0xfee00000);
            exit(1);
        }
    }

    uint8_t lo1 = l1->Read8(TranslateAddr(seg, addr++), false);
    uint8_t lo2 = l1->Read8(TranslateAddr(seg, addr++), false);
    uint8_t hi1 = l1->Read8(TranslateAddr(seg, addr++), false);
    uint8_t hi2 = l1->Read8(TranslateAddr(seg, addr++), false);
    return lo1 | (lo2 << 8) | (hi1 << 16) | (hi2 << 24);
}

uint16_t TigerLake::Read16(Segments seg, uint64_t addr)
{
    uint8_t lo = l1->Read8(TranslateAddr(seg, addr++), false);
    uint8_t hi = l1->Read8(TranslateAddr(seg, addr++), false);
    return lo | (hi << 8);
}

uint8_t TigerLake::Read8(Segments seg, uint64_t addr)
{
    return l1->Read8(TranslateAddr(seg, addr), false);
}

void TigerLake::Write32(Segments seg, uint64_t addr, uint32_t val)
{
    if (addr >= 0xfee00000 && addr <= 0xfee003f0)
    {
        switch (addr-0xfee00000)
        {
        case 0xf0:
            lapic->WriteSpuriousVec(val);
            break;
        case 0x320:
            lapic->WriteTimerLVT(val);
            break;
        case 0x380:
            lapic->WriteInitialCount(val);
            break;
        default:
            printf("Write32 to unknown LAPIC register 0x%08lx\n", addr-0xfee00000);
            exit(1);
        }
        return;
    }

    l1->Write32(addr, val);
}

void TigerLake::Push8(uint8_t val)
{
    regs[RSP].reg64--;
    l1->Write8(TranslateAddr(SS, regs[RSP].reg64), val);
}

void TigerLake::Push32(uint32_t val)
{
    regs[RSP].reg64 -= 4;
    Write32(SS, regs[RSP].reg64, val);
}

void TigerLake::Push64(uint64_t val)
{
    regs[RSP].reg64 -= 8;
    l1->Write64(TranslateAddr(SS, regs[RSP].reg64), val);
}

uint64_t TigerLake::Pop64()
{
    uint64_t data = l1->Read64(TranslateAddr(SS, regs[RSP].reg64), false);
    regs[RSP].reg64 += 8;
    return data;
}

uint32_t TigerLake::Pop32()
{
    uint32_t data = l1->Read32(TranslateAddr(SS, regs[RSP].reg64), false);
    regs[RSP].reg64 += 4;
    return data;
}

uint64_t TigerLake::TranslateAddr(Segments seg, uint64_t addr)
{
    if (mode == Mode::RealMode)
    {
        uint64_t ret = segs[seg].base + addr;
        if (!a20 && longJumpDone)
            ret &= 0xFFFFF;
        return ret;
    }
    else if (mode == Mode::ProtectedMode && !(cr[0] & (1 << 31)))
    {
        return segs[seg].base + addr;
    }
    else if (mode == Mode::LongMode && cr[0] & (1 << 31))
    {
        uint64_t pml4Index = (addr >> 39) & 0x1FF;
        uint64_t pdpIndex = (addr >> 30) & 0x1FF;
        uint64_t pdIndex = (addr >> 21) & 0x1FF;
        uint64_t ptIndex = (addr >> 12) & 0x1FF;

        uint64_t pml4e = Bus::Read64(cr[3] + (pml4Index*8));

        if (!(pml4e & 1))
        {
            printf("#PF: Accessing non-present pml4!\n");
            exit(1);
        }

        uint64_t pdpe = Bus::Read64((pml4e & 0xFFFFFFFFFF000) + (pdpIndex * 8));

        if (!(pdpe & 1))
        {
            printf("0x%08lx\n", addr);
            printf("#PF: Accessing non-present pdpt!\n");
            exit(1);
        }
        
        bool ps = (pdpe >> 7) & 1;

        if (ps)
        {
            return (pdpe & 0x3FFFFC0000000) + (addr & 0x3FFFFFFF);
        }

        uint64_t pde = Bus::Read64((pdpe & 0xFFFFFFFFFF000) + (pdIndex * 8));

        if (!(pde & 1))
        {
            printf("#PF: Accessing non-present pd!\n");
            exit(1);
        }
        
        ps = (pde >> 7) & 1;

        if (ps)
        {
            return (pde & 0xFFFFFFFE00000) + (addr & 0x1FFFFF);
        }

        uint64_t pte = Bus::Read64((pde & 0xFFFFFFFFFF000) + (ptIndex * 8));

        printf("0x%08lx\n", pte);

        exit(1);
    }
    else
    {
        printf("Unhandled mode %d\n", (int)mode);
        exit(1);
    }
}

uint8_t TigerLake::ReadImm8(bool is_instr)
{
    return l1->Read8(TranslateAddr(is_instr ? CS : prefix, rip++), is_instr);
}

uint16_t TigerLake::ReadImm16(bool is_instr)
{
    uint8_t lo = l1->Read8(TranslateAddr(is_instr ? CS : prefix, rip++), is_instr);
    uint8_t hi = l1->Read8(TranslateAddr(is_instr ? CS : prefix, rip++), is_instr);
    return lo | (hi << 8);
}

uint32_t TigerLake::ReadImm32(bool is_instr)
{
    uint8_t lo1 = l1->Read8(TranslateAddr(is_instr ? CS : prefix, rip++), is_instr);
    uint8_t lo2 = l1->Read8(TranslateAddr(is_instr ? CS : prefix, rip++), is_instr);
    uint8_t hi1 = l1->Read8(TranslateAddr(is_instr ? CS : prefix, rip++), is_instr);
    uint8_t hi2 = l1->Read8(TranslateAddr(is_instr ? CS : prefix, rip++), is_instr);
    return lo1 | (lo2 << 8) | (hi1 << 16) | (hi2 << 24);
}

uint64_t TigerLake::ReadImm64(bool is_instr)
{
    uint64_t low = ReadImm32(true);
    uint64_t high = ReadImm32(true);
    return (high << 32) | low;
}

void TigerLake::CacheSegment(Segments seg, uint16_t sel, uint32_t base, uint32_t lim, uint16_t access)
{
    segs[seg].selector = sel;
    segs[seg].base = base;
    segs[seg].limit = lim;
    segs[seg].access_rights = access;
}

void TigerLake::CacheSegment(Segments seg, uint16_t sel)
{
    if (mode == Mode::RealMode)
    {
        segs[seg].selector = sel;
        segs[seg].base = sel << 4;
    }
    else if (mode == Mode::ProtectedMode)
    {
        uint16_t limitLow = l1->Read16(gdtr.base+sel, false);
        uint16_t baseLow = l1->Read16(gdtr.base+sel+2, false);
        uint8_t baseMid = l1->Read8(gdtr.base+sel+4, false);
        uint8_t limitHigh = l1->Read8(gdtr.base+sel+6, false) & 0xF;
        uint8_t basehigh = l1->Read8(gdtr.base+sel+7, false);

        uint32_t base = baseLow | (baseMid << 16) | (basehigh << 24);
        uint32_t limit = limitLow | (limitHigh << 16);
        limit *= (4096);
        limit += 4095;

        segs[seg].base = base;
        segs[seg].limit = limit;
        segs[seg].selector = sel;
    }
    else
    {
        segs[seg].base = 0;
        segs[seg].limit = 0xFFFFFF;
    }
}

void TigerLake::CheckModeChange()
{
    if ((cr[0] & 1) && mode == Mode::RealMode)
        mode = Mode::ProtectedMode;
}

std::function<void()> exit_func;

TigerLake::TigerLake()
{
    l1 = new Cache();

    id = coreId++;

    cr[0] = 0x60000010;

    MakeOpcodeTables();

    canDisassemble = true;

    lapic = new LocalAPIC();
}

TigerLake::~TigerLake()
{
    Dump();
    delete l1;
}

void TigerLake::Reset()
{
    std::memset(regs, 0, sizeof(regs));
    
    CacheSegment(CS, 0xF000, 0xFFFF0000, 0xFFFF, 0x93);
    CacheSegment(SS, 0x0000, 0x00000000, 0xFFFF, 0x93);
    CacheSegment(DS, 0x0000, 0x00000000, 0xFFFF, 0x93);
    CacheSegment(ES, 0x0000, 0x00000000, 0xFFFF, 0x93);
    CacheSegment(FS, 0x0000, 0x00000000, 0xFFFF, 0x93);
    CacheSegment(GS, 0x0000, 0x00000000, 0xFFFF, 0x93);

    rflags.value = 0x2;

    rip = 0xfff0;

    is32 = is64 = false;
    mode = Mode::RealMode;

    if (id != 0)
        halted = true;
    
    efer.val = 0;

    if (id == 0)
        ia32_apic_base = 0xFEE00900;
    else
        ia32_apic_base = 0xFEE00800;

    a20 = true;
    longJumpDone = false;

    lapic->Reset();
}

void TigerLake::Clock()
{
    if (halted)
        return;

    if (canDisassemble)
        printf("0x%08lx: ", TranslateAddr(CS, rip));

    uint8_t opcode = l1->Read8(TranslateAddr(CS, rip++), true);

    auto table = &lookup16;

    is32 = (mode == Mode::ProtectedMode);
    is64 = (mode == Mode::LongMode);
    a64 = is64;
    a32 = is32;
    rep = false;

    rex.b = rex.x = 
        rex.r = rex.rex = false;

    prefix = Segments::DS;

    switch (mode)
    {
    case Mode::RealMode: break;
    case Mode::ProtectedMode: table = &lookup32; break;
    case Mode::LongMode: table = &lookup32; break;
    }

    fwait = false;
    
    bool isPrefix = true;

    while (isPrefix)
    {
        printf("0x%02x ", opcode);
        switch (opcode)
        {
        case 0x0f:
            switch (mode)
            {
            case Mode::RealMode: table = &extLookup16; break;
            case Mode::ProtectedMode: table = &extLookup32; break;
            case Mode::LongMode: if (table == &lookup64) table = &extLookup64; else table = &extLookup32; break;
            }
            opcode = ReadImm8(true);
            isPrefix = false;
            break;
        case 0x2e:
            prefix = Segments::CS;
            opcode = ReadImm8(true);
            break;
        case 0x3e:
            prefix = Segments::DS;
            opcode = ReadImm8(true);
            break;
        case 0x40:
            if (mode == Mode::LongMode)
            {
                rex.rex = true;
                opcode = ReadImm8(true);
            }
            else
                isPrefix = false;
            break;
        case 0x41:
            if (mode == Mode::LongMode)
            {
                rex.b = true;
                opcode = ReadImm8(true);
            }
            else
                isPrefix = false;
            break;
        case 0x44:
            if (mode == Mode::LongMode)
            {
                rex.r = true;
                opcode = ReadImm8(true);
            }
            else
                isPrefix = false;
            break;
        case 0x45:
            if (mode == Mode::LongMode)
            {
                rex.b = rex.r = true;
                opcode = ReadImm8(true);
            }
            else
                isPrefix = false;
            break;
        case 0x4C:
            if (mode == Mode::LongMode)
            {
                rex.r = true;
                is64 = true;
                if (table == &extLookup32)
                    table = &extLookup64;
                else
                    table = &lookup64;
                opcode = ReadImm8(true);
            }
            else
                isPrefix = false;
            break;
        // REX.WRB
        case 0x4D:
            if (mode == Mode::LongMode)
            {
                rex.r = true;
                rex.b = true;
                is64 = true;
                if (table == &extLookup32)
                    table = &extLookup64;
                else
                    table = &lookup64;
                opcode = ReadImm8(true);
            }
            else
                isPrefix = false;
            break;
        case 0x48:
            if (mode == Mode::LongMode)
            {
                is64 = true;
                if (table == &extLookup32)
                    table = &extLookup64;
                else
                    table = &lookup64;
                opcode = ReadImm8(true);
            }
            else
                isPrefix = false;
            break;
        case 0x49:
            if (mode == Mode::LongMode)
            {
                rex.b = true;
                is64 = true;
                if (table == &extLookup32)
                    table = &extLookup64;
                else
                    table = &lookup64;
                opcode = ReadImm8(true);
            }
            else
                isPrefix = false;
            break;
        case 0x66:
            if (table == &lookup16)
                table = &lookup32;
            else if (table == &extLookup16)
                table = &extLookup32;
            else if (table == &lookup32)
                table = &lookup16;
            else
                table = &extLookup32;
            opcode = ReadImm8(true);
            break;
        case 0x67:
            if (a64)
            {
                a64 = false;
                a32 = true;
            }
            else if (a32)
            {
                a32 = false;
            }
            opcode = ReadImm8(true);
            break;
        case 0xf3:
            rep = true;
            printf("rep ");
            opcode = ReadImm8(true);
            break;
        case 0x9B:
            fwait = true;
            opcode = ReadImm8(true);
            break;
        default:
            isPrefix = false;
            break;
        }
    }

    printf("\t");

    if (!(*table)[opcode])
    {
        if (table == &lookup16)
            printf("16-bit\n");
        if (table == &lookup32)
            printf("32-bit\n");
        if (table == &lookup64)
            printf("64-bit\n");
        if (table == &extLookup16)
            printf("extended 16-bit\n");
        if (table == &extLookup32)
            printf("extended 32-bit\n");
        if (table == &extLookup64)
            printf("extended 64-bit\n");
        printf("Unknown opcode 0x%02x\n", opcode);
        exit(1);
    }

    (*table)[opcode]();

    lapic->TickTimer();
}

void TigerLake::Dump()
{
    for (int i = 0; i < NUM_REGS; i++)
        printf("%s\t->\t0x%08lx\n", Reg64[i], regs[i].reg64);
    for (int i = 0; i < 8; i++)
        printf("cr%d\t->\t0x%08x\n", i, cr[i]);
    printf("rip (phys)\t->\t0x%08lx\n", rip);
    printf("rip\t->\t0x%08lx\n", TranslateAddr(CS, rip));
    printf("[%s%s%s%s%s]\n",
        rflags.flag_bits.zf ? "z" : ".",
        rflags.flag_bits.sf ? "s" : ".",
        rflags.flag_bits.cf ? "c" : ".",
        rflags.flag_bits.pf ? "p" : ".",
        rflags.flag_bits.of ? "o" : ".");
}
