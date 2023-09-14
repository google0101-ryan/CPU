#include "DramControllerTigerLake.h"

DramControllerTigerLake::DramControllerTigerLake(PCIBus *bus)
{
    pciexbar = 0;
    pci = bus;
    bus->RegisterDevice(0x00, 0x00, 0x00, this);
}

void DramControllerTigerLake::WriteConfigSpace8(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[Q35/DramControllerTigerLake]: Write8 to unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

void DramControllerTigerLake::WriteConfigSpace16(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[Q35/DramControllerTigerLake]: Write16 to unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

void DramControllerTigerLake::WriteConfigSpace32(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    case 0x48:
        printf("[emu/DramControllerTigerLake]: Write 0x%08x to MCHBAR\n", data);
        mchbar = data;
        break;
    case 0x60:
        printf("[emu/DramControllerTigerLake]: Write 0x%08x to PCIEXBAR\n", data);
        pciexbar = data;
        pci->MapPcie(pciexbar & ~1);
        break;
    default:
        printf("[Q35/DramControllerTigerLake]: Write32 to unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

uint8_t DramControllerTigerLake::ReadConfigSpace8(uint16_t offs)
{
    switch (offs)
    {
    default:
        printf("[Q35/DramControllerTigerLake]: Read8 from unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

uint16_t DramControllerTigerLake::ReadConfigSpace16(uint16_t offs)
{
    switch (offs)
    {
    default:
        printf("[Q35/DramControllerTigerLake]: Read16 from unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

uint32_t DramControllerTigerLake::ReadConfigSpace32(uint16_t offs)
{
    switch (offs)
    {
    case 0x60:
        return pciexbar;
    default:
        printf("[Q35/DramControllerTigerLake]: Read32 from unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}
