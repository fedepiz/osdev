#include <system.h>
#include <mmap.h>
#include <multiboot.h>

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
	putn(mmp->base_addr_low);
	puts(" Length: ");
	putn(mmp->length_low);
	puts(" Type: ");
	putn(mmp->type);
}


