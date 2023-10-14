#include "RealtekEthernet.h"

RealtekInternet::RealtekInternet(PCIBus *bus)
{
    name = "Realtek RTL8111 Ethernet";
    bus->RegisterDevice(0, 2, 0, this);
}

void RealtekInternet::WriteConfigSpace8(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[Net/RealtekInternet]: Write8 to unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

void RealtekInternet::WriteConfigSpace16(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[Net/RealtekInternet]: Write16 to unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

void RealtekInternet::WriteConfigSpace32(uint16_t offs, uint32_t data)
{
    switch (offs)
    {
    default:
        printf("[Net/RealtekInternet]: Write32 to unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

uint8_t RealtekInternet::ReadConfigSpace8(uint16_t offs)
{
    switch (offs)
    {
    default:
        printf("[Net/RealtekInternet]: Read8 from unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

uint16_t RealtekInternet::ReadConfigSpace16(uint16_t offs)
{
    switch (offs)
    {
    case 0x00:
        return 0x10ec; // Realtek
    default:
        printf("[Net/RealtekInternet]: Read16 from unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}

uint32_t RealtekInternet::ReadConfigSpace32(uint16_t offs)
{
    switch (offs)
    {
    default:
        printf("[Net/RealtekInternet]: Read32 from unknown config space register 0x%02x\n", offs);
        exit(1);
    }
}
