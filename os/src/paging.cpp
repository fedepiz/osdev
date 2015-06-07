#include <system.h>
#include <paging.h>

page_directory* default_dir;

unsigned long page_aligned(unsigned long address) {
	return address - (address % page_size);
}
unsigned long page_directory_index(unsigned long address) {
	return (unsigned long)address >> 22;
}

unsigned long page_table_index(unsigned long address) {
	return (unsigned long)address >> 12 & 0x03FF;
}

unsigned long page_directory_entry(page_table* table_ptr,bool used,bool supervisor) {
	unsigned long flags = 0;
	if(used) flags |= 1;
	if(supervisor) flags |= 2;
	return ((unsigned long)table_ptr) | flags;
}

unsigned long page_table_entry(unsigned long frame,bool used,bool supervisor) {
	unsigned long flags = 0;
	if(used) flags |= 1;
	if(supervisor) flags |= 2;
	return (frame*4096) | flags;
}

page_table* get_table_address(page_directory* dir, int n) {
		return (page_table*)(dir->directory_entries[n] & 0xFFFFF000);
}

void map_page_to_frame(page_directory* dir, int page, int frame, bool used,bool supervisor) {
	//Get virtual address
	unsigned long virtual_address = page*page_size;
	//Extract page directory and table indexes
	unsigned long pdi = page_directory_index(virtual_address);
	unsigned long pti = page_table_index(virtual_address);
	page_table* table = get_table_address(dir,pdi);
	//Write in the new mapping
	table->table_entries[pti] = page_table_entry(frame,used,supervisor);
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
void setup_empty_directory(page_directory* dir,unsigned char* memory) {
	for(int i = 0; i < 1024;i++) {	
		page_table* table = (page_table*)(memory);
		memory += 4096;
		dir->directory_entries[i] = page_directory_entry(table,true,true);
		for(int j = 0; j < 1024;j++) {
			table->table_entries[j] = page_table_entry(0,false,true);
		}
	}
}

/*
Identity maps a given region of physical memory
*/
void identity_map(page_directory* dir,int start_page,int end_page) {
	for(int i = start_page; i <= end_page;i++) {
		map_page_to_frame(dir,i,i,true,true);
	}
}

void page_fault_handler(regs* r) {
	PANIC("PAGE FAULT");
	/*
	unsigned int err_no = r->err_code;
	//Page not present
	if(err_no == 0 ||
	   err_no == 2 ||
	   err_no == 4 ||
	   err_no == 6) {
		
		unsigned long faulting_address = _read_cr2();
		int faulting_page = page_aligned(faulting_address)/4096;
		int free_frame = first_free_n_id(1);
		allocate_n_frames(free_frame,1);
		map_page_to_frame(default_dir,faulting_page,free_frame,true,true);
	} else {
		PANIC("Unhandled page fault!");
	}
	*/ 
}

void init_paging(unsigned char* memory) {
	if(page_aligned((unsigned long)memory) != (unsigned long)memory) {
		PANIC("FAILED PAGING INITIALIZATION\n"
		      "The physical address given to page initialisation\n"
			  "is not aligned on a page boundary");
	}
	//Create page directory
	page_directory* dir = (page_directory*)memory;
	default_dir = dir;
	//Create table
	setup_empty_directory(dir,memory + 4096);
	//Identity map the whole page 
	int total_num_pages = 1024*1024;
	identity_map(dir,0,total_num_pages);
	//Load page fault hander
	isr_install_handler(14,&page_fault_handler);
	//Finally, load the table and activate paging!
	load_page_directory((unsigned long)dir);
	activate_paging();
}