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

    uint32_t In32(uint16_t port) {assert(0);}
    uint16_t In16(uint16_t port) {assert(0);}
    uint8_t In8(uint16_t port);

    void Out32(uint16_t port, uint32_t data) {assert(0);}
    void Out8(uint16_t port, uint8_t data);
};