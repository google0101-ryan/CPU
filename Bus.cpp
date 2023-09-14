#include "Bus.h"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <fstream>

std::vector<MemoryDevice*> devices;

uint8_t car[0x80000]; // 512KiBs of cache, mapped at 0xfef00000 when CAR is enabled

size_t Bus::RegisterDevice(MemoryDevice *dev)
{
    printf("[emu/Bus]: Registered device %s from 0x%lx to 0x%lx\n", dev->GetName().c_str(), dev->GetStart(), dev->GetEnd());
    devices.push_back(dev);
    return devices.size()-1;
}

void Bus::UnregisterDevice(size_t i)
{
    devices.erase(devices.begin()+i);
}

void Bus::Dump()
{
    std::ofstream file("car.bin");
    file.write((char*)car, 0x80000);
    file.close();
}

uint8_t Bus::Read8(uint64_t addr)
{
    if (addr >= 0xfef00000 && addr < 0xfef80000)
        return car[addr & 0x7FFFF];
    
    for (uint64_t i = 0; i < devices.size(); i++)
    {
        if (devices[i]->GetStart() <= addr && devices[i]->GetEnd() >= addr)
            return devices[i]->Read8(addr);
    }

    printf("Read8 from unknown addr 0x%lx\n", addr);
    exit(1);
}

uint16_t Bus::Read16(uint64_t addr)
{
    if (addr >= 0xfef00000 && addr < 0xfef80000)
        return *(uint16_t*)&car[addr & 0x7FFFF];
    
    for (uint64_t i = 0; i < devices.size(); i++)
    {
        if (devices[i]->GetStart() <= addr && devices[i]->GetEnd() >= addr)
            return devices[i]->Read16(addr);
    }

    printf("Read16 from unknown addr 0x%lx\n", addr);
    exit(1);
}

uint32_t Bus::Read32(uint64_t addr)
{
    if (addr == 0xfed300a4) // Intel TXT
        return 0x00000000;
    if (addr >= 0xfef00000 && addr < 0xfef80000)
        return *(uint32_t*)&car[addr & 0x7FFFF];
    if ((addr & 0xff000000) == 0xfd000000)
        return 0; // Some kind of P2SB bus that I can't find docs on

    for (uint64_t i = 0; i < devices.size(); i++)
    {
        if (devices[i]->GetStart() <= addr && devices[i]->GetEnd() >= addr)
            return devices[i]->Read32(addr);
    }

    printf("Read32 from unknown addr 0x%lx\n", addr);
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
    if (addr >= 0xfef00000 && addr < 0xfef80000)
    {
        car[addr & 0x7FFFF] = data;
        return;
    }
    
    for (uint64_t i = 0; i < devices.size(); i++)
    {
        if (devices[i]->GetStart() <= addr && devices[i]->GetEnd() >= addr)
            return devices[i]->Write8(addr, data);
    }

    printf("Write8 to unknown addr 0x%lx\n", addr);
    exit(1);
}

void Bus::Write16(uint64_t addr, uint16_t data)
{
    if (addr >= 0xfef00000 && addr < 0xfef80000)
    {
        *(uint16_t*)&car[addr & 0x7FFFF] = data;
        return;
    }
    if ((addr & 0xff000000) == 0xfd000000)
        return; // Some kind of P2SB bus that I can't find docs on

    for (uint64_t i = 0; i < devices.size(); i++)
    {
        if (devices[i]->GetStart() <= addr && devices[i]->GetEnd() >= addr)
            return devices[i]->Write16(addr, data);
    }

    printf("Write16 to unknown addr 0x%lx\n", addr);
    exit(1);
}

void Bus::Write32(uint64_t addr, uint32_t data)
{
    if ((addr & 0xfff00000) == 0xfed00000)
        return;

    if (addr >= 0xfef00000 && addr < 0xfef80000)
    {
        *(uint32_t*)&car[addr & 0x7FFFF] = data;
        return;
    }
    if ((addr & 0xff000000) == 0xfd000000)
        return; // Some kind of P2SB bus that I can't find docs on

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
