#include <kernel_tests.h>
#include <system.h>
#include <multiboot.h>
#include <mmap.h>
#include <timer.h>
#include <keyboard.h>
#include <frame_manager.h>
#include <paging.h>
#include <heap.h>

void heap_test_x_y_x(){
	puts("Test heap x-y-x\n");
	unsigned long memory_size = 4096*4;
	putf("Total heap size is %d\n",&memory_size);
	unsigned char* memory = (unsigned char*)fmalloc(memory_size);
	
	Heap heap(memory,memory_size);
	int* x = (int*)heap.allocate(sizeof(int));
	/*int* y = (int*)*/ heap.allocate(sizeof(int));
	heap.printHeap();
	heap.free(x);
	puts("---------------------------\n");
	heap.printHeap();
}

void heap_test_x_y_y() {
	puts("Test heap x-y-y\n");
	unsigned long memory_size = 4096*4;
	putf("Total heap size is %d\n",&memory_size);
	unsigned char* memory = (unsigned char*)fmalloc(memory_size);
	Heap heap(memory,memory_size);
	/*int* x = (int*)*/heap.allocate(sizeof(int));
	int* y = (int*)heap.allocate(sizeof(int));
	heap.printHeap();
	heap.free(y);
	puts("---------------------------\n");
	heap.printHeap();
}

void heap_test_x_y_z() {
	puts("Test heap x-y-z\n");
	unsigned long memory_size = 4096*4;
	putf("Total heap size is %d\n",&memory_size);
	unsigned char* memory = (unsigned char*)fmalloc(memory_size);
	Heap heap(memory,memory_size);
	int* x = (int*)heap.allocate(sizeof(int));
	int* y = (int*)heap.allocate(sizeof(int));
	/*int* z = (int*)*/heap.allocate(sizeof(int));
	heap.printHeap();
	heap.free(x);
	heap.free(y);
	puts("---------------------------\n");
	heap.printHeap();
}