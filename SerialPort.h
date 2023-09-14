#pragma once

#include <IOBus.h>
#include <fstream>
#include <cassert>

class SerialPort : public IODevice
{
private:
    std::ofstream log;
    int base;
public:
    SerialPort(int com_number);

    virtual uint32_t In32(uint16_t port) {assert(0);}
    virtual uint16_t In16(uint16_t port) {assert(0);}
    uint8_t In8(uint16_t port) override;

    virtual void Out32(uint16_t port, uint32_t data) {assert(0);}
    void Out8(uint16_t port, uint8_t data) override;
};