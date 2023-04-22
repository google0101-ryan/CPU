#pragma once

#include <cstdint>
#include <unordered_map>
#include "IOBus.h"

class PCIDevice
{
public:
    virtual void WriteConfigSpace8(uint8_t offs, uint32_t data) = 0;
    virtual void WriteConfigSpace16(uint8_t offs, uint32_t data) = 0;
    virtual void WriteConfigSpace32(uint8_t offs, uint32_t data) = 0;

    virtual uint8_t ReadConfigSpace8(uint8_t offs) = 0;
    virtual uint16_t ReadConfigSpace16(uint8_t offs) = 0;
    virtual uint32_t ReadConfigSpace32(uint8_t offs) = 0;
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
    uint8_t Read8(uint16_t port) override;

    void Write32(uint16_t port, uint32_t data) override;
    void Write8(uint16_t port, uint8_t data) override;
};