cmake_minimum_required(VERSION 3.20)

project(x86)

set(SOURCES pci/PCI.cpp
            pci/LPCBridgeTigerLake.cpp
            pci/DramControllerTigerLake.cpp
            pci/P2SBTigerLake.cpp
            pci/SPITigerLake.cpp
            pci/TigerLakeSMBUS.cpp
            pci/LPCBridgeQ35.cpp
            pci/DramControllerQ35.cpp
            pci/VmwareSvga.cpp
            pci/RealtekEthernet.cpp
            rom.cpp
            ops.cpp
            IOBus.cpp
            ram.cpp
            Interrupts/LegacyPIC.cpp
            Interrupts/LocalAPIC.cpp
            main.cpp
            Cache.cpp
            Bus.cpp
            modrm.cpp
            x86.cpp
            cmos.cpp
            PS2Kbd.cpp
            SerialPort.cpp)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(x86 ${SOURCES})
