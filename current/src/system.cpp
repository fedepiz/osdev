#include <system.h>
void memcpy(unsigned char* dest, const unsigned char* src,int count) {
	for(int i = 0; i < count;i++) {
		dest[i] = src[i];
	}
}

void memset(unsigned char* dest,unsigned char val, int size) {
	for(int i = 0; i < size;i++) {
		dest[i] = val;
	}
} 

void memcpyw(unsigned short* dest, const unsigned short* src,int count){
	for(int i = 0; i < count;i++) {
		dest[i] = src[i];
	}
}

void memsetw(unsigned short *dest, unsigned short val, int count){
	for(int i = 0; i < count;i++) {
		dest[i] = val;
	}
}

int strlen(const char *str) {
	int c = 0;
	while(*str != '\0') {
		c++;
		str++;
	}
	return c;
}
unsigned char inportb (unsigned short _port) {
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;	
}
void outportb (unsigned short _port, unsigned char _data){
	 __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}
