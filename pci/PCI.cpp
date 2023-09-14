#include "PCI.h"

// An unimplemented PCI device
class GenericPCIDevice : public PCIDevice
{
public:
    GenericPCIDevice(int bus, int dev, int func, PCIBus* pci)
    {
        pci->RegisterDevice(bus, dev, func, this);
    }

    void WriteConfigSpace8(uint16_t offs, uint32_t data) override {}
    void WriteConfigSpace16(uint16_t offs, uint32_t data) override {}
    void WriteConfigSpace32(uint16_t offs, uint32_t data) override {}

    uint8_t ReadConfigSpace8(uint16_t offs) override {return 0xff;}
    uint16_t ReadConfigSpace16(uint16_t offs) override {return 0xffff;}
    uint32_t ReadConfigSpace32(uint16_t offs) override {return 0xffffffff;}
};

PCIBus::PCIBus()
: IODevice("PCI/PCIe high-speed bus"), MemoryDevice("PCI Express config space")
{
    pciAddr = 0;

    IOBus::RegisterDevice(this, 0xCF8, 0xCFF);

    GenericPCIDevice* dev = new GenericPCIDevice(0, 3, 0, this);
    for (int i = 4; i < 32; i++)
        dev = new GenericPCIDevice(0, i, 0, this);
}

void PCIBus::RegisterDevice(int bus, int dev, int func, PCIDevice *device)
{
    uint32_t addr = (bus << 16) | (dev << 11) | (func << 8);
    devices[addr] = device;
    printf("Registered device %d:%d.%d (0x%08x)\n", bus, dev, func, addr);
}

void PCIBus::MapPcie(uint64_t addr)
{
    if (pcie_base != 0)
        Bus::UnregisterDevice(index);
    pcie_base = addr;

    index = Bus::RegisterDevice(this);
}

uint32_t PCIBus::In32(uint16_t port)
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
            printf("ERROR: Can't in32 from non-existant PCI device 0x%08x\n", pciAddr & 0x7FFFFF00);
            exit(1);
        }

        return devices[pciAddr & 0x7FFFFF00]->ReadConfigSpace32(reg);
    }
    default:
        printf("Unhandled read from PCI port 0x%04x\n", port);
        exit(1);
    }
}

uint16_t PCIBus::In16(uint16_t port)
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
        if (bus == 0 && dev == 31 && func == 2)
            return 0xffff;
        if (!devices[pciAddr & 0x7FFFFF00])
        {
            printf("ERROR: Can't in16 from non-existant PCI device 0x%08x\n", pciAddr & 0x7FFFFF00);
            exit(1);
        }

        return devices[pciAddr & 0x7FFFFF00]->ReadConfigSpace16(reg);
    }
    default:
        printf("Unhandled read from PCI port 0x%04x\n", port);
        exit(1);
    }
}

uint8_t PCIBus::In8(uint16_t port)
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
            printf("ERROR: Can't in8 from non-existant PCI device 0x%08x\n", pciAddr & 0x7FFFFF00);
            exit(1);
        }

        return devices[pciAddr & 0x7FFFFF00]->ReadConfigSpace8(reg);
    }
    default:
        printf("Unhandled read from PCI port 0x%04x\n", port);
        exit(1);
    }
}

void PCIBus::Out32(uint16_t port, uint32_t data)
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

void PCIBus::Out16(uint16_t port, uint32_t data)
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
            printf("ERROR: Can't out8 to non-existant PCI device 0x%08x\n", pciAddr & 0x7FFFFF00);
            exit(1);
        }

        devices[pciAddr & 0x7FFFFF00]->WriteConfigSpace16(reg, data);
        break;
    }
    default:
        printf("Unhandled write8 to PCI port 0x%04x\n", port);
        exit(1);
    }
}

void PCIBus::Out8(uint16_t port, uint8_t data)
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
            printf("ERROR: Can't out8 to non-existant PCI device 0x%08x\n", pciAddr & 0x7FFFFF00);
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

void DecodePCIeAddr(uint64_t addr, uint8_t& bus, uint8_t& device, uint8_t& func)
{
    bus = (addr >> 20) & 0xFF;
    device = (addr >> 15) & 0x1F;
    func = (addr >> 12) & 0x7;
}

uint8_t PCIBus::Read8(uint64_t addr)
{
    uint8_t bus, device, func;
    DecodePCIeAddr(addr, bus, device, func);

    uint32_t pci_addr = (bus << 16) | (device << 11) | (func << 8);
    if (!devices[pci_addr & 0x7FFFFF00])
    {
        printf("ERROR: Can't read8 from non-existant PCI device 0x%08x\n", pci_addr & 0x7FFFFF00);
        exit(1);
    }

    return devices[pci_addr & 0x7FFFFF00]->ReadConfigSpace8(addr & 0xFFF);
}

uint16_t PCIBus::Read16(uint64_t addr)
{
    uint8_t bus, device, func;
    DecodePCIeAddr(addr, bus, device, func);

    uint32_t pci_addr = (bus << 16) | (device << 11) | (func << 8);
    if (!devices[pci_addr & 0x7FFFFF00])
    {
        printf("ERROR: Can't read16 from non-existant PCI device 0x%08x\n", pci_addr & 0x7FFFFF00);
        exit(1);
    }

    return devices[pci_addr & 0x7FFFFF00]->ReadConfigSpace16(addr & 0xFFF);
}


uint32_t PCIBus::Read32(uint64_t addr)
{
    uint8_t bus, device, func;
    DecodePCIeAddr(addr, bus, device, func);

    uint32_t pci_addr = (bus << 16) | (device << 11) | (func << 8);
    if (!devices[pci_addr & 0x7FFFFF00])
    {
        printf("ERROR: Can't read32 from non-existant PCI device 0x%08x\n", pci_addr & 0x7FFFFF00);
        exit(1);
    }

    return devices[pci_addr & 0x7FFFFF00]->ReadConfigSpace32(addr & 0xFFF);
}

void PCIBus::Write8(uint64_t addr, uint8_t data)
{
    uint8_t bus, device, func;
    DecodePCIeAddr(addr, bus, device, func);

    uint32_t pci_addr = (bus << 16) | (device << 11) | (func << 8);
    if (!devices[pci_addr & 0x7FFFFF00])
    {
        printf("ERROR: Can't write8 to non-existant PCI device 0x%08x (%d:%d.%d)\n", pci_addr & 0x7FFFFF00, bus, device, func);
        exit(1);
    }

    devices[pci_addr & 0x7FFFFF00]->WriteConfigSpace8(addr & 0xFFF, data);
}

void PCIBus::Write16(uint64_t addr, uint16_t data)
{
    uint8_t bus, device, func;
    DecodePCIeAddr(addr, bus, device, func);

    uint32_t pci_addr = (bus << 16) | (device << 11) | (func << 8);
    if (!devices[pci_addr & 0x7FFFFF00])
    {
        printf("ERROR: Can't write to non-existant PCI device 0x%08x\n", pci_addr & 0x7FFFFF00);
        exit(1);
    }

    devices[pci_addr & 0x7FFFFF00]->WriteConfigSpace16(addr & 0xFFF, data);
}

void PCIBus::Write32(uint64_t addr, uint32_t data)
{
    uint8_t bus, device, func;
    DecodePCIeAddr(addr, bus, device, func);

    uint32_t pci_addr = (bus << 16) | (device << 11) | (func << 8);
    if (!devices[pci_addr & 0x7FFFFF00])
    {
        printf("ERROR: Can't write to non-existant PCI device 0x%08x\n", pci_addr & 0x7FFFFF00);
        exit(1);
    }

    devices[pci_addr & 0x7FFFFF00]->WriteConfigSpace32(addr & 0xFFF, data);
}