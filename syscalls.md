# TomatOS Syscalls

## Calling syscalls
To call the syscall you call the interrupt `128` (`0x80`)
The syscall id goes into the `EAX` register

Each Parameter goes to the corresponding register:
  1. `EBX`
  2. `ECX`
  3. `EDX`
  4. `ESI`
  5. `EDI`
  
The `EAX` register is used for the return value

For example to call write:
```asm
mov eax, 0x1
mov ebx, STRING_TO_PRINT
int 0x80
```

In the future we might also use the stack but it might complicate some stuff...

## Syscall types
```cpp
#include <stdint.h>
#include <stdbool.h>

typedef uint32_t window_t;

typedef struct event {
  uint32_t kind;
  uint32_t data[4];
} event_t;
```

TODO: Add more explanation about the event system

## Syscall List 

### General syscalls

ID | Name | Description | Returns | Param 1 | Param 2 | Param 3 | Param 4 | Param 5
---|------|-------------|---------|---------|---------|---------|---------|---------
0 | Start Alive | Only called once from the kernel to start the alive process, which manages all other processes | | | | | |

### Terminal Syscalls

ID | Name | Description | Returns | Param 1 | Param 2 | Param 3 | Param 4 | Param 5
---|------|-------------|---------|---------|---------|---------|---------|---------
1 | Write | Writes to the screen | | `char*` text | | | |
2 | Clear | Clears the screen | | | | | |
3 | Set Text Color | Sets the text color to use when writing | | `uint8_t` color | | | |
4 | Set Background Color | Sets the background to use when writing | | `uint8_t` color | | | |
5 | Get Text Color | Gets the current text color | `uint8_t` color | | | | |
6 | Get Background Color | Gets the current background color | `uint8_t` color | | | | |
7 | Set Cursor Position | Sets the position of the writing cursor | | `uint16_t` x | `uint16_t` y | | | 
8 | Get Cursor X | Get the cursor X position | `uint16_t` x | | | | |
9 | Get Cursor Y | Get the cursor y position | `uint16_t` y | | | | |
10 | Scroll | Scrolls down the terminal | | `uint16_t` how much | | | |
11 | Get Width | Gets the terminal width | `uint16_t` width | | | | |
12 | Get Height | Gets the terminal height | `uint16_t` height | | | | |
13 | Clear Line | clear a line in the terminal | | `uint16_t` line | | | |
14 | Redirect | Redirect the terminal to the given window | `window_t` previous window | `window_t` window | | | |
15 | Current | Returns the current window | `window_t` current window | | | | |
16 | Native | Returns the native window | `window_t` window | | | | |

### Window Syscalls
ID | Name | Description | Returns | Param 1 | Param 2 | Param 3 | Param 4 | Param 5
---|------|-------------|---------|---------|---------|---------|---------|---------
17 | Create Window | Creates a new window | `window_t` window | `window_t` parent  | `uint16_t` x | `uint16_t` y | `uint16_t` width | `uint16_t` height
18 | Delete Window | Deleted the window (must be called since the kernel will not handle it by default)
19 | Write | Writes to the window | | `window_t` window | `char*` text | | |
20 | Clear | Clears the window | | `window_t` window | | | |
21 | Set Text Color | Sets the text color to use when writing | | `window_t` window | `uint8_t` color | | |
22 | Set Background Color | Sets the background to use when writing | | `window_t` window | `uint8_t` color | | |
23 | Get Text Color | Gets the current text color | `uint8_t` color | `window_t` window | | | |
24 | Get Background Color | Gets the current background color | `uint8_t` color | `window_t` window | | | |
25 | Set Cursor Position | Sets the position of the writing cursor | | `window_t` window | `uint16_t` x | `uint16_t` y | |
26 | Get Cursor X | Get the cursor X position | `uint16_t` x | `window_t` window | | | |
27 | Get Cursor Y | Get the cursor y position | `uint16_t` y | `window_t` window | | | |
28 | Scroll | Scrolls down the window | | `window_t` window | `uint16_t` how much | | |
29 | Get Width | Gets the window width | `uint16_t` width | `window_t` window | | | |
30 | Get Height | Gets the window height | `uint16_t` height | `window_t` window | | | |
31 | Clear Line | clear a line in the window | | `window_t` window | `uint16_t` line | | |
32 | Set Visible | set the window to be visible | | `window_t` window | `bool` visible | | |
33 | Redraw | Redraw the window on to the parent | | `window_t` window | | | |
34 | Get X | Returns the x position on the parent | `uint16_t` x | `window_t` window | | | |
35 | Get Y | Returns the y position on the parent | `uint16_t` y | `window_t` window | | | |
36 | Restore Cursor | Restore cursor position to the window cursor pos | | `window_t` window | | | |
37 | Reposition | Reposition the window | | `window_t` window | `uint16_t` x | `uint16_t` y | `uint16_t` width | `uint16_t` height

### Heap Syscalls
ID | Name | Description | Returns | Param 1 | Param 2 | Param 3 | Param 4 | Param 5
---|------|-------------|---------|---------|---------|---------|---------|---------
38 | Allocate | Allocated data on the heap | `uintptr_t` pointer | `size_t` size | | | | 
39 | Reallocate | Reallocate a pointer to the given size | `uintptr_t` new pointer | `uintptr_t` current pointer | `size_t` new size | | |
40 | Free | Free an allocated pointer (assuming it is on the heap) | | `uintptr_t` pointer | | | |
41 | Get Pointer Size | Get the size of the given pointer on the heap (assuming it is on the heap) | `size_t` size | `uintptr_t` ptr | | | |
42 | Get Heap size | Gets the amoung of space used in the heap | `size_t` size | | | | |

### OS Syscalls
These syscalls might change as they are not fully implemented yet

ID | Name | Description | Returns | Param 1 | Param 2 | Param 3 | Param 4 | Param 5
---|------|-------------|---------|---------|---------|---------|---------|---------
43 | Pull Event | Will get the top most event in the event queue, if none will wait for one, note that the returned pointer is always constant for the process, so you should copy the data in it before calling pull event again | `event_t*` event | | | | | |
44 | Queue Event | Will queue an event for a process, it must have higher privilage level than the process or, if the uid is 0, it will queue the message to the current process | | `event_t*` event | `uint32_t` uid | | |
45 | Start Timer | Will start a timer, the timer will fire a TIMER event once finished | `uint32_t` tid | `uint32_t` millis | | | |
46 | Cancel Timer | Cancel a timer | | `uint32_t` tid | | | |
57 | Kill Process | Kills the process with the given uid, if the uid is 0 it will kill the current process. | | `uint32_t` uid | | | |
