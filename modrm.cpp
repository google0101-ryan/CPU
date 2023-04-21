#include "x86.h"

#include <cstdio>
#include <cstdlib>

void IvyBridge::FetchModrm()
{
    if (a64)
        FetchModrm64();
    else if (a32)
        FetchModrm32();
    else
        FetchModrm16();
}

void IvyBridge::FetchModrm16()
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

void IvyBridge::FetchModrm64()
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

void IvyBridge::FetchSib32()
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

void IvyBridge::FetchModrm32()
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
    case 3:
        break;
    default:
        printf("Unhandled fetch for modrm.mod = %d\n", modrm.mod);
        exit(1);
    }
}


uint64_t IvyBridge::DecodeModrmAddr(std::string& disasm)
{
    if (!a32 && !a64)
        return DecodeModrmAddr16(disasm);
    else if (!a64)
        return DecodeModrmAddr32(disasm);
    else
        return DecodeModrmAddr64(disasm);
}

uint64_t IvyBridge::DecodeModrmAddr16(std::string &disasm)
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
            disasm += "0x" + convert_int(disp16);
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
}

uint64_t IvyBridge::DecodeModrmAddr32(std::string &disasm)
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
            return regs[RAX].reg32;
        case 3:
            disasm += "ebx]";
            return regs[RBX].reg32;
        case 4:
        {
            uint64_t addr = DecodeSIBAddr(disasm);
            disasm += "]";
            return addr;
        }
        case 5:
            disasm += convert_int(disp32) + "]";
            return disp32;
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
            return regs[RAX].reg32 + disp8;
        case 3:
            disasm += "ebx + " + convert_int(disp8) + "]";
            return regs[RBX].reg32 + disp8;
        case 5:
            disasm += "ebp + " + convert_int(disp8) + "]";
            return regs[RBP].reg32 + disp8;
        default:
            printf("Unhandled modr/m 32 with mod=1, rm=%d\n", modrm.rm);
            exit(1);
        }
        break;
    default:
        printf("Unhandled modr/m 32 with mod=%d\n", modrm.mod);
        exit(1);
    }
}

uint64_t IvyBridge::DecodeModrmAddr64(std::string &disasm)
{
    switch (modrm.mod)
    {
    case 0:
        switch (modrm.rm)
        {
        case 12:
        case 4:
        {
            disasm = "[";
            uint64_t addr = DecodeSIBAddr(disasm);
            disasm += "]";
            return addr;
        }
        case 5:
        case 13:
            disasm = "[rel 0x" + convert_int(rip+disp32) + "]";
            return rip + disp32;
        default:
            disasm += "[" + std::string(Reg64[modrm.rm]) + "]";
            return regs[modrm.rm].reg64;
        }
    case 1:
        switch (modrm.rm)
        {
        case 5:
            disasm = "[rbp+" + convert_int(disp8) + "]";
            return regs[RBP].reg64 + disp8;
        default:
            printf("Unhandled modr/m 64 with mod=1, rm=%d\n", modrm.rm);
            exit(1);
        }
    case 2:
        switch (modrm.rm)
        {
        case 5:
            disasm = "[rbp+" + convert_int(disp32) + "]";
            return regs[RBP].reg64 + disp32;
        default:
            printf("Unhandled modr/m 64 with mod=2, rm=%d\n", modrm.rm);
            exit(1);
        }
    default:
        printf("Unhandled modr/m 64 with mod=%d\n", modrm.mod);
        exit(1);
    }
}

uint64_t IvyBridge::DecodeSIBAddr(std::string &disasm)
{
    int scale = 1;
    switch (sib.scale)
    {
    case 0: break;
    case 1: scale = 2; break;
    case 2: scale = 4; break;
    case 3: scale = 8; break;
    }

    uint32_t res;
    switch (sib.index)
    {
    case 4:
    case 12: 
        res = 0; break;
    default:
        res = (a64 ? regs[sib.index].reg64 : regs[sib.index].reg32) * scale;
        break;
    }
    
    if (sib.index != 4)
        disasm += (a64 ? Reg64[sib.index] : Reg32[sib.index]) + std::string("*") + std::to_string(scale) + "+";
    
    switch (sib.base)
    {
    case 5:
    case 13:
    {
        switch (modrm.mod)
        {
        case 0: disasm += convert_int(sib_disp32); return sib_disp32 + res;
        case 1: disasm += (a64 ? "r" : "e") + std::string("bp+") + convert_int(disp8); return disp8 + (a64 ? regs[RBP].reg64 : regs[RBP].reg32) + res;
        case 2: disasm += (a64 ? "r" : "e") + std::string("bp+") + convert_int(disp32); return disp32 + (a64 ? regs[RBP].reg64 : regs[RBP].reg32) + res;
        }
        break;
    }
    default:
        disasm += a64 ? Reg64[sib.base] : Reg32[sib.base];
        return (a64 ? regs[sib.base].reg64 : regs[sib.base].reg32) + res;
    }
}

uint64_t IvyBridge::ReadModrm64(std::string &disasm)
{
    if (modrm.mod == 3)
    {
        disasm = Reg32[modrm.rm];
        return regs[modrm.rm].reg64;
    }
    else
    {
        uint32_t data = Read64(prefix, DecodeModrmAddr(disasm));
        disasm = "qword" + disasm;
        return data;
    }
}

uint32_t IvyBridge::ReadModrm32(std::string &disasm)
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

uint16_t IvyBridge::ReadModrm16(std::string &disasm)
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

uint8_t IvyBridge::ReadModrm8(std::string &disasm)
{
    if (modrm.mod == 3)
    {
        disasm = Reg8[modrm.rm];
        return ReadReg8((Registers)modrm.rm);
    }
    else
    {
        uint16_t data = Read8(prefix, DecodeModrmAddr(disasm));
        disasm = "byte" + disasm;
        return data;
    }
}

void IvyBridge::WriteModrm8(std::string &disasm, uint8_t val)
{
    if (modrm.mod == 3)
    {
        disasm = Reg8[modrm.rm];
        WriteReg8((Registers)modrm.rm, val);
    }
    else
    {
        l1->Write8(DecodeModrmAddr(disasm), val);
        disasm = "byte" + disasm;
    }
}

void IvyBridge::WriteModrm16(std::string &disasm, uint16_t val)
{
    if (modrm.mod == 3)
    {
        regs[modrm.rm].reg16 = val;
        disasm = Reg16[modrm.rm];
    }
    else
    {
        l1->Write16(DecodeModrmAddr(disasm), val);
        disasm = "word" + disasm;
    }
}

void IvyBridge::WriteModrm32(std::string &disasm, uint32_t val)
{
    if (modrm.mod == 3)
    {
        regs[modrm.rm].reg32 = val;
        disasm = Reg32[modrm.rm];
    }
    else
    {
        l1->Write32(DecodeModrmAddr(disasm), val);
        disasm = "dword" + disasm;
    }
}

void IvyBridge::WriteModrm64(std::string &disasm, uint64_t val)
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

void IvyBridge::WriteReg8(Registers reg, uint8_t val)
{
    if (rex.r || rex.b || rex.x || rex.rex)
    {
        switch (reg)
        {
        case 11:
        case 3:
        {
            uint8_t& r = (rex.r ? regs[RBX].lo : regs[R11].lo);
            r = val;
            break;
        }
        default:
            printf("Write to unknown 8-bit register with REX %d\n", reg);
            exit(1);
        }
    }
    else
    {
        if (reg < 4)
            regs[reg].lo = val;
        else
            regs[reg].hi = val;
    }
}

uint8_t IvyBridge::ReadReg8(Registers reg)
{
    if (rex.r || rex.b || rex.x || rex.rex)
    {
        switch (reg)
        {
        case 6:
            return regs[RSI].lo; // New register, sil
        case 11:
        case 3:
            return (rex.r ? regs[RBX].lo : regs[R11].lo);
        default:
            printf("Read from unknown 8-bit register with REX %d\n", reg);
            exit(1);
        }
    }
    else
    {
        if (reg < 4)
            return regs[reg].lo;
        else
            return regs[reg].hi;
    }
}
