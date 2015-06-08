#include <heap.h>
#include <system.h>

Heap::Heap(unsigned char* memory,unsigned long memory_size) {
	this->memory = memory;
	this->memory_size = memory_size;
}

unsigned char* Heap::getMemoryPtr() {
	return this->memory;
}

unsigned long Heap::getMemorySize() {
	return this->memory_size;
}