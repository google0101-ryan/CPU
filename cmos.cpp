#include "cmos.h"

CMOS::CMOS()
: IODevice("Complementary Metal-Oxide Semiconductor")
{
    IOBus::RegisterDevice(this, 0x70, 0x71);
}

uint8_t CMOS::Read8(uint16_t port)
{
    switch (port)
    {
    case 0x71:
    {
        switch (selected_port)
        {
        case 0x0f: // Power-down status register
            printf("[CMOS]: Reading from POST status register\n");
            return 0; // Soft reset or hard reboot
        default:
            printf("Read from unknown CMOS reg 0x%02x!\n", selected_port);
            exit(1);
        }
    }
    break;
    default:
        printf("Read from unknown CMOS port 0x%02x!\n", port);
        exit(1);
    }
}

void CMOS::Write8(uint16_t port, uint8_t data)
{
    switch (port)
    {
    case 0x70:
        selected_port = data;
        break;
    default:
        printf("Write to unknown CMOS port 0x%02x!\n", port);
        exit(1);
    }
}
