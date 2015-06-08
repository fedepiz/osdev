#ifndef _HEAP_H
#define _HEAP_H

struct heap_block_tag {
	unsigned long size;
	unsigned short magic;
	bool taken;
};

struct heap_block_footer{
	heap_block_tag* header;
};



class Heap {
private:
	unsigned char* memory;
	unsigned long memory_size;
	unsigned char* next_block(unsigned char* ptr);
	bool in_bounds(unsigned char* addr);
public:
	Heap(unsigned char* memory,unsigned long memory_size);
	unsigned char* getMemoryPtr();
	unsigned long getMemorySize();
	void insert_block(unsigned char* ptr,unsigned long total_size);
	bool split_block(unsigned char* ptr, unsigned long split_size);
	unsigned char* first_free_block(unsigned long min_size);

};
#endif