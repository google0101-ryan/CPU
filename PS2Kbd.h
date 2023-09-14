#pragma once

#include <IOBus.h>
#include <cassert>

class PS2Kbd : public IODevice
{
private:
    uint8_t control_a;
public:
    PS2Kbd();

    virtual uint32_t In32(uint16_t port) {assert(0);}
    virtual uint16_t In16(uint16_t port) {assert(0);}
    uint8_t In8(uint16_t port) override;

    virtual void Out32(uint16_t port, uint32_t data) {assert(0);}
    void Out8(uint16_t port, uint8_t data) override;
};