#include "VmwareSvga.h"

VmwareSvga::VmwareSvga(PCIBus *pci)
{
    pci->RegisterDevice(0, 1, 0, this);
}

void VmwareSvga::WriteConfigSpace8(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[GPU/VmwareSvga]: Write8 to unknown register 0x%02X\n", offs);
        exit(1);
    }
}

void VmwareSvga::WriteConfigSpace16(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[GPU/VmwareSvga]: Write16 to unknown register 0x%02X\n", offs);
        exit(1);
    }
}

void VmwareSvga::WriteConfigSpace32(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[GPU/VmwareSvga]: Write32 to unknown register 0x%02X\n", offs);
        exit(1);
    }
}

uint8_t VmwareSvga::ReadConfigSpace8(uint16_t offs)
{
    switch (offs)
    {
    default:
        printf("[GPU/VmwareSvga]: Read8 from unknown register 0x%02X\n", offs);
        exit(1);
    }
}

uint16_t VmwareSvga::ReadConfigSpace16(uint16_t offs)
{
    switch (offs)
    {
    case 0x00:
        return 0x15ad; // SVGA-II
    default:
        printf("[GPU/VmwareSvga]: Read16 from unknown register 0x%02X\n", offs);
        exit(1);
    }
}

uint32_t VmwareSvga::ReadConfigSpace32(uint16_t offs)
{
    switch (offs)
    {
    default:
        printf("[GPU/VmwareSvga]: Read32 from unknown register 0x%02X\n", offs);
        exit(1);
    }
}
