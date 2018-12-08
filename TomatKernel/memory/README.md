# Tomato Kernel Memory Mapping
## Physical Memory
TODO

## Virtual Memory
Kernel is located at 1MB
The kernel HEAP is located at 5MB and is until 10MB
The application heap is located at 10MB, growing up to 4GB
Note that when the heap is loaded, it will also be used to allocate the space for the stack and for the executeable
