#pragma once

#include <cstdint>
#include <unordered_map>
#include "IOBus.h"

class PCIDevice
{
public:
    virtual uint16_t ReadConfigSpace16(uint8_t offs) = 0;
};

class PCIBus : public IODevice
{
private:
    uint32_t pciAddr; // 0xCF8

    std::unordered_map<uint32_t, PCIDevice*> devices;
public:
    PCIBus();

    void RegisterDevice(int bus, int dev, int func, PCIDevice* device);

    uint32_t Read32(uint16_t port) override;
    uint16_t Read16(uint16_t port) override;

    void Write32(uint16_t port, uint32_t data) override;
};