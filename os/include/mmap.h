#ifndef _MMAP
#define _MMAP
#include <multiboot.h>
void print_memory_map_info(multiboot_info_t* mbd);
void print_memory_map_entries(multiboot_info_t* mbd);
void print_memory_map_entry(memory_map_t* mmp);

struct mem_block {
	unsigned long address;
	unsigned long length;
};

extern "C" int _kernel_start; //Just symbols
extern "C" int _kernel_end; //Just symbols
extern const unsigned long kernel_limit;//Last address used by kernel
#endif