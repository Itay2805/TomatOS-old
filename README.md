# TomatOS

Rewrite number idk

Already got working:
* GDT and IDT
* Paging
* Heap
* libc
* kernel terminal library
* some stuff that are needed for C++ to work
* ATA driver
* Processes - Still needs more testing to check if scheduling actually works :shrug:

Working on currently:
* FileSystem - I have some plan on how it will be but still needs implementing

The current goals to get some basic shell process working, Then I would like to start working on some exe format and allow to start binary files as processes, and then have a compiler that will generate and link an assembly that will be able to run, and once that is finished maybe get the OS to self host (long long future)
