# TomatOS

This is just a small OS I am working on. Nothing much.

# Features

- working memory management (`malloc`, `free`, `realloc` and `calloc`)
- terminal, with the ability to implement custom backends for it (the native one just uses the 0xb8000 video address)
- window, it implements the terminal so it can be used as a backend but you can choose it's width, height and x,y on the screen.
- simple libc implementation that contains:
    * string.h
    * stdio.h (only `printf` right now)
    * stdint.h
    * stddef.h (`NULL` macro)
    * stdbool.h
    * stdarg.h
    * setjmp.h
- event system, allowing to queue and poll events like input
- timer API (interrupt based), used with the event system
- keyboard API (interrupt based), used with the event system
- coroutine API
- parallel API, allowing to run multiple coroutines until either one, or all of them finish.

# Building

just run the `build.sh` it will take care of the rest

# Running

I use `qemu-system-i386`
