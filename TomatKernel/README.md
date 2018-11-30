# TomatKernel
This is the kernel of the OS, it will handle the initialization and boot of the operating system as well manage the drivers and more...

# To do
- [X] Initialize a full GDT (Kernel and User segments)
- [X] Initialize IDT (remap IRQs and prepare the syscall interrupt)
- [ ] Initialize paging (identity map the parts we need)
- [ ] Enter long mode
- [ ] A full storage device library
    - [ ] ATA PIO mode
    - [ ] ATA DMA mode
    - [ ] Provider system, allowing for users to create aditional drivers to support more storage devices
- [ ] A full filesystem
    - [ ] SFS - Simple File System, the main file system for the OS
    - [ ] FAT32 (?) - Maybe support it, cause why not
    - [ ] EXT2 (??) - Maybe support it
- [ ] A graphics library
    - [ ] Text mode, will be simple, until we can get a proper bootloader to setup the display
    - [ ] Graphics mode, will be more proper and modern
- [ ] Executable loading, for now only ELF support
- [ ] Processes, allows for having services and user-space apps run at the same time
- [ ] Initialize the TomatSleep service (a service which runs when there is nothing else to run)
- [ ] Load and schedule all the different processes which are enabled
- [ ] Load and schedule TomatOS (The desktop enviroment basically)
- [ ] Start the scheduling