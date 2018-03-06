# TomatOS

This is just a small OS I am working on. Nothing much.

# Features

- working `malloc` `free` and `realloc`
- terminal, with the ability to implement custom backends for it (the native one just uses the 0xb8000 video address thingy)
- window, it implements the terminal so it can be used as a backend but you can choose it's width, height and x,y on the screen.
- simple libc implementation

# Building

if you really wanna build it you should have a gcc cross compiler in `/usr/local/i386elfgcc`

after that just run the `build.sh` it will take care of the rest

# Running

I use `qemu-system-i386`