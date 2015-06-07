#include <system.h>
#include <multiboot.h>
#include <mmap.h>
#include <timer.h>
#include <keyboard.h>
#include <k_memory.h>

void startup_checklist();
void init_devices();

void startup_checklist(multiboot_info_t* mbd) {
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
	
	//Detect memory
	init_memory_map(mbd);
	
}

void init_devices() {
	timer_install();
	keyboard_install();
}

void frame_allocator_tests() {
	putn(get_last_kernel_frame());putnl();
	putn(first_free_n_id(1));putnl();
    fmalloc(frame_size);
	putn(first_free_n_id(1));putnl();
	fmalloc(frame_size+1);
	putn(first_free_n_id(1));putnl();
}

extern "C" void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
	startup_checklist(mbd);
	cls();
	puts("Welcome to PizOS 0.0000000.....000001\n");
	init_memory_manager();
	int free_frame = first_free_n_id(1);
	int* x = (int*)(free_frame*4096);
	*x = 5;
	putn(*x);
}