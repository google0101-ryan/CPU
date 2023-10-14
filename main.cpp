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

#define TEST

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

#ifdef TEST

class TestMemory : public MemoryDevice
{
public:
private:
    uint8_t* data;

    uint64_t start, end;
public:
    TestMemory(uint8_t* data, size_t size, bool high = true)
    : MemoryDevice("Test Memory (Used for unit tests)"),
    data(data)
    {
        start = 0xFFFF0000;
        end = start + size;

        Bus::RegisterDevice(this);
    }
    
    virtual uint64_t GetStart() const {return start;}
    virtual uint64_t GetEnd() const {return end;}

    virtual uint8_t  Read8(uint64_t addr) {return data[addr - start];}
    virtual uint16_t Read16(uint64_t addr) {return *(uint16_t*)&data[addr - start];}
    virtual uint32_t Read32(uint64_t addr) {return *(uint32_t*)&data[addr - start];}
    virtual uint64_t Read64(uint64_t addr) {return *(uint64_t*)&data[addr - start];}

    virtual void Write8(uint64_t addr, uint8_t data) {this->data[addr - start] = data;}
    virtual void Write16(uint64_t addr, uint16_t data) {*(uint16_t*)(&this->data[addr - start]) = data;}
    virtual void Write32(uint64_t addr, uint32_t data) {*(uint32_t*)(&this->data[addr - start]) = data;}
    virtual void Write64(uint64_t addr, uint64_t data) {*(uint64_t*)(&this->data[addr - start]) = data;}
};

int main()
{
    TigerLake cpu;
    cpu.Reset();
    cpu.DoTestSetup();

    uint8_t data[0xFFFF];
    data[0] = 0xB8;
    data[1] = 0xFE;
    data[2] = 0x00;
    data[3] = 0x00;
    data[4] = 0x00;
    data[5] = 0x83;
    data[6] = 0xF8;
    data[7] = 0xFF;

    TestMemory test(data, 0xFFFF);

    cpu.Clock();
    cpu.Clock();

    assert(cpu.GetReg(RAX) == 0xFE);
    assert(cpu.GetFlags().flag_bits.cf == 1);
    assert(cpu.GetFlags().flag_bits.zf == 0);
    assert(cpu.GetFlags().flag_bits.sf == 1);
}

#else

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

#endif