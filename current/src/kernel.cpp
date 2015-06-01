#include <system.h>
#include <scrn.h> 
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main() {
	init_video();
	cls();
	puts("Welcome to PizOS 0.0000000.....000001");
}