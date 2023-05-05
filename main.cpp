#include "x86.h"
#include "rom.h"
#include "ram.h"
#include "pci/PCI.h"
#include "pci/DramControllerTigerLake.h"
#include "pci/LPCBridgeTigerLake.h" 
#include "Interrupts/LegacyPIC.h"
#include "cmos.h"

TigerLake* cores[8];
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
    ram = new RAM(1024*1024*128);

    // PCI bus, to which most important devices are connected
    PCIBus* pci = new PCIBus();

    // ICH9 DRAM controller
    DramController* q35 = new DramController(pci);
    // ICH9 Low pin count bridge
    LPCBridge* lpc = new LPCBridge(pci);

    // 8259 Programmable Interrupt Controller
    LegacyPIC* pic = new LegacyPIC();

    // CMOS RTC (generic)
    CMOS* cmos = new CMOS();

    // An Ivy Bridge processor with 4 cores
    // All but core #0 are halted on startup, core #0 is the BSP
    for (int i = 0; i < 8; i++)
    {
        cores[i] = new TigerLake();
        cores[i]->Reset();
    }

    IOBus::InitDebugcon();

    std::atexit(atexit_func);

    while (1)
    {
        for (int i = 0; i < 8; i++)
            cores[i]->Clock();
    }

    return 0;
}