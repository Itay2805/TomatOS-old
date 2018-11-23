# Process Memory Mapping

All the pages are in Virtual Addressing

## Page 0
Always set to taken, this is just because otherwise it might cause a fault if the page is allocated

## Pages 1-255
Unused (so the GDT will be simpler)
 
## Pages 256-kernel_end
Kernel Pages, the user is not allowed to write or excute code in them

## 11520 Pages
Kernel heap, the user is not allowed to write or excute code in it

## 1280 Pages
Unused, to use as buffer between kernel heap and program

## program_size Pages
Where the program will be loaded to

## 256 Pages
Unused, to use as buffer between program and stack

## 5120 Pages
Process stack (20MB)

## 256 Pages
Unused, to use as buffer between stack and heap

## All of the left pages
Will be used as the heap of the process