#include<frame_manager.h>
#include<system.h>
#include<mmap.h>
/*
This long array maps the ith entry  to the ith frame. Bit 0 is set if the 
frame has already been allocated. The other bits are, for now, unused
*/
unsigned char frame_map[num_frames];
//Utility functions...
unsigned long frame_aligned_address(unsigned long addr) {
	return addr - (addr % frame_size);
}

unsigned long frame_address(int frame_id) {
	return frame_size * frame_id;
}

unsigned long frame_id(unsigned long addr) {
	return frame_aligned_address(addr)/frame_size;
}

int frames_required(unsigned long size) {
	unsigned int n = size / frame_size;
	if (size % frame_size > 0)
		n++;
	return n;
}

int get_last_kernel_frame() {
	return frame_id(kernel_limit);
}

bool is_frame_free(int n) {
	return frame_map[n] == 0;
}

unsigned char allocated_frame() {
	return 1;
}

unsigned char non_allocated_frame() {
	return 0;
}

int first_free_n_id(int n) {
	if(n == 0) {
		PANIC("Requested 0 frame allocation");
		return -1;
	}
	for(int i = 0; i < num_frames - n + 1;i++) {
		bool all_free = true;
		for(int j = 0; j < n; j++) {
			all_free &= is_frame_free(i + j);
		}
		if (all_free)
			return i;
	}
	return -1;
}

void allocate_frame(int index) {
	if(is_frame_free(index)){
		frame_map[index] = allocated_frame();
	} else {
		PANIC("FRAME ALLOCATION ERROR\n"
		      "Attempted allocation of non free frame");
	}
}


void deallocate_frame(int index) {
	if(!is_frame_free(index)){
		frame_map[index] = non_allocated_frame();
	} else {
		PANIC("FRAME DEALLOCATION ERROR\n"
		      "Attempted deallocation of free frame");
	}
}


void allocate_n_frames(int index, int n) {
	if(index == -1) {
		PANIC("FRAME ALLOCATION ERROR\n"
		      "Negative frame index passed to allocation"
			  "routine");		return;
	}
	for(int i = 0; i < n;i++) {
		allocate_frame(index+i);
	}
}

void free_n_frames(int index,int n) {
	if(index == -1) {
		PANIC("FRAME DEALLOCATION ERROR\n"
		      "Negative frame index passed to deallocation"
			  "routine");
		return;
	}
	for(int i = 0; i < n;i++) {
		deallocate_frame(index+i);
	}
}

void* fmalloc(int size,unsigned long address) {
	unsigned long f_id = frame_id(address);
	unsigned long count = frames_required(size);
	bool all_free = true;
	for(unsigned int i = 0; i < count;i++) {
		all_free &= is_frame_free(f_id + i);
	}
	if(all_free) {
		allocate_n_frames(f_id,count);
		return (void*)(f_id * frame_size);
	} else {
		return 0;
	}
}

void* fmalloc(int size) {
	unsigned int n = frames_required(size);
	int index = first_free_n_id(n);
	allocate_n_frames(index,n);
	return (void*)(frame_address(index));
}

void ffree(void* ptr,int size) {
	int index = frame_id((unsigned long)ptr);
	int count = frames_required(size);
	free_n_frames(index,count);
}

//Initialize the frame allocator
void init_frame_manager() {
	for(int i = 0; i < num_frames;i++) {
		if(i <= get_last_kernel_frame()) {
			frame_map[i] = allocated_frame();
		} else {
			frame_map[i] = non_allocated_frame();
		}
	}
}

void print_range(int start,int end,bool used) {
	unsigned long addr_start = start*4096;
	unsigned long addr_end  = end*4096;
	void* arr[] = {&addr_start, &start, &addr_end, &end, &used};
	putf("start=%h(%d),end=%h(%d),alloc=%b\n",arr);
}

void print_frame_map() {
	bool prev_used = true;
	int range_base = 0;
	puts("Frame allocator map\n");
	for(int i = 0; i < num_frames;i++) {
		bool current_used = !is_frame_free(i);
		if(prev_used & current_used) {
			//Nothing happens
		} else if (prev_used & !current_used) {
			print_range(range_base,i-1,true);
			range_base = i;
		} else if (!prev_used & current_used) {
			print_range(range_base,i-1,false);
			range_base = i;
		} else {
			//Nothing happens
		}
		prev_used = current_used;
	}
}