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
    bool in_initialization = false;
public:
    LegacyPIC();

    uint32_t Read32(uint16_t port) {assert(0);}
    uint16_t Read16(uint16_t port) {assert(0);}
    uint8_t Read8(uint16_t port);

    void Write32(uint16_t port, uint32_t data) {assert(0);}
    void Write8(uint16_t port, uint8_t data);
};