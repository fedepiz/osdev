#include <k_memory.h>
#include <system.h>
#include <mmap.h>

/*
This long array maps the ith entry  to the ith frame. Bit 0 is set if the 
frame has already been allocated. The other bits are, for now, unused
*/
unsigned char frame_map[num_frames];
//Utility functions...
unsigned long frame_aligned_address(unsigned long addr) {
	return addr - (addr % frame_size);
}

unsigned long frame_address(int frame_id) {
	return frame_size * frame_id;
}

unsigned long frame_id(unsigned long addr) {
	return frame_aligned_address(addr)/frame_size;
}

int get_last_kernel_frame() {
	return frame_id(kernel_limit);
}

bool is_frame_free(int n) {
	return frame_map[n] == 0;
}

unsigned char allocated_frame() {
	return 1;
}

unsigned char non_allocated_frame() {
	return 0;
}

int first_free_n_id(int n) {
	if(n == 0) {
		PANIC("Requested 0 frame allocation");
		return -1;
	}
	for(int i = 0; i < num_frames - n + 1;i++) {
		bool all_free = true;
		for(int j = 0; j < n; j++) {
			all_free &= is_frame_free(i + j);
		}
		if (all_free)
			return i;
	}
	return -1;
}

void allocate_n_frames(int index, int n) {
	if(index == -1) {
		PANIC("Bad frame index");
		return;
	}
	for(int i = 0; i < n;i++) {
		frame_map[index + i] = allocated_frame();
	}
}

void free_n_frames(int index,int n) {
	if(index == -1) {
		PANIC("Bad frame index");
		return;
	}
	for(int i = 0; i < n;i++) {
		frame_map[index + i] = non_allocated_frame();
	}
}

void* fmalloc(int size) {
	unsigned int n = size / frame_size;
	if (size % frame_size > 0)
		n++;
	int index = first_free_n_id(n);
	allocate_n_frames(index,n);
	return (void*)(frame_address(index));
}

void ffree(void* ptr) {
	int index = frame_id((unsigned long)ptr);
	int count = (unsigned long)ptr / frame_size;
	if((unsigned long)ptr % frame_size)
		count++;
	free_n_frames(index,count);
}

//Initialize the frame allocator
void init_frame_allocator() {
	for(int i = 0; i < num_frames;i++) {
		if(i <= get_last_kernel_frame()) {
			frame_map[i] = allocated_frame();
		} else {
			frame_map[i] = non_allocated_frame();
		}
	}
}

//PAGING

unsigned long page_aligned(unsigned long address) {
	return address - (address % page_size);
}
unsigned long page_directory_index(unsigned long address) {
	return (unsigned long)address >> 22;
}

unsigned long page_table_index(unsigned long address) {
	return (unsigned long)address >> 12 & 0x03FF;
}

unsigned long make_page(unsigned long frame,bool used,bool supervisor) {
	unsigned long flags = 0;
	if(used) flags |= 1;
	if(supervisor) flags |= 2;
	return (frame*4096) | flags;
}

page_table* get_table_address(page_directory* dir, int n) {
		return (page_table*)(dir->directory_entries[n] & 0xFFFFF000);
}

void map_page_to_frame(page_directory* dir, int page, int frame, bool used) {
	//Get virtual address
	unsigned long virtual_address = page*page_size;
	//Extract page directory and table indexes
	unsigned long pdi = page_directory_index(virtual_address);
	unsigned long pti = page_table_index(virtual_address);
	page_table* table = get_table_address(dir,pdi);
	//Write in the new mapping
	table->table_entries[pti] = make_page(frame,used,true);
}

void load_page_directory(unsigned long dir_address) {
	_write_cr3(dir_address);
}
void activate_paging() {
	_write_cr0(_read_cr0() | 0x80000000);	
}
/*
Allocate and enable all page tables,
then set each page as not present.
*/
void setup_empty_directory(page_directory* dir) {
	for(int i = 0; i < 1024;i++) {	
		page_table* table = (page_table*)fmalloc(sizeof(page_table));
		dir->directory_entries[i] = (unsigned long)table | 3;
		for(int j = 0; j < 1024;j++) {
			table->table_entries[j] = make_page(0,false,true);
		}
	}
}
/*
Identity maps a given region of physical memory
*/
void identity_map(page_directory* dir,int start_page,int end_page) {
	for(int i = start_page; i <= end_page;i++) {
		map_page_to_frame(dir,i,i,true);
	}
}

void init_paging() {
	//Create page directory
	page_directory* dir = (page_directory*)fmalloc(sizeof(page_directory));
	int first_free_frame = first_free_n_id(1);
	//Create table
	setup_empty_directory(dir);
	//Identity map all that is allocated so far 
	//(At the time of writing, kernel binary + page directory
	//and tables)
	identity_map(dir,0,first_free_frame-1);
	//Finally, load the table and activate paging!
	load_page_directory((unsigned long)dir);
	activate_paging();
}

//MEMORY MANAGER
void init_memory_manager() {
	init_frame_allocator();
	init_paging();
}
