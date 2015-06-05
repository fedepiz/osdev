#include <system.h>
#include <timer.h>
#include <keyboard.h>

void startup_checklist();
void init_devices();

void startup_checklist() {
	//Init the vga driver, needed by all things
	init_video();
	//Install global descriptor table, needed by all following things
	gdt_install();
	//Install the interrupt descriptor table
	idt_install();
	//Install the common handler for interrupt service routines
	isr_install();
	//CUSTOM ISR HANDLERS GO HERE
	
	//Install the common handler for interrupt requests
	irq_install();
	
	//CUSTOM IRQ HANDLERS/DEVICE DRIVERS
	//GO HERE
	init_devices();
	
	//Turn on interrupt requests
	irq_enable();
}

void init_devices() {
	timer_install();
	keyboard_install();
}

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main() {
	startup_checklist();
	cls();
	putn(52);
	puts("\nWelcome to PizOS 0.0000000.....000001\n");
}