#ifndef _FRAME_MANAGER_H
#define _FRAME_MANAGER_H
unsigned long const frame_size = 4096;
long const num_frames = 1000000;
//Initalize the frame manager. All frames between indexes 0 and the frame(kernel_address) are
//set as in use, all the remaining are set as free.
void init_frame_manager();
//Similar to malloc and free, only they deal in frames. 
//fmalloc allocate a sequence of frames which span the given size.
//ffree frees the frames started at the given pointer.
void* fmalloc(int size);
void* fmalloc(int size,unsigned long address);
void ffree(void* ptr,int count);

//Diagnostics
int first_free_n_id(int n);
int get_last_kernel_frame();
void print_frame_map();
#endif