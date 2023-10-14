#include "LPCBridgeQ35.h"

LPCBridge::LPCBridge(PCIBus *pci)
{
    name = "Q35 LPC Bridge";
    pci->RegisterDevice(0x00, 0x1F, 0x00, this);
}

void LPCBridge::WriteConfigSpace8(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    case 0x44:
        acpi_cntl = data;
        break;
    default:
        printf("[Q35/LPCBridge]: Write8 to unknown register 0x%02X\n", offs);
        exit(1);
    }
}

void LPCBridge::WriteConfigSpace16(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[Q35/LPCBridge]: Write16 to unknown register 0x%02X\n", offs);
        exit(1);
    }
}

void LPCBridge::WriteConfigSpace32(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    case 0x40:
        data &= 0xFF80;
        pmbase &= ~0xFF80;
        pmbase |= data;
        break;
    default:
        printf("[Q35/LPCBridge]: Write32 to unknown register 0x%02X\n", offs);
        exit(1);
    }
}

uint8_t LPCBridge::ReadConfigSpace8(uint16_t offs)
{
    switch (offs)
    {
    case 0x44:
        return acpi_cntl;
    default:
        printf("[Q35/LPCBridge]: Read8 from unknown register 0x%02X\n", offs);
        exit(1);
    }
}

uint16_t LPCBridge::ReadConfigSpace16(uint16_t offs)
{
    switch (offs)
    {
    case 0x00:
        return 0x8086;
    default:
        printf("[Q35/LPCBridge]: Read16 from unknown register 0x%02X\n", offs);
        exit(1);
    }
}

uint32_t LPCBridge::ReadConfigSpace32(uint16_t offs)
{
    switch (offs)
    {
    case 0x40:
        return pmbase;
    default:
        printf("[Q35/LPCBridge]: Read32 from unknown register 0x%02X\n", offs);
        exit(1);
    }
}
