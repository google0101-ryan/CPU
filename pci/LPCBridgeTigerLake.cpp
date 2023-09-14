#include "LPCBridgeTigerLake.h"

LPCBridgeTigerLake::LPCBridgeTigerLake(PCIBus *pci)
{
    pci->RegisterDevice(0x00, 0x1F, 0x00, this);
}

void LPCBridgeTigerLake::WriteConfigSpace8(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[TigerLake/LPCBridgeTigerLake]: Write8 to unknown register 0x%02X\n", offs);
        exit(1);
    }
}

void LPCBridgeTigerLake::WriteConfigSpace16(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[TigerLake/LPCBridgeTigerLake]: Write16 to unknown register 0x%02X\n", offs);
        exit(1);
    }
}

void LPCBridgeTigerLake::WriteConfigSpace32(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[TigerLake/LPCBridgeTigerLake]: Write32 to unknown register 0x%02X\n", offs);
        exit(1);
    }
}

uint8_t LPCBridgeTigerLake::ReadConfigSpace8(uint16_t offs)
{
    switch (offs)
    {
    default:
        printf("[TigerLake/LPCBridgeTigerLake]: Read8 from unknown register 0x%02X\n", offs);
        exit(1);
    }
}

uint16_t LPCBridgeTigerLake::ReadConfigSpace16(uint16_t offs)
{
    switch (offs)
    {
    case 0x02:
        return 0xa082;
    default:
        printf("[TigerLake/LPCBridgeTigerLake]: Read16 from unknown register 0x%02X\n", offs);
        exit(1);
    }
}

uint32_t LPCBridgeTigerLake::ReadConfigSpace32(uint16_t offs)
{
    switch (offs)
    {
    default:
        printf("[TigerLake/LPCBridgeTigerLake]: Read32 from unknown register 0x%02X\n", offs);
        exit(1);
    }
}
