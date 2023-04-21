#pragma once

#include <cstdint>
#include "Bus.h"

class ROM : public MemoryDevice
{
private:
    uint8_t* data;

    uint64_t start, end;
public:
    ROM(std::string filename);
    
    virtual uint64_t GetStart() const {return start;}
    virtual uint64_t GetEnd() const {return end;}

    virtual uint8_t  Read8(uint64_t addr);
    virtual uint16_t Read16(uint64_t addr);
    virtual uint32_t Read32(uint64_t addr);
    virtual uint64_t Read64(uint64_t addr);

    virtual void Write8(uint64_t addr, uint8_t data)   {}
    virtual void Write16(uint64_t addr, uint16_t data) {}
    virtual void Write32(uint64_t addr, uint32_t data) {}
    virtual void Write64(uint64_t addr, uint64_t data) {}
};