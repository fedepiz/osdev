#include <system.h>
#include <mmap.h>
#include <multiboot.h>

const unsigned long kernel_limit = (unsigned long)&_kernel_end;
mem_block main_memory_block;

void init_memory_map(multiboot_info_t* mbd) {
	main_memory_block = find_main_ram_block(mbd);
}
unsigned long get_available_physical_memory() {
	return main_memory_block.length - kernel_limit;
}

bool physical_address_exists(unsigned long addr) {
	return addr < main_memory_block.address + main_memory_block.length;
}



memory_map_t* first_mmap(multiboot_info_t* mbd) {
	return (memory_map_t*)mbd->mmap_addr;
}

bool mmap_is_in_bounds(memory_map_t* mmap, multiboot_info_t* mbd) {
	return (unsigned long)mmap < mbd->mmap_addr + mbd->mmap_length;
}

memory_map_t* next_map(memory_map_t* mmap) {
		unsigned long current_ptr = (unsigned long)mmap;
		return(memory_map_t*)(current_ptr + mmap->size + 4);	
}

mem_block mem_block_from_mmap(memory_map_t* mmap) {
	mem_block block;
	block.address = mmap->base_addr_low;
	block.length = mmap->length_low;
	block.magic = 0xFEDE;
	return block;
}

mem_block find_main_ram_block(multiboot_info_t* mbd) {
	memory_map_t* mmap = first_mmap(mbd);
	while(mmap_is_in_bounds(mmap,mbd)) {
		if(mmap->base_addr_low == 0x100000) {
			return mem_block_from_mmap(mmap);
		}
		mmap = next_map(mmap);
	}
	PANIC("Cannot find main ram block, expected at address 0x100000.\nIf your system has ISA memory gap enabled, please disable it");
	mem_block empty;
	return empty;
}

//DIAGNOSTICS AND PRINTING

void print_memory_map_info(multiboot_info_t* mbd);
void print_memory_map_entries(multiboot_info_t* mbd);
void print_memory_map_entry(memory_map_t* mmp);

void print_memory_info(multiboot_info_t* mbd) {
	puts("Kernel start:"); puth((unsigned long)&_kernel_start);putnl();
	puts("Kernel limit: "); puth(kernel_limit);putnl();
	print_memory_map_info(mbd);
}

void print_memory_map_info(multiboot_info_t* mbd) {
	puts("Printing multiboot info structure contents....\n");
	puts("Flags: ");
	putb(mbd->flags);
	puts("\nMemory map length:");
	putn(mbd->mmap_length);
	puts("\nMemory map blocks...");
	
	print_memory_map_entries(mbd);
}

void print_memory_map_entries(multiboot_info_t* mbd) {
	memory_map_t* mmap = (memory_map_t*)mbd->mmap_addr;
	int block_count = 0;
	while((unsigned long)mmap < mbd->mmap_addr + mbd->mmap_length) {
		block_count++;
		puts("\nBlock n.: ");
		putn(block_count);
		print_memory_map_entry(mmap);
		unsigned long current_ptr = (unsigned long)mmap;
		mmap = (memory_map_t*)(current_ptr + mmap->size + 4);
	}
}

void print_memory_map_entry(memory_map_t* mmp) {
	puts(" Addr: ");
	puth(mmp->base_addr_low);
	puts(" Length: ");
	putn(mmp->length_low);
	puts(" Type: ");
	putn(mmp->type);
}
