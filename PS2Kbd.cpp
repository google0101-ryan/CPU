#include "PS2Kbd.h"

PS2Kbd::PS2Kbd()
: IODevice("PS/2 Keyboard")
{
    IOBus::RegisterDevice(this, 0x90, 0x9F);
}

uint8_t PS2Kbd::In8(uint16_t port)
{
    switch (port)
    {
    case 0x92:
        return control_a;
    default:
        printf("Read from unknown PS/2 port 0x%02x\n", port);
        exit(1);
    }
}

void PS2Kbd::Out8(uint16_t port, uint8_t data)
{
    switch (port)
    {
    case 0x92:
        if (data & 1)
        {
            printf("TODO: Pulse CPU reset pin\n");
            exit(1);
        }
        control_a = data;
        break;
    default:
        printf("Write to unknown PS/2 port 0x%02x\n", port);
        exit(1);
    }
}
