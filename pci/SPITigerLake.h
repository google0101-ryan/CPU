#pragma once

#include "pci/PCI.h"

// Low pin count bridge, device 00:1F.0
class SPITigerLake : public PCIDevice
{
private:
    uint32_t bios_spi_ct = 2<<2; // BIOS SPI Control    | offset 0xDC, 32-bits
public:
    SPITigerLake(PCIBus* pci);

    void WriteConfigSpace8(uint16_t offs, uint32_t data) override;
    void WriteConfigSpace16(uint16_t offs, uint32_t data) override;
    void WriteConfigSpace32(uint16_t offs, uint32_t data) override;

    uint8_t ReadConfigSpace8(uint16_t offs) override;
    uint16_t ReadConfigSpace16(uint16_t offs) override;
    uint32_t ReadConfigSpace32(uint16_t offs) override;
};