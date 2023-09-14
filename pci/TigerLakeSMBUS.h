#pragma once

#include "PCI.h"

class TigerLakeSMBUS : public PCIDevice
{
private:
    uint16_t pcicmd;  // PCI Command   | offset: 0x04-0x06
    uint32_t smbbase; // SMBUS base    | offset: 0x20-0x24
    uint32_t tcobase; // TCO control   | offset: 0x50-0x54
    uint32_t tcoctl;  // TCO control   | offset: 0x54-0x58
public:
    TigerLakeSMBUS(PCIBus* pci);

    void WriteConfigSpace8(uint16_t offs, uint32_t data) override;
    void WriteConfigSpace16(uint16_t offs, uint32_t data) override;
    void WriteConfigSpace32(uint16_t offs, uint32_t data) override;

    uint8_t ReadConfigSpace8(uint16_t offs) override;
    uint16_t ReadConfigSpace16(uint16_t offs) override;
    uint32_t ReadConfigSpace32(uint16_t offs) override;
};