#pragma once

#include <cstdint>
#include <unordered_map>
#include "IOBus.h"
#include "Bus.h"

class PCIDevice
{
public:
    virtual void WriteConfigSpace8(uint16_t offs, uint32_t data) = 0;
    virtual void WriteConfigSpace16(uint16_t offs, uint32_t data) = 0;
    virtual void WriteConfigSpace32(uint16_t offs, uint32_t data) = 0;

    virtual uint8_t ReadConfigSpace8(uint16_t offs) = 0;
    virtual uint16_t ReadConfigSpace16(uint16_t offs) = 0;
    virtual uint32_t ReadConfigSpace32(uint16_t offs) = 0;
};

class PCIBus : public IODevice, public MemoryDevice
{
private:
    uint32_t pciAddr; // 0xCF8
    uint32_t pcie_base;

    size_t index;

    std::unordered_map<uint32_t, PCIDevice*> devices;
public:
    PCIBus();

    void RegisterDevice(int bus, int dev, int func, PCIDevice* device);

    uint64_t GetStart() const override {return pcie_base;}
    uint64_t GetEnd() const override {return pcie_base+(256*1024*1024);}

    void MapPcie(uint64_t addr);

    uint32_t In32(uint16_t port) override;
    uint16_t In16(uint16_t port) override;
    uint8_t In8(uint16_t port) override;

    void Out32(uint16_t port, uint32_t data) override;
    void Out16(uint16_t port, uint32_t data) override;
    void Out8(uint16_t port, uint8_t data) override;

    virtual uint8_t Read8(uint64_t addr)  override;
    virtual uint16_t Read16(uint64_t addr)  override;
    virtual uint32_t Read32(uint64_t addr)  override;
    virtual uint64_t Read64(uint64_t addr)  {assert(0);}

    virtual void Write8(uint64_t addr, uint8_t data)  override;
    virtual void Write16(uint64_t addr, uint16_t data)  override;
    virtual void Write32(uint64_t addr, uint32_t data)  override;
    virtual void Write64(uint64_t addr, uint64_t data)  {assert(0);}
};