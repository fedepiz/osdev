#include <system.h>
#include <multiboot.h>
#include <mmap.h>
#include <timer.h>
#include <keyboard.h>
#include <frame_manager.h>
#include <paging.h>
#include <heap.h>

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
	set_keyboard_echo(false);
}

void init_memory_management() {
	//Initialize frame allocation
	init_frame_manager();
	//Allocate page book to actually hold pages
	page_book* holder = (page_book*)fmalloc(sizeof(page_book));
	//Initialize tables at given address, and turn paging on. The
	//whole memory space should be identity paged.
	init_paging(holder);
}

void print_header(heap_block_tag* header) {
	unsigned long addr = (unsigned long)header;
	void* arr[] = {&addr, &header->size, &header->magic };
	putf("Structure of header at %h: size = %d, magic = %h\n",arr);
}

extern "C" void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
	startup_checklist(mbd);
	cls();
	puts("Welcome to PizOS 0.0000000.....000001\n");
	init_memory_management();
	
	unsigned long memory_size = 4096*4;
	putf("Total heap size is %d\n",&memory_size);
	unsigned char* memory = (unsigned char*)fmalloc(memory_size);
	
	Heap heap(memory,memory_size);
	heap.insert_block(memory,memory_size);
	print_header((heap_block_tag*)memory);
	puts("\n\n");
	
	putbool(heap.split_blocks(memory,4096));
	putnl();
	
	print_header((heap_block_tag*)memory);
	print_header((heap_block_tag*)(memory + 4096));
	
	print_header((heap_block_tag*)heap.first_free_block(4096));
	
	putnl();puts("DONE");
}