#include "LegacyPIC.h"

LegacyPIC::LegacyPIC()
: IODevice("Legacy Interrupt Controller")
{
    slave.imr = master.imr = 0;

    IOBus::RegisterDevice(this, 0x20, 0x21);
    IOBus::RegisterDevice(this, 0xA0, 0xA1);
}

uint8_t LegacyPIC::In8(uint16_t port)
{
    switch (port)
    {
    default:
        printf("[8259_PIC]: Unimplemented read from port 0x%02x\n", port);
        exit(1);
    }
}

void LegacyPIC::Out8(uint16_t port, uint8_t data)
{
    switch (port)
    {
    case 0x21:
        if (!in_initialization) master.imr = data; break;
    case 0xA1:
        if (!in_initialization) slave.imr = data; break;
    default:
        printf("[8259_PIC]: Unimplemented write to port 0x%02x\n", port);
        exit(1);
    }
}
