#include "DramControllerQ35.h"

DramController::DramController(PCIBus *bus)
{
    bus->RegisterDevice(0x00, 0x00, 0x00, this);
}

void DramController::WriteConfigSpace8(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[Q35/DramController]: Write8 to unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

void DramController::WriteConfigSpace16(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[Q35/DramController]: Write16 to unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

void DramController::WriteConfigSpace32(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    case 0x90:
        *(uint32_t*)&pam[0] = data;
        return;
    case 0x94:
        *(uint32_t*)&pam[4] = data;
        return;
    default:
        printf("[Q35/DramController]: Write32 to unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

uint8_t DramController::ReadConfigSpace8(uint16_t offs)
{
    switch (offs)
    {
    case 0x90 ... 0x96:
        return pam[offs - 0x90];
    default:
        printf("[Q35/DramController]: Read8 from unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

uint16_t DramController::ReadConfigSpace16(uint16_t offs)
{
    switch (offs)
    {
    case 0x00:
        return 0x8086;
    case 0x02:
        return 0x29C0; // Q35 Northbridge
    case 0x2C:
        return 0x1af4;
    case 0x2E:
        return 0x1100;
    default:
        printf("[Q35/DramController]: Read16 from unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

uint32_t DramController::ReadConfigSpace32(uint16_t offs)
{
    switch (offs)
    {
    case 0x00:
        return 0x29C08086;
    case 0x90:
        return *(uint32_t*)&pam[0];
    case 0x94:
        return *(uint32_t*)&pam[4];
    default:
        printf("[Q35/DramController]: Read32 from unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}
