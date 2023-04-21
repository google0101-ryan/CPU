#include "x86.h"
#include "rom.h"
#include "ram.h"

IvyBridge* cores[4];
RAM* ram;

void atexit_func()
{
    for (int i = 0; i < 1; i++)
    {
        printf("Core %d:\n", i);
        cores[i]->Dump();
    }

    ram->Dump();
}

int main()
{
    ROM* rom = new ROM("OVMF.fd");

    // This needs to be registered last to avoid hogging other device's address space
    ram = new RAM(1024*1024*15);

    // An Ivy Bridge processor with 4 cores
    // All but core #0 are halted on startup, core #0 is the BSP
    for (int i = 0; i < 4; i++)
    {
        cores[i] = new IvyBridge();
        cores[i]->Reset();
    }

    std::atexit(atexit_func);

    while (1)
    {
        for (int i = 0; i < 4; i++)
            cores[i]->Clock();
    }

    return 0;
}