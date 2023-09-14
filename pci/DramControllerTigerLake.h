#include "PCI.h"

class DramControllerTigerLake : public PCIDevice
{
private:
    uint64_t pciexbar; // The memory location of the PCIe enhanced cfg space
    uint64_t mchbar; // The memory location of the Memory Controller Hub registers
    PCIBus* pci;
public:
    DramControllerTigerLake(PCIBus* bus);

    void WriteConfigSpace8(uint16_t offs, uint32_t data) override;
    void WriteConfigSpace16(uint16_t offs, uint32_t data) override;
    void WriteConfigSpace32(uint16_t offs, uint32_t data) override;

    uint8_t ReadConfigSpace8(uint16_t offs) override;
    uint16_t ReadConfigSpace16(uint16_t offs) override;
    uint32_t ReadConfigSpace32(uint16_t offs) override;
};