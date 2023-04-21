#include "PCI.h"

class DramController : public PCIDevice
{
public:
    DramController(PCIBus* bus);

    uint16_t ReadConfigSpace16(uint8_t offs) override;
};