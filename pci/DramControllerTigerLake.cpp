#include "DramControllerTigerLake.h"

DramController::DramController(PCIBus *bus)
{
    bus->RegisterDevice(0x00, 0x00, 0x00, this);
}

void DramController::WriteConfigSpace8(uint8_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[Q35/DramController]: Write8 to unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

void DramController::WriteConfigSpace16(uint8_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[Q35/DramController]: Write16 to unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

void DramController::WriteConfigSpace32(uint8_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[Q35/DramController]: Write32 to unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

uint8_t DramController::ReadConfigSpace8(uint8_t offs)
{
    switch (offs)
    {
    default:
        printf("[Q35/DramController]: Read8 from unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

uint16_t DramController::ReadConfigSpace16(uint8_t offs)
{
    switch (offs)
    {
    case 0x02:
        return 0x29C0; // Tiger Lake Northbridge
    default:
        printf("[Q35/DramController]: Read16 from unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

uint32_t DramController::ReadConfigSpace32(uint8_t offs)
{
    switch (offs)
    {
    default:
        printf("[Q35/DramController]: Read32 from unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}
