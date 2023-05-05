#include "LocalAPIC.h"

#include <cstdio>
#include <cstdlib>

LocalAPIC::LocalAPIC()
{
    Reset();
}

void LocalAPIC::TickTimer()
{
    if (mode == Mode::None)
        return;

    ticker++;
    if (!(ticker % divide))
        current_count--;

    if (current_count <= 0)
    {
        if (mode == Mode::Periodic)
            current_count = initial_count;
        else if (mode == Mode::OneShot)
            mode = Mode::None;
        bool lvt_mask = (lvt_timer >> 16) & 1;
        bool cmc1_mask = (cmc1 >> 16) & 1;

        if (!cmc1_mask && !lvt_mask)
        {
            printf("TODO: Timer interrupt\n");
            exit(1);
        }
    }
}

void LocalAPIC::Reset()
{
    spurious_vec = 0xff;
    lvt_timer = 0x10000;

    current_count = 0;
    initial_count = 0;
    divide = 1;

    cmc1 = 0x10000;
}

void LocalAPIC::WriteTimerLVT(uint32_t data)
{
    lvt_timer = data;

    printf("[LAPIC] 0x%08x -> LVT\n", data);

    switch ((lvt_timer >> 17) & 0x3)
    {
    case 0: mode = Mode::OneShot;
    case 1: mode = Mode::Periodic;
    case 2: mode = Mode::TscDeadline;
    }

    current_count = initial_count;
}

void LocalAPIC::WriteInitialCount(uint32_t data)
{
    printf("[LAPIC] 0x%08x -> Initial Count\n", data);
 
    initial_count = data;
    current_count = data;
}
