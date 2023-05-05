# CPU
This project is to provide a (hopefully) easy-to-understand amd64 emulator, with the long-term goal of being a tool in understanding amd64's design choices

# Short term goals
Boot some form of Linux, install DOOM, play DOOM

# Even shorter term goals
Get OVMF actually working

# Features
- [ ] Full support for base amd64 instruction set
- [x] APIC timer
- [x] RAM
- [x] amd64 paging
- [x] 32-bit mode, 64-bit mode, and switching between them
- [x] The rarely-emulated segment register caching
- [ ] amd64 SIMD extensions like MMX, SSE, SSE2, AVX, etc
- [ ] Support for the VMWare SVGA virtual device
- [ ] Support for PCIe
- [ ] Support for AHCI
- [ ] Support for NVMe
- [ ] Support for Intel Graphics (maybe? Kind of far-fetched, tbh)

# FAQ

<b>Q: Will there be a Windows port?</b>

A: no

<b>Q: Why not?</b>

A: Because Windows is difficult to develop for, and it adds a level of difficulty to an already difficult project that I do not need

<b>Q: Will it run (insert software name here)?</b>

A: No.