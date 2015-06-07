#ifndef _FRAME_MANAGER_H
#define _FRAME_MANAGER_H
unsigned long const frame_size = 4096;
long const num_frames = 1000000;

void init_frame_manager();
void* fmalloc(int size);
void ffree(void* ptr);

//Diagnostics
int first_free_n_id(int n);
int get_last_kernel_frame();
#endif