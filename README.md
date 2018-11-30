# TomatOS

# Built-ins
## System Programs
- [ ] Setup program (for configuration)
- [ ] File Manager
- [ ] Settings

## Built-in Services
- [ ] TomatAlive (The core kernel service, handles everything from Timers to Scheduling) - can not be turned off
- [ ] Internet (basically handles everything to do with IP/TCP/UDP, will also handle incoming ARP and ICMP packets, this service must be on for internet related syscalls to work, also note this is not a driver by rather a handler)
- [ ] AsyncATA (A service that listens on interrupts of ATA for making so other processes will run while a disk read is pending)

## User programs (Removeable)
- [ ] Simple Text Editor
- [ ] Simple Image Editor
- [ ] Rich Text Editor
- [ ] Advanced Image Editor

