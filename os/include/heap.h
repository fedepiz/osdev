#ifndef _HEAP_H
#define _HEAP_H

/*
This header describes the functionality for a simple heap structure.
A heap is constructed from a contiguous chunk of memory. Instead of relying
on a linked list/bitmap like structure to keep track of block allocation,
this implementation partitions the memory it is given in the following way

|head_tag|...block..|tail_tag|head_tag|...block...|tail_tag|...

Each tag is an instance of the block_descriptor structure defined below. 

Initially, the heap is made by one big tag filling the whole space. As memory
is requested, new tags are created from this one, by "splitting them" off. 
Whenever memory is requested, the heap traverses the blocks, finds
a sufficiently large one and then proceede to split it, allocating the split off part
and keeping the rest free for future allocations. When a block is freed, it's neighbors
are checked: if they are also freed, then the blocks are coalesced together.

Whenever a block is too small to be splitted (that is, the remaing part is not sufficient to
fit the to tags) then the block is wholly allocated without splitting.

NOTE: In the functions below, the parameter "unsigned char* ptr" generally refers to
the pointer to the start of the payload of the block, and not to the tag itself. This is after
all the pointer that is returned to the requestor on allocation, and the one used to request 
memory deallocation. So, the head_tag's addres can be found generally by ptr - sizeof(block_descriptor).
However, the function initialize_descriptors, which is meant to take a block and write in the initial 
descriptors, does not take a pointer to the payload but a pointer to the whole of the memory to be
initialized itself.
*/

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

void initialize_descriptors(unsigned char* ptr, unsigned long size);
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