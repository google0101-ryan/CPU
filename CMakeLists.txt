cmake_minimum_required(VERSION 3.20)

project(x86)

set(SOURCES pci/PCI.cpp
            pci/LPCBridgeTigerLake.cpp
            pci/DramControllerTigerLake.cpp
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
            cmos.cpp)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(x86 ${SOURCES})
