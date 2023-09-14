#include "x86.h"
#include "rom.h"
#include "ram.h"
#include "pci/PCI.h"
#include "pci/DramControllerQ35.h"
#include "pci/LPCBridgeQ35.h"
#include "pci/DramControllerTigerLake.h"
#include "pci/LPCBridgeTigerLake.h"
#include "pci/P2SBTigerLake.h"
#include "pci/SPITigerLake.h"
#include "pci/TigerLakeSMBUS.h"
#include "pci/VmwareSvga.h"
#include "pci/RealtekEthernet.h"
#include "Interrupts/LegacyPIC.h"
#include "cmos.h"
#include "PS2Kbd.h"
#include "SerialPort.h"
#include <csignal>
#include <cstring>

TigerLake* cores[8];
RAM* ram;

void sighandler(int)
{
    exit(1);
}

void atexit_func()
{
    for (int i = 0; i < 1; i++)
    {
        printf("Core %d:\n", i);
        cores[i]->Dump();
    }

    ram->Dump();

    Bus::Dump();
}

enum Machine
{
    Q35 = 0,
    Tiger = 1,
};

int main(int argc, char** argv)
{
    Machine machineType = Machine::Q35;

    if (argc < 2)
    {
        printf("Usage: %s <bios>\n", argv[0]);
        return 0;
    }

    if (argc > 2)
    {
        for (int i = 2; i < argc; i++)
        {
            if (!strncmp(argv[i], "-mTigerLake", 11))
            {
                machineType = Machine::Tiger;
                continue;
            }
            else
            {
                printf("Unknown option '%s'\n", argv[i]);
                return -1;
            }
        }
    }


    ROM* rom = new ROM(argv[1]);
    ROM* lowRom = new ROM(argv[1], false);

    // This needs to be registered last to avoid hogging other device's address space
    ram = new RAM(1024*1024*128);

    // PCI bus, to which most important devices are connected
    PCIBus* pci = new PCIBus();

    if (machineType == Q35)
    {
        // ICH9 DRAM controller
        DramController* q35 = new DramController(pci);
        // ICH9 Low pin count bridge
        LPCBridge* lpc = new LPCBridge(pci);
    }
    else
    {
        DramControllerTigerLake* tigerLake = new DramControllerTigerLake(pci);
        LPCBridgeTigerLake* lpc = new LPCBridgeTigerLake(pci);
        SPITigerLake* spi = new SPITigerLake(pci);
        P2SBTigerLake* p2sb = new P2SBTigerLake(pci);
        TigerLakeSMBUS* smbus = new TigerLakeSMBUS(pci);
    }

    VmwareSvga* svga = new VmwareSvga(pci);
    RealtekInternet* net = new RealtekInternet(pci);

    // 8259 Programmable Interrupt Controller
    LegacyPIC* pic = new LegacyPIC();

    // CMOS RTC (generic)
    CMOS* cmos = new CMOS();

    // IBM PS/2 Keyboard controller (also has access to A20 and RESET for some reason)
    PS2Kbd* kbd = new PS2Kbd();

    // UART Serial port
    SerialPort* com0 = new SerialPort(0);
    SerialPort* com1 = new SerialPort(1);
    SerialPort* com2 = new SerialPort(2);
    SerialPort* com3 = new SerialPort(3);

    // An Ivy Bridge processor with 4 cores
    // All but core #0 are halted on startup, core #0 is the BSP
    for (int i = 0; i < 8; i++)
    {
        cores[i] = new TigerLake();
        cores[i]->Reset();
    }

    IOBus::InitDebugcon();

    std::atexit(atexit_func);
    std::signal(SIGINT, sighandler);

    while (1)
    {
        for (int i = 0; i < 8; i++)
            cores[i]->Clock();
    }

    return 0;
}