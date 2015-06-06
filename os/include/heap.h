#ifndef _HEAP_H
#define _HEAP_H

struct block_descriptor {
	bool used;
	bool head;
	unsigned long size;
	int magic;
};

block_descriptor head_block_descriptor(unsigned long len);
block_descriptor tail_block_descriptor(unsigned long len);
block_descriptor invalid_descriptor();
void print_descriptor_info(block_descriptor desc);

void writeInDescriptors(unsigned char* ptr, unsigned long size);
block_descriptor* get_head_descriptor(unsigned char* ptr);
block_descriptor* get_tail_descriptor(unsigned char* ptr);

unsigned int const min_block_size = 2*sizeof(block_descriptor);

class Heap {
private:
	unsigned long size;
	unsigned char* mem_ptr;
public:
	Heap(unsigned char* mem_ptr, unsigned long size);
	unsigned long getSize();
	unsigned char* getMemoryPtr();
	unsigned char* getFirstBlockPtr();
};
#endif