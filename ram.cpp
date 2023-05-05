#include "ram.h"
#include <fstream>
#include <cstring>

RAM::RAM(size_t size)
: MemoryDevice("Random-Access Memory (" + std::to_string(size/1024) + " KiBs)")
{
    data = new uint8_t[size];

    end = size;

    std::memset(data, 0, size);

    Bus::RegisterDevice(this);
}

void RAM::Dump()
{
    printf("Dumping RAM\n");
    std::ofstream file("ram.bin", std::ios::binary);
    file.write((char*)data, end);
    file.close();
}

uint8_t RAM::Read8(uint64_t addr)
{
    return data[addr];
}

uint16_t RAM::Read16(uint64_t addr)
{
    return *(uint16_t*)&data[addr];
}

uint32_t RAM::Read32(uint64_t addr)
{
    return *(uint32_t*)&data[addr];
}

uint64_t RAM::Read64(uint64_t addr)
{
    return *(uint64_t*)&data[addr];
}

void RAM::Write8(uint64_t addr, uint8_t val)
{
    data[addr] = val;
}

void RAM::Write16(uint64_t addr, uint16_t val)
{
    *(uint16_t*)&data[addr] = val;
}

void RAM::Write32(uint64_t addr, uint32_t val)
{
    *(uint32_t*)&data[addr] = val;
}

void RAM::Write64(uint64_t addr, uint64_t val)
{
    *(uint64_t*)&data[addr] = val;
}
