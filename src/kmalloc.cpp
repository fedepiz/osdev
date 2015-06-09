#include <system.h>
#include <frame_manager.h>
#include <heap.h>

Heap kernel_heap(0,0);

void init_kernel_heap() {
	unsigned long heap_size = 256*4096;
	unsigned char* memory_ptr = (unsigned char*)fmalloc(heap_size,0xD0000000);//256 pages = 1MB
	Heap kh(memory_ptr,heap_size);
	kernel_heap = kh;
}

unsigned char* kmalloc(unsigned long length) {
	return kernel_heap.allocate(length);
}

void kfree(void* ptr){
	kernel_heap.free(ptr);
}

Heap* getKernelHeap() {
	return &kernel_heap;
}