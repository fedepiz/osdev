#ifndef _PAGING_H
#define _PAGING_H
struct page_table {
	unsigned long table_entries[1024];
};

struct page_directory {
	unsigned long directory_entries[1024];
};

struct page_book {
	page_directory directory;
	page_table tables[1024];
};

unsigned long const page_size = 4096;
int const num_pages = 1024*1024;

void map_page_to_frame(page_directory* dir, int page, int frame, bool used,bool supervisor);
void map_page_to_frame(int page,int frame);
//Pointer to the page_book used during initiation.
//This is a physical value
page_book* main_page_book();
//Pointer to the page_book currently loaded
//This is valid only if the currently loaded
//pagebook has been mapped in at the last
//1025 pages. Virtual address.
page_book* loaded_page_book();
//Initializes page structure at given memory address.
//After initialization, the whole memory space will be
//identity paged.
//It must be aligned!
void init_paging(page_book* holder);
#endif