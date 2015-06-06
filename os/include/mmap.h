#ifndef _MMAP
#define _MMAP
#include <multiboot.h>
void print_memory_map_info(multiboot_info_t* mbd);
void print_memory_info(multiboot_info_t* mbd);

struct mem_block {
	unsigned long address;
	unsigned long length;
};

mem_block find_main_ram_block(multiboot_info_t* mbd);

extern mem_block main_memory_block;
void init_memory_map(multiboot_info_t* mbd);
#endif