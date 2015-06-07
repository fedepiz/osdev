#include <paging.h>
#include <system.h>
#include <mmap.h>
#include <frame_allocator.h>
namespace paging {
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
	void page_fault_handler(regs* reg);
	page_directory* main_dir;
	void init_paging() {
	   //Create page directory
	   page_directory* dir = (page_directory*)frame_alloc::allocate_first_free_frame();
	   main_dir = dir;
	   //Fill directory entries (page table pointers)
	   for(int i = 0;i < 1024;i++) {
		   dir->tables[i] = (unsigned long)frame_alloc::allocate_first_free_frame();
		   dir->tables[i] = dir->tables[i] | 3;
	   }
	   //Fill pages, up to limit set valid
	   int limit = (int)(frame_alloc::first_free_frame());
	   for(int i = 0; i < 1024;i++) {
		   for(int j = 0; j < 1024;j++) {
			   int page_n = 1024*i + j;
			   page_table* table = get_table_address(dir,i);
			   table->pages[j] = make_page(page_n,page_n < limit);
		   }
	   }
	   //Then install page fault handler
	   isr_install_handler(14,&page_fault_handler);
	   
	   _write_cr3((unsigned long)dir);
	   _write_cr0(_read_cr0() | 0x80000000);
	}

	void page_fault_handler(regs* reg) {
		//If the error is because of a non-present page
		if((reg->err_code == 0 ||
			reg->err_code == 2 ||
			reg->err_code == 4)||
			reg->err_code == 6){
			//Reconstructe pdi and pti from cr2 register contents
			unsigned long cr2 = _read_cr2();
			unsigned long pdi = page_directory_index(cr2);
			unsigned long pti = page_table_index(cr2);
			//Get the address of the page table from pdi...
			page_table* table = get_table_address(main_dir,pdi);
			//Find out first frame n and allocate it
			int frame_n = frame_alloc::first_free_frame();
			frame_alloc::allocate_first_free_frame();
			//Map page indicated by pti with frame_n and in use
			table->pages[pti] = make_page(frame_n,true);
		} else {
			puts("Other paging error\nErr code:");
			putb(reg->err_code);
			for(;;);
		}
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
}