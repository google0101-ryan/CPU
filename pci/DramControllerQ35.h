#include "PCI.h"

class DramController : public PCIDevice
{
private:
    uint8_t pam[7];
public:
    DramController(PCIBus* bus);

    void WriteConfigSpace8(uint16_t offs, uint32_t data) override;
    void WriteConfigSpace16(uint16_t offs, uint32_t data) override;
    void WriteConfigSpace32(uint16_t offs, uint32_t data) override;

    uint8_t ReadConfigSpace8(uint16_t offs) override;
    uint16_t ReadConfigSpace16(uint16_t offs) override;
    uint32_t ReadConfigSpace32(uint16_t offs) override;
};