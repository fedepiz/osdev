#ifndef __SYSTEM_H
#define __SYSTEM_H
void memcpy(unsigned char *dest, const unsigned char *src, int count);
void memset(unsigned char *dest, unsigned char val, int count);
void memcpyw(unsigned short* dest, const unsigned short* src,int count);
void memsetw(unsigned short *dest, unsigned short val, int count);
int strlen(const char *str);
unsigned char inportb (unsigned short _port);
void outportb (unsigned short _port, unsigned char _data);
#endif