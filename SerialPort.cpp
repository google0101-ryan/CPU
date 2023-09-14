#include "SerialPort.h"

uint16_t ComBase[] = {0x3F8, 0x2F8, 0x3E8, 0x2E8};

SerialPort::SerialPort(int com_number)
: IODevice("COM" + std::to_string(com_number) + " serial port")
{
    base = ComBase[com_number];
    IOBus::RegisterDevice(this, ComBase[com_number], ComBase[com_number]+7);
    log.open("com" + std::to_string(com_number) + ".log");
}

uint8_t SerialPort::In8(uint16_t port)
{
    if (port == base+5)
        return 0x60; // Always ready to receive new data
    return 0;
}

void SerialPort::Out8(uint16_t port, uint8_t data)
{
    if (port == base)
        log << (char)data;
}
