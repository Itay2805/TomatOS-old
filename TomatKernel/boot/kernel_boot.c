#include <stdint.h>

#include "gdt/gdt.h"
#include "interrupts/idt.h"

#include <string.h>

#include <core/graphics/term.h>
#include <core/memory/paging.h>
#include <core/memory/heap.h>

#include <core/process/syscall.h>
#include <core/process/process.h>
#include <core/process/perm.h>

#include <core/drivers/ata.h>

#include <core/drivers/ps2_keyboard.h>

#include <core/timer.h>

extern void kmain();

void kernel_boot(const void* multiboot_structure, uint32_t multiboot_magic) {
	UNUSED(multiboot_structure);
	UNUSED(multiboot_magic);
	
	// the kernel boot will basically load the basics needed for the kernel
	// to run like segments all the way to registering syscalls

	term_init();
	term_clear();
	term_write("Booting into TomatKernel...\n\n");

	term_write("[TomatoBoot] initializing paging\n");
	paging_init();

	term_write("[TomatoBoot] initializing gdt\n");
	gdt_init();
	term_write("[TomatoBoot] initializing idt\n");
	idt_init();
	term_write("[TomatoBoot] initializing syscalls\n");
	syscall_init();

	term_write("[TomatoBoot] registering terminal and window syscalls\n");
	term_register_syscalls();
	window_register_syscalls();

	term_write("[TomatoBoot] registering heap sysalls\n");
	heap_register_syscalls();

	term_write("[TomatoBoot] registering process syscalls and initializing alive\n");
	process_init();

	term_write("[TomatoBoot] registering and initializing timer\n");
	timer_init();

	term_write("[TomatoBoot] registering and initializing PS2 keyboard driver\n");
	ps2_keyboard_init();

	term_write("ATA Test: \n");
	char* names[4] = { "PRIMARY/MASTER", "PRIMARY/SLAVE", "SECONDARY/MASTER", "SECONDARY/SLAVE" };
	for (int bus = 0; bus < 2; bus++) {
		for (int drive = 0; drive < 2; drive++) {
			term_write(names[drive + bus * 2]);
			term_write(": ");

			ata_bus(bus ? ATA_BUS_PRIMARY : ATA_BUS_SECONDARY);
			ata_drive(drive ? ATA_DRIVE_MASTER : ATA_DRIVE_SLAVE);
			
			if(ata_connected()) {
				ata_identify_t identify;
				ata_identify(&identify);
				char buf[41];
				buf[40] = 0;
				memcpy(buf, identify.model_number, 40);
				term_write(buf);
				term_write("\n");

				buf[20] = 0;
				memcpy(buf, identify.serial_number, 20);
				term_write("Serial Number: ");
				term_write(buf);
				term_write("\n");

				buf[8] = 0;
				memcpy(buf, identify.firmware_revision, 8);
				term_write("Firmware Revision: ");
				term_write(buf);
				term_write("\n");		
			}else {
				term_write("No HDD\n");
			}
		}
	}

	// from here we are technically done with boot and should
	// only focus on loading libraries and such
	term_clear();
	term_set_cursor_pos(0, 0);
	term_write("Welcome to TomatKernel!\n\n");
	
	term_write("[TomatoKernel] Loading core program\n");
	// TODO: This should look at configuration to load the assembly of the core program
	
	process_t foreground;
	process_create(&foreground, kmain, USER_ALIVE, true); // this will run as alive just because it must have the kernel segments
	foreground.core = true;
	process_start(&foreground);

	term_write("[TomatoKernel] Starting alive\n");
	__asm("int $0x80" : : "a"(SYSCALL_START));
	
	// Once alive has started it means that the control is given to the core program
	// or to the alive program. It should never return to here (Maybe make so it will 
	// only return if we want to close the program?)
	kpanic("SHOULD NOT HAVE REACHED HERE!!!");
}