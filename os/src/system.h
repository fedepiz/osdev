#ifndef __SYSTEM_H
#define __SYSTEM_H
void memcpy(unsigned char *dest, const unsigned char *src, int count);
void memset(unsigned char *dest, unsigned char val, int count);
void memcpyw(unsigned short* dest, const unsigned short* src,int count);
void memsetw(unsigned short *dest, unsigned short val, int count);
int strlen(const char *str);
unsigned char inportb (unsigned short _port);
void outportb (unsigned short _port, unsigned char _data);
int atoi(char *str);
void reverse(char* str, int length);
char* itoa(int num, char* str);

struct regs {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp ,ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
};
#endif