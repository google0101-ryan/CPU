#pragma once

#include <IOBus.h>
#include <cassert>

class CMOS : public IODevice
{
private:
    uint8_t reset_reason = 0;
    uint8_t selected_port = 0x00;

    bool nmi_disabled = true;
public:
    CMOS();

    virtual uint32_t In32(uint16_t port) {assert(0);}
    virtual uint16_t In16(uint16_t port) {assert(0);}
    uint8_t In8(uint16_t port) override;

    virtual void Out32(uint16_t port, uint32_t data) {assert(0);}
    void Out8(uint16_t port, uint8_t data) override;
};