#ifndef _MMAP
#define _MMAP
#include <multiboot.h>

extern "C" int _kernel_start; //Just symbols
extern "C" int _kernel_end; //Just symbols
extern const unsigned long kernel_limit;//Last address used by kernel

void print_memory_map_info(multiboot_info_t* mbd);
void print_memory_info(multiboot_info_t* mbd);

bool physical_address_exists(unsigned long addr);

struct mem_block {
	unsigned long address;
	unsigned long length;
	unsigned int magic; //0xFEDE means it's ok
};

mem_block find_main_ram_block(multiboot_info_t* mbd);
/*This entry (actually defined in mmap.cpp)
  contains the address and length of the main
  high memory chunk detected. It is filled by 
  a call to init_memory_map*/
extern mem_block main_memory_block;
void init_memory_map(multiboot_info_t* mbd);
unsigned long get_available_physical_memory();
#endif