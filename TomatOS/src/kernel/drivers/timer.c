#include "timer.h"

#include "../syscalls/os.h"

#include "../port.h"
#include "../interrupt.h"

#include <stddef.h>

#define PIC1			0x20
#define PIC2			0xA0   
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2+1)

#define ICW1_ICW4       0x01        
#define ICW1_SINGLE     0x02        
#define ICW1_INTERVAL4  0x04        
#define ICW1_LEVEL      0x08        
#define ICW1_INIT       0x10        

#define ICW4_8086       0x01    
#define ICW4_AUTO       0x02        
#define ICW4_BUF_SLAVE  0x08
#define ICW4_BUF_MASTER 0x0C    
#define ICW4_SFNM       0x10

#define PIT_COMMAND 0x43
#define PIT_DATA1   0x40
#define PIT_DATA2   0x41
#define PIT_DATA3   0x42

typedef struct timer_node_t {
	uint32_t id;
	uint32_t milis_left;
	struct timer_node_t* next;
} timer_node_t;

static timer_node_t* base;
static uint32_t lastid = 0;

uint32_t driver_start_timer(uint32_t milis) {
	timer_node_t* timer = (timer_node_t*)malloc(sizeof(timer_node_t));
	timer->id = lastid++;
	timer->milis_left = milis;
	timer->next = base;
	base = timer;
	return timer->id;
}

void driver_cancel_timer(uint32_t id) {
	if (base == NULL) return;
	timer_node_t* curr = base;
	timer_node_t* before = NULL;
	while (curr != NULL) {
		if (curr->id == id) {
			if (before != NULL) {
				before->next = curr->next;
			}
			else {
				base = curr->next;
			}
			free(curr);
		}
		else {
			before = curr;
		}
		curr = curr->next;
	}
}

void interrupt_timer_handle(registers_t* regs) {
	UNUSED(regs);
	if (base == NULL) return;
	timer_node_t* curr = base;
	timer_node_t* before = NULL;
	while (curr != NULL) {
		curr->milis_left -= 100;
		if (curr->milis_left <= 0) {
			if (before != NULL) {
				before->next = curr->next;
			}
			else {
				base = curr->next;
			}
			event_t event;
			event.type = TOMATO_EVENT_TIMER;
			event.data[0] = curr->id;
			os_kqueue_event(event);
			free(curr);
		}
		else {
			before = curr;
		}
		curr = curr->next;
	}
}

void driver_timer_init(void) {
	// initialize pic
	uint8_t a1 = inb(PIC1_DATA);
	uint8_t a2 = inb(PIC2_DATA);
	outb(PIC1_COMMAND, ICW1_INIT + ICW1_ICW4);
	outb(PIC2_COMMAND, ICW1_INIT + ICW1_ICW4);
	outb(PIC1_DATA, 0x20);
	outb(PIC2_DATA, 0x28);
	outb(PIC1_DATA, 4);
	outb(PIC2_DATA, 2);
	outb(PIC1_DATA, ICW4_8086);
	outb(PIC2_DATA, ICW4_8086);
	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);

	// initialize pit
	uint32_t divisor = 1193180 / 100;
	outb(PIT_COMMAND, 0x36);
	outb(PIT_DATA1, (uint8_t)(divisor & 0xFF));
	outb(PIT_DATA1, (uint8_t)((divisor >> 8) & 0xFF));
}

#undef PIC1
#undef PIC2
#undef PIC1_COMMAND
#undef PIC1_DATA
#undef PIC2_COMMAND
#undef PIC2_DATA

#undef ICW1_ICW4
#undef ICW1_SINGLE
#undef ICW1_INTERVAL4
#undef ICW1_LEVEL
#undef ICW1_INIT

#undef ICW4_8086
#undef ICW4_AUTO
#undef ICW4_BUF_SLAVE
#undef ICW4_BUF_MASTER
#undef ICW4_SFNM

#undef PIT_COMMAND
#undef PIT_DATA1
#undef PIT_DATA2
#undef PIT_DATA3