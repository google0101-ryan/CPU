#pragma once

#include <cstdint>

class LocalAPIC
{
private:
    uint32_t spurious_vec;
    uint32_t lvt_timer;

    uint32_t divide;
    uint32_t initial_count;
    int32_t current_count;

    uint32_t cmc1;

    int ticker;

    enum Mode : uint32_t
    {
        None = 0,
        OneShot,
        Periodic,
        TscDeadline
    } mode = Mode::None;
public:
    LocalAPIC();

    void TickTimer();

    void Reset();

    void WriteSpuriousVec(uint32_t val) {spurious_vec = val;}
    uint32_t ReadSpuriousVec() {return spurious_vec;}

    void WriteTimerLVT(uint32_t data);
    uint32_t ReadTimerLVT() {return lvt_timer;}

    void WriteInitialCount(uint32_t data);
};