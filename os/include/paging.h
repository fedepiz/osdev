#ifndef _PAGING_H
#define _PAGING_H
unsigned long const page_size = 4096;

struct page_table {
	unsigned long table_entries[1024];
};

struct page_directory {
	unsigned long directory_entries[1024];
};
//Initializes page structure at given memory address.
//After initialization, the whole memory space will be
//identity paged.
//It must be aligned!
void init_paging(unsigned char* memory);
#endif