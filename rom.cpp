#include "rom.h"

#include <fstream>

ROM::ROM(std::string filename, bool high)
: MemoryDevice("Read-only memory [" + filename + "]")
{
    std::ifstream file(filename, std::ios::ate);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    data = new uint8_t[size];
    file.read((char*)data, size);
    file.close();

    if (high)
    {
        end = 0xFFFFFFFF;
        start = 0xFFFFFFFF - size+1;
    }
    else
    {
        data += (size - (128*1024));
        start = 0x100000 - (128*1024);
        end = start + (128*1024);
    }

    Bus::RegisterDevice(this);
}

uint8_t ROM::Read8(uint64_t addr)
{
    return data[addr - start];
}

uint16_t ROM::Read16(uint64_t addr)
{
    return *(uint16_t*)&data[addr - start];
}

uint32_t ROM::Read32(uint64_t addr)
{
    return *(uint32_t*)&data[addr - start];
}

uint64_t ROM::Read64(uint64_t addr)
{
    return *(uint64_t*)&data[addr - start];
}

void ROM::Write8(uint64_t addr, uint8_t value)
{
    data[addr - start] = value;
}

void ROM::Write16(uint64_t addr, uint16_t value)
{
    *(uint16_t*)&data[addr - start] = value;
}

void ROM::Write32(uint64_t addr, uint32_t value)
{
    *(uint32_t*)&data[addr - start] = value;
}

void ROM::Write64(uint64_t addr, uint64_t value)
{
    *(uint64_t*)&data[addr - start] = value;
}
