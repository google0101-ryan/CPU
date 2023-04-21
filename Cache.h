#pragma once

#include <cstdint>

// 8-way associative cache
// 63                  11                  6 5                    0
// +-------------------+--------------------+--------------------+
// |        Tag        |        index       |    block offset    |
// +-------------------+--------------------+--------------------+

// We use index to look up the correct cache line, and then check all
// 8 ways for a matching tag


// TODO: Don't hardcode cache size values, so the user can configure them
// Also add the ability to disable cache simulation entirely

struct CacheLine
{
    struct Way
    {
        uint64_t tag;
        uint8_t data[64];
        bool valid = false;
        int age = 0;
    } ways[8];
};

class Cache
{
private:
    CacheLine ilines[64]; // 64 lines of 8 ways, each with 64 bytes of data (32 KiBs of cache)
    CacheLine dlines[64]; // 64 lines of 8 ways, each with 64 bytes of data (32 KiBs of cache)

    uint8_t Read8I(uint64_t addr);
    uint8_t Read8D(uint64_t addr);
public:
    void Write64(uint64_t addr, uint64_t data);
    void Write32(uint64_t addr, uint32_t data);
    void Write16(uint64_t addr, uint32_t data);
    void Write8(uint64_t addr, uint8_t data);

    uint8_t Read8(uint64_t addr, bool is_instr);
    uint16_t Read16(uint64_t addr, bool is_instr);
    uint32_t Read32(uint64_t addr, bool is_instr);
    uint64_t Read64(uint64_t addr, bool is_instr);
};