#include <system.h>
#include <scrn.h> 
#include <gdt.h>
#include <idt.h>
#include <isr.h>

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main() {
	init_video();
	gdt_install();
	idt_install();
	isr_install();
	cls();
	puts("Welcome to PizOS 0.0000000.....000001\n");
}