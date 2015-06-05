#include <system.h>
#include <scrn.h> 
#include <gdt.h>
#include <idt.h>
#include <isr.h>
#include <irq.h>
#include <timer.h>

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
	timer_install();
	
	
	//Turn on interrupt requests
	irq_enable();
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