#include <frame_allocator.h>
#include <system.h>
unsigned const long frame_size = 4096; //4KB Frames
 /*Bitmap for up to 4GB :
   4GB memory / 4KB frames = 1.000.000 frames.
   We need 1 byte per 8 frames, so
   1.000.000 / 8 = 125.000 bytes bitmap.
   
   Note that we exclude kernel binary and 
   holes in memory map, therefore we only
   use bitmap to keep track of things, but
   it is not an accurate model of memory
 */
unsigned const long bitmap_size = 125000;
unsigned char allocator_bitmap[bitmap_size];

void frame_allocator_init() {
	memset((unsigned char*)&allocator_bitmap,0,bitmap_size);
}