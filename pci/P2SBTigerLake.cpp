#include "P2SBTigerLake.h"

P2SBTigerLake::P2SBTigerLake(PCIBus *pci)
{
    pci->RegisterDevice(0x00, 0x1F, 0x01, this);

    pcicmd = 0;
}

void P2SBTigerLake::WriteConfigSpace8(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    case 0x04:
        pcicmd = data & 3;
        break;
    case 0x60:
        hptc_bar = data;
        printf("[TigerLake/P2SBTigerLake]: Mapped HPET to 0x%08lx\n", hptc_bar & 0xFFF00000);
        break;
    default:
        printf("[TigerLake/P2SBTigerLake]: Write8 to unknown register 0x%02X\n", offs);
        exit(1);
    }
}

void P2SBTigerLake::WriteConfigSpace16(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[TigerLake/P2SBTigerLake]: Write16 to unknown register 0x%02X\n", offs);
        exit(1);
    }
}

void P2SBTigerLake::WriteConfigSpace32(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    case 0x10:
        p2sb_bar = data;
        printf("[TigerLake/P2SBTigerLake]: Mapped P2SB to 0x%08x\n", p2sb_bar & 0xFF000000);
        break;
    default:
        printf("[TigerLake/P2SBTigerLake]: Write32 to unknown register 0x%02X\n", offs);
        exit(1);
    }
}

uint8_t P2SBTigerLake::ReadConfigSpace8(uint16_t offs)
{
    switch (offs)
    {
    case 0x04:
        return pcicmd;
    case 0x60:
        return hptc_bar;
    default:
        printf("[TigerLake/P2SBTigerLake]: Read8 from unknown register 0x%02X\n", offs);
        exit(1);
    }
}

uint16_t P2SBTigerLake::ReadConfigSpace16(uint16_t offs)
{
    switch (offs)
    {
    default:
        printf("[TigerLake/P2SBTigerLake]: Read16 from unknown register 0x%02X\n", offs);
        exit(1);
    }
}

uint32_t P2SBTigerLake::ReadConfigSpace32(uint16_t offs)
{
    switch (offs)
    {
    default:
        printf("[TigerLake/P2SBTigerLake]: Read32 from unknown register 0x%02X\n", offs);
        exit(1);
    }
}
