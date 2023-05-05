#pragma once

#include <cstdint>
#include <string>

class MemoryDevice
{
private:
    std::string name;
public:
    MemoryDevice(std::string n) : name(n) {}

    virtual uint64_t GetStart() const = 0;
    virtual uint64_t GetEnd() const = 0;

    const std::string& GetName() const {return name;}

    virtual uint8_t Read8(uint64_t addr) = 0;
    virtual uint16_t Read16(uint64_t addr) = 0;
    virtual uint32_t Read32(uint64_t addr) = 0;
    virtual uint64_t Read64(uint64_t addr) = 0;

    virtual void Write8(uint64_t addr, uint8_t data) = 0;
    virtual void Write16(uint64_t addr, uint16_t data) = 0;
    virtual void Write32(uint64_t addr, uint32_t data) = 0;
    virtual void Write64(uint64_t addr, uint64_t data) = 0;
};

namespace Bus
{

void RegisterDevice(MemoryDevice* dev);

uint8_t Read8(uint64_t addr);
uint16_t Read16(uint64_t addr);
uint32_t Read32(uint64_t addr);
uint64_t Read64(uint64_t addr);

void Write8(uint64_t addr, uint8_t data);
void Write16(uint64_t addr, uint16_t data);
void Write32(uint64_t addr, uint32_t data);
void Write64(uint64_t addr, uint64_t data);

}