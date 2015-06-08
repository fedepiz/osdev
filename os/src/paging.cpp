#include <system.h>
#include <paging.h>

page_book* _main_page_book;

page_book* main_page_book() {
	return _main_page_book;
}

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

void map_page_to_frame(int page,int frame) {
	map_page_to_frame(&_main_page_book->directory,page,frame,true,true);
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
void setup_empty_directory(page_book* holder) {
	for(int i = 0; i < 1024;i++) {	
		page_table* table = &holder->tables[i];
		holder->directory.directory_entries[i] = page_directory_entry(table,true,true);
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

void map_holder_to_end(page_book* holder) {
	int holder_size = ((sizeof(page_book))/4096);
	int start_page = num_pages - holder_size;
	int start_frame = (int)holder/4096;
	for(int i = 0; i < holder_size;i++) {
		map_page_to_frame(start_page + i, start_frame + i); 
	}
}

page_book* loaded_page_book(){
	int holder_size = ((sizeof(page_book))/4096);
	int start_page = num_pages - holder_size;
	return (page_book*)(start_page*4096);
}

void page_fault_handler(regs* r) {
	PANIC("PAGE FAULT");
}

void init_paging(page_book* holder) {
	//The holder must be page aligned. Otherwise, the system
	//would triple fault when paging is switched on
	if(page_aligned((unsigned long)holder) != (unsigned long)holder) {
		PANIC("FAILED PAGING INITIALIZATION\n"
		      "The physical address of the page book\n"
 			  "given to page initialisation\n is not a"
			  "ligned on a page boundary");
	}
	//Dir points to the directory. Used by
	//some functions below
	page_directory* dir = &holder->directory;
	//Record address of holder in _main_page_book.
	_main_page_book = holder;
	//Create directory, filled with tables whose pages
	//are all invalid. Holder is the page_book structure
	//which will actually hold the value.
	setup_empty_directory(holder);
	//Identity map the whole address space. This makes
	//sense as so far the kernel is the only process,
	//and this obviosuly avoids triple faults due to
	//addresses not making sense anymore
	identity_map(dir,0,num_pages);
	//Map last 1025 pages to physical address of holder,
	//so that it may be retrieved using loaded_page_book()
	map_holder_to_end(holder);
	//Load page fault handler. Right now, it's just
	//an empty stub that causes the system to halt, since
	//the whole space is identity mapped
	isr_install_handler(14,&page_fault_handler);
	//Finally, load the table and activate paging!
	load_page_directory((unsigned long)dir);
	activate_paging();
}