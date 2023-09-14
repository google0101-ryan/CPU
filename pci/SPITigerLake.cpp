#include "SPITigerLake.h"

SPITigerLake::SPITigerLake(PCIBus *pci)
{
    pci->RegisterDevice(0x00, 0x1F, 0x05, this);
}

void SPITigerLake::WriteConfigSpace8(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[TigerLake/SPITigerLake]: Write8 to unknown register 0x%02X\n", offs);
        exit(1);
    }
}

void SPITigerLake::WriteConfigSpace16(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[TigerLake/SPITigerLake]: Write16 to unknown register 0x%02X\n", offs);
        exit(1);
    }
}

void SPITigerLake::WriteConfigSpace32(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[TigerLake/SPITigerLake]: Write32 to unknown register 0x%02X\n", offs);
        exit(1);
    }
}

uint8_t SPITigerLake::ReadConfigSpace8(uint16_t offs)
{
    switch (offs)
    {
    default:
        printf("[TigerLake/SPITigerLake]: Read8 from unknown register 0x%02X\n", offs);
        exit(1);
    }
}

uint16_t SPITigerLake::ReadConfigSpace16(uint16_t offs)
{
    switch (offs)
    {
    default:
        printf("[TigerLake/SPITigerLake]: Read16 from unknown register 0x%02X\n", offs);
        exit(1);
    }
}

uint32_t SPITigerLake::ReadConfigSpace32(uint16_t offs)
{
    switch (offs)
    {
    case 0xDC:
        return bios_spi_ct;
    default:
        printf("[TigerLake/SPITigerLake]: Read32 from unknown register 0x%02X\n", offs);
        exit(1);
    }
}
