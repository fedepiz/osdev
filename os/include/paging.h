#ifndef _PAGING_H
#define _PAGING_H

struct page
{
   unsigned int present    : 1;   // Page present in memory
   unsigned int rw         : 1;   // Read-only if clear, readwrite if set
   unsigned int user       : 1;   // Supervisor level only if clear
   unsigned int wt 		   : 1;   // Write through
   unsigned int cd		   : 1;   // Cache disabled
   unsigned int accessed   : 1;   // Has the page been accessed since last refresh?
   unsigned int dirty      : 1;   // Has the page been written to since last refresh?
   unsigned int unused     : 5;   // Amalgamation of unused and reserved bits
   unsigned int frame      : 20;  // Frame address (shifted right 12 bits)
};

struct page_table {
	unsigned long pages[1024];
};

struct page_directory{
	unsigned long tables[1024];
};


extern "C" void _write_cr3(unsigned long ptr);
extern "C" unsigned long _read_cr3();
extern "C" void _write_cr0(unsigned long x);
extern "C" unsigned long _read_cr0();

void init_paging();
#endif