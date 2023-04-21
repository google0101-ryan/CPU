#include "Bus.h"

#include <cstdio>
#include <cstdlib>
#include <vector>

std::vector<MemoryDevice*> devices;

void Bus::RegisterDevice(MemoryDevice *dev)
{
    printf("[emu/Bus]: Registered device %s from 0x%lx to 0x%lx\n", dev->GetName().c_str(), dev->GetStart(), dev->GetEnd());
    devices.push_back(dev);
}

uint8_t Bus::Read8(uint64_t addr)
{
    for (uint64_t i = 0; i < devices.size(); i++)
    {
        if (devices[i]->GetStart() <= addr && devices[i]->GetEnd() >= addr)
            return devices[i]->Read8(addr);
    }

    printf("Read8 from unknown addr 0x%lx\n", addr);
    exit(1);
}

uint64_t Bus::Read64(uint64_t addr)
{
    for (uint64_t i = 0; i < devices.size(); i++)
    {
        if (devices[i]->GetStart() <= addr && devices[i]->GetEnd() >= addr)
            return devices[i]->Read64(addr);
    }

    printf("Read64 from unknown addr 0x%lx\n", addr);
    exit(1);
}

void Bus::Write8(uint64_t addr, uint8_t data)
{
    for (uint64_t i = 0; i < devices.size(); i++)
    {
        if (devices[i]->GetStart() <= addr && devices[i]->GetEnd() >= addr)
            return devices[i]->Write8(addr, data);
    }

    printf("Write8 to unknown addr 0x%lx\n", addr);
    exit(1);
}

void Bus::Write32(uint64_t addr, uint32_t data)
{
    for (uint64_t i = 0; i < devices.size(); i++)
    {
        if (devices[i]->GetStart() <= addr && devices[i]->GetEnd() >= addr)
            return devices[i]->Write32(addr, data);
    }

    printf("Write32 to unknown addr 0x%lx\n", addr);
    exit(1);
}

void Bus::Write64(uint64_t addr, uint64_t data)
{
    for (uint64_t i = 0; i < devices.size(); i++)
    {
        if (devices[i]->GetStart() <= addr && devices[i]->GetEnd() >= addr)
            return devices[i]->Write64(addr, data);
    }

    printf("Write64 to unknown addr 0x%lx\n", addr);
    exit(1);
}
