#ifndef KERNEL_PORT_H
#define KERNEL_PORT_H

#include <stdint.h>

void outb(uint16_t port, uint8_t data);
uint8_t inb(uint16_t port);

void outw(uint16_t port, uint16_t data);
uint16_t inw(uint16_t port);

void outl(uint16_t port, uint32_t data);
uint32_t inl(uint16_t port);

#endif