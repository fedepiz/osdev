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

bool Heap::split_blocks(unsigned char* ptr, unsigned long split_size) {
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

void Heap::merge_blocks(unsigned char* ptr1, unsigned char* ptr2) {
	heap_block_tag* second_tag = (heap_block_tag*)ptr2;
	unsigned long total_second_size = payload_to_total(second_tag->size);
	unsigned long in_between_pointers = abs_diff((unsigned long)ptr1,(unsigned long)ptr2);
	unsigned long total_size = in_between_pointers + total_second_size;
	insert_block(ptr1,total_size);
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

unsigned char* Heap::previous_block(unsigned char* ptr) {
	heap_block_footer* footer = (heap_block_footer*)(ptr - sizeof(heap_block_footer));
	unsigned char* prev = (unsigned char*)(footer->header);
	if(in_bounds(prev)) {
		return prev;
	} else {
		return 0;
	}
}

unsigned char* Heap::first_free_block(unsigned long min_payload_size) {
	unsigned char* ptr = this->memory;
	while(this->in_bounds(ptr)) {
		heap_block_tag* tag = (heap_block_tag*)ptr;
		if(tag->size >= min_payload_size && !tag->taken) {
			return ptr;
		} else {
			ptr = next_block(ptr);
		}
	}
	return 0;
}

unsigned char* Heap::allocate(unsigned long payload_size) {
	unsigned char* ptr = this->first_free_block(payload_size);
	heap_block_tag* tag = (heap_block_tag*)ptr;
	//If bigger, try to split
	if(tag->size > payload_size) {
		this->split_blocks(ptr,payload_to_total(payload_size));
	}
	tag->taken = true;
	return ptr + sizeof(heap_block_tag);
}
void Heap::free(void* p) {
	unsigned char* ptr = (unsigned char*)p;
	ptr = ptr - sizeof(heap_block_tag);
	heap_block_tag* tag = (heap_block_tag*)ptr;
	tag->taken = false;
	unsigned char* next = this->next_block(ptr);
	heap_block_tag* next_tag = (heap_block_tag*)next;
	if(!next_tag->taken) {
		this->merge_blocks(ptr,next);
	}
}

Heap::Heap(unsigned char* memory,unsigned long memory_size) {
	this->memory = memory;
	this->memory_size = memory_size;
	this->insert_block(memory,memory_size);
}

unsigned char* Heap::getMemoryPtr() {
	return this->memory;
}

unsigned long Heap::getMemorySize() {
	return this->memory_size;
}

//Debug stuff
void print_heap_block_tag(heap_block_tag* header) {
	unsigned long addr = (unsigned long)header;
	void* arr[] = {&addr, &header->size, &header->magic, &header->taken };
	putf("Structure of header at %h: size = %d, magic = %i, taken = %b\n",arr);
}

void Heap::printHeap() {
	unsigned char* ptr = this->memory;
	while(in_bounds(ptr)) {
		heap_block_tag* tag = (heap_block_tag*)ptr;
		print_heap_block_tag(tag);
		ptr = next_block(ptr);
	}
}
