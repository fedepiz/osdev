#include <system.h>
#include <heap.h>

block_descriptor head_block_descriptor(unsigned long len) {
	block_descriptor desc = {false, true, len, 0xACAB };
	return desc;
}
block_descriptor tail_block_descriptor(unsigned long len) {
	block_descriptor desc = {false, false, len, 0xBACA };
	return desc;
}

block_descriptor invalid_descriptor() {
	block_descriptor desc = { false, false, 0, 0xBAD };
	return desc;
}


void print_descriptor_info(block_descriptor desc) {
	puts("Length: ");
	puth(desc.size);
	puts(" used: ");
	desc.used ? puts("yes") : puts("no");
	puts(" magic: ");
	puth(desc.magic);
}

void initialize_descriptors(unsigned char* ptr, unsigned long size){
	block_descriptor head_desc = head_block_descriptor(size);
	block_descriptor tail_desc = tail_block_descriptor(size);

	memcpy(ptr,&head_desc,sizeof(block_descriptor));
	memcpy(ptr + sizeof(block_descriptor) + size, &tail_desc, sizeof(block_descriptor));
}

block_descriptor* get_head_descriptor(unsigned char* ptr) {
	return (block_descriptor*)(ptr - sizeof(block_descriptor));
}
block_descriptor* get_tail_descriptor(unsigned char* ptr) {
	unsigned long length = get_head_descriptor(ptr)->size;
	return (block_descriptor*)(ptr + length);
}

Heap::Heap(unsigned char* memory,unsigned long size) {
	this->size = size;
	this->mem_ptr = memory;
}

unsigned long Heap::getSize() {
	return this->size;
}
unsigned char* Heap::getMemoryPtr() {
	return this->mem_ptr;
}

unsigned char* Heap::getFirstBlockPtr() {
	return this->getMemoryPtr() - sizeof(block_descriptor);
}
