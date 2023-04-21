#include "PCI.h"

PCIBus::PCIBus()
: IODevice("PCI/PCIe high-speed bus")
{
    pciAddr = 0;

    IOBus::RegisterDevice(this, 0xCF8, 0xCFF);
}

void PCIBus::RegisterDevice(int bus, int dev, int func, PCIDevice *device)
{
    uint32_t addr = (bus << 16) | (dev << 11) | (func << 8);
    devices[dev] = device;
}

uint32_t PCIBus::Read32(uint16_t port)
{
    switch (port)
    {
    case 0xCF8:
        return pciAddr;
    default:
        printf("Unhandled read from PCI port 0x%04x\n", port);
        exit(1);
    }
}

uint16_t PCIBus::Read16(uint16_t port)
{
    switch (port)
    {
    case 0xcfc ... 0xcff:
        if (!devices[pciAddr & 0x7FFFFF00])
        {
            printf("ERROR: Can't read from non-existant PCI device 0x%08x\n", pciAddr & 0x7FFFFF00);
            exit(1);
        }

        return devices[pciAddr & 0x7FFFFF00]->ReadConfigSpace16((pciAddr & 0xFF) + (port-0xcfc));
    default:
        printf("Unhandled read from PCI port 0x%04x\n", port);
        exit(1);
    }
}

void PCIBus::Write32(uint16_t port, uint32_t data)
{
    switch (port)
    {
    case 0xCF8:
        pciAddr = data;
        break;
    default:
        printf("Unhandled write to PCI port 0x%04x\n", port);
        exit(1);
    }
}
