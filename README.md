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
* [Syscalls](syscalls.md)
* Processes

Working on currently:
* FileSystem - I have some plan on how it will be but still needs implementing

The current goals to get some basic shell process working, Then I would like to start working on some exe format and allow to start binary files as processes, and then have a compiler that will generate and link an assembly that will be able to run, and once that is finished maybe get the OS to self host (long long future)

# Building and Running
For building all you need to make sure is that you have an environment with gcc, g++ and nasm.

I just use WSL with Ubuntu.

Once you have everything installed you can just open it VSCode and run the `Debug` configuration and it should handle everything else.