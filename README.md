# TomatOS

Rewrite number idk

Already got working:
* GDT and IDT (will have to visit it again to properly set it)
* Paging
* Heap
* libc
* kernel terminal library
* some stuff that are needed for C++ to work
* ATA with custom File System

The goal is to get two processes running in context switching, one for background and one for foreground.

the difference is that Foregroud process will receive events from the user (like keyboard) and that background process won't be able to access the terminal

The background process will manage tasks using co-op multitasking

The foreground process will most likely only have one process which would either be a GUI or a Shell

I also want to have a user and permission system and have some kind of package manager