#include "cmos.h"

CMOS::CMOS()
: IODevice("Complementary Metal-Oxide Semiconductor")
{
    IOBus::RegisterDevice(this, 0x70, 0x71);
}

uint8_t CMOS::In8(uint16_t port)
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
        case 0x34:
            return 0x00;
        case 0x35:
            return 0x07;
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

void CMOS::Out8(uint16_t port, uint8_t data)
{
    switch (port)
    {
    case 0x70:
        selected_port = data & 0x7f;
        nmi_disabled = data & 0x80;
        break;
    case 0x71:
    {
        switch (selected_port)
        {
        case 0x0f: // Power-down status register
            printf("[CMOS]: Writing 0x%02x to POST status register\n", data);
            return; // Soft reset or hard reboot
        default:
            printf("Read from unknown CMOS reg 0x%02x!\n", selected_port);
            exit(1);
        }
        break;
    }
    default:
        printf("Write to unknown CMOS port 0x%02x!\n", port);
        exit(1);
    }
}
