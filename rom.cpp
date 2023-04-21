#include "rom.h"

#include <fstream>

ROM::ROM(std::string filename)
: MemoryDevice("Read-only memory [" + filename + "]")
{
    std::ifstream file(filename, std::ios::ate);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    data = new uint8_t[size];
    file.read((char*)data, size);
    file.close();

    end = 0xFFFFFFFF;
    start = 0xFFFFFFFF - size+1;

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
