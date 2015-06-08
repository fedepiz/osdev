#ifndef _HEAP_H
#define _HEAP_H

struct heap_block_tag {
	unsigned long size;
	unsigned short magic;
	bool taken;
	bool front;
};

class Heap {
private:
	unsigned char* memory;
	unsigned long memory_size;
public:
	Heap(unsigned char* memory,unsigned long memory_size);
	
	unsigned char* getMemoryPtr();
	unsigned long getMemorySize();
};
#endif