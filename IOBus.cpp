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

uint8_t IOBus::In8(uint16_t port)
{
    if (port == 0x402)
        return 0xE9;

    if (!io_devices[port])
    {
        printf("ERROR: Access to unimplemented port 0x%04x\n", port);
        exit(1);
    }

    return io_devices[port]->Read8(port);
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

void IOBus::Out8(uint16_t port, uint8_t data)
{
    if (port == 0x402)
    {
        printf("[DebugCon]: 0x%02X\n", data);
        log << (char)data;
        log.flush();
        return;
    }

    if (!io_devices[port])
    {
        printf("ERROR: Access to unimplemented port 0x%04x\n", port);
        exit(1);
    }

    io_devices[port]->Write8(port, data);
}
