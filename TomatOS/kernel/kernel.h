#ifndef KERNEL_H
#define KERNEL_H

#define LOW16(address) (uint16_t)((address) & 0xFFFF)
#define HIGH16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

#endif