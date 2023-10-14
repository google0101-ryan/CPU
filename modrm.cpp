#include "x86.h"

#include <cstdio>
#include <cstdlib>

void TigerLake::FetchModrm()
{
    if (a64)
        FetchModrm64();
    else if (a32)
        FetchModrm32();
    else
        FetchModrm16();
}

void TigerLake::FetchModrm16()
{
    uint8_t val = ReadImm8(true);
    modrm.mod = (val >> 6) & 0x3;
    modrm.reg = (val >> 3) & 0x7;
    modrm.rm = val & 0x7;

    switch (modrm.mod)
    {
    case 0:
        if (modrm.rm == 6)
            disp16 = ReadImm16(true);
        break;
    case 3:
        break;
    default:
        printf("Unhandled fetch for modrm.mod = %d\n", modrm.mod);
        exit(1);
    }
}

void TigerLake::FetchModrm64()
{
    uint8_t val = ReadImm8(true);
    modrm.mod = (val >> 6) & 0x3;
    modrm.reg = (val >> 3) & 0x7;
    modrm.rm = val & 0x7;

    if (rex.r) modrm.reg += 8;
    if (rex.b) modrm.rm += 8;

    switch (modrm.mod)
    {
    case 0:
        if (modrm.rm == 4 || modrm.rm == 12)
        {
            FetchSib32();
        }
        else if (modrm.rm == 5)
            disp32 = ReadImm32(true);
        break;
    case 1:
        if (modrm.rm == 4 || modrm.rm == 12)
        {
            FetchSib32();
        }
        disp8 = ReadImm8(true);
        break;
    case 2:
        if (modrm.rm == 4  || modrm.rm == 12)
        {
            FetchSib32();
        }
        disp32 = ReadImm32(true);
        break;
    case 3:
        break;
    default:
        printf("Unhandled 64-bit fetch for modrm.mod = %d\n", modrm.mod);
        exit(1);
    }
}

void TigerLake::FetchSib32()
{
    sib_.value = ReadImm8(true);

    sib.base = sib_.base;
    sib.index = sib_.index;
    sib.scale = sib_.scale;

    if (rex.b) sib.base += 8;
    if (rex.x) sib.index += 8;

    if (sib.base == 5)
    {
        sib_disp32 = ReadImm32(true);
    }
}

void TigerLake::FetchModrm32()
{
    uint8_t val = ReadImm8(true);
    modrm.mod = (val >> 6) & 0x3;
    modrm.reg = (val >> 3) & 0x7;
    modrm.rm = val & 0x7;

    switch (modrm.mod)
    {
    case 0:
        if (modrm.rm == 4)
        {
            FetchSib32();
        }
        else if (modrm.rm == 5)
            disp32 = ReadImm32(true);
        break;
    case 1:
        if (modrm.rm == 4)
        {
            FetchSib32();
        }
        disp8 = ReadImm8(true);
        break;
    case 2:
        if (modrm.rm == 4)
        {
            FetchSib32();
        }
        disp32 = ReadImm32(true);
        break;
    case 3:
        break;
    default:
        printf("Unhandled fetch for modrm.mod = %d\n", modrm.mod);
        exit(1);
    }
}


uint64_t TigerLake::DecodeModrmAddr(std::string& disasm)
{
    if (!a32 && !a64)
        return DecodeModrmAddr16(disasm);
    else if (!a64)
        return DecodeModrmAddr32(disasm);
    else
        return DecodeModrmAddr64(disasm);
}

uint64_t TigerLake::DecodeModrmAddr16(std::string &disasm)
{
    switch (modrm.mod)
    {
    case 0:
        disasm = "[";
        if (prefix != Segments::DS)
            disasm += Segs[prefix] + std::string(":");
        switch (modrm.rm)
        {
        case 0:
            disasm += "bx+si]";
            return regs[RBX].reg16 + regs[RSI].reg16;
        case 1:
            disasm += "bx+di]";
            return regs[RBX].reg16 + regs[RDI].reg16;
        case 2:
            disasm += "bp+si]";
            return regs[RBP].reg16 + regs[RSI].reg16;
        case 3:
            disasm += "bp+di]";
            return regs[RBP].reg16 + regs[RDI].reg16;
        case 4:
            disasm += "si]";
            return regs[RSI].reg16;
        case 5:
            disasm += "di]";
            return regs[RSI].reg16;
        case 6:
            disasm += convert_int(disp16) + "]";
            return (uint64_t)disp16;
        case 7:
            disasm += "bx]";
            return regs[RBX].reg16;
        }
        break;
    default:
        printf("Unknown 16-bit mod decoding %d\n", modrm.mod);
        exit(1);
    }

    return 0;
}

uint64_t TigerLake::DecodeModrmAddr32(std::string &disasm)
{
    switch (modrm.mod)
    {
    case 0:
        disasm = "[";
        if (prefix != Segments::DS)
            disasm += Segs[prefix] + std::string(":");
        switch (modrm.rm)
        {
        case 0:
            disasm += "eax]";
            return a64 ? regs[RAX].reg64 : regs[RAX].reg32;
        case 1:
            disasm += "ecx]";
            return a64 ? regs[RCX].reg64 : regs[RCX].reg32;
        case 2:
            disasm += "edx]";
            return a64 ? regs[RDX].reg64 : regs[RDX].reg32;
        case 3:
            disasm += "ebx]";
            return a64 ? regs[RBX].reg64 : regs[RBX].reg32;
        case 4:
        {
            uint64_t addr = DecodeSIBAddr(disasm);
            disasm += "]";
            return addr;
        }
        case 5:
            disasm += convert_int((uint32_t)disp32) + "]";
            return (uint32_t)disp32;
        case 6:
            disasm += "esi]";
            return a64 ? regs[RSI].reg64 : regs[RSI].reg32;
        case 7:
            disasm += "edi]";
            return a64 ? regs[RDI].reg64 : regs[RDI].reg32;
        default:
            printf("Unhandled modr/m 32 with mod=0, rm=%d\n", modrm.rm);
            exit(1);
        }
        break;
    case 1:
        disasm = "[";
        if (prefix != Segments::DS)
            disasm += Segs[prefix] + std::string(":");
        switch (modrm.rm)
        {
        case 0:
            disasm += "eax + " + convert_int(disp8) + "]";
            return (a64 ? regs[RAX].reg64 : regs[RAX].reg32) + (int64_t)disp8;
        case 1:
            disasm += "ecx + " + convert_int(disp8) + "]";
            return (a64 ? regs[RCX].reg64 : regs[RCX].reg32) + (int64_t)disp8;
        case 2:
            disasm += "edx + " + convert_int(disp8) + "]";
            return (a64 ? regs[RDX].reg64 : regs[RDX].reg32) + (int64_t)disp8;
        case 3:
            disasm += "ebx + " + convert_int(disp8) + "]";
            return (a64 ? regs[RBX].reg64 : regs[RBX].reg32) + (int64_t)disp8;
        case 4:
        {
            uint64_t addr = DecodeSIBAddr(disasm) + (int64_t)disp8;
            disasm += "+" + convert_int(disp8) + "]";
            return addr;
        }
        case 5:
            disasm += "ebp + " + convert_int(disp8) + "]";
            return (a64 ? regs[RBP].reg64 : regs[RBP].reg32) + (int64_t)disp8;
        case 6:
            disasm += "esi + " + convert_int(disp8) + "]";
            return (a64 ? regs[RSI].reg64 : regs[RSI].reg32) + (int64_t)disp8;
        case 7:
            disasm += "edi + " + convert_int(disp8) + "]";
            return (a64 ? regs[RDI].reg64 : regs[RDI].reg32) + (int64_t)disp8;
        default:
            printf("Unhandled modr/m 32 with mod=1, rm=%d\n", modrm.rm);
            exit(1);
        }
    case 2:
        disasm = "[";
        if (prefix != Segments::DS)
            disasm += Segs[prefix] + std::string(":");
        switch (modrm.rm)
        {
        case 1:
            disasm += "ecx + " + convert_int(disp32) + "]";
            return (a64 ? regs[RCX].reg64 : regs[RCX].reg32) + (int64_t)disp32;
        case 2:
            disasm += "edx + " + convert_int(disp32) + "]";
            return (a64 ? regs[RDX].reg64 : regs[RDX].reg32) + (int64_t)disp32;
        case 3:
            disasm += "ebx + " + convert_int(disp32) + "]";
            return (a64 ? regs[RBX].reg64 : regs[RBX].reg32) + (int64_t)disp32;
        case 7:
            disasm += "edi + " + convert_int(disp32) + "]";
            return (a64 ? regs[RDI].reg64 : regs[RDI].reg32) + (int64_t)disp32;
        default:
            printf("Unhandled modr/m 32 with mod=2, rm=%d\n", modrm.rm);
            exit(1);
        }
        break;
    default:
        printf("Unhandled modr/m 32 with mod=%d\n", modrm.mod);
        exit(1);
    }
}

uint64_t TigerLake::DecodeModrmAddr64(std::string &disasm)
{
    switch (modrm.mod)
    {
    case 0:
        switch (modrm.rm)
        {
        case 4:
        case 12:
        {
            disasm = "[";
            uint64_t addr = DecodeSIBAddr(disasm);
            disasm += "]";
            return addr;
        }
        case 5:
        case 13:
            disasm = "[rel 0x" + convert_int(rip+disp32) + "]";
            return rip + (int64_t)disp32;
        default:
            disasm += "[" + std::string(Reg64[modrm.rm]) + "]";
            return regs[modrm.rm].reg64;
        }
    case 1:
        switch (modrm.rm)
        {
        case 0:
            disasm = "[rax+" + convert_int(disp8) + "]";
            return regs[RAX].reg64 + (int64_t)disp8;
        case 1:
            disasm = "[rcx+" + convert_int(disp8) + "] (" + convert_int(regs[RCX].reg64 + (int64_t)disp8) + ")";
            return regs[RCX].reg64 + (int64_t)disp8;
        case 2:
            disasm = "[rdx+" + convert_int(disp8) + "]";
            return regs[RDX].reg64 + (int64_t)disp8;
        case 3:
            disasm = "[rbx+" + convert_int(disp8) + "]";
            return regs[RBX].reg64 + (int64_t)disp8;
        case 4:
        case 12:
        {
            disasm = "[";
            uint64_t addr = DecodeSIBAddr(disasm);
            disasm += "+" + convert_int(disp8) + "]";
            return addr + (int64_t)disp8;
        }
        case 5:
            disasm = "[rbp+" + convert_int(disp8) + "]";
            return regs[RBP].reg64 + (int64_t)disp8;
        case 6:
            disasm = "[rsi+" + convert_int(disp8) + "]";
            return regs[RSI].reg64 + (int64_t)disp8;
        case 7:
            disasm = "[rdi+" + convert_int(disp8) + "]";
            return regs[RDI].reg64 + (int64_t)disp8;
        case 8:
            disasm = "[r8+" + convert_int(disp8) + "]";
            return regs[R8].reg64 + (int64_t)disp8;
        case 9:
            disasm = "[r9+" + convert_int(disp8) + "]";
            return regs[R9].reg64 + (int64_t)disp8;
        case 10:
            disasm = "[r10+" + convert_int(disp8) + "]";
            return regs[R10].reg64 + (int64_t)disp8;
        case 13:
            disasm = "[r13+" + convert_int(disp8) + "]";
            return regs[R13].reg64 + disp8;
        case 14:
            disasm = "[r14+" + convert_int(disp8) + "]";
            return regs[R14].reg64 + (int64_t)disp8;
        case 15:
            disasm = "[r15+" + convert_int(disp8) + "]";
            return regs[R15].reg64 + (int64_t)disp8;
        default:
            printf("Unhandled modr/m 64 with mod=1, rm=%d\n", modrm.rm);
            exit(1);
        }
    case 2:
        switch (modrm.rm)
        {
        case 0:
            disasm = "[rax+" + convert_int(disp32) + "]";
            return regs[RAX].reg64 + (int64_t)disp32;
        case 1:
            disasm = "[rcx+" + convert_int(disp32) + "]";
            return regs[RCX].reg64 + (int64_t)disp32;
        case 2:
            disasm = "[rdx+" + convert_int(disp32) + "]";
            return regs[RDX].reg64 + (int64_t)disp32;
        case 3:
            disasm = "[rbx+" + convert_int(disp32) + "]";
            return regs[RBX].reg64 + (int64_t)disp32;
        case 4:
        case 12:
        {
            disasm = "[";
            uint64_t addr = DecodeSIBAddr(disasm) + disp32;
            disasm += "+" + convert_int(disp32) + "]";
            return addr;
        }
        case 5:
            disasm = "[rbp+" + convert_int(disp32) + "]";
            return regs[RBP].reg64 + (int64_t)disp32;
        case 15:
            disasm = "[r15+" + convert_int(disp32) + "]";
            return regs[R15].reg64 + (int64_t)disp32;
        default:
            printf("Unhandled modr/m 64 with mod=2, rm=%d\n", modrm.rm);
            exit(1);
        }
    default:
        printf("Unhandled modr/m 64 with mod=%d\n", modrm.mod);
        exit(1);
    }
}

uint64_t TigerLake::DecodeSIBAddr(std::string &disasm)
{
    int scale = 1;
    switch (sib.scale)
    {
    case 0: break;
    case 1: scale = 2; break;
    case 2: scale = 4; break;
    case 3: scale = 8; break;
    }

    uint64_t res;
    switch (sib.index)
    {
    case 4:
        res = 0; break;
    default:
        res = (a64 ? regs[sib.index].reg64 : regs[sib.index].reg32) * scale;
        break;
    }
    
    if (sib.index != 4 && sib.index != 12)
        disasm += (a64 ? Reg64[sib.index] : Reg32[sib.index]) + std::string("*") + std::to_string(scale) + "+";
    
    switch (sib.base)
    {
    case 5:
    case 13:
    {
        switch (modrm.mod)
        {
        case 0: disasm += convert_int(sib_disp32); return (int64_t)sib_disp32 + res;
        case 1:
        {
            if (!rex.b)
            {
                disasm += (a64 ? "r" : "e") + std::string("bp+") + convert_int(disp8); 
                return (a64 ? regs[RBP].reg64 : regs[RBP].reg32) + res;
            }
            else
            {
                disasm += "r13" + convert_int(disp8); 
                return regs[R13].reg64 + res;
            }
        }
        case 2:
        {
            if (!rex.b)
            {
                disasm += (a64 ? "r" : "e") + std::string("bp+") + convert_int(disp32); 
                return (a64 ? regs[RBP].reg64 : regs[RBP].reg32) + res;
            }
            else
            {
                disasm += "r13" + convert_int(disp32); 
                return regs[R13].reg64 + res;
            }
        }
        }
        break;
    }
    default:
        disasm += a64 ? Reg64[sib.base] : Reg32[sib.base];
        return (a64 ? regs[sib.base].reg64 : regs[sib.base].reg32) + res;
    }

    return 0;
}

uint64_t TigerLake::ReadModrmMMX(std::string &disasm)
{
    if (modrm.mod == 3)
    {
        disasm = MMX[modrm.rm];
        return mmx[modrm.rm].reg64;
    }
    else
    {
        uint64_t data = Read64(prefix, DecodeModrmAddr(disasm));
        disasm = "qword" + disasm;
        return data;
    }
}

uint64_t TigerLake::ReadModrm64(std::string &disasm)
{
    if (modrm.mod == 3)
    {
        disasm = Reg64[modrm.rm];
        return regs[modrm.rm].reg64;
    }
    else
    {
        uint64_t data = Read64(prefix, DecodeModrmAddr(disasm));
        disasm = "qword" + disasm;
        return data;
    }
}

uint32_t TigerLake::ReadModrm32(std::string &disasm)
{
    if (modrm.mod == 3)
    {
        disasm = Reg32[modrm.rm];
        return regs[modrm.rm].reg32;
    }
    else
    {
        uint32_t data = Read32(prefix, DecodeModrmAddr(disasm));
        disasm = "dword" + disasm;
        return data;
    }
}

uint16_t TigerLake::ReadModrm16(std::string &disasm)
{
    if (modrm.mod == 3)
    {
        disasm = Reg16[modrm.rm];
        return regs[modrm.rm].reg16;
    }
    else
    {
        uint16_t data = Read16(prefix, DecodeModrmAddr(disasm));
        disasm = "word" + disasm;
        return data;
    }
}

uint8_t TigerLake::ReadModrm8(std::string &disasm)
{
    if (modrm.mod == 3)
    {
        disasm = GetReg8((Registers)modrm.rm);
        return ReadReg8((Registers)modrm.rm);
    }
    else
    {
        uint8_t data = Read8(prefix, DecodeModrmAddr(disasm));
        disasm = "byte" + disasm;
        return data;
    }
}

void TigerLake::WriteModrm8(std::string &disasm, uint8_t val)
{
    if (modrm.mod == 3)
    {
        disasm = GetReg8((Registers)modrm.rm);
        WriteReg8((Registers)modrm.rm, val);
    }
    else
    {
        Write8(prefix, DecodeModrmAddr(disasm), val);
        disasm = "byte" + disasm;
    }
}

void TigerLake::WriteModrm16(std::string &disasm, uint16_t val)
{
    if (modrm.mod == 3)
    {
        regs[modrm.rm].reg16 = val;
        disasm = Reg16[modrm.rm];
    }
    else
    {
        Write16(prefix, DecodeModrmAddr(disasm), val);
        disasm = "word" + disasm;
    }
}

void TigerLake::WriteModrm32(std::string &disasm, uint32_t val)
{
    if (modrm.mod == 3)
    {
        regs[modrm.rm].reg64 = val;
        disasm = Reg32[modrm.rm];
    }
    else
    {
        Write32(prefix, DecodeModrmAddr(disasm), val);
        disasm = "dword" + disasm;
    }
}

void TigerLake::WriteModrm64(std::string &disasm, uint64_t val)
{
    if (modrm.mod == 3)
    {
        regs[modrm.rm].reg64 = val;
        disasm = Reg64[modrm.rm];
    }
    else
    {
        l1->Write64(DecodeModrmAddr(disasm), val);
        disasm = "qword" + disasm;
    }
}

void TigerLake::WriteModrmMMX(std::string &disasm, uint64_t val)
{
    if (modrm.mod == 3)
    {
        mmx[modrm.rm].reg64 = val;
        disasm = Reg64[modrm.rm];
    }
    else
    {
        l1->Write64(DecodeModrmAddr(disasm), val);
        disasm = "qword" + disasm;
    }
}

void TigerLake::WriteReg8(Registers reg, uint8_t val)
{
    if (rex.r || rex.b || rex.x || rex.rex)
    {
        regs[reg].lo = val;
    }
    else
    {
        if (reg < 4)
            regs[reg].lo = val;
        else
            regs[reg-4].hi = val;
    }
}

uint8_t TigerLake::ReadReg8(Registers reg)
{
    if (rex.r || rex.b || rex.x || rex.rex)
    {
        return regs[reg].lo;
    }
    else
    {
        if (reg < 4)
            return regs[reg].lo;
        else
            return regs[reg-4].hi;
    }
}

const char *TigerLake::GetReg8(Registers reg)
{
    if (rex.r || rex.b || rex.x || rex.rex)
    {
        return Reg8REX[reg];
    }
    else
    {
        return Reg8[reg];
    }
}