#ifndef _FRAME_ALLOCATOR_H
#define _FRAME_ALLOCATOR_H
void frame_allocator_init();
//Addresses/frame id helpers
int frame_id(unsigned long address);
unsigned long frame_address(int id);
unsigned long frame_aligned(unsigned long address);
//Internal functionality, still exposed
int first_free_frame();
unsigned char* allocate_frames();
void deallocate_frames(int start,int length);
//Main usage
unsigned char* allocate_first_free_frame();
void deallocate_frame(unsigned char* ptr);
#endif