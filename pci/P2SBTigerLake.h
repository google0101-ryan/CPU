#pragma once

#include "PCI.h"


class P2SBTigerLake : public PCIDevice
{
private:
    uint64_t hptc_bar; // Base address of the HPET timer
    uint32_t p2sb_bar; // Base address of the P2SB registers
    uint8_t pcicmd; // All but the bottom two bits are hardwired to 0
    PCIBus* pci;
public:
    P2SBTigerLake(PCIBus* bus);

    void WriteConfigSpace8(uint16_t offs, uint32_t data) override;
    void WriteConfigSpace16(uint16_t offs, uint32_t data) override;
    void WriteConfigSpace32(uint16_t offs, uint32_t data) override;

    uint8_t ReadConfigSpace8(uint16_t offs) override;
    uint16_t ReadConfigSpace16(uint16_t offs) override;
    uint32_t ReadConfigSpace32(uint16_t offs) override;
};