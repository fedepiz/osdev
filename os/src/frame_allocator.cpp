#include <frame_allocator.h>
#include <system.h>
#include <mmap.h>
unsigned const long frame_size = 4096;
unsigned const long frame_map_size = 1000000;
int _kernel_last_frame;

//1 MB allocator map, with one byte per frame.
unsigned char frame_map[frame_map_size];
unsigned long frame_aligned(unsigned long address) {
	unsigned long mod = address % frame_size;
	return address - mod;
}
int frame_id(unsigned long address) {
	return (int)(frame_aligned(address)/frame_size);
}

unsigned long frame_address(int id) {
	return (unsigned long)(id * frame_size);
}
int last_kernel_frame() {
	return _kernel_last_frame;
}
void frame_allocator_init() {
	unsigned long last_kernel_frame = frame_aligned(kernel_limit)/frame_size;
	_kernel_last_frame = last_kernel_frame;
	for(unsigned int i = 0;i <= last_kernel_frame;i++) {
		frame_map[i] = 1;
	}
	for(unsigned int i = last_kernel_frame + 1; i < frame_map_size;i++ ) {
		frame_map[i] = 0;
	}
}

int first_free_frame() {
	int frame_id = -1;
	for(unsigned int i = 0; i < frame_map_size;i++) {
		if(frame_map[i] == 0) {
			frame_id = (int)i;
			break;
		}
	}
	if (!physical_address_exists(frame_id * frame_size)) {
		PANIC("First empty frame is beyond memory limit ");
	}
	return frame_id;
}

unsigned char* allocate_frames(int start,int length) {
	for(int i = 0; i < length;i++) {
		frame_map[start + i] = 1;
	}
	return (unsigned char*)(start*frame_size);
}

void deallocate_frames(int start,int length) {
	for(int i = 0; i < length;i++) {
		frame_map[start + i] = 0;
	}
}

unsigned char* allocate_first_free_frame() {
	int first_free = first_free_frame();
	return allocate_frames(first_free,1);
}

void deallocate_frame(unsigned char* ptr) {
	int id = frame_id(frame_aligned((unsigned long)ptr));
	deallocate_frames(id,1);
}