#pragma once

#include <IOBus.h>
#include <cassert>

class CMOS : public IODevice
{
private:
    uint8_t reset_reason = 0;
    uint8_t selected_port = 0x00;
public:
    CMOS();

    virtual uint32_t Read32(uint16_t port) {assert(0);}
    virtual uint16_t Read16(uint16_t port) {assert(0);}
    uint8_t Read8(uint16_t port) override;

    virtual void Write32(uint16_t port, uint32_t data) {assert(0);}
    void Write8(uint16_t port, uint8_t data) override;
};