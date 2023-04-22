#pragma once

#include <IOBus.h>
#include <cassert>

struct PIC
{
    uint8_t imr;
};

class LegacyPIC : public IODevice
{
private:
    PIC slave, master;
public:
    LegacyPIC();

    uint32_t Read32(uint16_t port) {assert(0);}
    uint16_t Read16(uint16_t port) {assert(0);}
    uint8_t Read8(uint16_t port);

    virtual void Write32(uint16_t port, uint32_t data) {assert(0);}
    virtual void Write8(uint16_t port, uint8_t data) = 0;
};