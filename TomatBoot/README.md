# TomatBoot
This will eventually be the bootloader of the Operating System.

# To do
## TomatBootSector (Stage 1)
- [ ] Enable A20 line
- [ ] Setup boot GDT
- [ ] Initialize VBE (idealy 720p with 24/32bit color)
- [ ] Load TomatBoot
- [ ] Enable protected mode
- [ ] Jump to TomatBoot

# TomatBoot (Stage 2)
- [ ] Minimal ATA driver
- [ ] Minimal SFS implementation
- [ ] Load the TomatKernel ELF file
- [ ] Start the kernel