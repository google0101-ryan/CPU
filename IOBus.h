#pragma once

#include <cstdint>
#include <string>

class IODevice
{
private:
    std::string name;
public:
    IODevice(std::string name) : name(name) {}

    std::string GetName() {return name;}

    virtual uint32_t Read32(uint16_t port) = 0;
    virtual uint16_t Read16(uint16_t port) = 0;
    virtual uint8_t Read8(uint16_t port) = 0;

    virtual void Write32(uint16_t port, uint32_t data) = 0;
    virtual void Write8(uint16_t port, uint8_t data) = 0;
};

namespace IOBus
{

void InitDebugcon();

void RegisterDevice(IODevice* dev, uint16_t start, uint16_t end);

uint32_t In32(uint16_t port);
uint16_t In16(uint16_t port);
uint8_t In8(uint16_t port);

void Out32(uint16_t port, uint32_t data);
void Out8(uint16_t port, uint8_t data);

}