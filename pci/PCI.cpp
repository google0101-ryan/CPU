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
    devices[addr] = device;
    printf("Registered device %d:%d.%d (0x%08x)\n", bus, dev, func, addr);
}

uint32_t PCIBus::Read32(uint16_t port)
{
    switch (port)
    {
    case 0xCF8:
        printf("[PCI]: Reading PCI addr 0x%08x\n", pciAddr);
        return pciAddr;
    case 0xCFC:
    {
        int bus = (pciAddr >> 16) & 0xFF;
        int dev = (pciAddr >> 11) & 0x1F;
        int func = (pciAddr >> 8) & 0x7;
        int reg = (pciAddr & 0xFF);
        printf("[PCI]: Reading from PCI device %d:%d.%d register 0x%02x\n", bus, dev, func, reg);
        if (!devices[pciAddr & 0x7FFFFF00])
        {
            printf("ERROR: Can't read from non-existant PCI device 0x%08x\n", pciAddr & 0x7FFFFF00);
            exit(1);
        }

        return devices[pciAddr & 0x7FFFFF00]->ReadConfigSpace32(reg);
    }
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
    {
        int bus = (pciAddr >> 16) & 0xFF;
        int dev = (pciAddr >> 11) & 0x1F;
        int func = (pciAddr >> 8) & 0x7;
        int reg = (pciAddr & 0xFF) + (port-0xcfc);
        printf("[PCI]: Reading from PCI device %d:%d.%d register 0x%02x\n", bus, dev, func, reg);
        if (!devices[pciAddr & 0x7FFFFF00])
        {
            printf("ERROR: Can't read from non-existant PCI device 0x%08x\n", pciAddr & 0x7FFFFF00);
            exit(1);
        }

        return devices[pciAddr & 0x7FFFFF00]->ReadConfigSpace16(reg);
    }
    default:
        printf("Unhandled read from PCI port 0x%04x\n", port);
        exit(1);
    }
}

uint8_t PCIBus::Read8(uint16_t port)
{
    switch (port)
    {
    case 0xcfc ... 0xcff:
    {
        int bus = (pciAddr >> 16) & 0xFF;
        int dev = (pciAddr >> 11) & 0x1F;
        int func = (pciAddr >> 8) & 0x7;
        int reg = (pciAddr & 0xFF) + (port-0xcfc);
        printf("[PCI]: Read8 from PCI device %d:%d.%d register 0x%02x\n", bus, dev, func, reg);
        if (!devices[pciAddr & 0x7FFFFF00])
        {
            printf("ERROR: Can't read from non-existant PCI device 0x%08x\n", pciAddr & 0x7FFFFF00);
            exit(1);
        }

        return devices[pciAddr & 0x7FFFFF00]->ReadConfigSpace8(reg);
    }
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
        printf("[PCI]: Writing 0x%08x to CONFIG_ADDR\n", data);
        pciAddr = data;
        break;
    case 0xcfc ... 0xcff:
    {
        int bus = (pciAddr >> 16) & 0xFF;
        int dev = (pciAddr >> 11) & 0x1F;
        int func = (pciAddr >> 8) & 0x7;
        int reg = (pciAddr & 0xFF) + (port-0xcfc);
        printf("[PCI]: Write 0x%08x to PCI device %d:%d.%d register 0x%02x\n", data, bus, dev, func, reg);
        if (!devices[pciAddr & 0x7FFFFF00])
        {
            printf("ERROR: Can't write to non-existant PCI device 0x%08x\n", pciAddr & 0x7FFFFF00);
            exit(1);
        }

        devices[pciAddr & 0x7FFFFF00]->WriteConfigSpace32(reg, data);
        break;
    }
    default:
        printf("Unhandled write32 to PCI port 0x%04x\n", port);
        exit(1);
    }
}

void PCIBus::Write8(uint16_t port, uint8_t data)
{
    switch (port)
    {
    case 0xcfc ... 0xcff:
    {
        int bus = (pciAddr >> 16) & 0xFF;
        int dev = (pciAddr >> 11) & 0x1F;
        int func = (pciAddr >> 8) & 0x7;
        int reg = (pciAddr & 0xFF) + (port-0xcfc);
        printf("[PCI]: Write 0x%02x to PCI device %d:%d.%d register 0x%02x\n", data, bus, dev, func, reg);
        if (!devices[pciAddr & 0x7FFFFF00])
        {
            printf("ERROR: Can't write to non-existant PCI device 0x%08x\n", pciAddr & 0x7FFFFF00);
            exit(1);
        }

        devices[pciAddr & 0x7FFFFF00]->WriteConfigSpace8(reg, data);
        break;
    }
    default:
        printf("Unhandled write8 to PCI port 0x%04x\n", port);
        exit(1);
    }
}
