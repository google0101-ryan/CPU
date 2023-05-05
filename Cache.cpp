#include "Cache.h"
#include "Bus.h"

#include <cstdio>
#include <climits>

uint8_t Cache::Read8I(uint64_t addr)
{
    uint64_t blockOffs = (addr & 0x3F);
    uint64_t index = (addr >> 6) & 0x3F;
    uint64_t tag = (addr >> 12);

    auto& l = ilines[index];

    for (int i = 0; i < 8; i++)
    {
        if (!l.ways[i].valid) continue;

        if (l.ways[i].tag != tag) continue;

        l.ways[i].age++;

        return l.ways[i].data[blockOffs];
    }

    // Welp, no data was found, so a cache miss occured

    // Search for invalid entries first
    for (int i = 0; i < 8; i++)
    {
        if (!l.ways[i].valid)
        {
            l.ways[i].valid = true;
            l.ways[i].tag = tag;
            l.ways[i].age = 0;
            for (int j = 0; j < 64; j++)
                l.ways[i].data[j] = Bus::Read8((addr & ~0x3F)+j);
            return l.ways[i].data[blockOffs];
        }
    }

    // Search for the least recently used way and replace it
    int min = INT_MIN;
    int ind = 0;
    for (int i = 0; i < 8; i++)
    {
        if (l.ways[i].age > min)
        {
            min = l.ways[i].age;
            ind = i;
        }
    }

    l.ways[ind].age = 0;
    l.ways[ind].tag = tag;
    l.ways[ind].valid = true;
    for (int j = 0; j < 64; j++)
        l.ways[ind].data[j] = Bus::Read8((addr & ~0x3F)+j);
    return l.ways[ind].data[blockOffs];
}

uint8_t Cache::Read8D(uint64_t addr)
{
    uint64_t blockOffs = (addr & 0x3F);
    uint64_t index = (addr >> 6) & 0x3F;
    uint64_t tag = (addr >> 12);

    auto& l = dlines[index];

    for (int i = 0; i < 8; i++)
    {
        if (!l.ways[i].valid) continue;

        if (l.ways[i].tag != tag) continue;

        l.ways[i].age++;

        return l.ways[i].data[blockOffs];
    }

    // Welp, no data was found, so a cache miss occured

    // Search for invalid entries first
    for (int i = 0; i < 8; i++)
    {
        if (!l.ways[i].valid)
        {
            l.ways[i].valid = true;
            l.ways[i].tag = tag;
            l.ways[i].age = 0;
            for (int j = 0; j < 64; j++)
                l.ways[i].data[j] = Bus::Read8((addr & ~0x3F)+j);
            return l.ways[i].data[blockOffs];
        }
    }

    // Search for the least recently used way and replace it
    int min = INT_MIN;
    int ind = 0;
    for (int i = 0; i < 8; i++)
    {
        if (l.ways[i].age > min)
        {
            min = l.ways[i].age;
            ind = i;
        }
    }

    l.ways[ind].age = 0;
    l.ways[ind].tag = tag;
    l.ways[ind].valid = true;
    for (int j = 0; j < 64; j++)
        l.ways[ind].data[j] = Bus::Read8((addr & ~0x3F)+j);
    return l.ways[ind].data[blockOffs];
}

void Cache::Write64(uint64_t addr, uint64_t data)
{
    // uint64_t index = (addr >> 6) & 0x3F;
    // uint64_t tag = (addr >> 12);

    // auto& l = dlines[index];

    // for (int i = 0; i < 8; i++)
    // {
    //     if (!l.ways[i].valid) continue;

    //     if (l.ways[i].tag != tag) continue;
        
    //     // This way no longer contains valid data
    //     l.ways->valid = false;
    //     break;
    // }

    Bus::Write64(addr, data);
}

void Cache::Write32(uint64_t addr, uint32_t data)
{
    // uint64_t index = (addr >> 6) & 0x3F;
    // uint64_t tag = (addr >> 12);

    // auto& l = dlines[index];

    // for (int i = 0; i < 8; i++)
    // {
    //     if (!l.ways[i].valid) continue;

    //     if (l.ways[i].tag != tag) continue;
        
    //     // This way no longer contains valid data
    //     l.ways->valid = false;
    //     break;
    // }

    Bus::Write32(addr, data);
}

void Cache::Write16(uint64_t addr, uint32_t data)
{
    Bus::Write16(addr, data);
}

void Cache::Write8(uint64_t addr, uint8_t data)
{
    // uint64_t blockOffs = (addr & 0x3F);
    // uint64_t index = (addr >> 6) & 0x3F;
    // uint64_t tag = (addr >> 12);

    // auto& l = dlines[index];

    // for (int i = 0; i < 8; i++)
    // {
    //     if (!l.ways[i].valid) continue;

    //     if (l.ways[i].tag != tag) continue;
        
    //     // This way no longer contains valid data
    //     l.ways->valid = false;
    //     break;
    // }

    Bus::Write8(addr, data);
}

uint8_t Cache::Read8(uint64_t addr, bool is_instr)
{
    // if (is_instr)
    //     return Read8I(addr);
    // else
    //     return Read8D(addr);
    return Bus::Read8(addr);
}

uint16_t Cache::Read16(uint64_t addr, bool is_instr)
{
    return Bus::Read16(addr);
}

uint32_t Cache::Read32(uint64_t addr, bool is_instr)
{
    return Bus::Read32(addr);
}

uint64_t Cache::Read64(uint64_t addr, bool is_instr)
{
    return Bus::Read64(addr);
}
