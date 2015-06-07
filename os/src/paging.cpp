#include <paging.h>
#include <system.h>
#include <mmap.h>
#include <frame_allocator.h>

unsigned long page_size = 4096;
unsigned long page_aligned(unsigned long address) {
	return address - (address % page_size);
}
unsigned long page_directory_index(unsigned long address) {
	return (unsigned long)address >> 22;
}

unsigned long page_table_index(unsigned long address) {
	  return (unsigned long)address >> 12 & 0x03FF;
}


unsigned long make_page(unsigned long frame,bool used) {
	if(used) return ((frame*4096) | 3);
	else return ((frame*4096)|2);
}

page_table* get_table_address(page_directory* dir, int n) {
	return (page_table*)(dir->tables[n] & 0xFFFFF000);
}

void init_paging() {
   page_directory* dir = (page_directory*) allocate_first_free_frame();
 
   for(int i = 0;i < 1024;i++) {
	   dir->tables[i] = (unsigned long)allocate_first_free_frame();
	   dir->tables[i] = dir->tables[i] | 3;
   }
   
   page_table* first = get_table_address(dir,0);
   for(int i = 0; i < 1024;i++) {
	   first->pages[i] = make_page(i,true);
   }
   
   _write_cr3((unsigned long)dir);
   _write_cr0(_read_cr0() | 0x80000000);
}
/*
SACRED CODE OF PAGING THAT WORKS
unsigned long *page_directory;
unsigned long *page_table;

void map(unsigned int frame, unsigned int vframe){
   page_table[frame] = vframe*4096 | 3;
}


void init_paging(){
   page_directory = (unsigned long *) allocate_first_free_frame();
   page_table = (unsigned long *) allocate_first_free_frame();

   unsigned int i;
   for(i=0; i<1024; i++){
      map(i,i);
   };      

   page_directory[0] = (unsigned long)page_table;
   page_directory[0] = page_directory[0] | 3;

   for(i=1; i<1024; i++){
      page_directory[i] = 0 | 2;
   };
   _write_cr3((unsigned long)page_directory);
   _write_cr0(_read_cr0() | 0x80000000);
}*/