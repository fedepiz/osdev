#include <heap.h>
#include <system.h>

void Heap::insert_block(unsigned char* ptr,unsigned long total_size) {
	unsigned long payload_size = total_size - sizeof(heap_block_tag) - sizeof(heap_block_footer);
	heap_block_tag head = { payload_size, 0xacab, false };
	heap_block_footer footer = { (heap_block_tag*)ptr };
	memcpy(ptr,&head,sizeof(heap_block_tag));
	memset(ptr + sizeof(heap_block_tag),0,payload_size);
	memcpy(ptr + sizeof(heap_block_tag) + payload_size, &footer, sizeof(heap_block_footer));
}

unsigned long total_to_palyoad(unsigned long size) {
	return size - sizeof(heap_block_tag) - sizeof(heap_block_footer);
}

unsigned long payload_to_total(unsigned long size) {
		return size + sizeof(heap_block_tag) + sizeof(heap_block_footer);

}

bool Heap::split_block(unsigned char* ptr, unsigned long split_size) {
	heap_block_tag* header = (heap_block_tag*)ptr;
	unsigned long original_total_size =  payload_to_total(header->size);
	unsigned long remaining_size = original_total_size - split_size;	
	if(remaining_size <= payload_to_total(0)) {
		return false;
	}
	unsigned char* second_target = ptr + split_size;	
	insert_block(ptr,split_size);
	insert_block(second_target, remaining_size);
	return true;
}

bool Heap::in_bounds(unsigned char* addr) {
	return addr >= memory && addr < memory + memory_size;
}

unsigned char* Heap::next_block(unsigned char* ptr) {
	heap_block_tag* tag = (heap_block_tag*)ptr;
	unsigned char* next = ptr + payload_to_total(tag->size);
	if(in_bounds(next)) {
		return next;
	} else {
		return 0;
	}
}

unsigned char* Heap::first_free_block(unsigned long min_size) {
	unsigned char* ptr = this->memory;
	while(this->in_bounds(ptr)) {
		heap_block_tag* tag = (heap_block_tag*)ptr;
		if(tag->size >= min_size && !tag->taken) {
			return ptr;
		} else {
			ptr = next_block(ptr);
		}
	}
	return 0;
}

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