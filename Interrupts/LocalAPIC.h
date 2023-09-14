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

    uint32_t target_apic_id = 0;

    union
    {
        uint32_t data;
        struct
        {
            uint32_t vector : 8;
            uint32_t dest_mode : 3;
            uint32_t log_mode : 1;
            uint32_t delivery_stat : 1;
            uint32_t : 1;
            uint32_t init_clear : 1; // This bit is clear for an INIT-level deassert
            uint32_t init_set : 1; // This bit is set for an INIT-level deassert
            uint32_t : 2;
            uint32_t dest_type : 2;
            uint32_t : 12;
        };
    } icr0;
public:
    LocalAPIC();

    void TickTimer();

    void Reset();

    void WriteSpuriousVec(uint32_t val) {spurious_vec = val;}
    uint32_t ReadSpuriousVec() {return spurious_vec;}

    void WriteTimerLVT(uint32_t data);
    uint32_t ReadTimerLVT() {return lvt_timer;}

    void WriteICR0(uint32_t data); // APIC_BASE + 0x300 (THIS SENDS AN INTERRUPT!)
    uint32_t ReadICR0() {return icr0.data;}

    void WriteInitialCount(uint32_t data);
};