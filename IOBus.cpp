#include "IOBus.h"

IODevice* io_devices[0xFFFF];

void IOBus::RegisterDevice(IODevice *dev, uint16_t start, uint16_t end)
{
    printf("Registered IO device \"%s\" at 0x%04x to 0x%04x\n", dev->GetName().c_str(), start, end);
    for (auto i = start; i < end; i++)
        io_devices[i] = dev;
}

uint32_t IOBus::In32(uint16_t port)
{
    if (!io_devices[port])
    {
        printf("ERROR: Access to unimplemented port 0x%04x\n", port);
        exit(1);
    }

    return io_devices[port]->Read32(port);
}

uint16_t IOBus::In16(uint16_t port)
{
    if (!io_devices[port])
    {
        printf("ERROR: Access to unimplemented port 0x%04x\n", port);
        exit(1);
    }

    return io_devices[port]->Read16(port);
}

void IOBus::Out32(uint16_t port, uint32_t data)
{
    if (!io_devices[port])
    {
        printf("ERROR: Access to unimplemented port 0x%04x\n", port);
        exit(1);
    }

    io_devices[port]->Write32(port, data);
}
