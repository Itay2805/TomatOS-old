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
0x00 | Start Alive | Only called once from the kernel to start the alive process, which manages all other processes | | | | | |

### Terminal Syscalls

ID | Name | Description | Returns | Param 1 | Param 2 | Param 3 | Param 4 | Param 5
---|------|-------------|---------|---------|---------|---------|---------|---------
0x01 | Write | Writes to the screen | | `char*` text | | | |
0x02 | Clear | Clears the screen | | | | | |
0x03 | Set Text Color | Sets the text color to use when writing | | `uint8_t` color | | | |
0x04 | Set Background Color | Sets the background to use when writing | | `uint8_t` color | | | |
0x05 | Get Text Color | Gets the current text color | `uint8_t` color | | | | |
0x06 | Get Background Color | Gets the current background color | `uint8_t` color | | | | |
0x07 | Set Cursor Position | Sets the position of the writing cursor | | `uint16_t` x | `uint16_t` y | | | 
0x08 | Get Cursor X | Get the cursor X position | `uint16_t` x | | | | |
0x09 | Get Cursor Y | Get the cursor y position | `uint16_t` y | | | | |
0x0a | Scroll | Scrolls down the terminal | | `uint16_t` how much | | | |
0x0b | Get Width | Gets the terminal width | `uint16_t` width | | | | |
0x0c | Get Height | Gets the terminal height | `uint16_t` height | | | | |
0x0d | Clear Line | clear a line in the terminal | | `uint16_t` line | | | |
0x0e | Redirect | Redirect the terminal to the given window | `window_t` previous window | `window_t` window | | | |
0x0f | Current | Returns the current window | `window_t` current window | | | | |
0x10 | Native | Returns the native window | `window_t` window | | | | |

### Window Syscalls
ID | Name | Description | Returns | Param 1 | Param 2 | Param 3 | Param 4 | Param 5
---|------|-------------|---------|---------|---------|---------|---------|---------
0x11 | Create Window | Creates a new window | `window_t` window | `window_t` parent  | `uint16_t` x | `uint16_t` y | `uint16_t` width | `uint16_t` height
0x12 | Delete Window | Deleted the window (must be called since the kernel will not handle it by default)
0x13 | Write | Writes to the window | | `window_t` window | `char*` text | | |
0x14 | Clear | Clears the window | | `window_t` window | | | |
0x15 | Set Text Color | Sets the text color to use when writing | | `window_t` window | `uint8_t` color | | |
0x16 | Set Background Color | Sets the background to use when writing | | `window_t` window | `uint8_t` color | | |
0x17 | Get Text Color | Gets the current text color | `uint8_t` color | `window_t` window | | | |
0x18 | Get Background Color | Gets the current background color | `uint8_t` color | `window_t` window | | | |
0x19 | Set Cursor Position | Sets the position of the writing cursor | | `window_t` window | `uint16_t` x | `uint16_t` y | |
0x1a | Get Cursor X | Get the cursor X position | `uint16_t` x | `window_t` window | | | |
0x1b | Get Cursor Y | Get the cursor y position | `uint16_t` y | `window_t` window | | | |
0x1c | Scroll | Scrolls down the window | | `window_t` window | `uint16_t` how much | | |
0x1d | Get Width | Gets the window width | `uint16_t` width | `window_t` window | | | |
0x1e | Get Height | Gets the window height | `uint16_t` height | `window_t` window | | | |
0x1f | Clear Line | clear a line in the window | | `window_t` window | `uint16_t` line | | |
0x20 | Set Visible | set the window to be visible | | `window_t` window | `bool` visible | | |
0x21 | Redraw | Redraw the window on to the parent | | `window_t` window | | | |
0x22 | Get X | Returns the x position on the parent | `uint16_t` x | `window_t` window | | | |
0x23 | Get Y | Returns the y position on the parent | `uint16_t` y | `window_t` window | | | |
0x24 | Restore Cursor | Restore cursor position to the window cursor pos | | `window_t` window | | | |
0x25 | Reposition | Reposition the window | | `window_t` window | `uint16_t` x | `uint16_t` y | `uint16_t` width | `uint16_t` height

### Heap Syscalls
ID | Name | Description | Returns | Param 1 | Param 2 | Param 3 | Param 4 | Param 5
---|------|-------------|---------|---------|---------|---------|---------|---------
0x26 | Allocate | Allocated data on the heap | `uintptr_t` pointer | `size_t` size | | | | 
0x27 | Reallocate | Reallocate a pointer to the given size | `uintptr_t` new pointer | `uintptr_t` current pointer | `size_t` new size | | |
0x28 | Free | Free an allocated pointer (assuming it is on the heap) | | `uintptr_t` pointer | | | |
0x29 | Get Pointer Size | Get the size of the given pointer on the heap (assuming it is on the heap) | `size_t` size | `uintptr_t` ptr | | | |
0x2a | Get Heap size | Gets the amoung of space used in the heap | `size_t` size | | | | |

### OS Syscalls
These syscalls might change as they are not fully implemented yet

ID | Name | Description | Returns | Param 1 | Param 2 | Param 3 | Param 4 | Param 5
---|------|-------------|---------|---------|---------|---------|---------|---------
0x30 | Pull Event | Will get the top most event in the event queue, if none will wait for one, note that the returned pointer is always constant for the process, so you should copy the data in it before calling pull event again | `event_t*` event | | | | | |
0x31 | Queue Event | Will queue an event for a process, it must have higher privilage level than the process or, if the uid is 0, it will queue the message to the current process | | `event_t*` event | `uint32_t` uid | | |
0x32 | Start Timer | Will start a timer, the timer will fire a TIMER event once finished | `uint32_t` tid | `uint32_t` millis | | | |
0x33 | Cancel Timer | Cancel a timer | | `uint32_t` tid | | | |
