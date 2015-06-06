#include <system.h>

unsigned int placement_address = kernel_limit+1;

unsigned int _kmalloc(unsigned int sz, bool align, unsigned int *phys)
{
  if (align&& (placement_address & 0xFFFFF000)) // If the address is not already page-aligned
  {
    // Align it.
    placement_address &= 0xFFFFF000;
    placement_address += 0x1000;
  }
  if (phys)
  {
    *phys = placement_address;
  }
  unsigned int  tmp = placement_address;
  placement_address += sz;
  return tmp;
}
unsigned int kmalloc(unsigned int sz) {
	return _kmalloc(sz,false,0);
}
unsigned int kmalloc_a(unsigned int sz){
	return _kmalloc(sz,true,0);
}
unsigned int kmalloc_p(unsigned int sz, unsigned int *phys){
	return _kmalloc(sz,false,phys);
}
unsigned int kmalloc_ap(unsigned int sz, unsigned int *phys) {
	return _kmalloc(sz,true,phys);
}