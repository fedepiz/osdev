#ifndef _K_MEMORY_H
#define _K_MEMORY_H

unsigned long const frame_size = 4096;
long const num_frames = 1000000;

void init_frame_allocator();
void* fmalloc(int size);
void ffree(void* ptr);

//Diagnostics
int first_free_n_id(int n);
int get_last_kernel_frame();

//Paging
unsigned long const page_size = 4096;

struct page_table {
	unsigned long table_entries[1024];
};

struct page_directory {
	unsigned long directory_entries[1024];
};
void init_paging();
#endif