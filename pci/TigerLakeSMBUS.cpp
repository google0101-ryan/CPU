#include "TigerLakeSMBUS.h"

TigerLakeSMBUS::TigerLakeSMBUS(PCIBus *pci)
{
    pci->RegisterDevice(0x00, 0x1F, 0x04, this);

    tcoctl = 0;
    tcobase = 1;
    smbbase = 1;
}

void TigerLakeSMBUS::WriteConfigSpace8(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    case 0x04:
        pcicmd = data & 0x543;
        break;
    case 0x40:
        break;
    case 0x55:
        tcoctl &= ~(0xff00);
        tcoctl |= (data << 8);
        break;
    default:
        printf("[TigerLake/TigerLakeSMBUS]: Write8 to unknown register 0x%02X\n", offs);
        exit(1);
    }
}

void TigerLakeSMBUS::WriteConfigSpace16(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    case 0x20:
        smbbase = data & 0xFFE0;
        printf("[emu/SMBUS]: Mapped SMBUS registers to 0x%04x (I/O 0x%08x)\n", smbbase, data);
        break;
    case 0x50:
        tcobase = data & 0xFFE0;
        printf("[emu/SMBUS]: Mapped SMBUS TCO registers to 0x%04x (I/O)\n", tcobase);
        break;
    case 0x54:
        tcoctl = data;
        break;
    default:
        printf("[TigerLake/TigerLakeSMBUS]: Write16 to unknown register 0x%02X\n", offs);
        exit(1);
    }
}

void TigerLakeSMBUS::WriteConfigSpace32(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    case 0x64:
        break;
    default:
        printf("[TigerLake/TigerLakeSMBUS]: Write32 to unknown register 0x%02X\n", offs);
        exit(1);
    }
}

uint8_t TigerLakeSMBUS::ReadConfigSpace8(uint16_t offs)
{
    switch (offs)
    {
    case 0x04:
        return pcicmd;
    case 0x40:
        return 0;
    case 0x54:
        return tcoctl;
    case 0x55:
        return (tcoctl >> 8) & 0xff;
    default:
        printf("[TigerLake/TigerLakeSMBUS]: Read8 from unknown register 0x%02X\n", offs);
        exit(1);
    }
}

uint16_t TigerLakeSMBUS::ReadConfigSpace16(uint16_t offs)
{
    switch (offs)
    {
    case 0x00:
        return 0x8086;
    case 0x50:
        return tcobase;
    case 0x54:
        return tcoctl;
    default:
        printf("[TigerLake/TigerLakeSMBUS]: Read16 from unknown register 0x%02X\n", offs);
        exit(1);
    }
}

uint32_t TigerLakeSMBUS::ReadConfigSpace32(uint16_t offs)
{
    switch (offs)
    {
    default:
        printf("[TigerLake/TigerLakeSMBUS]: Read32 from unknown register 0x%02X\n", offs);
        exit(1);
    }
}
