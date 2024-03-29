#pragma once

#include "pci/PCI.h"

// Low pin count bridge, device 00:1F.0
class LPCBridgeTigerLake : public PCIDevice
{
private:
    uint32_t pmbase = 1;   // ACPI base address      |  offset: 0x40-0x43
    uint8_t acpi_cntl = 0; // ACPI control register  |  offset: 0x44
public:
    LPCBridgeTigerLake(PCIBus* pci);

    void WriteConfigSpace8(uint16_t offs, uint32_t data) override;
    void WriteConfigSpace16(uint16_t offs, uint32_t data) override;
    void WriteConfigSpace32(uint16_t offs, uint32_t data) override;

    uint8_t ReadConfigSpace8(uint16_t offs) override;
    uint16_t ReadConfigSpace16(uint16_t offs) override;
    uint32_t ReadConfigSpace32(uint16_t offs) override;
};