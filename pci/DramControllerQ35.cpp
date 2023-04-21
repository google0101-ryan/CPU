#include "DramControllerQ35.h"

DramController::DramController(PCIBus *bus)
{
    bus->RegisterDevice(0x00, 0x00, 0x00, this);
}

uint16_t DramController::ReadConfigSpace16(uint8_t offs)
{
    switch (offs)
    {
    case 0x02:
        return 0x29C0; // Q35 Northbridge
    default:
        printf("[Q35/DramController]: Read from unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}