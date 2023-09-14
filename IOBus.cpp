#include "IOBus.h"
#include <fstream>

IODevice* io_devices[0xFFFF];
std::ofstream log;

void IOBus::InitDebugcon()
{
    log = std::ofstream("debug.log");
}

void IOBus::RegisterDevice(IODevice *dev, uint16_t start, uint16_t end)
{
    printf("Registered IO device \"%s\" at 0x%04x to 0x%04x\n", dev->GetName().c_str(), start, end);
    for (auto i = start; i < end+1; i++)
        io_devices[i] = dev;
}

uint32_t IOBus::In32(uint16_t port)
{
    if (!io_devices[port])
    {
        printf("ERROR: Read32 from unimplemented port 0x%04x\n", port);
        exit(1);
    }

    return io_devices[port]->In32(port);
}

uint16_t IOBus::In16(uint16_t port)
{
    if (!io_devices[port])
    {
        printf("ERROR: Read16 from unimplemented port 0x%04x\n", port);
        exit(1);
    }

    return io_devices[port]->In16(port);
}

uint8_t IOBus::In8(uint16_t port)
{
    if (port == 0x402)
        return 0xE9;
    if (port == 0x511)
        return 0;

    if (!io_devices[port])
    {
        printf("ERROR: Read8 from unimplemented port 0x%04x\n", port);
        exit(1);
    }

    return io_devices[port]->In8(port);
}

void IOBus::Out32(uint16_t port, uint32_t data)
{
    if (!io_devices[port])
    {
        printf("ERROR: Write32 to unimplemented port 0x%04x\n", port);
        exit(1);
    }

    io_devices[port]->Out32(port, data);
}

void IOBus::Out16(uint16_t port, uint16_t data)
{
    if (port == 0x510)
        return;

    if (!io_devices[port])
    {
        printf("ERROR: Write16 to unimplemented port 0x%04x\n", port);
        exit(1);
    }

    io_devices[port]->Out16(port, data);
}

void IOBus::Out8(uint16_t port, uint8_t data)
{
    if (port == 0x80)
        return; // POST code port

    if (port == 0x402)
    {
        log << (char)data;
        log.flush();
        return;
    }

    if (!io_devices[port])
    {
        printf("ERROR: Write8 to unimplemented port 0x%04x\n", port);
        exit(1);
    }

    io_devices[port]->Out8(port, data);
}
